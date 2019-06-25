using System.Collections;
using System.Collections.Generic;
using GameDevManager.Employees.Enums;
using GameDevManager.Windows;
using Thovex.Utility;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace GameDevManager.Employees {
	public class EmployeeSocket : MonoBehaviour {
		[Header ("Employee Object")]
		[SerializeField] private Employee assignedEmployee;

		[Header ("Employee Information References")]
		[SerializeField] private SpriteRenderer employeeSprite;
		[SerializeField] private EEmployeeLookingDirection employeeLookingDirection;

		[Header ("Employee Window References")]
		[SerializeField] private GameObject employeeWindowPrefab;
		private GameObject employeeWindow;

		public Employee AssignedEmployee {
			get {
				return assignedEmployee;
			}

			set {
				assignedEmployee = value;
			}
		}

		private void Start () {
			SetEmployee (assignedEmployee);
		}

		public void SetEmployee (Employee newEmployee) {
			if (newEmployee != null) {
				assignedEmployee = newEmployee;
				ChangedEmployee ();
			}
		}

		private void ChangedEmployee () {
			if (assignedEmployee != null) {
				Sprite selectedSprite;

				assignedEmployee.EmployeeSprite.TryGetValue (employeeLookingDirection, out selectedSprite);

				SetEmployeeValues (
					assignedEmployee.employeePersonal.EmployeeFirstName + " " + assignedEmployee.employeePersonal.EmployeeLastName,
					GetEnumDescription.GetDescription (assignedEmployee.employeePersonal.EmployeePrimarySkill),
					GetEnumDescription.GetDescription (assignedEmployee.employeePersonal.EmployeeSpecialSpecialization),
					selectedSprite
				);

			} else {
				SetEmployeeValues ("Available", "", "", null);
			}
		}

		private void OnMouseDown () {
			if (!GameManager.Instance.MouseInUse) {
				EmployeeWindow employeeWindow = EmployeeWindowCheck ();
				employeeWindow.gameObject.SetActive (true);

				if (employeeWindow.IsMinimized) {
					employeeWindow.SetMinimizeOpen ();
				}

				if (employeeWindow.IsClosed) {
					employeeWindow.SetCloseOpen ();
				}
			}
		}

		private EmployeeWindow EmployeeWindowCheck () {
			if (employeeWindow == null) {
				employeeWindow = Instantiate (
					employeeWindowPrefab,
					employeeWindowPrefab.transform.position,
					employeeWindowPrefab.transform.rotation
				);

				employeeWindow.GetComponent<EmployeeWindow> ().EmployeeSocket = this;
			}

			EmployeeWindow instancedWindow = employeeWindow.GetComponent<EmployeeWindow> ();
			return instancedWindow;
		}

		private void SetEmployeeValues (string nameText, string primaryText, string specializationText, Sprite selectedImage) {
			employeeSprite.sprite = selectedImage;

			if (selectedImage == null) {
				employeeSprite.color = new Color (0, 0, 0, 0);
			} else {
				employeeSprite.color = new Color (1, 1, 1, 1);
			}
		}

		private void OnDestroy () {
			Destroy (employeeWindow);
		}
	}
}