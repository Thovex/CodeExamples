using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace Thovex.GameScript {
    public abstract class WeaponBase : MonoBehaviour, IWeapon {

        [HideInInspector]
        public float weaponCooldown = 1f;
   
        [HideInInspector]
        public float weaponDamage = 5f;

        [HideInInspector]
        public int id;

        [HideInInspector]
        public bool isPlayer = false;

        [HideInInspector]
        public float cooldownTimer;

        [HideInInspector]
        public List<float> enemyDistance;

        [HideInInspector]
        public GameObject shootingTarget;

        [HideInInspector]
        public GameObject parent;

        [HideInInspector]
        public float weaponRange = Mathf.Infinity;

        public virtual void Awake() {
            if (transform.root.name == "Player") {
                isPlayer = true;
            }

            parent = transform.parent.gameObject;
        }

        private void Update() {

        }

        public virtual void SetDamage() {
            // deprecated
        }

        public virtual void FindEnemies() {
            enemyDistance.Clear();
            for (int i = 0; i < EnemyManager.Instance.enemies.Count; i++) {
                RaycastHit2D hit = Physics2D.Linecast(transform.position, EnemyManager.Instance.enemies [i].transform.position);

                if (hit.collider != null) {
                    enemyDistance.Add(hit.distance);
                }
            }
            if (EnemyManager.Instance.enemies.Count != 0) {
                if (enemyDistance.Min() < weaponRange) { 
                shootingTarget = EnemyManager.Instance.enemies [enemyDistance.IndexOf(enemyDistance.Min())].transform.gameObject;
                } else {
                    shootingTarget = null;
                }
            } else {
                shootingTarget = null;
            }
        }

        public abstract void SetID();

    }
}
