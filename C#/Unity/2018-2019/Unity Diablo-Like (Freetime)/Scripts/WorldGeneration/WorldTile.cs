using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Thovex.WorldGeneration {	
	public class WorldTile : MonoBehaviour {
		[SerializeField] private List<GameObject> childTileCollisions = new List<GameObject>();
		private List<WorldTileCollisionChecker> childTileCollisionChecks = new List<WorldTileCollisionChecker>();

		private WorldGenerator worldGenerator;

        public WorldGenerator WorldGenerator {
            get {
                return worldGenerator;
            } set {
                worldGenerator = value;
            }
        }

        private void Start() {
            StartCoroutine(CreateCollisionChecks());
        }

        private IEnumerator CreateCollisionChecks() {
            for (int i = 0; i < transform.childCount; i++) {
                if (transform.GetChild(i).tag == "TileSide") {
                    childTileCollisions.Add(transform.GetChild(i).gameObject);
                }
            }

            foreach (GameObject childTileCollision in childTileCollisions) {
                childTileCollisionChecks.Add(childTileCollision.AddComponent<WorldTileCollisionChecker>());
            }

			bool isFinished = false;

			while (!isFinished) {
				int countFinished = 0;

				foreach (WorldTileCollisionChecker childTileCollisionCheck in childTileCollisionChecks) {
					if (childTileCollisionCheck.finished) {
						countFinished++;
					}
				}

				if (countFinished == childTileCollisionChecks.Count) {
					isFinished = true;
				} else {
					yield return new WaitForEndOfFrame();
				}
			}

			SelectNewPosition();
        }

        private void SelectNewPosition() {
			List<int> indexesFree = new List<int>();

			foreach (WorldTileCollisionChecker collisionCheck in childTileCollisionChecks) {
				for (int i = 0; i < collisionCheck.sidesFree.Length; i++) {
					if (collisionCheck.sidesFree[i]) {
						indexesFree.Add(i);
					}
				}
			}
			
			if (indexesFree.Count > 0) {
				int index = indexesFree[Random.Range(0, indexesFree.Count)];
				Vector3 spawnLocation = transform.position;

				switch (index) {
					case 0: { spawnLocation += Vector3.forward * worldGenerator.tileSet.tileSize.z; } break;
					case 1: { spawnLocation += Vector3.back * worldGenerator.tileSet.tileSize.z; } break;
					case 2: { spawnLocation += Vector3.right * worldGenerator.tileSet.tileSize.x; } break;
					case 3: { spawnLocation += Vector3.left * worldGenerator.tileSet.tileSize.x; } break;
				}
				
				foreach (WorldTileCollisionChecker collisionCheck in childTileCollisionChecks) {
					//Destroy(collisionCheck);
				}

				worldGenerator.TileFinish(true, spawnLocation);
			} else {
				Debug.Log("Nowhere to go...");
				worldGenerator.TileFinish(false, transform.position);
			}
		}
	}
}
