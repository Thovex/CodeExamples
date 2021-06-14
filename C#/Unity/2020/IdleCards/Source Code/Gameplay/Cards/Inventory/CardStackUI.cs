using System;
using BaerAndHoggo.Gameplay.Inventories;
using TMPro;
using UnityEngine;

namespace BaerAndHoggo.UI
{
    public class CardStackUI : MonoBehaviour
    {
        [SerializeField] private MasterCardUI cardPreview;
        [SerializeField] private CardStack cardStack;
        [SerializeField] private TMP_Text cardStackCounterText;

        public void SetCardStack(CardStack cardStack)
        {
            // Unsubscribe from existing stack if we reuse it.
            if (this.cardStack != null) this.cardStack.OnUpdatedStack -= UpdateCounter;

            this.cardStack = cardStack;
            cardStackCounterText.text = this.cardStack.CardCount.ToString();

            this.cardStack.OnUpdatedStack += UpdateCounter;

            cardPreview.SetNewCard(this.cardStack.PeekCard());
        }

        private void UpdateCounter(CardStackUpdateType updateType)
        {
            cardStackCounterText.text = cardStack.CardCount.ToString();
        }
    }
}