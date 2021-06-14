using Sirenix.OdinInspector;
using UnityEngine;
using BaerAndHoggo.Gameplay;
using DG.Tweening;

namespace BaerAndHoggo.UI
{
    public abstract class ManagedMenu : SerializedMonoBehaviour
    {
        [Title("ManagedMenu Screen Base Class")]
        [SerializeField] private Vector3 easeDirection = new Vector3(-1080, 0, 0);
        [SerializeField] private Ease easeTypeIn;
        [SerializeField] private Ease easeTypeOut;
        [SerializeField] private RectTransform menuMoveRectTransform;
        
        public bool moveAwayOnRegister = true;
        private bool _isMoving;

        protected virtual void Awake()
        {
            MenuManager.Instance.RegisterMenu(this);
        }
        
        public virtual bool MoveAway(float duration = 1)
        {
            if (_isMoving) return false;
            _isMoving = true;

            var tween = menuMoveRectTransform.DOAnchorPos(
                easeDirection, duration);
            
            tween.SetEase(easeTypeOut);

            tween.onComplete += () =>
            {
                transform.GetChild(0).gameObject.SetActive(false);
                _isMoving = false;
            };

            return true;
        }
        
        public virtual bool MoveInScreen(float duration = 1)
        {
            if (_isMoving) return false;
            _isMoving = true;

            transform.GetChild(0).gameObject.SetActive(true);

            var tween = menuMoveRectTransform.DOAnchorPos(
                new Vector3(0, 0, 0), duration);

            tween.SetEase(easeTypeIn);

            tween.onComplete += () =>
            {
                _isMoving = false;
            };

            return true;
        }

        public virtual void MenuShake(float duration, float strength, bool doScaleShake = true)
        {
            transform.DOShakePosition(duration, strength);
            
            if (doScaleShake)
                transform.DOShakeScale(duration / 10, .05F);
        }

        public virtual void MenuPunch(float duration, Vector3 strength)
        {
            transform.DOPunchPosition(strength, duration);
        }
    }
}