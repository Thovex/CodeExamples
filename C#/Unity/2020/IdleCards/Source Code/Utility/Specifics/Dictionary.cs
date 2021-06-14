using System.Collections.Generic;

namespace BaerAndHoggo.Utilities
{
    public static partial class Utility
    {
        public static void AddOrUpdate<T1, T2>(Dictionary<T1, T2> dictionary, T1 key, T2 value)
        {
            if (dictionary.ContainsKey(key))
                dictionary[key] = value;
            else
                dictionary.Add(key, value);
        }

        public static T2 TryGet<T1, T2>(Dictionary<T1, T2> dictionary, T1 key, T2 defaultValue)
        {
            if (dictionary.ContainsKey(key))
            {
                dictionary.TryGetValue(key, out var value);
                return value;
            }

            return defaultValue;
        }
    }
}