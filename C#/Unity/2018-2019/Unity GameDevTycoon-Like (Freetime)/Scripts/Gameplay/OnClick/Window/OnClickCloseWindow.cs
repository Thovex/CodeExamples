﻿using System.Collections;
using System.Collections.Generic;
using GameDevManager.Windows;
using Thovex.Types;
using UnityEngine;
using UnityEngine.EventSystems;

namespace GameDevManager.OnClick {
	public class OnClickCloseWindow : OnClickWindow {
		public override void OnPointerDown (PointerEventData eventData) {

			if (GameManager.Instance != null) {
				GameManager.Instance.SetMouseUsage (true);
			}
			
			if (!window.IsWindowBusy) {
				window.IsWindowBusy = true;
				window.SetClose ();
			}
		}

		public override void OnPointerUp (PointerEventData eventData) {
			if (GameManager.Instance != null) {
				GameManager.Instance.SetMouseUsage (false);
			}
		}
	}
}