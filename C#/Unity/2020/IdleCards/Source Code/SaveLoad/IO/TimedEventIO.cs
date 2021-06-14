using System;
using BaerAndHoggo.SaveData;

namespace BaerAndHoggo.Gameplay.Time
{
    [Serializable]
    public abstract class TimedEventIO : IIO
    {
        public int Multiplier { get; set; }
        public bool Pause { get; set; }
        public double Seconds { get; set; }
        public double Duration { get; set; }
        public string Type { get; set; }
    }
}