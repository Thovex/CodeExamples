using GameDevManager.Employees.Enums;
using Sirenix.OdinInspector;
using UnityEngine;

namespace GameDevManager.Employees {

    [System.Serializable]
    public class EmployeePersonal {
        [TabGroup ("Personal")][SerializeField] private string employeeFirstName;
        public string EmployeeFirstName {
            get { return employeeFirstName; }
        }

        [TabGroup ("Personal")][SerializeField] private string employeeLastName;
        public string EmployeeLastName {
            get { return employeeLastName; }
        }

        [TabGroup ("Personal")][SerializeField] private int employeeAge;
        public int EmployeeAge {
            get { return employeeAge; }
        }

        [TabGroup ("Skills")][EnumToggleButtons][SerializeField] private EEmployeeSkills employeePrimarySkill;
        public EEmployeeSkills EmployeePrimarySkill {
            get { return employeePrimarySkill; }
        }

        [TabGroup ("Skills")][EnumToggleButtons][SerializeField] private EEmployeeSpecialization employeeSpecialization = EEmployeeSpecialization.None;
        public EEmployeeSpecialization EmployeeSpecialSpecialization {
            get { return employeeSpecialization; }
        }
    }
}