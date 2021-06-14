using System.Collections.Generic;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.UI;
using BaerAndHoggo.Utilities;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Inventories
{
    public abstract class Inventory<T1, T2, T3> : Singleton<Inventory<T1, T2, T3>>, ISaveData<T3>
    {
        public List<CardInventoryUI> inventoryUi;
        public List<T2> inventory;
        public abstract void LoadInventory(IO.SaveData loadData);
        public abstract void Add(T1 obj, bool forceUpdate = true);
        public abstract void Add(T1[] obj);
        public abstract void Remove(T1 obj, int quantity = 1);
        public abstract T3 OnSave_Implementation();
        public abstract void OnLoad_Implementation(T3 loadData);
        public abstract void RegisterInventoryUI(CardInventoryUI cardInventory);
        protected abstract void UpdateUi();

    }
    
}