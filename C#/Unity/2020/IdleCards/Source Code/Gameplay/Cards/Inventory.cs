using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using BaerAndHoggo.Gameplay.Cards;
using Sirenix.OdinInspector;
using BaerAndHoggo.Utilities;

namespace BaerAndHoggo.Gameplay.Inventories
{
    public abstract class Inventory<T1, T2, T3> : Singleton<Inventory<T1, T2, T3>>
    {
        [SerializeField] public List<T2> inventory;
        public abstract void LoadInventory(T3[] loadData);
        public abstract void Add(T1 obj, bool forceUpdate = true);
        public abstract void Add(T1[] obj);
        public abstract void Remove(T1 obj, int quantity = 1);

    }
}
