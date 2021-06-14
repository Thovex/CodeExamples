using System;

namespace BaerAndHoggo.Gameplay.Time
{
    [Serializable]
    public class ConstructionEventIO : TimedEventIO
    {
        public long Building { get; set; }
    }
}