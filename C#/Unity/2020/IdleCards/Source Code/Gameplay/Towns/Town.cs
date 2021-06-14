using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Towns
{
    [Serializable]
    public class BuildProgress
    {
        public enum State { Finished, UnderConstruction, Purchasable }

        [SerializeField] public State buildingState;
    }

    [Serializable]
    [CreateAssetMenu(fileName = "New Town", menuName = "Idle Cards/Town", order = 2)]
    public class Town : SerializedScriptableObject, 
        ISaveData<TownIO>,
        IIdentifiable, 
        ITimeEventCaller
    {
        public Dictionary<Building, BuildProgress> buildings = new Dictionary<Building, BuildProgress>();
        public string description = "No description";

        [Title("Required")] 
        [ReadOnly] public long id = -1;

        public string townName = "Unnamed Town";
        public long tier = 1;
        
        public TownIO OnSave_Implementation()
        {
            var buildingsSaveData
                = buildings.ToDictionary(
                    building 
                    => building.Key.id, 
                    building
                    => building.Value);

            return new TownIO()
            {
                Id = id,
                Name = townName,
                Tier = tier,
                Buildings = buildingsSaveData,
            };
        }

        public void OnLoad_Implementation(TownIO loadData)
        {
            var buildingsLoadData 
                = loadData.Buildings.ToDictionary(
                    building 
                    => BuildingDB.Instance.GetItemById(building.Key), 
                    building 
                    => building.Value);

            id = loadData.Id;
            townName = loadData.Name;
            tier = loadData.Tier;
            buildings = buildingsLoadData;
        }

        public long GetID() => id;
        public string GetName() => townName;
        public CallerEventType GetEventType() => CallerEventType.Town;
        

#if UNITY_EDITOR
        [Title("Tooling", "Small tools to help define times")] [SerializeField] [TextArea(0, 25)] [ReadOnly]
        private string thatWouldBeTime;

        protected virtual void OnValidate()
        {
            if (id == -1)
            {
                var towns = Resources.LoadAll<Town>("Towns");
                id = towns.Max(t => t.id)+1;
            }

            
            var secondsArray = buildings.Values.ToArray();
            var typesArray = buildings.Keys.ToArray();

            var secondTimersArray = new string[secondsArray.Length];

            for (var i = 0; i < secondsArray.Length; i++)
                secondTimersArray[i] = $"{typesArray[i].buildingName}: {Utility.DefineTimer(typesArray[i].constructionTime)}";

            thatWouldBeTime = string.Join(Environment.NewLine, secondTimersArray);

            try
            {
                var assetPath = AssetDatabase.GetAssetPath(GetInstanceID());
                AssetDatabase.RenameAsset(assetPath, $"{id}-{townName}");
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