using System.Collections;
using System.Collections.Generic;
using GameDevManager.Windows;
using Thovex.Types;
using UnityEngine;
using UnityEngine.EventSystems;

namespace GameDevManager.OnClick {
	public class OnClickResizeWindow : OnClickWindow {
		private Vector2 transformInitialSize;
		private Vector2 pointerDownMousePosition;
		public override void OnPointerDown (PointerEventData eventData) {
			pointerIsDown = true;
			window.IsWindowBusy = true;
			transformInitialSize = window.WindowRectTransform.sizeDelta;
			pointerDownMousePosition = Input.mousePosition;

			if (GameManager.Instance != null) {
				GameManager.Instance.SetMouseUsage (true);
			}
		}

		public override void OnPointerUp (PointerEventData eventData) {
			pointerIsDown = false;
			window.IsWindowBusy = false;
			if (GameManager.Instance != null) {
				GameManager.Instance.SetMouseUsage (false);
			}
		}

		private void Update () {
			if (pointerIsDown) {
				ResizeWindow ();
			}
		}

		private void ResizeWindow () {
			Vector2 difference = (pointerDownMousePosition - (Vector2) Input.mousePosition) * Camera.main.aspect;

			difference.x = Mathf.Abs (difference.x - transformInitialSize.x);
			difference.y = difference.y + transformInitialSize.y;

			CheckBounds (ref difference);

			window.SetWindowSize (difference);

		}

		private void CheckBounds (ref Vector2 difference) {
			if (difference.x < window.windowSizeRestrictions.Min.x) {
				difference.x = window.windowSizeRestrictions.Min.x;
			} else if (difference.x > window.windowSizeRestrictions.Max.x) {
				difference.x = window.windowSizeRestrictions.Max.x;
			}

			if (difference.y < window.windowSizeRestrictions.Min.y) {
				difference.y = window.windowSizeRestrictions.Min.y;
			} else if (difference.y > window.windowSizeRestrictions.Max.y) {
				difference.y = window.windowSizeRestrictions.Max.y;
			}
		}
	}
}