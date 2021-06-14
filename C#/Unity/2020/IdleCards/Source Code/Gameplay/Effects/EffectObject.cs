using System;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace Gameplay.Effects
{
    [CreateAssetMenu(fileName = "New Effect", menuName = "Idle Cards/New Effect", order = 100)]
    public class EffectObject : SerializedScriptableObject, IIdentifiable
    {
        [Title("Required")] 
        [ReadOnly] public long id = -1;
        public string effectName = "Unnamed Effect";

        public GameObject effectPrefab;
        
        #region IIdentifiable
        public long GetID() => id;
        public string GetName() => effectName;

        #endregion IIdentifiable
        
#if UNITY_EDITOR
        private void OnValidate()
        {
            if (id == -1)
            {
                var effects = Resources.LoadAll<EffectObject>("Effects");
                id = effects.Max(e => e.id)+1;
            }
            
            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{effectName}");
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