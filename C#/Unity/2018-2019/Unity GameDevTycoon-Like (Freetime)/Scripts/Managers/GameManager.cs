using System.Collections;
using System.Collections.Generic;
using Thovex.Utility;
using UnityEngine;

namespace GameDevManager {
	public class GameManager : Singleton<GameManager> {

		[SerializeField] private bool mouseInUse;

		public bool MouseInUse {
			get {
				return mouseInUse;
			}
		}

		protected override void Awake () {
			base.Awake ();
		}

		protected override void OnDestroy () {
			base.OnDestroy ();
		}

		public void SetMouseUsage(bool newUsage) {
			mouseInUse = newUsage;
		}
	}
}