using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public class None : CardBuff
    {
        public override string GetName() => $"";
        public override bool Activate_Minion() => true;
        public override bool Activate_Captain() => true;
        protected override bool CanActivate_Minion() => true;
        protected override bool CanActivate_Captain() => true;
        protected override void DoFunctionality_Minion() { }
        protected override void DoFunctionality_Captain() { }
        public override string GetDescription(int power, string hex) => $"";
    }
}