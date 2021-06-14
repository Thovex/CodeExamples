using System.Linq;

namespace BaerAndHoggo.Utilities
{
    public partial class Utility
    {
        public static string ToDebugString<T>(this T[] array)
        {
            if (array == null) return "Empty Array";
            if (array.Length == 0) return "Empty Array";
            
            var debugString = "";
            
            for (int i = 0; i < array.Length; i++)
            {
                debugString += $"{i}: ({array[i].ToString()})";
            }

            return debugString;
        }
    }
}