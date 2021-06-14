using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;

namespace BaerAndHoggo.Gameplay.Buildings
{
    public class BuildingDB : Singleton<BuildingDB>
    {
        [SerializeField] [ReadOnly] private Dictionary<long, Building> buildingDB = new Dictionary<long, Building>();

        public Building[] GetAllBuildings() => this.buildingDB.Values.ToArray();

        private void Awake()
        {
            // Grab buildings we have defined in the Resources/Buildings folder.
            Building[] buildings = Resources.LoadAll<Building>("Buildings");

            foreach (Building building in buildings)
            {
                // Add an instantiated clone from the existing building. 
                // We do not ever want to make changes on the files by programming here.
                buildingDB.Add(building.Id, Instantiate(building));
            }
        }


        // Returns a nullptr on false.
        public bool GetBuilding(int index, out Building building)
        {
            building = null;

            // If DB contains element at this index.
            if (buildingDB.ElementAt(index).Value)
            {
                // Grab a ref from this building to give to the player.
                building = buildingDB.ElementAt(index).Value;
                return true;
            }

            new System.Exception($"Invalid building requested of index {index}. But not available in BuildingDB.");
            return false;
        }

        public long GetIdFromName(string Name)
        {
            foreach (var building in buildingDB)
            {
                if (building.Value.Name == Name)
                {
                    return building.Value.Id;
                }
            }

            Debug.LogError($"GetIdFromName Failed! No such Id found for Name {Name}");
            return -1;
        }

        public string GetNameFromId(long Id)
        {
            foreach (var building in buildingDB)
            {
                if (building.Value.Id == Id)
                {
                    return building.Value.Name;
                }
            }

            Debug.LogError($"GetNameFromId Failed! No such Name found for Id {Id}");
            return "NULL";
        }

        public long GetIdFromIndex(int index)
        {
            //Debug.LogError($"GetIdFromIndex Failed! No such Id found for Name {Name}");
            return buildingDB.ElementAt(index).Key;
        }

        public int GetIndexFromId(long id)
        {
            for (int i = 0; i < GetBuildingCount; i++)
            {
                if (buildingDB.ElementAt(i).Key == id)
                {
                    return i;
                }
            }
            Debug.LogError($"GetIndexFromId Failed! No such Index found for Id {id}");
            return -1;
        }

        public int GetBuildingCount => buildingDB.Count;
    }
}
