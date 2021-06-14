using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Buildings;

namespace BaerAndHoggo.Gameplay.Towns
{
    public class TownDB : DB<Town, TownIO[]>
    {
        public new static TownDB Instance => (TownDB)GetInstance();

        public virtual void UpdateBuildingState(long id, Building building, BuildProgress.State newState )
        {
            db[id].buildings[building].buildingState = newState;
        }

        public Town FindTownIdByBuildingId(long id)
        {
            foreach (var town in db.Where(town => 
                town.Value.buildings.Any(building => building.Key.id == id)))
            {
                return town.Value;
            }

            throw new SystemException($"Tying to find a town with building {id} but none was found.");
        }

        protected override void InitializeDB()
        {
            // Read all Buildings from BuildingDB so we have the exact load copy.
            foreach (var town in db)
            {
                var inGameBuildings = new Dictionary<Building, BuildProgress>();

                for (int i = 0; i < town.Value.buildings.Count; i++)
                {
                    inGameBuildings.Add(
                        BuildingDB.Instance.GetItemById(town.Value.buildings.ElementAt(i).Key.id),
                        town.Value.buildings.ElementAt(i).Value);
                }
                
                // If we load a save game we wanna get times here too

                town.Value.buildings = inGameBuildings;
            }

            IsInitialized = true;
        }

        protected override void StartLoad(IO.SaveData loadData)
        {
            OnLoad_Implementation(loadData.TownIO);
        }

        public override TownIO[] OnSave_Implementation()
        {
            var data = new TownIO[db.Count];
            
            for (int i = 0; i < db.Count; i++)
            {
                data[i] = db.ElementAt(i).Value.OnSave_Implementation();
            }
            
            return data;
        }

        public override void OnLoad_Implementation(TownIO[] loadData)
        {
            if (loadData == null) return; 
            if (loadData.Length == 0) return;

            // Foreach saved building IO, we go through the IDs and load with the new data.
            foreach (var townIO in loadData)
            {
                db[townIO.Id].OnLoad_Implementation(townIO);
            }
        }
    }
}