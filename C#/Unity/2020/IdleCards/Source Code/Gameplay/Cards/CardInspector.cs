using BaerAndHoggo.UI;
using BaerAndHoggo.Utilities;
using UnityEngine;
using UnityEngine.Serialization;

namespace BaerAndHoggo.Gameplay.Cards
{
    public class CardInspector : Singleton<CardInspector>
    {
        [SerializeField] private GameObject activeObject;
        [SerializeField] private MasterCardUI masterUi;

        public void ShowInspection(Card card)
        {
            activeObject.SetActive(true);

            if (card.GetType() == typeof(CardMinion)) masterUi.SetNewCard((CardMinion) card);
        }

        public void CloseInspection()
        {
            activeObject.SetActive(false);
        }
    }
}