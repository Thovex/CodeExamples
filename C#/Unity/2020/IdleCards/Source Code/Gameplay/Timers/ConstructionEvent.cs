using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.SaveData;

namespace BaerAndHoggo.Gameplay.Time
{
    public class ConstructionEvent : TimedEvent
    {
        public delegate void CallFunctionality(
            ConstructionEvent thisEvent, 
            ITimeEventCaller origin, 
            Building building
        );
        
        public override TimedEventIO OnSave_Implementation()
        {
            return new ConstructionEventIO()
            {
                Type = eventType,
                Multiplier = multiplier,
                Pause = pause,
                Seconds = seconds,
                Duration = duration,
                Building = building.id,
            };
        }

        public sealed override void OnLoad_Implementation(TimedEventIO loadData)
        {
            var loadDataEnrichment = (ConstructionEventIO) loadData;

            this.eventType = loadData.Type;
            this.multiplier = loadData.Multiplier;
            this.pause = loadData.Pause;
            this.seconds = loadData.Seconds;
            this.duration = loadData.Duration;
            this.building = BuildingDB.Instance.GetItemById(loadDataEnrichment.Building);
        }
        
        public event CallFunctionality OnCallFunctionality;

        public Building building;
        
        public ConstructionEvent(ITimeEventCaller origin, Building building) : base(origin)
        {
            this.eventType = this.GetType().Name;
            this.building = building;
        }
        
        public ConstructionEvent(ITimeEventCaller origin, ConstructionEventIO data) : base(origin)
        {
            this.origin = origin;
            OnLoad_Implementation(data);
        }

        
        public override void InvokeEvent()
        {
            base.InvokeEvent();
            OnCallFunctionality?.Invoke(this, origin, building);
        }

        public override long GetID() => 0;
        
    }
}