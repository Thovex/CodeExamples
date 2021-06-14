using System;
using System.Collections;
using System.Text.RegularExpressions;
using UnityEngine;
using UnityEngine.Networking;

namespace BaerAndHoggo.IO
{
    public enum FailReason
    {
        WorkerRunning,
        NetworkError,
        HttpError,
        RegexError
    }

    public static class WorldTimeApi
    {
        public delegate void GetTimeDelegate(DateTime currentDateTimeUtc);

        public delegate void WorkerFailDelegate(FailReason reason);

        private static readonly string RegexExpression =
            @"utc_datetime\:\s*(?'year'\d{4})\-(?'month'\d{2})\-(?'day'\d{2})T(?'hour'\d{2})\:(?'minute'\d{2})\:(?'second'\d+)\.(?'millisecond'\d+)\+00:00";

        private static Coroutine _worker;
        private static bool _workerActive;
        public static event GetTimeDelegate OnGetTime;
        public static event WorkerFailDelegate OnWorkerFail;

        public static void GetWorldDateTimeUtc(string apiUrl, MonoBehaviour monoBehaviour)
        {
            if (!_workerActive)
            {
                Debug.Log("Assigning worker.");
                _worker = monoBehaviour.StartCoroutine(FetchWorldDateTimeUtc(apiUrl, monoBehaviour));
            }
            else
            {
                Debug.LogError("Fetch Request Failed. (Worker already running!)");
                OnWorkerFail?.Invoke(FailReason.WorkerRunning);
            }
        }

        private static IEnumerator FetchWorldDateTimeUtc(string apiUrl, MonoBehaviour monoBehaviour)
        {
            _workerActive = true;

            Debug.Log($"Starting WebRequest access to {apiUrl}");
            var www = UnityWebRequest.Get(apiUrl);

            yield return www.SendWebRequest();

            if (www.isNetworkError)
            {
                Debug.LogError($"Fetch Request Failed. (Network Error to {apiUrl})");
                OnWorkerFail?.Invoke(FailReason.NetworkError);

                _workerActive = false;
                monoBehaviour.StopCoroutine(_worker);
            }

            if (www.isHttpError)
            {
                Debug.LogError($"Fetch Request Failed. (HTTP Error to {apiUrl})");
                OnWorkerFail?.Invoke(FailReason.HttpError);

                _workerActive = false;
                monoBehaviour.StopCoroutine(_worker);
            }

            var matches = Regex.Matches(www.downloadHandler.text, RegexExpression);

            if (matches.Count != 1)
            {
                Debug.LogError(
                    $"Fetch Request Failed. (Invalid amount of Regex Entries. Expected: 1 but got: {matches.Count})");
                OnWorkerFail?.Invoke(FailReason.RegexError);

                _workerActive = false;
                monoBehaviour.StopCoroutine(_worker);
            }

            var currentWorldDateTimeUtc = MatchToDateTime(matches[0]);

            Debug.Log($"Invoking OnGetTime with Parameter: {currentWorldDateTimeUtc}");
            OnGetTime?.Invoke(currentWorldDateTimeUtc);

            _workerActive = false;
            monoBehaviour.StopCoroutine(_worker);
        }

        private static DateTime MatchToDateTime(Match match)
        {
            return new DateTime(
                int.Parse(match.Groups["year"].ToString()), int.Parse(match.Groups["month"].ToString()),
                int.Parse(match.Groups["day"].ToString()), int.Parse(match.Groups["hour"].ToString()),
                int.Parse(match.Groups["minute"].ToString()), int.Parse(match.Groups["second"].ToString()));
        }
    }
}