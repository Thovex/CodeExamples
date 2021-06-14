using System;
using System.Collections.Generic;
using BaerAndHoggo.SaveData;

namespace BaerAndHoggo.Gameplay.Time
{
    [Serializable]
    public struct CallerEventTypeId
    {
        public CallerEventType Type { get; set; }
        public long Id { get; set; }
    }
    
    [Serializable]
    public class TimeEventIO : IIO
    {
        public Dictionary<CallerEventTypeId, TimedEventIO[]> TimerData { get; set; }
    }
}