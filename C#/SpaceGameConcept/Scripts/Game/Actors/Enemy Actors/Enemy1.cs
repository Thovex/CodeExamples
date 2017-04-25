using UnityEngine;
using System.Collections;
using System;

namespace Thovex.GameScript {
    public class Enemy1 : MonoBehaviour, IPoolable {

        public event DataSenderCallback onDeath;


        public void Recycle() {
        }

        // Use this for initialization
        void Start() {

        }

        // Update is called once per frame
        void Update() {

        }
    }
}
