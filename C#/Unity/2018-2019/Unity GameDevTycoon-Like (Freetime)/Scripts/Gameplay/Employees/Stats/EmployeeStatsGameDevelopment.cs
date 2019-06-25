using UnityEngine;

namespace GameDevManager.Employees {

    [System.Serializable]
    public class EmployeeStatsGameDevelopment : EmployeeStats {
        [Header ("Game Development Skills")]
        [SerializeField][Range (-100, 100)] private int developmentEngine;
        [SerializeField][Range (-100, 100)] private int developmentGameplay;
        [SerializeField][Range (-100, 100)] private int developmentNetworking;
        [SerializeField][Range (-100, 100)] private int developmentArtificialIntelligence;
        [SerializeField][Range (-100, 100)] private int developmentOptimization;
        [SerializeField][Range (-100, 100)] private int developmentRelease;
        [SerializeField][Range (-100, 100)] private int developmentTools;
        [SerializeField][Range (-100, 100)] private int developmentVR;
        [SerializeField][Range (-100, 100)] private int developmentMocap;

        public int DevelopmentEngine {
            get {
                return developmentEngine;
            }

            set {
                if (CheckValue (developmentEngine, value)) {
                    developmentEngine = value;
                }
            }
        }

        public int DevelopmentGameplay {
            get {
                return developmentGameplay;
            }

            set {
                if (CheckValue (developmentGameplay, value)) {
                    developmentGameplay = value;
                }
            }
        }

        public int DevelopmentNetworking {
            get {
                return developmentNetworking;
            }

            set {
                if (CheckValue (developmentNetworking, value)) {
                    developmentNetworking = value;
                }
            }
        }

        public int DevelopmentArtificialIntelligence {
            get {
                return developmentArtificialIntelligence;
            }

            set {
                if (CheckValue (developmentArtificialIntelligence, value)) {
                    developmentArtificialIntelligence = value;
                }
            }
        }

        public int DevelopmentOptimization {
            get {
                return developmentOptimization;
            }

            set {
                if (CheckValue (developmentOptimization, value)) {
                    developmentOptimization = value;
                }
            }
        }

        public int DevelopmentRelease {
            get {
                return developmentRelease;
            }

            set {
                if (CheckValue (developmentRelease, value)) {
                    developmentRelease = value;
                }
            }
        }

        public int DevelopmentTools {
            get {
                return developmentTools;
            }

            set {
                if (CheckValue (developmentTools, value)) {
                    developmentTools = value;
                }
            }
        }

        public int DevelopmentVR {
            get {
                return developmentVR;
            }

            set {
                if (CheckValue (developmentVR, value)) {
                    developmentVR = value;
                }
            }
        }

        public int DevelopmentMocap {
            get {
                return developmentMocap;
            }

            set {
                if (CheckValue (developmentMocap, value)) {
                    developmentMocap = value;
                }
            }
        }
    }
}