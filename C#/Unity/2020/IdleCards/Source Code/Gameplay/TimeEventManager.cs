using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using BaerAndHoggo.UI;
using BaerAndHoggo.Utilities;
using BaerAndHoggo.Gameplay.Buildings;

namespace BaerAndHoggo.Gameplay.Time
{

    public enum BuildingActionType { NULL, CardPack, Upgrade, Repair }

    [Serializable]
    public class TimedEvent
    {
        // Not Serializable 
        private DateTime startDate;
        [SerializeField] private int seconds;
        [SerializeField] private bool paused;
        public int Seconds { get => seconds; set => seconds = value; }
        public bool Paused { get => paused; set => paused = value; }
        public DateTime StartDate { get => startDate; set => startDate = value; }

        public TimedEvent(int seconds)
        {
            this.startDate = DateTime.Now;
            this.seconds = seconds;
            this.paused = false;
        }

        public void Pause()
        {
            Paused = true;
            Debug.Log($"Paused: {this.paused}");
        }
        public void UnPause()
        {
            Paused = false;
            Debug.Log($"Paused: {this.paused}");
        }

        public int GetRemainder()
        {
            return Seconds;
        }

        public void SetNewTime(int seconds)
        {
            StartDate = DateTime.Now;
            Seconds = seconds;
        }

        public int Decrement(int multiplier = 1)
        {
            return (Seconds -= 1 * multiplier);
        }

        // TODO: Calculate percentage and make it new %

    }

    public class TimeEventManager : Singleton<TimeEventManager>
    {

        [SerializeField]
        public Dictionary<Building, Dictionary<BuildingActionType, TimedEvent>> TimedEvents {get; set; }
            = new Dictionary<Building, Dictionary<BuildingActionType, TimedEvent>>();

        [SerializeField] private bool timerActive = true;
        
        // TODO: Make this not here
        [SerializeField] [Required] private BuildingUI buildingUI;


        private void Start()
        {
            StartCoroutine(Timer());
        }

        private IEnumerator Timer()
        {
            while (timerActive)
            {
                yield return new WaitForSecondsRealtime(1);
                foreach (var existingBuildings in TimedEvents)
                {
                    foreach (var existingEvents in existingBuildings.Value)
                    {
                        if (!existingEvents.Value.Paused)
                        {
                            int newSeconds = existingEvents.Value.Decrement();

                            // Only update timer when this container is an active UI.
                            if (buildingUI.Building == existingBuildings.Key)
                            {
                                buildingUI.UpdateTimer();
                            }


                            if (newSeconds <= 0)
                            {
                                existingEvents.Value.Pause();
                                buildingUI.ActionEventCall(existingBuildings.Key, existingEvents.Key);
                            }
                        }
                    }

                }
            }
        }

        public int GetTimeRemaining(Building origin, BuildingActionType type)
        {
            if (TimedEvents.ContainsKey(origin))
            {
                if (TimedEvents[origin].ContainsKey(type))
                {
                    return TimedEvents[origin][type].Seconds;
                }
            }

            return origin.TimeInSecondsPerAction[type];
        }

        public float GetTimeProceeded(Building origin, BuildingActionType type)
        {
            int secondsRemaining = GetTimeRemaining(origin, type);

            return Math.Abs(secondsRemaining - origin.TimeInSecondsPerAction[type]);
        }

        public BuildingActionType GetActiveType(Building origin)
        {
            if (TimedEvents.ContainsKey(origin))
            {
                foreach (var existingEvents in TimedEvents[origin])
                {
                    if (!existingEvents.Value.Paused)
                    {
                        return existingEvents.Key;
                    }
                }
            }

            return BuildingActionType.CardPack;
        }

        public void SetEvent(Building origin, BuildingActionType type, int seconds)
        {
            if (TimedEvents.TryGetValue(origin, out var originEventDictionary))
            {
                Debug.Log($"{origin} known in TimeEventManager");

                // First Pause all existing events if any.
                foreach (var existingEvent in originEventDictionary)
                {
                    existingEvent.Value.Pause();
                }

                // Check if we have any events of this type
                if (originEventDictionary.TryGetValue(type, out var existingTimedEvent))
                {
                    int queryDuration = existingTimedEvent.GetRemainder();

                    queryDuration = (queryDuration > seconds) ? seconds : queryDuration;

                    if (queryDuration <= 0)
                    {
                        Debug.Log($"Seems like this {type} was finished. Restarting from scratch.");

                        queryDuration = seconds;
                    }

                    Debug.Log($"Found {type} in existing events. Defining remainder {queryDuration}");

                    // Calculate a new time & Unpause.
                    existingTimedEvent.SetNewTime(queryDuration);
                    existingTimedEvent.UnPause();
                }
                else
                {
                    // We don't have this type of event for this building yet. Adding it now.
                    Debug.Log($"Could not find {type} yet. Adding with seconds: {seconds}");
                    originEventDictionary.Add(type, new TimedEvent(seconds));
                }
            }
            else
            {
                Debug.Log($"{origin} unknown in TimeEventManager. Adding origin & new dictionary with a {type} entry.");

                // We don't have an event yet. Add this as a new origin and add the current type & timer to the dictionaries.
                TimedEvents.Add(origin, new Dictionary<BuildingActionType, TimedEvent> { { type, new TimedEvent(seconds) } });
            }
        }
    }
}