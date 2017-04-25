using System.Collections.Generic;
using UnityEngine;

namespace Thovex.GameScript {
    public class ShipBase : MonoBehaviour {

        public List<GameObject> WeaponSockets;
        public List<GameObject> WingSockets;
        public List<GameObject> EngineSockets;

        public float currentHitpoints;
        public float currentShieldpoints;
        public float currentSpeed;

        public Sprite weaponTurret;
        public Sprite wingSocket;
        public Sprite engineSocket;

        public virtual void Awake() {
            currentHitpoints = 100f;
            currentShieldpoints = 0f;
            currentSpeed = 1f;
        }

        public virtual void SetWeaponSockets() {
            for (int i = 0; i <2; i++) {
                WeaponSockets.Add(new GameObject("WeaponSocket"));
                WeaponSockets [i].AddComponent<SpriteRenderer>().sprite = weaponTurret;

                if (i % 2 == 0) {
                    WeaponSockets [i].transform.position = new Vector3(transform.position.x - 32 / weaponTurret.pixelsPerUnit , transform.position.y, transform.position.z - 1f);
                } else {
                    WeaponSockets [i].transform.position = new Vector3(transform.position.x + 32 / weaponTurret.pixelsPerUnit, transform.position.y, transform.position.z - 1f);
                }

                WeaponSockets [i].transform.parent = transform;
            }
        }

        public virtual void SetWingSockets() {
            for (int i = 0; i < 1; i++) {
                WingSockets.Add(new GameObject("WingSocket"));
                WingSockets[i].AddComponent<SpriteRenderer>().sprite = wingSocket;
                WingSockets[i].transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z + 1f);
                WingSockets[i].transform.parent = transform;
                WingSockets[i].transform.localScale = new Vector3(1f, 1f, 1f);
            }
        }

        public virtual void SetEngineSockets() {
            for (int i = 0; i < 1; i++) {
                EngineSockets.Add(new GameObject("EngineSocket"));
                EngineSockets[i].AddComponent<SpriteRenderer>().sprite = engineSocket;
                EngineSockets[i].transform.position = new Vector3(transform.position.x, transform.position.y - 50 / engineSocket.pixelsPerUnit, transform.position.z + 1f);
                EngineSockets[i].transform.parent = transform;
                EngineSockets[i].transform.localScale = new Vector3(1f, 1f, 1f);
            }
        }

        public virtual void CurrentWings() {
            for (int i = 0; i < WingSockets.Count; i++) {
                GameObject wing = (GameObject)Instantiate(ItemManager.Instance.wings[0], WingSockets[i].transform.position, WingSockets[i].transform.rotation, WingSockets[i].transform);
                wing.name = ItemManager.Instance.wings[0].name;
            }
        }

        public virtual void CurrentEngine() {
            for (int i = 0; i < EngineSockets.Count; i++) {
                GameObject engine = (GameObject)Instantiate(ItemManager.Instance.engines[0], EngineSockets[i].transform.position, EngineSockets[i].transform.rotation, EngineSockets[i].transform);
                engine.name = ItemManager.Instance.engines[0].name;
            }
        }

        public virtual void CurrentWeapon() {
            for (int i = 0; i < WeaponSockets.Count;i++) {
                GameObject weapon = (GameObject)Instantiate(ItemManager.Instance.weapons [0], WeaponSockets [i].transform.position, WeaponSockets[i].transform.rotation, WeaponSockets[i].transform);
                weapon.name = ItemManager.Instance.weapons[0].name;
            }
        }
    }
}
