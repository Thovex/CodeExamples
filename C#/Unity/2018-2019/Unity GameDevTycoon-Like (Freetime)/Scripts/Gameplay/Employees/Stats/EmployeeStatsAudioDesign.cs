using UnityEngine;

namespace GameDevManager.Employees {
    [System.Serializable]
    public class EmployeeStatsAudioDesign : EmployeeStats {
        [Header ("Audio Design Skills")]
        [SerializeField][Range (-100, 100)] private int audioBackground;
        [SerializeField][Range (-100, 100)] private int audioSFX;
        [SerializeField][Range (-100, 100)] private int audioVoice;

        public int AudioBackground {
            get {
                return audioBackground;
            }

            set {
                if (CheckValue (audioBackground, value)) {
                    audioBackground = value;
                }
            }
        }

        public int AudioSFX {
            get {
                return audioSFX;
            }

            set {
                if (CheckValue (audioSFX, value)) {
                    audioSFX = value;
                }
            }
        }

        public int AudioVoice {
            get {
                return audioVoice;
            }

            set {
                if (CheckValue (audioVoice, value)) {
                    audioVoice = value;
                }
            }
        }
    }
}