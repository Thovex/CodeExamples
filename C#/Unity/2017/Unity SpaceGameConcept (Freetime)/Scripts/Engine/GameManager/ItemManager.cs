using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ItemManager : MonoBehaviour {

    public List<GameObject> weapons;
    public float [] damageValues;

    public List<GameObject> wings;
    public List<GameObject> engines;

    private static ItemManager instance = null;

    public static ItemManager Instance {
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
    }
}
