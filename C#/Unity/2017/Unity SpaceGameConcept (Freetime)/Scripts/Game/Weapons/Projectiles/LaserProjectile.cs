using UnityEngine;
using System.Collections;
using System;

namespace Thovex.GameScript {

    public class LaserProjectile : MonoBehaviour, IPoolable {

        public float maxLife = 5f;
        public bool isPlayer;
        public bool hasCollided;

        protected void Update() {
            maxLife -= Time.deltaTime;
            if (maxLife <= 0) {
                if (onDeath != null) {
                    onDeath(this, null);
                } else {
                    Destroy(gameObject);
                }
            }
            transform.Translate(transform.up * 0.05f, Space.World);

        }

        protected void OnTriggerEnter2D(Collider2D coll) {
            hasCollided = true;

            if (isPlayer) {
                if (coll.tag == "Enemy") {
                    EnemyManager.Instance.enemies.Remove(coll.gameObject);
                    Destroy(coll.gameObject);
                }
            } else {
                if (coll.tag == "Player") {

                }
            }
        }


        public void Recycle() {

        }

        public event DataSenderCallback onDeath;

    }
}
