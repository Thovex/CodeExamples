using System;
using System.Collections.Generic;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.Utilities;
using Gameplay.Currency;

namespace BaerAndHoggo.Gameplay.Buildings
{
    [Serializable]
    public class BuildingIO : IIO
    {        
        public long Id { get; set; }
        public string Name { get; set; }
        public long Tier { get; set; }
        public int Cost { get; set; }
        public Currencies Currency { get; set; }
        public double ConstructionTime { get; set; }
        public Dictionary<BuildingActionType, double> TimeInSecondsPerAction { get; set; }
        
        public override string ToString()
        {
            return
                $"BuildingData: Id ({Id}), Name ({Name}), Tier ({Tier}), Cost ({Cost}), Currency ({Currency}), ConstructionTime ({ConstructionTime})," +
                $"TimeInSecondsPerAction ({TimeInSecondsPerAction.ToDebugString()}"+
                $"{Environment.NewLine}";
        }

    }
}