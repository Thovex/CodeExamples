using UnityEngine;
using Thovex.Types;

namespace GameDevManager.Employees {
    public class EmployeeStats {
        private MinMax<float> statValues = new MinMax<float>(-100, 100);
        public bool CheckValue (int currentValue, int newValue) {
            if (newValue <= statValues.Min && newValue >= statValues.Max) {
                return true;
            }
            return false;
        }
    }
}