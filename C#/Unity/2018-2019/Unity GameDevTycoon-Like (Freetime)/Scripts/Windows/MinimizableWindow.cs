using Sirenix.OdinInspector;
using Thovex.Types;
using TMPro;
using UnityEngine;

namespace GameDevManager.Windows {
    public abstract class MinimizableWindow : Window {
        [Header ("Minimized Window References")]
        [AssetsOnly][SerializeField] private GameObject minimizedWindowPrefab;
        [ValidateInput("AnimatorSizeRestriction")][SerializeField] private Vector2 minimizedWindowAnimatorSizeRestriction;
        private Vector2 minimizedWindowSizeLerpTowards;
        private EWindowAction minimizedWindowLerpType;
        private Vector2 sizeDeltaBeforeMinimize;
        private bool minimizeLerpActive;
        private bool isMinimized;
        public bool IsMinimized {
            get {
                if (isWindowBusy) {
                    return false;
                }
                return isMinimized;
            }
        }
        private GameObject minimizedWindow;
        
        public override void SetWindowActive (EWindowAction windowAction) {
            base.SetWindowActive (windowAction);

            switch (windowAction) {
                case EWindowAction.Open:
                    {
                        isMinimized = false;
                    }
                    break;
                case EWindowAction.Minimize:
                    {
                        transform.SetParent (inactiveWindowsHeader.transform, false);
                        CheckForMinimizedWindow ();
                        isMinimized = true;
                        isWindowBusy = false;
                        gameObject.SetActive (false);
                    }
                    break;
            }
        }

        public void SetMinimize () {
            if (WindowAnimator != null) {
                sizeDeltaBeforeMinimize = windowRectTransform.sizeDelta;
                WindowAnimator.SetTrigger ("Minimize");
                minimizedWindowSizeLerpTowards = minimizedWindowAnimatorSizeRestriction;
                minimizedWindowLerpType = EWindowAction.Minimize;
                minimizeLerpActive = true;
            } else {
                SetWindowActive (EWindowAction.Minimize);
            }
        }

        public void SetMinimizeOpen () {
            if (WindowAnimator != null) {
                WindowAnimator.SetTrigger ("Open");
                minimizedWindowSizeLerpTowards = sizeDeltaBeforeMinimize;
                minimizedWindowLerpType = EWindowAction.Open;
                minimizeLerpActive = true;

                if (minimizedWindow != null) {
                    minimizedWindow.SetActive (false);
                }
            } else {
                SetWindowActive (EWindowAction.Open);
            }
        }

        private void CheckForMinimizedWindow () {
            if (minimizedWindow == null) {
                minimizedWindow = Instantiate (
                    minimizedWindowPrefab,
                    transform.position,
                    Quaternion.identity,
                    minimizedWindowsHeader.transform
                );
                minimizedWindow.GetComponent<MinimizedWindow> ().ParentWindow = this;
            }
            minimizedWindow.SetActive (true);
            minimizedWindow.transform.SetAsLastSibling ();
        }

        protected override void Update () {
            base.Update ();

            if (minimizeLerpActive) {
                isWindowBusy = true;

                if (LerpWindowAnimation (minimizedWindowLerpType, minimizedWindowSizeLerpTowards)) {
                    minimizeLerpActive = false;
                    isWindowBusy = false;
                }
            }
        }

        private void OnDestroy () {
            Destroy (minimizedWindow);
        }
    }
}