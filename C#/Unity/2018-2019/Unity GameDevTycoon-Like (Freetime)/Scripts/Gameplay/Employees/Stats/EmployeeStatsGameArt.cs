using UnityEngine;

namespace GameDevManager.Employees {

    [System.Serializable]
    public class EmployeeStatsGameArt : EmployeeStats {
        [Header ("Game Art Skills")]
        [SerializeField][Range (-100, 100)] private int art2D;
        [SerializeField][Range (-100, 100)] private int art3D;
        [SerializeField][Range (-100, 100)] private int artPixel;
        [SerializeField][Range (-100, 100)] private int artGraphicDesign;
        [SerializeField][Range (-100, 100)] private int artSculpting;
        [SerializeField][Range (-100, 100)] private int artAnimation;
        [SerializeField][Range (-100, 100)] private int artConcept;
        [SerializeField][Range (-100, 100)] private int artVFX;
        [SerializeField][Range (-100, 100)] private int artCinematics;

        public int Art2D {
            get {
                return art2D;
            }

            set {
                if (CheckValue (art2D, value)) {
                    art2D = value;
                }
            }
        }

        public int Art3D {
            get {
                return art3D;
            }

            set {
                if (CheckValue (art3D, value)) {
                    art3D = value;
                }
            }
        }

        public int ArtPixel {
            get {
                return artPixel;
            }

            set {
                if (CheckValue (artPixel, value)) {
                    artPixel = value;
                }
            }
        }

        public int ArtSculpting {
            get {
                return artSculpting;
            }

            set {
                if (CheckValue (artSculpting, value)) {
                    artSculpting = value;
                }
            }
        }

        public int ArtAnimation {
            get {
                return artAnimation;
            }

            set {
                if (CheckValue (artAnimation, value)) {
                    artAnimation = value;
                }
            }
        }

        public int ArtConcept {
            get {
                return artConcept;
            }

            set {
                if (CheckValue (artConcept, value)) {
                    artConcept = value;
                }
            }
        }

        public int ArtVFX {
            get {
                return artVFX;
            }

            set {
                if (CheckValue (artVFX, value)) {
                    artVFX = value;
                }
            }
        }

        public int ArtCinematics {
            get {
                return artCinematics;
            }

            set {
                if (CheckValue (artCinematics, value)) {
                    artCinematics = value;
                }
            }
        }
    }
}