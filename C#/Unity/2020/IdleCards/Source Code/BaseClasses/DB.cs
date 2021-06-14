using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.IO;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;
using UnityEngine;

namespace BaerAndHoggo.Gameplay
{
    public abstract class DB<T1, T2> : Singleton<DB<T1, T2>>, ISaveData<T2>
        where T1 : UnityEngine.Object, IIdentifiable
    {
        [SerializeField] protected string resourcePath; 
        [SerializeField] protected Dictionary<long, T1> db = new Dictionary<long, T1>();
        [SerializeField] [ReadOnly] protected Dictionary<long, T1> dbBase = new Dictionary<long, T1>();
        public new static DB<T1, T2> Instance => GetInstance();

        public int GetCount => db.Count;
        protected abstract void InitializeDB();
        protected bool IsInitialized = false;

        public T1[] GetAll()
        {
            return db.Values.ToArray();
        }

        protected virtual void Awake()
        {
            
            SaveManager.Instance.OnLoad += StartLoad;
            
            // Grab resources from Path
            var resources = Resources.LoadAll<T1>(resourcePath);

            foreach (var resource in resources)
            {
                // Add an instantiated clone from the existing building. 
                // We do not ever want to make changes on the files by programming here.
                db.Add(resource.GetID(), Instantiate(resource));
                dbBase.Add(resource.GetID(), Instantiate(resource));

            }
        }

        protected abstract void StartLoad(IO.SaveData loadData);
        public abstract T2 OnSave_Implementation();
        public abstract void OnLoad_Implementation(T2 loadData);

        protected virtual void Start()
        {
            this.InitializeDB();
        }
        
        public virtual bool GetItem(int index, out T1 item)
        {
            item = null;

            // If DB contains element at this index.
            if (!db.ElementAt(index).Value) return false;
            
            // Grab a ref from this building to give to the player.
            item = db.ElementAt(index).Value;
            return true;
        }
        
        public long GetIdFromIndex(int index)
        {
            //Debug.LogError($"GetIdFromIndex Failed! No such Id found for Name {Name}");
            return db.ElementAt(index).Key;
        }

        public int GetIndexFromId(long id)
        {
            for (var i = 0; i < GetCount; i++)
                if (db.ElementAt(i).Key == id)
                    return i;
            
            Debug.LogError($"GetIndexFromId Failed! No such Index found for Id {id}");
            return -1;
        }
        
        public long GetIdFromName(string name)
        {
            foreach (var item in db.Where(item => item.Value.GetName() == name))
                return item.Value.GetID();

            Debug.LogError($"GetIdFromName Failed! No such Id found for Name {name}");
            return -1;
        }

        public string GetNameFromId(long id)
        {
            foreach (var card in db.Where(card => card.Value.GetID() == id))
                return card.Value.GetName();

            Debug.LogError($"GetNameFromId Failed! No such Name found for Id {id}");
            return "NULL";
        }
        
        public bool GetBaseItem(long id, out T1 item)
        {
            item = null;

            if (!dbBase.TryGetValue(id, out var itemBase))
                throw new Exception($"Invalid building requested of ID {id}. But not available in BuildingDBBase.");
            
            item = itemBase;
            return true;
        }

        public virtual T1 GetItemById(long id)
        {
            return db.Where(item => id == item.Value.GetID()).Select(item => item.Value).FirstOrDefault();
        }
    }
}