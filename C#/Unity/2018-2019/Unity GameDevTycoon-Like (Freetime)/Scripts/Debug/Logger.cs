using System.ComponentModel;
using UnityEngine;

namespace Thovex.Utility {
    public static class Debugger {
        public enum ELogTypes {
            [Description ("Log everything")] All, 
            [Description ("Only log info Debug.Log")] Info, 
            [Description ("Only log warning Debug.LogWarning")] Warning, 
            [Description ("Only log error Debug.LogError")] Error, 
            [Description ("Log nothing")] None
        }

        private static ELogTypes logType = ELogTypes.All;

        public static void Log (object loggedObject) {
            if (logType == ELogTypes.Info || logType == ELogTypes.All) {
                Debug.Log (loggedObject);
            }
        }

        public static void LogWarning (object loggedObject) {
            if (logType == ELogTypes.Warning || logType == ELogTypes.All) {
                Debug.LogWarning(loggedObject);
            }
        }

        public static void LogError (object loggedObject) {
            if (logType == ELogTypes.Error || logType == ELogTypes.All) {
                Debug.LogError(loggedObject);
            }
        }
    }
}