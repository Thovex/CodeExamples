using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace Thovex.GameScript {
    public class ObjectPool<T> where T : Component, IPoolable {
        private Stack<GameObject> stack;
        private Object prefab;
        private int size;

        public ObjectPool(Object prefab, GameObject parent, int size, bool spawnNow = true) {
            this.prefab = prefab;
            this.size = size;
            stack = new Stack<GameObject>(size);

            if (spawnNow) {
                GameObject g;
                for (int i = 0; i < size; ++i) {
                    g = (GameObject)GameObject.Instantiate(prefab);
                    g.transform.parent = parent.transform;
                    stack.Push(g);
                    g.SetActive(false);
                }
            }
        }

        public T GetObject() {
            return GetObject(Vector3.zero, Quaternion.identity);
        }

        public T GetObject(Vector3 position) {
            return GetObject(position, Quaternion.identity);
        }

        public T GetObject(Vector3 position, Quaternion rotation) {
            GameObject g;

            if (stack.Count > 0) {
                g = stack.Pop();
                g.SetActive(true);
            } else {
                //create a new (temporary) when stack is empty, warn
                g = (GameObject)GameObject.Instantiate(prefab);
                Debug.LogWarning("Spawning temporary because pool size is too small " + typeof(T).ToString(), g);
            }

            g.transform.position = position;
            g.transform.rotation = rotation;

            T obj = g.GetComponent<T>();
            return obj;
        }

        public void ReleaseObject(T obj) {
            obj.Recycle();
            GameObject g = obj.gameObject;

            if (stack.Count < size) {
                g.SetActive(false);
                stack.Push(g);
            } else {
                //destroy this (temporary) object
                GameObject.Destroy(g);
            }
        }
    }
}