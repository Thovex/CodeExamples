using System.Collections.Generic;
using GameDevManager.Employees.Enums;
using UnityEngine;

namespace GameDevManager.Employees {
    [System.Serializable]
    public class EmployeeStatus {
        [Range (0, 100)][SerializeField] private int statusCurrentEnergy = 100;
        [Range (0, 100)][SerializeField] private int statusCurrentMoneySatisfaction = 100;
        [Range (0, 100)][SerializeField] private int statusCurrentPositionSatisfaction = 100;
        [SerializeField] private List<EEmployeeStatusEmotions> statusCurrentEmotions = new List<EEmployeeStatusEmotions> ();

        public int StatusCurrentEnergy {
            get {
                return statusCurrentEnergy;
            }

            set {
                statusCurrentEnergy = value;
            }
        }

        public int StatusCurrentMoneySatisfaction {
            get {
                return statusCurrentMoneySatisfaction;
            }

            set {
                statusCurrentMoneySatisfaction = value;
            }
        }

        public int StatusCurrentPositionSatisfaction {
            get {
                return statusCurrentPositionSatisfaction;
            }

            set {
                statusCurrentPositionSatisfaction = value;
            }
        }

        public List<EEmployeeStatusEmotions> StatusCurrentEmotions {
            get {
                return statusCurrentEmotions;
            }

            set {
                statusCurrentEmotions = value;
            }
        }
    }
}