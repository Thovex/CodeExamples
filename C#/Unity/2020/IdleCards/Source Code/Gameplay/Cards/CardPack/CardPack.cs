using System;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;
using Random = UnityEngine.Random;

namespace BaerAndHoggo.Gameplay.Cards
{
    [CreateAssetMenu(fileName = "New CardPack", menuName = "Idle Cards/Card Pack", order = 1)]
    [Serializable]
    public class CardPack : SerializedScriptableObject,
        IIdentifiable,
        ITimeEventCaller
    {        
        [ReadOnly] public long id;
        public string packName;
        public int cardCount;
        public double rarityMultiplier;
        public ChanceData chanceData;
        
        [PreviewField] public Sprite packSprite;

        public CallerEventType GetEventType() => CallerEventType.CardPack;
        public long GetID() => id;
        public string GetName() => packName;

        public CardPack()
        {
            chanceData = ChanceData.DefaultPack;
        }


#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            if (id == -1)
            {
                var buildings = Resources.LoadAll<CardPack>("CardPacks");
                id = buildings.Max(c => c.id)+1;
            }
            
            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{packName}");
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