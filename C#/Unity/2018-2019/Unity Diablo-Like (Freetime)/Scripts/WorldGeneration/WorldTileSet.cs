using UnityEngine;
using System.Collections.Generic;

namespace Thovex.WorldGeneration {

    public enum ETheme { Basic };

    [CreateAssetMenu(menuName = "Thovex/New Tileset")]
    public class WorldTileSet : ScriptableObject {

        [Header("Settings")]
        public Vector3 tileSize;
        public ETheme tileTheme;

        [Header("Prefabs")]
        public List<GameObject> tiles_0_0_0_0;
        public List<GameObject> tiles_100_0_0_0;
        public List<GameObject> tiles_100_100_0_0;
        public List<GameObject> tiles_100_100_100_0;
        public List<GameObject> tiles_100_100_100_100;
    }
}