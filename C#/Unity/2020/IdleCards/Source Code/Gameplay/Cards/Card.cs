using System;
using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public enum Type { None, Minion, Spell };
    public enum Rarity { Common, Uncommon, Rare, Epic, Legendary };
    public enum Category { None, Orcs };
    public enum Buffs { DoubleAttack, DoubleDamage, HealthRegen }

    [Serializable]
    public abstract class CardSaveData
    {
        public long Id { get; set; }
        public string Name { get; set; }
        public int Tier { get; set; }
        public Type Type { get; set; }
        public Category Category { get; set; }
    }

    [System.Serializable]
    public abstract class Card : SerializedScriptableObject
    {
        [Title("Required", "Id is required to be unique.")]
        public long Id = -1;
        public string Name = "Unnamed Base Card";
        public int Tier = -1;

        [Title("Gameplay")]
        [GUIColor("GetRarityColor")] public Rarity Rarity = Rarity.Common;
        public Type Type = Type.None;
        public Category Category = Category.None;

        [Title("Buffs & Debuffs", "Only classes inherited from Option")]
        public List<CardBuff> Buffs = new List<CardBuff>();
        public List<CardDebuff> Debuffs = new List<CardDebuff>();

        [Title("Upgrades", "Only use when you want this to be an upgradeable Card.")]
        public bool CanUpgrade = false;
        [ShowIf("CanUpgrade", true)] public Dictionary<Card, int> UpgradeRequirements = new Dictionary<Card, int>();
        [ShowIf("CanUpgrade", true)] public Card UpgradeBecomes = null;

        [Title("Art")]
        [PreviewField] public Sprite Portrait;


#if UNITY_EDITOR
        protected virtual void OnValidate()
        {
            try
            {
                string assetPath = AssetDatabase.GetAssetPath(this.GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{this.Id}-{this.Name}");
                AssetDatabase.SaveAssets();
            }
            catch (Exception) { }

        }

        private Color GetRarityColor()
        {
            return RarityDB.GetColorThemeByRarity(this.Rarity).General;
        }
#endif
    }
}