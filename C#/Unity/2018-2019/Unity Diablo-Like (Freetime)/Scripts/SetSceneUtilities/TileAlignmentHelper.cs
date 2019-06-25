using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class TileAlignmentHelper : MonoBehaviour {

	private void OnDrawGizmos() {
		Gizmos.DrawLine(transform.position, transform.forward * 250 + transform.position);

		Gizmos.color = Color.green;
		for (int i = 0; i < 250; i++) {
			if (i % 10 == 0) {
				Vector3 nextSectionVerticalPos = transform.position + transform.forward * i;
				Gizmos.DrawLine(nextSectionVerticalPos, transform.right * 250 + nextSectionVerticalPos);

				Vector3 nextSectionHorizontalPos = transform.position + transform.right * i;
				Gizmos.DrawLine(nextSectionHorizontalPos, transform.forward * 250 + nextSectionHorizontalPos);
            }
        }
		Gizmos.color = Color.white;
	}

	private void DrawTileLabel(Vector3 position, int spaceUsedLeft, int spaceUsedTop, int spaceUsedRight, int spaceUsedBot) {
		Handles.Label(position, spaceUsedLeft.ToString() + "% - " + spaceUsedTop.ToString() + "% - " + spaceUsedRight.ToString() + "% - " + spaceUsedBot.ToString() + "%");
	}
}
