using Sirenix.OdinInspector;
using Sirenix.Serialization;
using Thovex.Types;
using TMPro;
using UnityEngine;

namespace GameDevManager.Windows {
    [ShowOdinSerializedPropertiesInInspector]
    public abstract class Window : SerializedMonoBehaviour {
        public enum EWindowAction { Open, Close, Minimize }

        public delegate void WindowResize ();
        public event WindowResize OnWindowResize;

        protected string windowTitle;
        public string WindowTitle {
            get {
                return windowTitle;
            }
        }

        [Header ("Window References")]
        [Required][SerializeField] protected RectTransform windowRectTransform;
        public RectTransform WindowRectTransform {
            get {
                return windowRectTransform;
            }
        }

        [Header ("Window Restrictions")]
        [OdinSerialize][ValidateInput ("RestrictionValid")] public MinMax<Vector2> windowSizeRestrictions;
        private bool RestrictionValid (MinMax<Vector2> value) {
            return value.Min.x > 0 && value.Min.y > 0 && value.Max.x > 0 && value.Max.y > 0;
        }

        [ValidateInput ("AnimatorSizeRestriction")][SerializeField] private Vector2 closeWindowAnimatorSizeRestriction;
        private bool AnimatorSizeRestriction (Vector2 value) {
            return value.x > 0 && value.y > 0;
        }

        [Header ("Window TextMesh References")]
        [Required][SerializeField] protected TextMeshProUGUI windowTitleText;

        [Header ("Window Animator")]
        [SerializeField] protected Animator windowAnimator;
        public Animator WindowAnimator {
            get {
                return windowAnimator;
            }
        }
        private Vector2 sizeDeltaBeforeClose;
        private Vector2 closeWindowSizeLerpTowards;
        private EWindowAction closeWindowLerpType;
        private bool closeLerpActive;
        private bool isClosed;
        public bool IsClosed {
            get {
                if (isWindowBusy) {
                    return false;
                }
                return isClosed;
            }
        }
        protected bool isWindowBusy;
        public bool IsWindowBusy {
            get {
                return isWindowBusy;
            }

            set {
                isWindowBusy = value;
            }
        }

        protected GameObject activeWindowsHeader;
        protected GameObject inactiveWindowsHeader;
        protected GameObject minimizedWindowsHeader;

        protected virtual void Awake () {
            FindRequiredHeaders ();
        }

        private void FindRequiredHeaders () {
            activeWindowsHeader = GameObject.FindGameObjectWithTag ("ActiveWindows");
            inactiveWindowsHeader = GameObject.FindGameObjectWithTag ("InactiveWindows");
            minimizedWindowsHeader = GameObject.FindGameObjectWithTag ("MinimizedWindows");
        }

        protected virtual void Start () {
            SetWindowName ();
            SetWindowText ();
            SetWindowActive (EWindowAction.Open);
        }
        public virtual void SetWindowActive (EWindowAction windowAction) {
            switch (windowAction) {
                case EWindowAction.Open:
                    {
                        transform.SetParent (activeWindowsHeader.transform, false);
                        transform.SetAsLastSibling ();
                        isClosed = false;
                        gameObject.SetActive (true);
                    }
                    break;
                case EWindowAction.Close:
                    {
                        transform.SetParent (inactiveWindowsHeader.transform, false);
                        isClosed = true;
                        gameObject.SetActive (false);
                    }
                    break;
            }

            isWindowBusy = false;
        }

        public void SetClose () {
            if (WindowAnimator != null) {
                sizeDeltaBeforeClose = windowRectTransform.sizeDelta;
                WindowAnimator.SetTrigger ("Minimize");
                closeWindowSizeLerpTowards = closeWindowAnimatorSizeRestriction;
                closeWindowLerpType = EWindowAction.Close;
                closeLerpActive = true;
            } else {
                SetWindowActive (EWindowAction.Close);
            }
        }

        public void SetCloseOpen () {
            if (WindowAnimator != null) {
                WindowAnimator.SetTrigger ("Open");
                closeWindowSizeLerpTowards = sizeDeltaBeforeClose;
                closeWindowLerpType = EWindowAction.Open;
                closeLerpActive = true;

            } else {
                SetWindowActive (EWindowAction.Open);
            }
        }
        protected virtual void SetWindowName () {
            windowTitle = "Window";
        }
        protected virtual void SetWindowText () {
            windowTitleText.text = windowTitle;
        }

        protected bool LerpWindowAnimation (EWindowAction lerpType, Vector2 lerpTowards) {
            windowRectTransform.sizeDelta = Vector2.Lerp (windowRectTransform.sizeDelta, lerpTowards, Time.deltaTime * 5);

            if (Vector2.Distance (windowRectTransform.sizeDelta, lerpTowards) < 1) {
                windowRectTransform.sizeDelta = lerpTowards;
                SetWindowActive (lerpType);
                return true;
            }
            return false;
        }

        protected virtual void Update () {
            if (closeLerpActive) {
                isWindowBusy = true;

                if (LerpWindowAnimation (closeWindowLerpType, closeWindowSizeLerpTowards)) {
                    closeLerpActive = false;
                    isWindowBusy = false;
                }
            }
        }

        public void SetWindowSize (Vector2 newWindowSize) {
            windowRectTransform.sizeDelta = newWindowSize;

            if (OnWindowResize != null) {
                OnWindowResize ();
            }
        }
    }
}