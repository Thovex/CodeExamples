using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.Gameplay.Towns;
using BaerAndHoggo.IO;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.UI;
using BaerAndHoggo.Utilities;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Time
{
    public class TimeEventManager : Singleton<TimeEventManager>, 
        ISaveData<TimeEventIO>
    {
        [SerializeField] private BuildingUI buildingUI;
        [SerializeField] private TownUI townUI;
        
        private double Seconds { get; set; } = 0;
        public double SessionTimer { get; set; }
        public bool TimerActive { get; set; } = false;
        public DateTime SessionStart { get; set; }
        public DateTime CalculateNewSaveDate() => SessionStart + SessionTimer.ParseTimeSpan();

        public delegate void SecondTick();
        public static event SecondTick OnSecondTick;
        
        [SerializeField] public Dictionary<ITimeEventCaller, List<TimedEvent>> eventDb 
            = new Dictionary<ITimeEventCaller, List<TimedEvent>>();

        private void Awake()
        {
            SaveManager.Instance.OnLoad += LoadEventManagerData;
        }

        public void LoadEventManagerData(IO.SaveData loadData )
        {
            Debug.Log("Loaded TimeEventManager io");
            OnLoad_Implementation(loadData.TimeEventIO);
        }
        
        public TimeEventIO OnSave_Implementation()
        {
            var data = new Dictionary<CallerEventTypeId, TimedEventIO[]>();
            
            foreach (var dbValue in eventDb)
            {
                // Get type, such as: Building or Town and it's Id
                var eventType = dbValue.Key.GetEventType();
                var eventId = dbValue.Key.GetID();
                
                var timerData = new TimedEventIO[dbValue.Value.Count];

                for (var i = 0; i < dbValue.Value.Count; i++)
                {
                    timerData[i] = dbValue.Value[i].OnSave_Implementation();
                }
                
                data.Add(new CallerEventTypeId { Type = eventType, Id = eventId }, timerData);
            }

            return new TimeEventIO()
            {
                TimerData = data,
            };
        }

        public void OnLoad_Implementation(TimeEventIO loadData)
        {
            eventDb.Clear();

            foreach (var data in loadData.TimerData)
            {
                ITimeEventCaller caller;
                var callerType = data.Key.Type;
                
                switch (callerType)
                {
                    case CallerEventType.Building: caller = BuildingDB.Instance.GetItemById(data.Key.Id); break;
                    case CallerEventType.Town: caller = TownDB.Instance.GetItemById(data.Key.Id); break;
                    default: throw new ArgumentOutOfRangeException();
                }

                foreach (var thisEvent in data.Value)
                {
                    switch (thisEvent.Type)
                    {
                        case "ActionEvent":
                        {
                            var newEvent = new ActionEvent(caller, (ActionEventIO) thisEvent);                            
                            
                            newEvent.OnCallFunctionality += buildingUI.ActionEventCall;
                            SetTimedEvent(caller, newEvent, -1, false);
                            
                        } break;
                        case "ConstructionEvent":
                        {
                            var newEvent = new ConstructionEvent(caller, (ConstructionEventIO) thisEvent);

                            newEvent.OnCallFunctionality += townUI.FinishConstruction;
                            SetTimedEvent(caller, newEvent, -1, false);
                            
                        } break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                }
            }
        }
        
        public void FixedUpdate()
        {
            if (!TimerActive) return;

            Seconds += UnityEngine.Time.fixedUnscaledDeltaTime;

            if (!(Seconds >= 1.00)) return;
            
            SessionTimer++;
            Seconds--;
            
            foreach (var dbValue in eventDb)
            {
                // Needs to be for- as we modify this enumeration.
                for (int i = 0; i < dbValue.Value.Count; i++)
                {
                    dbValue.Value[i].Decrement();
                }
            }
            
            OnSecondTick?.Invoke();
        }

        public void RemoveExistingEvent(ITimeEventCaller caller, TimedEvent removeEvent)
        {
            eventDb[caller].Remove(removeEvent);
        }

        public bool GetCallerCurrentActiveEvent (ITimeEventCaller caller, out TimedEvent value)
        {
            value = null;

            if (!eventDb.ContainsKey(caller))
            {
                return false;
            }
            
            for (int i = 0; i < eventDb[caller].Count; i++)
            {
                if (eventDb[caller][i].pause) continue;
                
                value = eventDb[caller][i];
                return true;
            }

            return false;
        }

        public void SetTimedEvent(ITimeEventCaller caller, TimedEvent timedEvent, double duration, bool setDuration = true)
        {
            if (setDuration)
            {
                timedEvent.SetTimeAndStart(duration);
                timedEvent.duration = duration;
            }

            timedEvent.OnComplete += RemoveExistingEvent;

            if (eventDb.ContainsKey(caller))
            {
                for (int i = 0; i < eventDb[caller].Count; i++)
                {
                    eventDb[caller][i].pause = true;
                    // Note! We pause ALL events from building. Even construction, maybe will be an issue later?

                    if (timedEvent == eventDb[caller][i])
                    {
                        timedEvent.seconds = eventDb[caller][i].seconds;
                        this.RemoveExistingEvent(caller, eventDb[caller][i]);
                    }
                }
                
                eventDb[caller].Add(timedEvent);
            }
            else
            {
                eventDb.Add(caller, new List<TimedEvent> { timedEvent });
            }
        }
    }
}