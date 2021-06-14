using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.Gameplay.Inventories;
using System.Linq;
using System.Text.RegularExpressions;
using UnityEngine.Networking;

[Serializable]
public class SaveData
{
    public DateTime SaveDate { get; set; }
    //public BuildingSaveData[] BuildingData { get; set; }
    public CardStackSaveData[] CardInventory { get; set; }
}

public class SaveManager : MonoBehaviour
{
    public void SaveGame()
    {
        StartCoroutine(SaveGameCoroutine());
    }

    private IEnumerator SaveGameCoroutine()
    {
        UnityWebRequest www = UnityWebRequest.Get("http://worldtimeapi.org/api/timezone/Europe/Amsterdam.txt");
        yield return www.SendWebRequest();

        if (www.isNetworkError || www.isHttpError)
        {
            Debug.LogError("SAVE FAILED. Reason: connecting to worldtimeapi.org error.");
            // Should probably make a backup save functionality atm.

        }
        else
        {
            string timeRaw = www.downloadHandler.text;
            MatchCollection matches = Regex.Matches(timeRaw, @"utc_datetime\:\s*(?'year'\d{4})\-(?'month'\d{2})\-(?'day'\d{2})T(?'hour'\d{2})\:(?'minute'\d{2})\:(?'second'\d+)\.(?'millisecond'\d+)\+00:00");

            if (matches.Count == 0)
            {
                Debug.LogError($"SAVE FAILED. Reason: invalid regex supplied for text: {timeRaw}.");
            }
            else
            {
                BinaryFormatter binaryFormatter = new BinaryFormatter();
                FileStream file = File.Create($"{Application.persistentDataPath}/MySaveData.dat");

                Debug.Log($"{Application.persistentDataPath}/MySaveData.dat");

                SaveData data = new SaveData();
                // data.BuildingData = GetBuildings();

                List<CardStackSaveData> cardStackSaveData = new List<CardStackSaveData>();
                foreach (CardStack cardStack in CardInventory.Instance.inventory)
                {
                    cardStackSaveData.Add(cardStack.ToSaveData());
                }

                data.SaveDate = MatchToDateTime(matches[0]);

                Debug.Log($"Saved at: {data.SaveDate}");

                data.CardInventory = cardStackSaveData.ToArray();

                binaryFormatter.Serialize(file, data);
                file.Close();

                Debug.Log("SAVE SUCCESSFUL.");
            }
        }
    }

    public void LoadGame()
    {
        StartCoroutine(LoadGameCoroutine());
    }

    private IEnumerator LoadGameCoroutine()
    {
        if (File.Exists($"{Application.persistentDataPath}/MySaveData.dat"))
        {
            UnityWebRequest www = UnityWebRequest.Get("http://worldtimeapi.org/api/timezone/Europe/Amsterdam.txt");
            yield return www.SendWebRequest();

            if (www.isNetworkError || www.isHttpError)
            {
                Debug.LogError("LOAD FAILED. Reason: connecting to worldtimeapi.org error.");
                // Should probably make a backup save functionality atm.

            }
            else
            {
                string timeRaw = www.downloadHandler.text;
                MatchCollection matches = Regex.Matches(timeRaw, @"utc_datetime\:\s*(?'year'\d{4})\-(?'month'\d{2})\-(?'day'\d{2})T(?'hour'\d{2})\:(?'minute'\d{2})\:(?'second'\d+)\.(?'millisecond'\d+)\+00:00");

                if (matches.Count == 0)
                {
                    Debug.LogError($"LOAD FAILED. Reason: invalid regex supplied for text: {timeRaw}.");
                }
                else
                {
                    BinaryFormatter binaryFormatter = new BinaryFormatter();
                    FileStream file = File.Open($"{Application.persistentDataPath}/MySaveData.dat", FileMode.Open);
                    SaveData data = (SaveData)binaryFormatter.Deserialize(file);

                    file.Close();

                    var difference = MatchToDateTime(matches[0]) - data.SaveDate;

                    if (difference.TotalMinutes < 0) {
                        //Invalid save? Idk
                    }

                    Debug.Log($"Existing save data was saved at: {data.SaveDate}. Difference: {difference}");
                    CardInventory.Instance.LoadInventory(data.CardInventory);

                    Debug.Log("Game data loaded!");
                }
            }
        }
        else
        {
            Debug.LogError("There is no save data!");
        }
    }


    private DateTime MatchToDateTime(Match match)
    {
        int yyyy = int.Parse(match.Groups["year"].ToString());
        int mm = int.Parse(match.Groups["month"].ToString());
        int dd = int.Parse(match.Groups["day"].ToString());
        int h = int.Parse(match.Groups["hour"].ToString());
        int m = int.Parse(match.Groups["minute"].ToString());
        int s = int.Parse(match.Groups["second"].ToString());

        return new DateTime(yyyy, mm, dd, h, m, s);
    }


}
