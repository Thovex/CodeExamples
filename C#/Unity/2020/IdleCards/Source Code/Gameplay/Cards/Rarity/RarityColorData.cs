using System;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using Sirenix.OdinInspector;
using UnityEngine;
using static BaerAndHoggo.Colors.ColorTheme;

namespace BaerAndHoggo.Colors
{
    // Only enable when we want more rarities to be created.
    // [CreateAssetMenu(fileName = "New Rarity Color Data", menuName = "Colors/Rarity Color Data", order = 0)]
    public class RarityColorData : SerializedScriptableObject
    {
        [Title("Color DataBase")] [SerializeField]
        private Dictionary<Variation, Color> colors;

        [SerializeField] private Sprite emblem;

        [Title("Rarity Color Structure", "Changeable only by editting the file manually.")]
        [GUIColor("GetRarityColor")] [ReadOnly] [SerializeField] private Rarity rarity;

        public RarityColorData()
        {
            Initialize();
        }

        private void Initialize()
        {
            colors = new Dictionary<Variation, Color>();

            foreach (var variation in (Variation[]) Enum.GetValues(typeof(Variation)))
                colors.Add(variation, new Color(0, 0, 0, 0));
        }

        public Dictionary<Variation, Color> GetDictionary() => colors;
        public Rarity GetRarity() => rarity;
        public Sprite GetEmblem() => emblem;
        

#if UNITY_EDITOR
        [Title("Tooling", "Small tools to help define and set coloring.")] [SerializeField]
        private string hexConversion;

        [SerializeField] private Color colorResult;

        [ButtonGroup("Conversion")]
        [Button("Hex > Color", ButtonSizes.Small)]
        private void ConvertHex()
        {
            if (!hexConversion.Contains("#")) hexConversion = $"#{hexConversion}";

            ColorUtility.TryParseHtmlString(hexConversion, out var color);
            colorResult = color;
        }

        [ButtonGroup("Conversion")]
        [Button("Color > Hex", ButtonSizes.Small)]
        private void ConvertColor()
        {
            hexConversion = $"#{ColorUtility.ToHtmlStringRGB(colorResult)}";
        }

        [BoxGroup("Apply")] [HideLabel] [EnumToggleButtons] [SerializeField]
        private Variation applyToEnum;

        [BoxGroup("Apply")]
        [HideLabel]
        [Button("Apply to Variation", ButtonSizes.Large)]
        private void ApplyToEnum()
        {
            if (colors.ContainsKey(applyToEnum)) colors[applyToEnum] = colorResult;
        }

        private Color GetRarityColor()
        {
            return RarityDB.GetColorThemeByRarity(rarity).General;
        }

#endif
    }
}