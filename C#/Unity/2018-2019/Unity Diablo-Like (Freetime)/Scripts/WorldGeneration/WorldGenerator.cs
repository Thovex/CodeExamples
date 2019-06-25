using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

namespace Thovex.WorldGeneration {
	public class WorldGenerator : MonoBehaviour {
		public WorldTileSet tileSet;
		private List<WorldTile> tileList = new List<WorldTile>();
		private List<NavMeshSurface> tileSurfaces = new List<NavMeshSurface>();

        public List<NavMeshSurface> TileSurfaces {
            get {
                return tileSurfaces;
            } set {
                tileSurfaces = value;
            }
        }

        private void Start() {
			PlaceTile(Vector3.zero);
        }

		public WorldTileData PlaceTile(Vector3 position) {
			GameObject baseTile = Instantiate(tileSet.tiles_0_0_0_0[0], position, Quaternion.identity) as GameObject;
			WorldTile tileComp = baseTile.GetComponent<WorldTile>();

			tileSurfaces.Add(baseTile.GetComponent<NavMeshSurface>());
			
			tileList.Add(tileComp);
			tileComp.WorldGenerator = this;
			
			return new WorldTileData(baseTile, tileComp);
		}

		public void GenerateNavMesh() {
			for (int i = 0; i < tileSurfaces.Count; i++) {
				tileSurfaces[i].BuildNavMesh();
			}
		}

        internal void TileFinish(bool pathsAvailable, Vector3 spawnPos) {
            if (pathsAvailable) {
				PlaceTile(spawnPos);
			} else {
				GenerateNavMesh();
			}
        }
    }
}
