using System;
using BaerAndHoggo.UI;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Towns
{
    public class FinishedCard : TownBuildingCard
    {
        [SerializeField] private TMP_Text currentActionTimerRef;
        [SerializeField] private Button openBuildingButton;
        [SerializeField] private Slider currentActionRef;
        [SerializeField] private Image buildingColorBackgroundRef;
        [SerializeField] protected Image buildingImage;


        public override Button GetUseButton() => openBuildingButton;

        protected override void InitializeCard()
        {
            buildingImage.sprite = Building.image;
            
            TimerUI.Instance.AddTimerElement(
                new TimerElement(
                    currentActionRef.GetComponent<RectTransform>(), currentActionRef, currentActionTimerRef),
                EventCaller);
            
            TimerUI.Instance.ForceUpdate(EventCaller);

            buildingColorBackgroundRef.color = Building.backgroundColor;
        }
    }
}