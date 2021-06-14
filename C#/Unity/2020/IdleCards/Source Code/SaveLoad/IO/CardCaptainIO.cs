using System;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public class CardCaptainIO : CardIO
    {
        public int Hp { get; set; }
        public int StartHp  { get; set; }
        public int StartMana  { get; set; }
        public int ManaGrowth  { get; set; }
        public int MaxMana { get; set; }
        public int ActiveManaCost { get; set; }
        public long ActiveSpell { get; set; }
        public long PassiveSpell { get; set; }


    }
}