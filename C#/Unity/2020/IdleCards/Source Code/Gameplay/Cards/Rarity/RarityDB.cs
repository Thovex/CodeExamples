using System.Collections;
using Sirenix.OdinInspector;
using BaerAndHoggo.Colors;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public static class RarityDB
    {

        static RarityDB()
        {
            RarityColorData[] colors = Resources.LoadAll<RarityColorData>("Colors/Rarities");

            foreach (RarityColorData colorData in colors)
            {
                GetColorThemeByRarity(colorData.GetRarirty()) = new ColorTheme(colorData.GetDictionary());
            }

            Debug.Log("Created Rarity DB colors");
        }

        public static ref ColorTheme GetColorThemeByRarity(Rarity rarity)
        {
            switch (rarity)
            {
                case Rarity.Common:
                    {
                        return ref COMMON_THEME;
                    }
                case Rarity.Uncommon:
                    {
                        return ref UNCOMMON_THEME;
                    }
                case Rarity.Rare:
                    {
                        return ref RARE_THEME;
                    }
                case Rarity.Epic:
                    {
                        return ref EPIC_THEME;
                    }
                case Rarity.Legendary:
                    {
                        return ref LEGENDARY_THEME;
                    }
                default:
                    {
                        return ref COMMON_THEME;
                    }
            }
        }

        public static ColorTheme COMMON_THEME = new ColorTheme();
        public static ColorTheme UNCOMMON_THEME = new ColorTheme();
        public static ColorTheme RARE_THEME = new ColorTheme();
        public static ColorTheme EPIC_THEME = new ColorTheme();
        public static ColorTheme LEGENDARY_THEME = new ColorTheme();

    }
}
