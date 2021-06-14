using UnityEngine;

namespace Types
{
    public class MinMax<T>
    {
        public MinMax()
        {
            Min = default;
            Max = default;
        }

        public MinMax(T minValue, T maxValue)
        {
            Min = minValue;
            Max = maxValue;
        }

        [field: SerializeField] public T Min { get; set; }

        [field: SerializeField] public T Max { get; set; }

        public override string ToString()
        {
            return $"(Min: {Min}, Max: {Max})";
        }
    }
}