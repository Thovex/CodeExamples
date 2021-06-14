using System;
using System.Collections.Generic;
using System.Linq;

namespace BaerAndHoggo.Utilities
{
    public partial class Utility
    {
        public static string ToDebugString<TKey, TValue> (this IDictionary<TKey, TValue> dictionary)
        {
            return $"{Environment.NewLine}{string.Join(Environment.NewLine, dictionary.Select(kv => kv.Key + ": (" + kv.Value.ToString()+")").ToArray())}";
        }
    }
}