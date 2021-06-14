using System;
using BaerAndHoggo.UI;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public class CardSlot : MonoBehaviour
    {
        [SerializeField] public bool captainSlot = false;
        [SerializeField] public bool inUse = false;
        [SerializeField] public Card currentCard;

        [SerializeField] private GameObject cardGameObject;

        public void SetCard(Card card)
        {
            currentCard = card;
            inUse = currentCard;

            if (!inUse)
            {
                if (cardGameObject)
                {
                    Destroy(cardGameObject);
                    return;
                }
            }

            if (!gameObject.activeInHierarchy)
                gameObject.SetActive(true);

            cardGameObject = Instantiate(CardDB.Instance.GetMasterCardPrefab(), transform);
            var masterCardUI = cardGameObject.GetComponent<MasterCardUI>();
            
            masterCardUI.SetNewCard(currentCard);
            masterCardUI.SetSize(GetComponent<RectTransform>().sizeDelta);
        }
    }
}