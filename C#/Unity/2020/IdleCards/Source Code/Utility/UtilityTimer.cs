using System;
using UnityEngine;

namespace BaerAndHoggo.Utilities
{
    public static partial class Utility
    {
        public static string DefineTimer(double dSecs)
        {
            var secs = Convert.ToInt32(dSecs);
            var hours = secs / 3600;
            var mins = secs % 3600 / 60;
            secs = secs % 60;

            var timeValue = $"{secs:D2}";

            if (mins > 0) timeValue = $"{mins:D2}:{timeValue}";
            if (hours > 0) timeValue = $"{hours:D2}:{timeValue}";
            
            return timeValue;
        }

        public static TimeSpan ParseTimeSpan(this double value)
        {
            return TimeSpan.FromSeconds(value);
        }
    }
}