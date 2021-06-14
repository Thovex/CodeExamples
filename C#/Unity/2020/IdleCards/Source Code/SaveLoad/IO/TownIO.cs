using System;
using System.Collections.Generic;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.Utilities;

namespace BaerAndHoggo.Gameplay.Towns
{
    [Serializable]
    public class TownIO : IIO
    {
        public long Id { get; set; }
        public string Name { get; set; }
        public long Tier { get; set; }
        public Dictionary<long, BuildProgress> Buildings { get; set; }
        
        public override string ToString()
        {
            return $"Town Data: Id ({Id}), Name ({Name}), Tier ({Tier}), Buildings ({Buildings.ToDebugString()})" +
                   $"{Environment.NewLine}";
        }
    }
}