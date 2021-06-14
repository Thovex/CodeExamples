using System;
using System.Collections.Generic;
using System.Linq;

namespace BaerAndHoggo.Utilities
{
    public static partial class Utility
    {
        public static T RandomElement<T>(this IEnumerable<T> enumerable)
        {
            return enumerable.RandomElementUsing(new Random(UnityEngine.Random.Range(int.MinValue, int.MaxValue)));
        }

        public static T RandomElementUsing<T>(this IEnumerable<T> enumerable, Random rand)
        {
            return enumerable.ElementAt(rand.Next(0, enumerable.Count()));
        }
    }
}