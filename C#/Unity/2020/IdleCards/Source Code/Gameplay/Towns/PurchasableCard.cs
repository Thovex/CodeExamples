using System;
using System.Globalization;
using Gameplay.Currency;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Towns
{
    public class PurchasableCard : TownBuildingCard
    {
        [SerializeField] private Button purchaseButton;
        
        [SerializeField] private TMP_Text purchaseCostTextRef;

        public override Button GetUseButton() => purchaseButton;

        protected override void InitializeCard()
        {
            purchaseCostTextRef.text =
                $"<color=green>Purchasable</color> {Environment.NewLine}" +
                $"<b>{Building.cost.AsCurrency(CultureInfo.GetCultureInfo("nl-NL"))}</b> {Building.currency}";
        }
    }
}