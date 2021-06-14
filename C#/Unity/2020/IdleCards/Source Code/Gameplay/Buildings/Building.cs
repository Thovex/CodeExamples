using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.Utilities;
using Gameplay.Currency;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Buildings
{
    public enum BuildingType
    {
        Fortress,
        Blacksmith
    }

    public enum BuildingActionType
    {
        Null,
        Cardpack,
        Upgrade,
        Repair,
        Research,
        Experiment
    }

    [Serializable]
    public abstract class Building : SerializedScriptableObject, 
        ISaveData<BuildingIO>, 
        IIdentifiable, 
        ITimeEventCaller
    {

        [ReadOnly] public BuildingType buildingType;

        [Title("Required")] 
        [ReadOnly] public long id = -1;
        public string buildingName = "Unnamed Building";

        [Title("Art")] 
        [PreviewField] public Sprite image;
        public Color backgroundColor = Color.red;

        [Title("Gameplay")]
        public string description = "No description";
        public Dictionary<BuildingActionType, double> timeInSecondsPerAction = new Dictionary<BuildingActionType, double>();
        public long tier = 1;
        public double constructionTime;
        public int cost;
        public Currencies currency;

        #region SaveGame

        public BuildingIO OnSave_Implementation()
        {
            return new BuildingIO
            {
                Id = id,
                Name = buildingName,
                Tier = tier,
                Cost = cost,
                Currency = currency,
                ConstructionTime = constructionTime,
                TimeInSecondsPerAction = timeInSecondsPerAction,
            };
        }

        public void OnLoad_Implementation(BuildingIO loadData)
        {
            id = loadData.Id;
            buildingName = loadData.Name;
            tier = loadData.Tier;
            cost = loadData.Cost;
            currency = loadData.Currency;
            constructionTime = loadData.ConstructionTime;
            timeInSecondsPerAction = loadData.TimeInSecondsPerAction;
            
            BuildingDB.Instance.GetBaseItem(id, out var baseBuilding);
            image = baseBuilding.image;
        }

        #endregion SaveGame

        #region IIdentifiable

        public long GetID() => id;
        public string GetName() => buildingName;

        #endregion IIdentifiable
        
        #region ITimeEventCaller
        public CallerEventType GetEventType() => CallerEventType.Building;
        
        #endregion ITimeEventCaller
        
#if UNITY_EDITOR

        [Title("Tooling", "Small tools to help define and set coloring.")] [SerializeField] [TextArea(0, 25)] [ReadOnly]
        public string thatWouldBeTime;
        
        protected virtual void OnValidate()
        {
            if (id == -1)
            {
                var buildings = Resources.LoadAll<Building>("Buildings");
                id = buildings.Max(b => b.id)+1;
            }
            
            var secondsArray = timeInSecondsPerAction.Values.ToArray();
            var typesArray = timeInSecondsPerAction.Keys.ToArray();

            var secondTimersArray = new string[secondsArray.Length];

            for (var i = 0; i < secondsArray.Length; i++)
                secondTimersArray[i] = $"{typesArray[i].ToString()}: {Utility.DefineTimer(secondsArray[i])}";

            thatWouldBeTime = string.Join(Environment.NewLine, secondTimersArray);

            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{buildingName}");
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