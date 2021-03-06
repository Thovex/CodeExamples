using System;
using System.Collections.Generic;
using UnityEngine;

namespace BaerAndHoggo.Colors
{
    [Serializable]
    public struct ColorTheme
    {
        public enum Variation
        {
            Light,
            General,
            Dark
        }

        [SerializeField] private Dictionary<Variation, Color> colorDictionary;

        public ColorTheme(Dictionary<Variation, Color> colorDictionary)
        {
            this.colorDictionary = colorDictionary;
        }

        public ColorTheme(Color[] colors)
        {
            colorDictionary = new Dictionary<Variation, Color>();

            var variations = (Variation[]) Enum.GetValues(typeof(Variation));

            for (var i = 0; i < variations.Length; i++)
            {
                var color = colors.Length <= i ? new Color() : colors[i];
                colorDictionary.Add(variations[i], color);
            }
        }

        public Color Light => ColorFromVariation(Variation.Light);
        public Color General => ColorFromVariation(Variation.General);
        public Color Dark => ColorFromVariation(Variation.Dark);

        public Dictionary<Variation, Color> ColorDictionary
        {
            get => colorDictionary;
            set => colorDictionary = value;
        }

        public Color ColorFromVariation(Variation variation)
        {
            if (colorDictionary == null) return new Color();
            return colorDictionary.TryGetValue(variation, out var color) ? color : new Color();
        }

        public string HexFromVariation(Variation variation)
        {
            return ColorUtility.ToHtmlStringRGB(ColorFromVariation(variation));
        }
    }
}