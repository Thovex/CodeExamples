using UnityEngine;

namespace GameDevManager.Employees {

    [System.Serializable]
    public class EmployeeStatsMarketing : EmployeeStats {
        [Header ("Marketing Skills")]
        [SerializeField][Range (-100, 100)] private int marketingPR;
        [SerializeField][Range (-100, 100)] private int marketingCommunity;
        [SerializeField][Range (-100, 100)] private int marketingSmallTarget;
        [SerializeField][Range (-100, 100)] private int marketingMediumTarget;
        [SerializeField][Range (-100, 100)] private int marketingLargeTarget;
        [SerializeField][Range (-100, 100)] private int marketingAAATarget;

        public int MarketingPR {
            get {
                return marketingPR;
            }

            set {
                if (CheckValue (marketingPR, value)) {
                    marketingPR = value;
                }
            }
        }

        public int MarketingCommunity {
            get {
                return marketingCommunity;
            }

            set {
                if (CheckValue (marketingCommunity, value)) {
                    marketingCommunity = value;
                }
            }
        }

        public int MarketingSmallTarget {
            get {
                return marketingSmallTarget;
            }

            set {
                if (CheckValue (marketingSmallTarget, value)) {
                    marketingSmallTarget = value;
                }
            }
        }

        public int MarketingMediumTarget {
            get {
                return marketingMediumTarget;
            }

            set {
                if (CheckValue (marketingMediumTarget, value)) {
                    marketingMediumTarget = value;
                }
            }
        }

        public int MarketingLargeTarget {
            get {
                return marketingLargeTarget;
            }

            set {
                if (CheckValue (marketingLargeTarget, value)) {
                    marketingLargeTarget = value;
                }
            }
        }

        public int MarketingAAATarget {
            get {
                return marketingAAATarget;
            }

            set {
                if (CheckValue (marketingAAATarget, value)) {
                    marketingAAATarget = value;
                }
            }
        }
    }
}