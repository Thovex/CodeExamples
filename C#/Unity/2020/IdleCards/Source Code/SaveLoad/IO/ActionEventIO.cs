using System;
using BaerAndHoggo.Gameplay.Buildings;

namespace BaerAndHoggo.Gameplay.Time
{
    [Serializable]
    public class ActionEventIO : TimedEventIO
    {
        public long Building { get; set; }
        public BuildingActionType ActionType { get; set; }
    }
}