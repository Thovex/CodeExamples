using System;
using BaerAndHoggo.Utilities;
using TMPro;
using UnityEngine;
using UnityEngine.Serialization;

namespace BaerAndHoggo.IO
{
    public class LoadingManager : Singleton<LoadingManager>
    {
         public bool loading;
        [SerializeField] private Animator loadingScreenAnimator;
        [SerializeField] private TMP_Text loadingScreenAwayTimeText;

        private void Start()
        {
            Application.targetFrameRate = Screen.currentResolution.refreshRate;
        }

        public void SetLoadAwayTime(TimeSpan awayTime)
        {
            loadingScreenAwayTimeText.text =
                $"You have been away for {Environment.NewLine}{Utility.DefineTimer((int) awayTime.TotalSeconds)}";
        }

        public void SetLoadRetry(int retries)
        {
            loadingScreenAwayTimeText.text =
                $"We ran into an error, trying reload.{Environment.NewLine}Attempt #{retries}";
        }

        public void SetLoadMaxRetries(int maxRetries)
        {
            loadingScreenAwayTimeText.text =
                $"Exhausted retries {maxRetries}.{Environment.NewLine}{Environment.NewLine}Restart the application{Environment.NewLine}{Environment.NewLine}if this issue persists contact the developer.";
        }

        public void StartLoading(string text = "")
        {
            loadingScreenAnimator.SetBool("Loading", true);
            loadingScreenAwayTimeText.text = text;
            loading = true;
        }
        
        public void StopLoading()
        {
            loadingScreenAnimator.SetBool("Loading", false);
            loading = false;
        }

        public void ActivateChildren()
        {
            for (var i = 0; i < loadingScreenAnimator.transform.childCount; i++)
                loadingScreenAnimator.transform.GetChild(i).gameObject.SetActive(true);
        }

        public void DeactivateChildren()
        {
            for (var i = 0; i < loadingScreenAnimator.transform.childCount; i++)
                loadingScreenAnimator.transform.GetChild(i).gameObject.SetActive(false);
        }
    }
}