using System;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.UI
{
    [Serializable]
    public class TimerElement : IEquatable<TimerElement>
    {
        public RectTransform rectTransform;
        public Slider slider;
        public TMP_Text timer;

        public TimerElement( RectTransform rectTransform, Slider slider)
        {
            this.rectTransform = rectTransform;
            this.slider = slider;
        }

        public TimerElement(RectTransform rectTransform, TMP_Text timer)
        {
            this.rectTransform = rectTransform;
            this.timer = timer;
        }

        public TimerElement(RectTransform rectTransform, Slider slider, TMP_Text timer)
        {
            this.rectTransform = rectTransform;
            this.slider = slider;
            this.timer = timer;
        }

        public static bool operator ==(TimerElement a, TimerElement b)
        {
            return a.rectTransform == b.rectTransform;
        }

        public static bool operator !=(TimerElement a, TimerElement b)
        {
            return !(a == b);
        }

        public bool Equals(TimerElement other) => this == other;
        
        public override bool Equals(object obj)
        {
            if (obj.GetType() == this.GetType())
                return (TimerElement) obj == this;
            return false;
        }

        public override int GetHashCode()
        {
            return (rectTransform != null ? rectTransform.GetHashCode() : 0);
        }
    }
}