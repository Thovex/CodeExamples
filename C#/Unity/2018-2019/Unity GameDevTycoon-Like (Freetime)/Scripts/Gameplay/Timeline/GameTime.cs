using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Thovex.Utility;

public class GameTime : MonoBehaviour {

	private long timePerTick = -100;
	
	private DateTime startingDatetime;
	private long currentTicks;
	[SerializeField] private CanvasManager canvasManager;

	private void Start() {
		startingDatetime = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
	}

	private void FixedUpdate () {
		currentTicks += timePerTick;

		if (canvasManager != null) {
			canvasManager.timeWidget.text = startingDatetime.AddMilliseconds(currentTicks).ToString();
		}
	}
}
