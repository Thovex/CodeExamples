using UnityEngine;

namespace GameDevManager.Employees {

    [System.Serializable]
    public class EmployeeStatsInteractionDesign : EmployeeStats {
        [Header ("Interaction Design Skills")]
        [SerializeField][Range (-100, 100)] private int interactionPlaytesting;
        [SerializeField][Range (-100, 100)] private int interactionControllers;
        [SerializeField][Range (-100, 100)] private int interactionItems;
        [SerializeField][Range (-100, 100)] private int interactionPlayability;

        public int InteractionPlaytesting {
            get {
                return interactionPlaytesting;
            }

            set {
                if (CheckValue (interactionPlaytesting, value)) {
                    interactionPlaytesting = value;
                }
            }
        }

        public int InteractionControllers {
            get {
                return interactionControllers;
            }

            set {
                if (CheckValue (interactionControllers, value)) {
                    interactionControllers = value;
                }
            }
        }

        public int InteractionItems {
            get {
                return interactionItems;
            }

            set {
                if (CheckValue (interactionItems, value)) {
                    interactionItems = value;
                }
            }
        }

        public int InteractionPlayability {
            get {
                return interactionPlayability;
            }

            set {
                if (CheckValue (interactionPlayability, value)) {
                    interactionPlayability = value;
                }
            }
        }
    }
}