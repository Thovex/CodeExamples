using System.Collections;
using System.Collections.Generic;
using GameDevManager.Employees.Enums;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using UnityEngine;

namespace GameDevManager.Employees {

	[CreateAssetMenu (menuName = "Employees/New Employee")]
	[ShowOdinSerializedPropertiesInInspector]
	public class Employee : SerializedScriptableObject {

		[Header ("Employee Details")]
		public EmployeePersonal employeePersonal;

		[Header ("Employee Skills")]
		public EmployeeSkillsWorker employeeSkillsWorker;
		public EmployeeSkillsExtra employeeSkillsExtra;

		[InlineProperty][OdinSerialize] private Dictionary<EEmployeeLookingDirection, Sprite> employeeSprite = new Dictionary<EEmployeeLookingDirection, Sprite> ();

		[Header ("Current Status")]
		public EmployeeStatus employeeStatus;

		public Dictionary<EEmployeeLookingDirection, Sprite> EmployeeSprite {
			get {
				return employeeSprite;
			}
		}
	}
}