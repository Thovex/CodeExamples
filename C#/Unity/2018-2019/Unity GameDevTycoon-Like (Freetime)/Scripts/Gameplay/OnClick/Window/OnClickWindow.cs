using System.Collections;
using System.Collections.Generic;
using GameDevManager.Windows;
using Sirenix.OdinInspector;
using Thovex.Types;
using UnityEngine;
using UnityEngine.EventSystems;

namespace GameDevManager.OnClick {
    public abstract class OnClickWindow : MonoBehaviour, IPointerDownHandler, IPointerUpHandler {
        [Required]
        [SerializeField] protected MinimizableWindow window;
        protected bool pointerIsDown;
        public abstract void OnPointerDown (PointerEventData eventData);
        public abstract void OnPointerUp (PointerEventData eventData);

    }
}