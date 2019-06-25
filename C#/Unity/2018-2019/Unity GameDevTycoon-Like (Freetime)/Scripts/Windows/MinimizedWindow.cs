using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;

namespace GameDevManager.Windows {
	public class MinimizedWindow : MonoBehaviour, IPointerClickHandler {
		private MinimizableWindow parentWindow;
		[Required][SerializeField] private TextMeshProUGUI windowTitleText;

		public MinimizableWindow ParentWindow {
			set {
				parentWindow = value;
			}
		}

		private void Start () {
			windowTitleText.text = parentWindow.WindowTitle;
		}

		private void OpenWindow () {
			parentWindow.gameObject.SetActive (true);
			parentWindow.SetMinimizeOpen ();
		}

		public void OnPointerClick (PointerEventData eventData) {
			OpenWindow ();
			gameObject.SetActive (false);

			//TODO: Implement animation
		}
	}
}