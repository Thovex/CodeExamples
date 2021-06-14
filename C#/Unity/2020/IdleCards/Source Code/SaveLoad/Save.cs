using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Inventories;
using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Gameplay.Towns;
using UnityEngine;

namespace BaerAndHoggo.IO
{
    public static class Save
    {
        public delegate void SaveCompleted();

        public static bool IsSaving { get; set; }
        public static event SaveCompleted OnSaveCompleted;

        public static void SaveGame(string fileName)
        {
            IsSaving = true;

            Debug.Log($"Called Save Game ({fileName})");
            CreateSaveFileIo(GetGameSaveData(), fileName);

            IsSaving = false;
        }

        /* Save Data Classes */
        private static SaveData GetGameSaveData()
        {
            var saveData = new SaveData
            {
                saveDate = TimeEventManager.Instance.CalculateNewSaveDate(),
                
                TimeEventIO =  TimeEventManager.Instance.OnSave_Implementation(),
                TownIO = TownDB.Instance.OnSave_Implementation(),
                CardIO = CardDB.Instance.OnSave_Implementation(),
                CardStackIO = CardInventory.Instance.OnSave_Implementation(),
                BuildingIO = BuildingDB.Instance.OnSave_Implementation(),
            };

            Debug.Log($"SaveData (SaveDate): {saveData.saveDate}");
            
            return saveData;
        }

        private static void CreateSaveFileIo(SaveData data, string fileName)
        {
            Debug.Log($"Start i/o savefile: ({fileName})");
            
            Debug.Log(data);

            var binaryFormatter = new BinaryFormatter();
            var file = File.Create($"{Application.persistentDataPath}/{fileName}.dat");
            binaryFormatter.Serialize(file, data);
            file.Close();

            Debug.Log("End i/o savefile");
        }

        public static void RemoveSaveFileIo(string fileName)
        {
            File.Delete($"{Application.persistentDataPath}/{fileName}.dat");
            Application.Quit();
        }
    }
}