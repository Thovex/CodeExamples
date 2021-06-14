using System;
using BaerAndHoggo.SaveData;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public abstract class CardIO : IIO
    {
        public long Id { get; set; }
        public string Name { get; set; }
        public int Tier { get; set; }
        public Category Category { get; set; }
        public Type Type { get; set; }
        public Rarity Rarity { get; set; }
        public int PlayCost { get; set; }
        
        public override string ToString()
        {
            return
                $"CardData: Id ({Id}), Name ({Name}), Tier ({Tier}), Category ({Category}), Type ({Type}), Rarity ({Rarity}), PlayCost ({PlayCost})," +
                $"{Environment.NewLine}";
        }
    }
}