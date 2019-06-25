using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Thovex.Gameplay {
	public class PlayerCommands : MonoBehaviour {
		[SerializeField] private PlayerBase playerBase;

		private void Awake() {
			if (playerBase == null) {
				playerBase = transform.parent.GetComponent<PlayerBase>();
			}
		}
	}
}