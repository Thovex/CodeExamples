
using System.Linq;

namespace BaerAndHoggo.Gameplay.Buildings
{
    public class BuildingDB : DB<Building, BuildingIO[]>
    {
        public new static BuildingDB Instance => (BuildingDB)GetInstance();

        protected override void InitializeDB()
        {
            IsInitialized = true;
        }


        protected override void StartLoad(IO.SaveData loadData)
        {
            OnLoad_Implementation(loadData.BuildingIO);
        }

        public override BuildingIO[] OnSave_Implementation()
        {
            var data = new BuildingIO[db.Count];
            
            for (int i = 0; i < db.Count; i++)
            {
                data[i] = db.ElementAt(i).Value.OnSave_Implementation();
            }
            
            return data;
        }

        public override void OnLoad_Implementation(BuildingIO[] loadData)
        {
            if (loadData == null) return; 
            if (loadData.Length == 0) return;

            // Foreach saved building IO, we go through the IDs and load with the new data.
            foreach (var buildingIO in loadData)
            {
                db[buildingIO.Id].OnLoad_Implementation(buildingIO);
            }
        }
    }
}