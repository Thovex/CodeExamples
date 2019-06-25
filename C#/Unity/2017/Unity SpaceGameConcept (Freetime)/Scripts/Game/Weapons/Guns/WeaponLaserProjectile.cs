using UnityEngine;

namespace Thovex.GameScript {
    public class WeaponLaserProjectile : WeaponBase {

        ObjectPool<LaserProjectile> laserProjectiles;
        public float thisRange;
        public float thisDamage;
        public float thisCooldown;

        public override void Awake() {
            base.Awake();
            cooldownTimer = weaponCooldown;
            laserProjectiles = new ObjectPool<LaserProjectile>(Resources.Load("LaserProjectile"), GameObject.Find("WeaponLaserProjectilePool"), 10, true);
        }

        public override void SetID() {
            id = 0;
        }

        public override void SetDamage() {
            base.SetDamage();
            weaponDamage = thisDamage;
        }

        private void Start() {
            SetID();
            SetDamage();
        }

        private void FixedUpdate() {
            if (cooldownTimer > 0) {
                cooldownTimer -= Time.deltaTime;
            }

            if (cooldownTimer <= 0) {
                Shoot();
                cooldownTimer = thisCooldown;
            }
        }

        public override void FindEnemies() {
            weaponRange = thisRange;
            base.FindEnemies();
        }

        private void Shoot() {
            if (isPlayer) {
                FindEnemies();
                if (shootingTarget != null) {
                    Quaternion targetRotation = Quaternion.Euler(new Vector3(0f, 0f, (Mathf.Atan2(shootingTarget.transform.position.y - transform.position.y, shootingTarget.transform.position.x - transform.position.x) * Mathf.Rad2Deg - 90f)));
                    LaserProjectile l = laserProjectiles.GetObject(transform.position, targetRotation);
                    parent.transform.rotation = targetRotation;
                    l.GetComponent<Rigidbody2D>().velocity = (shootingTarget.transform.position - transform.position) * 2f;
                    l.maxLife = 5f;
                    l.isPlayer = true;
                    l.onDeath += HitSomething;
                } else {
                    parent.transform.rotation = transform.root.rotation;
                }
            }
        }

        private void HitSomething(object sender, object hitInfo) {
            LaserProjectile l = sender as LaserProjectile;
            l.onDeath -= HitSomething;
            laserProjectiles.ReleaseObject(l);
        }
    }
}
