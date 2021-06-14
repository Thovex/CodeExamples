using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;
using Random = UnityEngine.Random;

namespace BaerAndHoggo.Gameplay.Battle
{
    public enum EnemyDifficulty { Easy, Beginner, Novice, Normal, Medium, Intermediate, Hard, Expert, Ungodly }
    public enum EnemyArchetype { Rush, Control, Heal }
    
    [CreateAssetMenu(fileName = "AI", menuName = "Idle Cards/AI Opponent", order = 0)]
    public class EnemyAI : SerializedScriptableObject, IIdentifiable
    {
        [Title("Required")] 
        [ReadOnly] public long id = -1;
        public string aiName = "Unnamed AI";

        [Title("Gameplay")] 
        public string lore = "No lore";

        public Sprite portrait;
        
        public EnemyDifficulty difficulty;
        public EnemyArchetype archetype;

        public List<CardDeck> decks;
        public CardDeck GetRandomDeck() => (CardDeck) decks.RandomElement().Clone();
        
        
        #region IIdentifiable

        public long GetID() => id;
        public string GetName() => aiName;

        #endregion IIdentifiable
        
#if UNITY_EDITOR
        
        protected virtual void OnValidate()
        {
            if (id == -1)
            {
                var ais = Resources.LoadAll<EnemyAI>("AI");
                id = ais.Max(a => a.id)+1;
            }

            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{aiName}");
                AssetDatabase.SaveAssets();
            }
            catch (Exception)
            {
                // ignored
            }
        }
#endif
    }
}