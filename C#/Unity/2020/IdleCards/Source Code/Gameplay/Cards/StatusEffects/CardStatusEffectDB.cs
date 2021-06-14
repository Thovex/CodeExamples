using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Utilities;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public class CardStatusEffectIO
    {
        
    }
    
    public class CardStatusEffectDB : DB<CardStatusEffectObject, CardStatusEffectIO>
    {
        public new static CardStatusEffectDB Instance => (CardStatusEffectDB)GetInstance();

        protected override void InitializeDB()
        {
            IsInitialized = true;
        }
        
        // Return new instantiation of object we found.
        public override CardStatusEffectObject GetItemById(long id)
        {
            return Instantiate(db.Where(item => id == item.Value.GetID()).
                Select(item => item.Value).FirstOrDefault());
        }

        protected override void StartLoad(IO.SaveData loadData)
        {
            //
        }

        public override CardStatusEffectIO OnSave_Implementation()
        {
            return new CardStatusEffectIO();
        }

        public override void OnLoad_Implementation(CardStatusEffectIO loadData)
        {
            //
        }
    }
}