using UnityEngine;

namespace GameDevManager.Employees {

    [System.Serializable]
    public class EmployeeStatsProducer : EmployeeStats {

        [Header ("Producing Skills")]
        [SerializeField][Range (-100, 100)] private int producerProjectManagement;
        [SerializeField][Range (-100, 100)] private int producerTeamManagement;
        [SerializeField][Range (-100, 100)] private int producerReleasePlan;
        [SerializeField][Range (-100, 100)] private int producerScrum;

        public int ProducerProjectManagement {
            get {
                return producerProjectManagement;
            }

            set {
                if (CheckValue (producerProjectManagement, value)) {
                    producerProjectManagement = value;
                }
            }
        }

        public int ProducerTeamManagement {
            get {
                return producerTeamManagement;
            }

            set {
                if (CheckValue (producerTeamManagement, value)) {
                    producerTeamManagement = value;
                }
            }
        }

        public int ProducerReleasePlan {
            get {
                return producerReleasePlan;
            }

            set {
                if (CheckValue (producerReleasePlan, value)) {
                    producerReleasePlan = value;
                }
            }
        }

        public int ProducerScrum {
            get {
                return producerScrum;
            }

            set {
                if (CheckValue (producerScrum, value)) {
                    producerScrum = value;
                }
            }
        }
    }
}