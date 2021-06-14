using BaerAndHoggo.SaveData;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Time
{
    public abstract class TimedEvent : 
        IIdentifiable, ISaveData<TimedEventIO>
    {
        public delegate void CompleteTimedEvent(ITimeEventCaller origin, TimedEvent thisEvent);
        public event CompleteTimedEvent OnComplete;

        public abstract TimedEventIO OnSave_Implementation();
        public abstract void OnLoad_Implementation(TimedEventIO loadData);
        
        public ITimeEventCaller origin;
        public string eventType;
        
        public int multiplier = 1;
        public bool pause = true;
        public double seconds;
        public double duration;
        public float percentage;

        protected TimedEvent(ITimeEventCaller origin)
        {
            this.eventType = this.GetType().Name;
            this.origin = origin;
        }

        public void SetTimeAndStart(double seconds)
        {
            this.seconds = seconds;
            this.pause = false;
        }

        public virtual bool Decrement()
        {
            if (this.pause) return true;

            this.seconds -= 1 * this.multiplier;
            CalculatePercentage();

            if (this.seconds <= 0)
            {
                this.pause = true;
                InvokeEvent();
                return false;
            }

            return true;
        }

        public float CalculatePercentage()
        {
            percentage = (float) (seconds / duration);
            return percentage;
        }

        public float CalculateOppositePercentage()
        {
            return 1 - CalculatePercentage();
        }

        public virtual void InvokeEvent()
        {
            OnComplete?.Invoke(origin, this);
        }

        public abstract long GetID();
        public virtual string GetName() => GetType().Name;

        public static bool operator ==(TimedEvent a, TimedEvent b)
        {
            return (a.GetID() == b.GetID()) && (a.GetName() == b.GetName());
        }

        public static bool operator !=(TimedEvent a, TimedEvent b) => !(a == b);

        protected bool Equals(TimedEvent other) => this == other;

        public override int GetHashCode() => origin.GetHashCode();

        public override bool Equals(object obj)
        {
            if (obj.GetType() == this.GetType())
                return (TimedEvent)obj == this;
            return false;
        }
    }
}