using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Utilities;
using DG.Tweening;
using UnityEngine;

namespace BaerAndHoggo.UI
{
    public class TimerUI : Singleton<TimerUI>
    {

        [SerializeField] private Dictionary<TimerElement, ITimeEventCaller> subscribedTimers 
            = new Dictionary<TimerElement, ITimeEventCaller>();

        private List<TimerElement> destroyList = new List<TimerElement>();

        private Camera _mainCamera;

        private void Awake()
        {
            _mainCamera = Camera.main;
            TimeEventManager.OnSecondTick += UpdateTimers;
        }

        public void UpdateTimers()
        {
            if (destroyList.Count > 0)
            {
                foreach (var destroyElement in destroyList)
                {
                    subscribedTimers.Remove(destroyElement);
                }
            }
            
            destroyList.Clear();

            foreach (var subscribedTimer in subscribedTimers)
            {
                if (subscribedTimer.Key.rectTransform)
                {
                    if (subscribedTimer.Key.rectTransform.gameObject.activeInHierarchy)
                    {
                        DoUpdate(subscribedTimer);
                    }
                }
                else
                {
                    destroyList.Add(subscribedTimer.Key);
                }
            }
        }

        public void ForceUpdate(ITimeEventCaller caller)
        {
            if (!subscribedTimers.ContainsValue(caller)) return;
            
            foreach (var subscribedTimer in subscribedTimers)
            {
                if (subscribedTimer.Value != caller) continue;
                if (subscribedTimer.Key.rectTransform)
                {
                    DoUpdate(subscribedTimer);
                }
            }
        }
        
        private static void DoUpdate(KeyValuePair<TimerElement, ITimeEventCaller> subscribedTimer, bool tween = true)
        {
            if (TimeEventManager.Instance.GetCallerCurrentActiveEvent(subscribedTimer.Value, out var theEvent))
            {
                if (subscribedTimer.Key.timer)
                    subscribedTimer.Key.timer.text = Utility.DefineTimer(theEvent.seconds);

                if (!subscribedTimer.Key.slider) return;
                if (tween)
                    subscribedTimer.Key.slider.DOValue(theEvent.CalculateOppositePercentage(), .5F);
                else
                    subscribedTimer.Key.slider.value = theEvent.CalculateOppositePercentage();
            }
        }

        public void AddTimerElement(TimerElement timerElement, ITimeEventCaller eventCaller)
        {
            if (subscribedTimers.TryGetValue(timerElement, out var caller))
            {
                if (caller != eventCaller)
                {
                    subscribedTimers[timerElement] = eventCaller;
                }
            }
            else
            {
                // Force update timer to similar position before we lerp.
                DoUpdate(new KeyValuePair<TimerElement, ITimeEventCaller>(timerElement, eventCaller), false);
                subscribedTimers.Add(timerElement, eventCaller);
            }
            
            UpdateTimers();
        }
    }
}