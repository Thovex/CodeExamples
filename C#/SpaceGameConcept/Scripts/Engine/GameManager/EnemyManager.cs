using UnityEngine;
using System.Collections.Generic;

namespace Thovex.GameScript {
    public class EnemyManager : MonoBehaviour {

        public TextAsset wd;
        string [] waveData;

        public List<GameObject> enemy;
        public List<GameObject> enemies;

        ObjectPool<Enemy1> enemy1Pool;

        private static EnemyManager instance = null;

        public static EnemyManager Instance {
            get {
                return instance;
            }
        }

        private void Awake() {
            if (instance != null && instance != this) {
                Destroy(gameObject);
                return;
            }
            DontDestroyOnLoad(gameObject);
            instance = this;

            enemy1Pool = new ObjectPool<Enemy1>(Resources.Load("Enemy"), GameObject.Find("Enemy1Pool"), 15, true);

        }

        void Start() {
            GetData();
            SpawnEnemies();
        }

        private void GetData() {
            string waveTemp = wd.text;
            waveData = waveTemp.Split(' ');
        }

        private void Update() {
            if (enemies.Count == 0) {
                SpawnEnemies();
            }
        }

        private void SpawnEnemies() {
            for (int i = 0; i < 10; i++) {
                GameObject daidad = (GameObject)Instantiate(enemy [0], transform.position, transform.rotation);
                enemies.Add(daidad);
            }
        }
    }
}
