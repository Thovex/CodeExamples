using UnityEngine;

namespace BaerAndHoggo.Utilities
{
    public static partial class Utility
    {
        public static Color ToColor(int hexVal)
        {
            var r = (byte) ((hexVal >> 16) & 0xFF);
            var g = (byte) ((hexVal >> 8) & 0xFF);
            var b = (byte) (hexVal & 0xFF);
            return new Color(r, g, b, 255);
        }
    }
}