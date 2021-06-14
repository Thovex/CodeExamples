using System;
using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;
using UnityEngine;

namespace BaerAndHoggo.IO
{
    public class SaveManager : Singleton<SaveManager>
    {
        private readonly string _apiUrl = "http://worldtimeapi.org/api/timezone/Europe/Amsterdam.txt";
        private bool _isLoading = false;

        private bool _isSaving = false;
        [SerializeField] [Required] private GameObject loadingManager;
        [SerializeField] private string saveFileName = "IC_Save_Data";

        public delegate void LoadedGame(SaveData saveData);

        public event LoadedGame OnLoad;

        private void Awake()
        {
            loadingManager.SetActive(true);
        }

#if UNITY_EDITOR
        private void Start()
        {
            Debug.Log("Editor only load");

            LoadingManager.Instance.StartLoading();

            WorldTimeApi.OnGetTime += WorldClockTime;
            WorldTimeApi.GetWorldDateTimeUtc(_apiUrl, this);
        }
#endif

        [Button]
        public void RequestSaveGame()
        {
            Save.SaveGame(saveFileName);
        }

        [Button]
        public void RequestLoadGame()
        {            
            LoadingManager.Instance.StartLoading();
            
            WorldTimeApi.OnGetTime += WorldClockTime;
            WorldTimeApi.GetWorldDateTimeUtc(_apiUrl, this);
        }

#if (UNITY_ANDROID || UNITY_IOS) && !UNITY_EDITOR
        private void OnApplicationFocus(bool focusStatus)
        {
            Debug.Log("Android & iOS only save & load");
            if (focusStatus)
            {
                // Loading into the game!
                LoadingManager.Instance.StartLoading();

                WorldTimeApi.OnGetTime += WorldClockTime;
                WorldTimeApi.GetWorldDateTimeUtc(_apiUrl, this);
            }
            else
            {
                TimeEventManager.Instance.TimerActive = false;
                Save.SaveGame(saveFileName);
            }
        }
#endif


        private void WorldClockTime(DateTime dateTime)
        {
            // Unsubscribe to the event.
            WorldTimeApi.OnGetTime -= WorldClockTime;

            TimeEventManager.Instance.SessionTimer = 0;
            TimeEventManager.Instance.SessionStart = dateTime;

            if (Load.LoadGame(saveFileName, out var saveData))
            {
                OnLoad?.Invoke(saveData);

                TimeEventManager.Instance.TimerActive = true;
                LoadingManager.Instance.StopLoading();
            }
        }
    }
}