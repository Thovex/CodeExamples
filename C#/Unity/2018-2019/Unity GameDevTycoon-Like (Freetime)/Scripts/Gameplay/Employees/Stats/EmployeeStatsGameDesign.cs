using UnityEngine;

namespace GameDevManager.Employees {
    [System.Serializable]
    public class EmployeeStatsGameDesign : EmployeeStats {
        [Header ("Game Design Skills")]
        [SerializeField][Range (-100, 100)] private int designGameplay;
        [SerializeField][Range (-100, 100)] private int designLevel;
        [SerializeField][Range (-100, 100)] private int designWorld;
        [SerializeField][Range (-100, 100)] private int designNarrative;
        [SerializeField][Range (-100, 100)] private int designUI;
        [SerializeField][Range (-100, 100)] private int designMonetization;

        public int DesignGameplay {
            get {
                return designGameplay;
            }

            set {
                if (CheckValue (designGameplay, value)) {
                    designGameplay = value;
                }
            }
        }

        public int DesignLevel {
            get {
                return designLevel;
            }

            set {
                if (CheckValue (designLevel, value)) {
                    designLevel = value;
                }
            }
        }

        public int DesignWorld {
            get {
                return designWorld;
            }

            set {
                if (CheckValue (designWorld, value)) {
                    designWorld = value;
                }
            }
        }

        public int DesignNarrative {
            get {
                return designNarrative;
            }

            set {
                if (CheckValue (designNarrative, value)) {
                    designNarrative = value;
                }
            }
        }

        public int DesignUI {
            get {
                return designUI;
            }

            set {
                if (CheckValue (designUI, value)) {
                    designUI = value;
                }
            }
        }

        public int DesignMonetization {
            get {
                return designMonetization;
            }

            set {
                if (CheckValue (designMonetization, value)) {
                    designMonetization = value;
                }
            }
        }
    }
}