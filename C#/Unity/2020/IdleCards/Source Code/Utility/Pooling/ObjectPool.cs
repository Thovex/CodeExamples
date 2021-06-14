using System.Collections.Generic;
using UnityEngine;

namespace BaerAndHoggo.Pooling
{
    public class ObjectPool<T> where T : Component, IPoolable
    {
        private readonly Object _prefab;
        private readonly int _size;
        private readonly Stack<GameObject> _stack;

        public ObjectPool(Object prefab, GameObject parent, int size, bool spawnNow = true)
        {
            _prefab = prefab;
            _size = size;
            _stack = new Stack<GameObject>(size);

            if (spawnNow)
            {
                GameObject g;
                for (var i = 0; i < size; ++i)
                {
                    g = (GameObject) Object.Instantiate(prefab);
                    g.transform.parent = parent.transform;
                    _stack.Push(g);
                    g.SetActive(false);
                }
            }
        }

        public T GetObject()
        {
            return GetObject(Vector3.zero, Quaternion.identity);
        }

        public T GetObject(Vector3 position)
        {
            return GetObject(position, Quaternion.identity);
        }

        public T GetObject(Vector3 position, Quaternion rotation)
        {
            GameObject g;

            if (_stack.Count > 0)
            {
                g = _stack.Pop();
                g.SetActive(true);
            }
            else
            {
                //create a new (temporary) when stack is empty, warn
                g = (GameObject) Object.Instantiate(_prefab);
                Debug.LogWarning("Spawning temporary because pool size is too small " + typeof(T), g);
            }

            g.transform.position = position;
            g.transform.rotation = rotation;

            var obj = g.GetComponent<T>();
            return obj;
        }

        public void ReleaseObject(T obj)
        {
            obj.Recycle();
            var g = obj.gameObject;

            if (_stack.Count < _size)
            {
                g.SetActive(false);
                _stack.Push(g);
            }
            else
            {
                //destroy this (temporary) object
                Object.Destroy(g);
            }
        }
    }
}