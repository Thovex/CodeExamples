using UnityEngine;

namespace Thovex.Types {
    public class MinMax<T> {

        public MinMax (T _minValue, T _maxValue) {
            minValue = _minValue;
            maxValue = _maxValue;
        }

        [SerializeField] private T minValue;
        [SerializeField] private T maxValue;

        public T Min {
            get {
                return minValue;
            }

            set {
                minValue = value;
            }
        }

        public T Max {
            get {
                return maxValue;
            }

            set {
                maxValue = value;
            }
        }
    }
}