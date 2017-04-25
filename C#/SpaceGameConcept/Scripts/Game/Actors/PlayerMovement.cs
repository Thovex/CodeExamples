using UnityEngine;
using System.Collections.Generic;

namespace Thovex.GameScript {
    public class PlayerMovement : MonoBehaviour, IController {

        private Dictionary<string, KeyCode> KeyCodeValues = new Dictionary<string, KeyCode>();

        public void SetKeys() {
            KeyCodeValues.Add("up", KeyCode.UpArrow);
            KeyCodeValues.Add("down", KeyCode.DownArrow);
            KeyCodeValues.Add("left", KeyCode.LeftArrow);
            KeyCodeValues.Add("right", KeyCode.RightArrow);
        }

        private void Awake() {
            SetKeys();
        }

        private void Update() {
            if (Input.GetKey(KeyCodeValues ["up"])) {
                transform.position += Vector3.up / 50f;
            }

            if (Input.GetKey(KeyCodeValues ["down"])) {
                transform.position += Vector3.down / 50f;
            }

            if (Input.GetKey(KeyCodeValues ["left"])) {
                transform.position += Vector3.left / 50f;
            }

            if (Input.GetKey(KeyCodeValues ["right"])) {
                transform.position += Vector3.right / 50f;
            }
        }
    }
}
