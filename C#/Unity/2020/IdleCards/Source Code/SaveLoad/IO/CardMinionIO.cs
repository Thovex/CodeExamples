using System;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public class CardMinionIO : CardIO
    {
        public int Hp { get; set; }
        public int Damage { get; set; }
        public int Defense { get; set; }
        public MinionType MinionType { get; set; }

        public override string ToString()
        {
            return
                $"CardMinionData: Id ({Id}), Name ({Name}), Tier ({Tier}), Category ({Category}), Type ({Type}), Rarity ({Rarity}), PlayCost ({PlayCost})," +
                $"Hp ({Hp}), Damage ({Damage}), Defense ({Defense}), MinionType ({MinionType})" +
                $"{Environment.NewLine}";
        }
    }
}

