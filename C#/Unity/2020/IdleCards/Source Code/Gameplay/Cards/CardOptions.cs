using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    [System.Serializable]
    public abstract class CardOption
    {
        protected Card cardRef { get; set; }
        public abstract bool Activate();
        protected abstract bool CanActivate();
        protected abstract void DoFunctionality();
    }

    [System.Serializable]
    public abstract class CardBuff : CardOption { }

    [System.Serializable]
    public abstract class CardDebuff : CardOption { }

    [System.Serializable]
    public class HealthRegen : CardBuff
    {
        [SerializeField] private int regenAmount = 0;
        public HealthRegen(CardMinion minionCardRef, int regenAmount)
        {
            this.cardRef = minionCardRef;
        }

        public override bool Activate()
        {
            if (CanActivate())
            {
                DoFunctionality();
                return true;
            }
            return false;
        }

        protected override bool CanActivate()
        {
            return Minion.hp > 0;
        }

        protected override void DoFunctionality()
        {
            Minion.hp += regenAmount;
        }

        protected CardMinion Minion => (CardMinion)this.cardRef;
    }
}