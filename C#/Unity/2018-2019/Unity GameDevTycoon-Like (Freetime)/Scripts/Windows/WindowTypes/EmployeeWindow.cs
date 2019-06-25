using System;
using GameDevManager.Employees;
using TMPro;
using UnityEngine;

namespace GameDevManager.Windows {
    public class EmployeeWindow : MinimizableWindow {
        private EmployeeSocket employeeSocket;

        [Header ("Employee Information References")]
        [SerializeField] private TextMeshPro employeeNameText;
        [SerializeField] private TextMeshPro employeePrimarySkillText;
        [SerializeField] private TextMeshPro employeeSpecializationText;

        public EmployeeSocket EmployeeSocket {
            get {
                return employeeSocket;
            }

            set {
                employeeSocket = value;
            }
        }

        protected override void SetWindowName () {
            base.SetWindowName();
            
            if (employeeSocket != null) {
                windowTitle = employeeSocket.AssignedEmployee.employeePersonal.EmployeeFirstName;
            }
        }
    }
}