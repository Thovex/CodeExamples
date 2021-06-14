using System;
using BaerAndHoggo.SaveData;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    [SerializeField]
    public class CardPackIO : IIO
    {
        
    }

    public class CardPackDB : DB<CardPack, CardPackIO[] >
    {
        protected override void InitializeDB()
        {
            IsInitialized = true;
        }

        protected override void StartLoad(IO.SaveData loadData)
        {
            OnLoad_Implementation(loadData.CardPackIO);
        }

        public override CardPackIO[] OnSave_Implementation()
        {
            throw new NotImplementedException();
        }

        public override void OnLoad_Implementation(CardPackIO[] loadData)
        {
            //
        }
    }
}