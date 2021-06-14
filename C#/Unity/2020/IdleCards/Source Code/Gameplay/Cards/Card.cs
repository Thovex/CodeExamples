using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.SaveData;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;
using UnityEngine.Serialization;

namespace BaerAndHoggo.Gameplay.Cards
{
    public enum Type
    {
        Minion,
        Spell,
        Captain,
    }

    public enum Rarity
    {
        Common,
        Uncommon,
        Rare,
        Epic,
        Legendary
    }

    public enum Category
    {
        None,
        Orcs,
        DarkElf,
        Elf,
        Rat,
        Hedgehog,
        Vampire
    }

    public enum Buffs
    {
        DoubleAttack,
        DoubleDamage,
        HealthRegen
    }

    [Serializable]
    public abstract class Card : SerializedScriptableObject, IIdentifiable
    {

        [Title("Required", "Id is required to be unique.")]
        [ReadOnly] public long id = -1;
        public string cardName = "Unnamed Base Card";

        public Category category = Category.None;
        public Type type = Type.Minion;
        
        [Title("Art")] 
        [PreviewField] public Sprite portrait;

        [Title("Gameplay")] 
        [GUIColor("GetRarityColor")] public Rarity rarity = Rarity.Common;
        public int manaCost = -1;
        
        public long GetID() => id;
        public string GetName() => cardName;
        
        public delegate void UpdateInfo();
        public event UpdateInfo OnUpdate;
        
        public void CallUpdate()
        {
            OnUpdate?.Invoke();
        }


#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            if (id == -1)
            {
                var cards = Resources.LoadAll<Card>("Cards");
                id = cards.Max(c => c.id)+1;
            }
            
            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{cardName}");
                AssetDatabase.SaveAssets();
            }
            catch (Exception)
            {
                // ignored
            }
        }

        private Color GetRarityColor()
        {
            return RarityDB.GetColorThemeByRarity(rarity).General;
        }
#endif
    }
}