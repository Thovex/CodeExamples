using BaerAndHoggo.Colors;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public static class RarityDB
    {
        public static ColorTheme CommonTheme;
        public static ColorTheme UncommonTheme;
        public static ColorTheme RareTheme;
        public static ColorTheme EpicTheme;
        public static ColorTheme LegendaryTheme;

        public static Sprite CommonEmblem;
        public static Sprite UncommonEmblem;
        public static Sprite RareEmblem;
        public static Sprite EpicEmblem;
        public static Sprite LegendaryEmblem;

        static RarityDB()
        {
            var colors = Resources.LoadAll<RarityColorData>("Colors/Rarities");

            foreach (var colorData in colors)
            {
                GetColorThemeByRarity(colorData.GetRarity()) = new ColorTheme(colorData.GetDictionary());
                GetEmblemByRarity(colorData.GetRarity()) = colorData.GetEmblem();
            }

            Debug.Log("Created Rarity DB colors");
        }

        public static ref ColorTheme GetColorThemeByRarity(Rarity rarity)
        {
            switch (rarity)
            {
                case Rarity.Common:
                {
                    return ref CommonTheme;
                }
                case Rarity.Uncommon:
                {
                    return ref UncommonTheme;
                }
                case Rarity.Rare:
                {
                    return ref RareTheme;
                }
                case Rarity.Epic:
                {
                    return ref EpicTheme;
                }
                case Rarity.Legendary:
                {
                    return ref LegendaryTheme;
                }
                default:
                {
                    return ref CommonTheme;
                }
            }
        }

        public static ref Sprite GetEmblemByRarity(Rarity rarity)
        {
            switch (rarity)
            {
                case Rarity.Common:
                {
                    return ref CommonEmblem;
                }
                case Rarity.Uncommon:
                {
                    return ref UncommonEmblem;
                }
                case Rarity.Rare:
                {
                    return ref RareEmblem;
                }
                case Rarity.Epic:
                {
                    return ref EpicEmblem;
                }
                case Rarity.Legendary:
                {
                    return ref LegendaryEmblem;
                }
                default:
                {
                    return ref CommonEmblem;
                }
            }
        }
    }
}