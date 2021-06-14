using UnityEngine;

namespace BaerAndHoggo.Utilities
{
    public static class ColorHelper
    {
        public static string ToBetterWorseText(this int value, int comparison, string colorGood, string colorBad = "#8A5045", bool moreIsBetter = true)
        {
            if (value == comparison)
                return value.ToString();
            
            return $"<color={(value > comparison ? moreIsBetter ? colorGood : colorBad :  moreIsBetter ? colorBad : colorGood)}>" +
                   $"{value.ToString()}</color>";
            
        }
        
        public static string ToBetterWorseText(this int value, int comparison, bool moreIsBetter = true)
        {
            return value.ToBetterWorseText(comparison, "#688544", "#8A5045", moreIsBetter);
        }
    }
}