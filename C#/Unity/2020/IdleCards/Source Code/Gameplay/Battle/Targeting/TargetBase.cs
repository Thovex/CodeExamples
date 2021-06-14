using System.ComponentModel;
using BaerAndHoggo.UI;
using DG.Tweening;
using UnityEngine;
using UnityEngine.EventSystems;

namespace BaerAndHoggo.Gameplay.Battle.Targeting
{

    [System.Serializable]
    public class TargetData
    {
        public RectTransform DragTransform { get; set; }
        public MasterCardUI MasterCardUI { get; set; }
    }
    
    public abstract class TargetBase<T> : MonoBehaviour, IBeginDragHandler, IDragHandler, IEndDragHandler
    where T: TargetData
    {
        [SerializeField] protected T Data;
        
        protected bool IsInitialized = false;
        
        protected bool IsBusy = false;
        protected bool IsDragging = false;
        
        public delegate void PlayCard();
        public event PlayCard OnPlay;

        protected virtual void Awake()
        {
            if (Data == null) throw new System.Exception(
                "No data initialized. Please override Awake from TargetBase and add base declaration to the end. ");
            
            Data.DragTransform = this.GetComponent<RectTransform>();
            Data.MasterCardUI = this.GetComponent<MasterCardUI>();
        }

        public virtual void Initialize(T data)
        {
            IsInitialized = true;
        }
        
        private bool InitializedCheck()
        {
            if (!IsInitialized)
            {
                throw new System.Exception($"{nameof(TargetBase<T>)} not initialized");
            }
            return true;
        }


        protected abstract bool Checks_Implementation(bool beginPlay = false);
        protected abstract void OnBeginDrag_Implementation(PointerEventData eventData);
        protected abstract void OnDrag_Implementation(PointerEventData eventData);
        protected abstract void OnEndDrag_Implementation(PointerEventData eventData);

        
        public virtual void OnBeginDrag(PointerEventData eventData)
        {
            if (!InitializedCheck()) return;
            if (!Checks_Implementation(true))
            {
                ShakeCard();
                return;
            }

            IsDragging = true;
            
            OnBeginDrag_Implementation(eventData);
        }

        public virtual void OnDrag(PointerEventData eventData)
        {
            if (!InitializedCheck()) return;
            if (!Checks_Implementation()) return;
            
            OnDrag_Implementation(eventData);
        }

        public virtual void OnEndDrag(PointerEventData eventData)
        {
            IsDragging = false;
            
            if (!InitializedCheck()) return;
            if (!Checks_Implementation()) return;
            
            OnEndDrag_Implementation(eventData);
        }

        protected void ShakeCard()
        {
            var tweenCard = Data.DragTransform.DOShakeRotation(.5F, 10F);
            tweenCard.onComplete += () =>
            {
                Data.DragTransform.localEulerAngles = Vector3.zero;
            };
        }

        protected virtual void CallOnPlay()
        {
            OnPlay?.Invoke();
        }
    }
}