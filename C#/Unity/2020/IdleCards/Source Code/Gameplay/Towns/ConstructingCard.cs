using System;
using BaerAndHoggo.UI;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Towns
{
    public class ConstructingCard : TownBuildingCard
    {
        public override Button GetUseButton() => null;

        [SerializeField] private Slider constructionTimerSliderRef;
        [SerializeField] private TMP_Text constructionTimerRef;

        protected override void InitializeCard()
        {
            TimerUI.Instance.AddTimerElement(
                new TimerElement(
                    constructionTimerRef.GetComponent<RectTransform>(), constructionTimerRef), 
                EventCaller);
            
            TimerUI.Instance.ForceUpdate(EventCaller);
        }
    }
}