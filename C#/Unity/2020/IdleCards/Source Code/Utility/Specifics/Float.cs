using System;
using System.Linq;

namespace BaerAndHoggo.Utilities
{
    public static partial class Utility
    {
        public static float SetScale(float currentValue, float oldMinScale, float oldMaxScale, float newMinScale,
            float newMaxScale)
        {
            return (currentValue - oldMinScale) * (newMaxScale - newMinScale) / (oldMaxScale - oldMinScale) +
                   newMinScale;
        }

        public static float Max(float x, float y)
        {
            return Math.Max(x, y);
        }

        public static float Max(float x, float y, float z)
        {
            return Math.Max(x, Math.Max(y, z));
        }

        public static float Max(float w, float x, float y, float z)
        {
            return Math.Max(w, Math.Max(x, Math.Max(y, z)));
        }

        public static float Max(params float[] values)
        {
            return values.Max();
        }
    }
}