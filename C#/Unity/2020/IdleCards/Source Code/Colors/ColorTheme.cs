using System.Collections.Generic;
using UnityEngine;


namespace BaerAndHoggo.Colors
{

    [System.Serializable]
    public struct ColorTheme
    {
        public enum Variation { Light, General, Dark };

        [SerializeField] private Dictionary<Variation, Color> colorDictionary;
        public ColorTheme(Dictionary<Variation, Color> colorDictionary)
        {
            this.colorDictionary = colorDictionary;
        }

        public ColorTheme(Color[] colors)
        {
            this.colorDictionary = new Dictionary<Variation, Color>();

            Variation[] variations = (Variation[])Variation.GetValues(typeof(Variation));

            for (int i = 0; i < variations.Length; i++)
            {
                Color color = colors.Length <= i ? new Color() : colors[i];
                colorDictionary.Add(variations[i], color);
            }
        }

        public Color Light => ColorFromVariation(Variation.Light);
        public Color General => ColorFromVariation(Variation.General);
        public Color Dark => ColorFromVariation(Variation.Dark);
        public Dictionary<Variation, Color> ColorDictionary { get => colorDictionary; set => colorDictionary = value; }
        public Color ColorFromVariation(Variation variation)
        {
            if (colorDictionary != null)
            {
                if (colorDictionary.TryGetValue(variation, out var color))
                {
                    return color;
                }
            }

            return new Color();
        }
    }
}
