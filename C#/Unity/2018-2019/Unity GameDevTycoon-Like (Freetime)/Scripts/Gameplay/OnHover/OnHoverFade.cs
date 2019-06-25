using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using Thovex.Types;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace GameDevManager.OnHover {
	[RequireComponent (typeof (Collider2D))][ShowOdinSerializedPropertiesInInspector]
	public class OnHoverFade : SerializedMonoBehaviour {
		[SerializeField] private SpriteRenderer[] checkedElements;

		[ValidateInput ("AlphaValid")][OdinSerialize] private MinMax<float> alphaValues;
		private bool AlphaValid (MinMax<float> value) {
			if (value.Min != value.Max) {
				if (value.Min < value.Max) {
					if (value.Min < byte.MaxValue && value.Max <= byte.MaxValue) {
						return true;
					}
				}
			}
			return false;
		}

		[SerializeField] private int alphaDecreaseValue;
		private int currentAlpha = 255;
		private bool isSelected = false;

		private void OnMouseOver () {
			if (GameManager.Instance != null) {
				if (!GameManager.Instance.MouseInUse) {
					isSelected = true;
				} else {
					isSelected = false;
				}
			}
		}

		private void OnMouseExit () {
			isSelected = false;
		}

		private void FixedUpdate () {
			if (isSelected) {
				if (currentAlpha > alphaValues.Min) {
					currentAlpha -= alphaDecreaseValue;
					SetAlpha ();
				}

			} else {
				if (currentAlpha < alphaValues.Max) {
					currentAlpha += alphaDecreaseValue;
					SetAlpha ();
				}
			}
		}

		private void SetAlpha () {
			for (int i = 0; i < checkedElements.Length; i++) {
				Color32 newColor = checkedElements[i].material.color;
				newColor.a = (byte) currentAlpha;
				checkedElements[i].material.color = newColor;
			}
		}
	}
}