using System;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public class HealthRegen : CardBuff
    {
        public HealthRegen(CardMinion minionCardRef, int regenAmount)
        {
            MinionRef = minionCardRef;
            statusEffectPower = regenAmount;
        }
        
        public override string GetName()
        {
            return "Regeneration";
        }

        public override bool Activate_Minion()
        {
            if (CanActivate_Minion())
            {
                AddToStatusEffects_Minion();

                DoFunctionality_Minion();
                return true;
            }

            return false;
        }

        public override bool Activate_Captain()
        {
            if (CanActivate_Captain())
            {
                AddToStatusEffects_Captain();

                DoFunctionality_Captain();;
                return true;
            }

            return false;
        }

        protected override bool CanActivate_Minion()
        {
            return MinionRef.hp > 0;
        }

        protected override bool CanActivate_Captain()
        {
            return CaptainRef.hp > 0;
        }

        protected override void DoFunctionality_Captain()
        {
            CaptainRef.Heal(statusEffectPower);
        }

        protected override void DoFunctionality_Minion()
        {
            MinionRef.Heal(statusEffectPower);
        }

        public override string GetDescription(int power, string hex = "FF00FF")
        {
            return $"<b><color=#{hex}>{GetName().ToUpper()} {power}</color></b> per turn.";
        }
    }
}