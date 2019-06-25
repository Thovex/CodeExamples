using UnityEngine;

namespace GameDevManager.Employees {

    [System.Serializable]
    public class EmployeeStatsLegal : EmployeeStats {
        [Header ("Legal Skills")]
        [SerializeField][Range (-100, 100)] private int legalCopyright;

        public int LegalCopyright {
            get {
                return legalCopyright;
            }

            set {
                if (CheckValue (legalCopyright, value)) {
                    legalCopyright = value;
                }
            }
        }
    }
}