using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using UnityEngine;

namespace GameDevManager.Windows {
	public class WindowContainer : MonoBehaviour {
		[Required][SerializeField] private Window window;
		private Vector2 oldWindowSize;

		private void Start() {
			oldWindowSize = window.WindowRectTransform.sizeDelta;
		}

		private void OnEnable () {
			window.OnWindowResize += AdjustContainer;
		}

		private void OnDisable () {
			window.OnWindowResize -= AdjustContainer;
		}
		
		private void AdjustContainer() {
			if (oldWindowSize != window.WindowRectTransform.sizeDelta) {

			}
		}
	}
}