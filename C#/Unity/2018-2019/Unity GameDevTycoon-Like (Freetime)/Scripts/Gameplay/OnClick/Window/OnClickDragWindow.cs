using System;
using GameDevManager.Windows;
using Thovex.Types;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace GameDevManager.OnClick {
	public class OnClickDragWindow : OnClickWindow, IDragHandler {

		private CanvasScaler canvasScaler;

		private void Start () {
			canvasScaler = transform.root.GetComponent<CanvasScaler> ();
		}
		public void OnDrag (PointerEventData eventData) {
			Vector2 delta = eventData.delta;

			if (canvasScaler != null) {
				Vector2 referenceResolution = canvasScaler.referenceResolution;
				Vector2 currentResolution = new Vector2 (Screen.width, Screen.height);

				float widthRatio = currentResolution.x / referenceResolution.x;
				float heightRatio = currentResolution.y / referenceResolution.y;
				float ratio = Mathf.Lerp (widthRatio, heightRatio, canvasScaler.matchWidthOrHeight);

				delta = eventData.delta / ratio;
			}

			window.WindowRectTransform.anchoredPosition += delta;
		}

		public override void OnPointerDown (PointerEventData eventData) {
			if (GameManager.Instance != null) {
				GameManager.Instance.SetMouseUsage (true);
			}
			window.transform.SetSiblingIndex (window.transform.parent.childCount);
		}

		public override void OnPointerUp (PointerEventData eventData) {
			if (GameManager.Instance != null) {
				GameManager.Instance.SetMouseUsage (false);
			}
		}
	}
}