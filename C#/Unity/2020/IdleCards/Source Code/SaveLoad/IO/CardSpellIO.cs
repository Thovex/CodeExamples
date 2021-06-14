using System;
using System.Collections.Generic;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public class CardSpellIO : CardIO
    {
        public List<SpellIdWithPower> Spells { get; set; }
        public bool SpellOneShot { get; set; }
        public SpellType SpellType { get; set; }
        public override string ToString()
        {
            return $"CardSpellData: Id ({Id}), Name ({Name}), Tier ({Tier}), Category ({Category}), Type ({Type}), Rarity ({Rarity}), PlayCost ({PlayCost})" +
                   $"{Environment.NewLine}";
        } 
    }
}