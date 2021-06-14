using Sirenix.OdinInspector;
using UnityEngine;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;

using static BaerAndHoggo.Colors.ColorTheme;

namespace BaerAndHoggo.Colors
{
    // Only enable when we want more rarities to be created.
    // [CreateAssetMenu(fileName = "New Rarity Color Data", menuName = "Colors/Rarity Color Data", order = 0)]
    public class RarityColorData : SerializedScriptableObject
    {
        [Title("Rarity Color Structure", "Changeable only by editting the file manually.")]
        [GUIColor("GetRarityColor")] [ReadOnly] [SerializeField] private Rarity rarity;

        [Title("Color DataBase")]
        [SerializeField] private Dictionary<Variation, Color> colors;

        public RarityColorData()
        {
            Initialize();
        }

        private void Initialize()
        {
            this.colors = new Dictionary<Variation, Color>();

            foreach (Variation variation in (Variation[])System.Enum.GetValues(typeof(Variation)))
            {
                this.colors.Add(variation, new Color(0, 0, 0, 0));
            }
        }

        public Dictionary<Variation, Color> GetDictionary() => colors;
        public Rarity GetRarirty() => rarity;

#if UNITY_EDITOR
        [Title("Tooling", "Small tools to help define and set coloring.")]
        [SerializeField] private string hexConversion;

        [SerializeField] private Color colorResult;

        [ButtonGroup("Conversion")]
        [Button("Hex > Color", ButtonSizes.Small)]
        private void ConvertHex()
        {
            if (!hexConversion.Contains("#"))
            {
                hexConversion = $"#{hexConversion}";
            }

            ColorUtility.TryParseHtmlString(hexConversion, out var color);
            colorResult = color;
        }

        [ButtonGroup("Conversion")]
        [Button("Color > Hex", ButtonSizes.Small)]
        private void ConvertColor()
        {
            hexConversion = $"#{ColorUtility.ToHtmlStringRGB(colorResult)}";
        }

        [BoxGroup("Apply"), HideLabel] [EnumToggleButtons] [SerializeField] private Variation applyToEnum;

        [BoxGroup("Apply"), HideLabel]
        [Button("Apply to Variation", ButtonSizes.Large)]
        private void ApplyToEnum()
        {
            if (colors.ContainsKey(applyToEnum))
            {
                colors[applyToEnum] = colorResult;
            }
        }

        private Color GetRarityColor()
        {
            return RarityDB.GetColorThemeByRarity(this.rarity).General;
        }

#endif
    }
}