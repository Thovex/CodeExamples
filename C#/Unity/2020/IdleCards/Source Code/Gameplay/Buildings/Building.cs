using System;
using System.Collections;
using System.Collections.Generic;

using UnityEditor;
using UnityEngine;

using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Utilities;

using Sirenix.OdinInspector;
using System.Linq;
using BaerAndHoggo.SaveData;

namespace BaerAndHoggo.Gameplay.Buildings
{

    [Serializable]
    public class BuildingSaveData
    {
        public long Id { get; set; }
        public Dictionary<BuildingActionType, int> TimeInSecondsPerAction { get; set; }

        // Don't forget to add new properties we need to save to the To & From Save Data functionality.
    }

    [CreateAssetMenu(fileName = "Building", menuName = "Building")]
    [Serializable]
    public class Building : SerializedScriptableObject, ISaveData<BuildingSaveData>
    {

        [Title("Required")]
        public long Id = -1;
        public string Name = "Unnamed Building";

        [Title("Gameplay")]
        public Dictionary<BuildingActionType, int> TimeInSecondsPerAction = new Dictionary<BuildingActionType, int> {
            {BuildingActionType.CardPack, int.MaxValue}, {BuildingActionType.Repair, int.MaxValue}, {BuildingActionType.Upgrade, int.MaxValue}};

        [Title("Art")]
        [PreviewField] public Sprite Image;

        public void FromSaveData(BuildingSaveData loadData)
        {
            if (this.Id == loadData.Id)
            {
                this.TimeInSecondsPerAction = loadData.TimeInSecondsPerAction;
            }
        }

        public BuildingSaveData ToSaveData()
        {
            BuildingSaveData saveData = new BuildingSaveData();
            saveData.Id = this.Id;
            saveData.TimeInSecondsPerAction = this.TimeInSecondsPerAction;
            return saveData;
        }

#if UNITY_EDITOR
        [Title("Tooling", "Small tools to help define and set coloring.")]
        [SerializeField] [TextArea(0, 25)] [ReadOnly] private string ThatWouldBeTime;

        protected virtual void OnValidate()
        {

            int[] secondsArray = TimeInSecondsPerAction.Values.ToArray();
            BuildingActionType[] typesArray = TimeInSecondsPerAction.Keys.ToArray();

            string[] secondTimersArray = new string[secondsArray.Length];

            for (int i = 0; i < secondsArray.Length; i++)
            {
                secondTimersArray[i] = $"{typesArray[i].ToString()}: {Utility.DefineTimer(secondsArray[i])}";
            }

            ThatWouldBeTime = String.Join(Environment.NewLine, secondTimersArray);

            try
            {
                string assetPath = AssetDatabase.GetAssetPath(this.GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{this.Id}-{this.Name}");
                AssetDatabase.SaveAssets();
            }
            catch (Exception) { }
        }
#endif
    }
}