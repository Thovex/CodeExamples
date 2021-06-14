using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public class Damage : CardDebuff
    {
        public override string GetName()
        {
            return "Damage";
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

                DoFunctionality_Captain();
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

        protected override void DoFunctionality_Minion()
        {
            MinionRef.TakeDamage(statusEffectPower);
        }

        protected override void DoFunctionality_Captain()
        {
            CaptainRef.TakeDamage(statusEffectPower);
        }

        public override string GetDescription(int power, string hex = "FF00FF")
        {
            return $"<b><color=#{hex}>{GetName().ToUpper()} {power}</color></b> per turn.";
        }
    }
}