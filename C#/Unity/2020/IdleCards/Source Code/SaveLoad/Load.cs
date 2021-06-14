using System;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using BaerAndHoggo.Gameplay.Time;
using UnityEngine;

namespace BaerAndHoggo.IO
{
    public static class Load
    {
        private static readonly int MaxRetries = 5;
        public static bool IsLoading { get; set; }

        public static bool Exists(string fileName)
        {
            return File.Exists($"{Application.persistentDataPath}/{fileName}.dat");
        }

        public static bool LoadGame(string fileName, out SaveData saveData, int currentRetries = 0)
        {
            var binaryFormatter = new BinaryFormatter();

            saveData = new SaveData();

            if (!Exists(fileName))
            {
                Debug.Log($"No save data yet. {fileName} Creating!");
                Save.SaveGame(fileName);
            }

            IsLoading = true;

            while (currentRetries < MaxRetries)
            {
                try
                {
                    var file = File.Open($"{Application.persistentDataPath}/{fileName}.dat", FileMode.Open);
                    saveData = (SaveData) binaryFormatter.Deserialize(file);
                    file.Close();

                    SetLoadedData(saveData);
                }
                catch (Exception)
                {
                    currentRetries++;
                    continue;
                }

                IsLoading = false;
                return true;
            }

            Debug.Log("Exhausted retries.");
            LoadingManager.Instance.SetLoadMaxRetries(MaxRetries);
            return false;
        }

        private static void SetLoadedData(SaveData data)
        {
            var difference = TimeEventManager.Instance.SessionStart - data.saveDate;

            // Only load if we have more than 5 seconds of afk.
            //if (difference.TotalSeconds > 5) {
            LoadingManager.Instance.SetLoadAwayTime(difference);

            //}

            Debug.Log(
                $"Start ({data.saveDate}). " +
                $"End ({TimeEventManager.Instance.SessionStart}). " +
                $"Difference: {difference}");
        }
    }
}