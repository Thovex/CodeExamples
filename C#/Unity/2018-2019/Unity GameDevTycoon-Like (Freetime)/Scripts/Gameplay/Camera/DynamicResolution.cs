using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DynamicResolution : MonoBehaviour {
	[SerializeField] private Camera mainCamera;
	[SerializeField] private float[] orthographicSizeStates = new float[3] { 25, 35, 45 };
	private Vector2 screenResolution;
	private bool isUpdated;
	private float orthographicSizeLerpTowards;

	private void Start () {
		orthographicSizeLerpTowards = orthographicSizeStates[0];
		screenResolution = SetScreenResolution ();
		CheckWindowNewState ();
	}

	private void Update () {
		CheckWindowSizeChange ();
	}

	private void CheckWindowSizeChange () {
		if (screenResolution.x != Screen.width || screenResolution.y != Screen.height) {
			Reset ();
			CheckWindowNewState ();
		}

		if (isUpdated) {
			LerpOrthographicSize ();
		}
	}

	private void Reset () {
		isUpdated = false;
		screenResolution = SetScreenResolution ();
	}

	private void CheckWindowNewState () {
		if (mainCamera.orthographicSize > orthographicSizeStates[0] && mainCamera.aspect >= 1.5f) {
			orthographicSizeLerpTowards = orthographicSizeStates[0];
		}

		if ((mainCamera.orthographicSize < orthographicSizeStates[1] || mainCamera.orthographicSize > orthographicSizeStates[1]) && (mainCamera.aspect < 1.5f && mainCamera.aspect > 1.0f)) {
			orthographicSizeLerpTowards = orthographicSizeStates[1];
		}
		
		if (mainCamera.orthographicSize < orthographicSizeStates[2] && mainCamera.aspect <= 1.0f) {
			orthographicSizeLerpTowards = orthographicSizeStates[2];
		}

		isUpdated = true;
	}

	private void LerpOrthographicSize () {
		mainCamera.orthographicSize = Mathf.Lerp (mainCamera.orthographicSize, orthographicSizeLerpTowards, Time.deltaTime * 3);
		if (Mathf.Abs (mainCamera.orthographicSize - orthographicSizeLerpTowards) < 0.05f) {
			mainCamera.orthographicSize = orthographicSizeLerpTowards;
			isUpdated = false;
		}
	}

	private static Vector2 SetScreenResolution () {
		return new Vector2 (Screen.width, Screen.height);
	}
}