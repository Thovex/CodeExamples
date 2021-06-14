using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.SaveData;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Time
{
    public class ActionEvent : TimedEvent
    {
        public delegate void CallFunctionality(
            ActionEvent thisEvent, 
            ITimeEventCaller origin, 
            Building building, 
            BuildingActionType type
        );
        
        public override TimedEventIO OnSave_Implementation()
        {
            return new ActionEventIO()
            {
                Type = eventType,
                Multiplier = multiplier,
                Pause = pause,
                Seconds = seconds,
                Duration = duration,
                Building = building.id,
                ActionType = actionType,
            };
        }
        public sealed override void OnLoad_Implementation(TimedEventIO loadData)
        {
            var loadDataEnrichment = (ActionEventIO) loadData;

            this.eventType = loadData.Type;
            this.multiplier = loadData.Multiplier;
            this.pause = loadData.Pause;
            this.seconds = loadData.Seconds;
            this.duration = loadData.Duration;
            this.building = BuildingDB.Instance.GetItemById(loadDataEnrichment.Building);
            this.actionType = loadDataEnrichment.ActionType;
        }
        
        public event CallFunctionality OnCallFunctionality;

        public Building building;
        public BuildingActionType actionType;

        public ActionEvent(ITimeEventCaller origin, Building building,  BuildingActionType actionType)
            : base(origin)
        {
            this.eventType = this.GetType().Name;
            this.building = building;
            this.actionType = actionType;
        }

        public ActionEvent(ITimeEventCaller origin, ActionEventIO data) : base(origin)
        {
            this.origin = origin;
            OnLoad_Implementation(data);
        }
        
        public override void InvokeEvent()
        {
            base.InvokeEvent();
            OnCallFunctionality?.Invoke(this, origin, building, actionType);
        }

        public override long GetID() => (long) actionType * 1111 + building.id * 1111;
         
    }
}