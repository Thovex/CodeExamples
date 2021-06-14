using UnityEngine;

namespace BaerAndHoggo.Gameplay.Battle
{
    public class EnemyAIIO
    {
        
    }
    
    public class AiDB : DB<EnemyAI, EnemyAIIO>
    {
        protected override void InitializeDB()
        {
            IsInitialized = true;
        }

        protected override void StartLoad(IO.SaveData loadData)
        {
            // Ignored
        }

        public override EnemyAIIO OnSave_Implementation()
        {
            return new EnemyAIIO();
        }

        public override void OnLoad_Implementation(EnemyAIIO loadData)
        {
            // Ignored
        }
    }
}