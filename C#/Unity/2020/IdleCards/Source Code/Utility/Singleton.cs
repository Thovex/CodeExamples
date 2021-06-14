using Sirenix.OdinInspector;
using UnityEngine;

namespace BaerAndHoggo.Utilities
{
    public class Singleton<T> : SerializedMonoBehaviour where T : SerializedMonoBehaviour
    {
        private static bool _mShuttingDown;
        private static readonly object MLock = new object();
        private static T _mInstance;

        public static T Instance => GetInstance();

        public static T GetInstance()
        {
            if (_mShuttingDown)
            {
                Debug.LogWarning("[Singleton] Instance '" + typeof(T) +
                                 "' already destroyed. Returning null.");
                return null;
            }

            lock (MLock)
            {
                if (_mInstance == null)
                {
                    // Search for existing instance.
                    _mInstance = (T) FindObjectOfType(typeof(T));

                    // Create new instance if one doesn't already exist.
                    if (_mInstance == null)
                    {
                        // Need to create a new GameObject to attach the singleton to.
                        var singletonObject = new GameObject();
                        _mInstance = singletonObject.AddComponent<T>();
                        singletonObject.name = typeof(T) + " (Singleton)";

                        // Make instance persistent.
                        DontDestroyOnLoad(singletonObject);
                    }
                }

                return _mInstance;
            }
        }


        private void OnApplicationQuit()
        {
            _mShuttingDown = true;
        }


        private void OnDestroy()
        {
            _mShuttingDown = true;
        }
    }
}