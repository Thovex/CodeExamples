using System;
using BaerAndHoggo.Gameplay;
using DG.Tweening;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;

namespace BaerAndHoggo.UI
{
    public abstract class UpdateAbleData
    {
        public long Id { get; set; }
        public string Name { get; set; }
    }

    [Serializable]
    public abstract class UpdateAbleReferences<T> where T: UpdateAbleData
    {
        [Title("References", "Base | Text")]
        [SerializeField] protected TMP_Text nameRef;
        public abstract void Update(T data);
    }

    public abstract class UpdateAbleScreenUI<T1, T2, T3, T4, T5> : ManagedMenu
        where T1 : DB<T2, T5>
        where T2 : UnityEngine.Object, IIdentifiable
        where T3 : UpdateAbleReferences<T4>
        where T4 : UpdateAbleData
    {
        [SerializeField] protected T2 item;
        [SerializeField] protected T3 references;



        protected T4 data;

        public void SetItemFromIndex(int index)
        {
            if (DB<T2, T5>.Instance.GetItem(index, out var item))
            {
                this.item = item;
            }
        }

        public virtual void SetItemFromId(long id)=>SetItemFromIndex(
            DB<T2, T5>.Instance.GetIndexFromId(id));
        
        public abstract void InitializeReferences();
        

    }
}