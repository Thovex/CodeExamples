using UnityEngine;

namespace Thovex.WorldGeneration {
    public class WorldTileCollisionChecker : MonoBehaviour {
        public int percentageFree = 0;

        public bool[] sidesFree = new bool[4];
        public bool finished = false;

        private void Update() {
            CollisionCheck();
        }

        private void CollisionCheck() {
            for (float i = -5 ;i <= 5; i++) {
                if (transform.localScale.z > 1 && transform.localPosition.x < 0) {
                    Vector3 thisPosition = new Vector3(transform.position.x + .1f, transform.position.y, transform.position.z - (i + -1 * .1f));

                    RaycastHit hitInfo;

                    if (Physics.Raycast(thisPosition, -transform.right, out hitInfo, 10)) {
                        Debug.DrawRay(thisPosition, -transform.right, Color.cyan);
                        sidesFree[0] = false;
                    } else {
                        Debug.DrawRay(thisPosition, -transform.right, Color.green);
                        sidesFree[0] = true;
                    }
                    // Left Side
                }

                if (transform.localScale.x > 1 && transform.localPosition.z > 0) {
                     // Front
                    Vector3 thisPosition = new Vector3((transform.position.x - (i + -i *.1f)), transform.position.y, transform.position.z -.1f);

                    RaycastHit hitInfo;

                    if (Physics.Raycast(thisPosition, transform.forward, out hitInfo, 10)) {
                        Debug.DrawRay(thisPosition, transform.forward, Color.red);
                        sidesFree[1] = false;
                    } else {
                        Debug.DrawRay(thisPosition, transform.forward, Color.green);
                        sidesFree[1] = true;
                    }
                }

                if (transform.localScale.z > 1 && transform.localPosition.x > 0) {
                    Vector3 thisPosition = new Vector3(transform.position.x -.1f, transform.position.y, transform.position.z - (i + -1 * .1f));

                    RaycastHit hitInfo;

                    if (Physics.Raycast(thisPosition, transform.right, out hitInfo, 10)) {
                        Debug.DrawRay(thisPosition, transform.right, Color.red);
                        sidesFree[2] = false;
                    } else {
                        Debug.DrawRay(thisPosition, transform.right, Color.green);
                        sidesFree[2] = true;
                    }
                    // Right Side
                }

                if (transform.localScale.x > 1 && transform.localPosition.z < 0) {
                    Vector3 thisPosition = new Vector3(transform.position.x - (i + -1 * .1f), transform.position.y, transform.position.z + .1f);
                    
                    RaycastHit hitInfo;

                    if (Physics.Raycast(thisPosition, -transform.forward, out hitInfo, 10)) {
                        Debug.DrawRay(thisPosition, -transform.forward, Color.red);
                        sidesFree[3] = false;
                    } else {
                        Debug.DrawRay(thisPosition, -transform.forward, Color.green);
                        sidesFree[3] = true;
                    }

                    // Behind
                }
            }

            finished = true;
        }
    }
}