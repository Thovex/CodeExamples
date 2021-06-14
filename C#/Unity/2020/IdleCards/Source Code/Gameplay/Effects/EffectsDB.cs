using BaerAndHoggo.Gameplay;
using BaerAndHoggo.SaveData;
using Gameplay.Effects;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Effects
{
    public class EffectIO : IIO
    {
        
    }
    
    public class EffectsDB : DB<EffectObject, EffectIO>
    {
        protected override void InitializeDB()
        {
            IsInitialized = true;
        }

        protected override void StartLoad(IO.SaveData loadData)
        {
            // Ignored
        }

        public override EffectIO OnSave_Implementation()
        {
            return new EffectIO();
        }

        public override void OnLoad_Implementation(EffectIO loadData)
        {
            // Ignored
        }
    }
}