using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Serialization;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public abstract class CardStatusEffect
    {
        [SerializeField] protected int statusEffectPower = 0;

        [SerializeField] protected bool isOneShot = false;

        public bool IsOneShot => isOneShot;
        public int GetPower => statusEffectPower;

        public void SetPower(int power)
        {
            statusEffectPower = power;
        }
        public void SetTargetMinion(CardMinion minion)
        {
            MinionRef = minion;
        }

        public void SetTargetCaptain(CardCaptain captain)
        {
            CaptainRef = captain;
        }

        public void SetOneShot(bool oneShot)
        {
            isOneShot = oneShot;
        }
        
        protected CardMinion MinionRef { get; set; }
        protected CardCaptain CaptainRef { get; set; }
        public abstract string GetName();

        public abstract bool Activate_Minion();
        public abstract bool Activate_Captain();
        
        protected void AddToStatusEffects_Minion()
        {
            if (isOneShot) return;
            
            if (!MinionRef.statusEffects.Contains(this))
                MinionRef.statusEffects.Add(this);
        }
        
        protected void AddToStatusEffects_Captain()
        {
            if (isOneShot) return;
            
            if (!CaptainRef.statusEffects.Contains(this))
                CaptainRef.statusEffects.Add(this);
        }
        
        protected abstract bool CanActivate_Minion();
        protected abstract bool CanActivate_Captain();
        
        protected abstract void DoFunctionality_Minion();
        protected abstract void DoFunctionality_Captain();

        public abstract string GetDescription(int power, string hex = "FF00FF");
    }
}