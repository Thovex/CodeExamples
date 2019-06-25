using UnityEngine;

namespace Thovex.WorldGeneration {
    public class WorldTileData {
        public WorldTileData(GameObject game, WorldTile tile) {
            tileGameObject = game;
            tileComponent = tile;
        }

        public GameObject tileGameObject;
        public WorldTile tileComponent;
    }
}