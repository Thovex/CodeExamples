using System;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.Utilities;
using Type = BaerAndHoggo.Gameplay.Cards.Type;

namespace BaerAndHoggo.Gameplay.Inventories
{
    [Serializable]
    public class CardStackIO : IIO
    {
        public long Id { get; set; }
        public string Name { get; set; }
        public Type Type { get; set; }
        public CardIO[] Stack { get; set; }
        
        public override string ToString()
        {
            return $"CardStack Data: Id ({Id}), Name ({Name}), Type ({Type}), Cards ({Stack.ToDebugString()})" +
                   $"{Environment.NewLine}";
        }
    }
}