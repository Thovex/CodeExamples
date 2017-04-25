
using UnityEngine;

namespace Thovex.GameScript {
    public class Player : ShipBase, IShipStatus {

        public override void Awake() {
            base.Awake();
            base.SetWeaponSockets();
            base.SetWingSockets();
            base.SetEngineSockets();
            CurrentWeapon();
            CurrentWings();
            CurrentEngine();
        }

        public void TakeDamage(float damageAmount) {
            if (CheckShield()) { 
                currentHitpoints -= damageAmount;
            } else {
                currentShieldpoints -= damageAmount;
            }
        }

        public bool CheckShield() {
            if (currentShieldpoints > 0) {
                return true;
            } else {
                return false;
            }
        }

        public override void CurrentEngine() {
            base.CurrentEngine();
        }


        public override void CurrentWings() {
            base.CurrentWings();
        }

        public override void CurrentWeapon() {
            base.CurrentWeapon();
        }
    }
}
