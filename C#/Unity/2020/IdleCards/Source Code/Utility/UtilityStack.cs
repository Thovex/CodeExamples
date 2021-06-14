using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace BaerAndHoggo.Utilities
{
    public static partial class Utility 
    {
                
        public static void Shuffle<T>(this Stack<T> stack)
        {
            var rnd = new System.Random();
            
            var values = stack.ToArray();
            stack.Clear();
            
            foreach (var value in values.OrderBy(x => rnd.Next()))
                stack.Push(value);
        }
    }
}