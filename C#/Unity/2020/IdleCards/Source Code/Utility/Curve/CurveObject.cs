using System;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Curve
{
    [CreateAssetMenu(fileName = "New Curve", menuName = "Idle Cards/New Curve", order = 100)] 
    [Serializable]
    public sealed class CurveObject : ScriptableObject, IIdentifiable
    {
        [Title("Required")] 
        [ReadOnly] public long id = -1;
        public string curveName = "Unnamed Curve";

        public AnimationCurve curve;
        
        #region IIdentifiable

        public long GetID() => id;
        public string GetName() => curveName;

        #endregion IIdentifiable
        
#if UNITY_EDITOR
        private void OnValidate()
        {
            if (id == -1)
            {
                var curves = Resources.LoadAll<CurveObject>("Curves");
                id = curves.Max(t => t.id)+1;
            }
            
            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{curveName}");
                AssetDatabase.SaveAssets();
            }
            catch (Exception)
            {
                // ignored
            }
        }
#endif
        
    }
}