using System;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    [CreateAssetMenu(fileName = "New Status Effect", menuName = "Idle Cards/Status Effect", order = 1000)]
    public class CardStatusEffectObject : SerializedScriptableObject, IIdentifiable
    {
        [Title("Required", "Id is required to be unique.")]
        [ReadOnly] public long id = -1;
        
        public CardStatusEffect effect;
        
        public long GetID() => id;
        public string GetName() => effect.GetName();

#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            if (id == -1)
            {
                var statusEffects = Resources.LoadAll<CardStatusEffectObject>("StatusEffects");
                id = statusEffects.Max(s => s.id)+1;
            }
            
            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{GetName()}");
                AssetDatabase.SaveAssets();
            }
            catch (Exception)
            {
            }
        }
#endif
    }
}