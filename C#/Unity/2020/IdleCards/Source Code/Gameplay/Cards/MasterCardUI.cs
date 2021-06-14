using System;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Utilities.Timeline;
using DG.Tweening;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.Serialization;
using Type = BaerAndHoggo.Gameplay.Cards.Type;

namespace BaerAndHoggo.UI
{
    [Serializable]
    public struct LargeSmallCard<T>
    {
         public T smallCard;
         public T largeCard;
    }

    public class MasterCardUI : SerializedMonoBehaviour
    {
        [SerializeField] public Card Card;
        
        [Title("Card References")] 
        [SerializeField] private LargeSmallCard<CardUI> cardUi;

        private Card _oldCard;

        [Title("Type Prefabs")] 
        [SerializeField] private Dictionary<Type, LargeSmallCard<GameObject>> prefabDictionary
            = new Dictionary<Type, LargeSmallCard<GameObject>>();

        [SerializeField] [Required] public RectTransform rectTransform;

        [Title("Swap small To large card width")] [SerializeField]
        private float transformMinWidth = 256;
        
        public void Awake()
        {
            cardUi = new LargeSmallCard<CardUI>();
        }

        private void Start()
        {
            SetSize(transform.parent.GetComponent<RectTransform>().rect.size);
        }

        public void SetSize(Vector2 size)
        {
            rectTransform.DOSizeDelta(size, .5F);
        }

        public void Explode()
        {
            var explodeTween = rectTransform.DOShakeRotation(.5F, 15F);
            explodeTween.onComplete += () =>
            {
                var canvasGroup = gameObject.AddComponent<CanvasGroup>();
                
                var fadeTween = canvasGroup.DOFade(0, .5F);
                fadeTween.onComplete += () =>
                {
                    Destroy(gameObject);
                };
            };
        }
        
        public void SetNewCard(Card card)
        {
            _oldCard = Card;
            
            if (_oldCard)
            {
                if(_oldCard.type == Type.Minion)
                    ((CardMinion) _oldCard).OnUpdate -= UpdateCardData;
            }
            
            Card = card;

            if (Card)
            {
                if (Card.type == Type.Minion)
                    ((CardMinion) Card).OnUpdate += UpdateCardData; 
            }

            if (!_oldCard || Card.type != _oldCard.type) {
                if (cardUi.smallCard)
                    DestroyImmediate(cardUi.smallCard.gameObject);
                    
                if (cardUi.largeCard)
                    DestroyImmediate(cardUi.largeCard.gameObject);

                var smallCard = Instantiate(prefabDictionary[Card.type].smallCard, transform);
                var largeCard = Instantiate(prefabDictionary[Card.type].largeCard, transform);

                cardUi.smallCard = smallCard.GetComponent<CardUI>();
                cardUi.largeCard = largeCard.GetComponent<CardUI>();
            }

            DetermineCardSize();
            UpdateCardData();
        }

        private void UpdateCardData()
        {
            if (!cardUi.smallCard || !cardUi.largeCard) return;
            cardUi.smallCard.UpdateCardData(Card);
            cardUi.largeCard.UpdateCardData(Card);
        }

        private void LateUpdate()
        {
            if (cardUi.smallCard && cardUi.largeCard) DetermineCardSize();
        }

        private void DetermineCardSize()
        {
            if (rectTransform.sizeDelta.x < transformMinWidth)
            {
                cardUi.largeCard.gameObject.SetActive(false);
                cardUi.smallCard.gameObject.SetActive(true);
            }
            else
            {
                cardUi.largeCard.gameObject.SetActive(true);
                cardUi.smallCard.gameObject.SetActive(false);
            }
        }
    }
}