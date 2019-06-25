using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerNegateRotation : MonoBehaviour {

	private Quaternion startingRotation;

	private void Awake() {
		startingRotation = transform.rotation;
	}

	private void LateUpdate () {
		transform.rotation = startingRotation;
	}
}
