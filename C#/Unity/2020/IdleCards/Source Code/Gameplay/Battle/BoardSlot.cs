using BaerAndHoggo.Gameplay.Battle.Targeting;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Effects;
using BaerAndHoggo.UI;
using DG.Tweening;
using UnityEngine;
using UnityEngine.Serialization;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Battle
{
    public class BoardSlot : TurnSlot
    {

        [SerializeField] private Image highlightImage;
        [SerializeField] public bool highlighted = false;
        private Color _originalColor;
        private Tween _colorTween;
        
        private MasterCardUI _masterCard;
        
        private void Awake() {
            _originalColor = highlightImage.color;
        }

        public void SetCard(MasterCardUI cardUi)
        {
            _masterCard = cardUi;

            if (_masterCard)
            {
                Highlight(false);

                CanUse = false;
                Card = cardUi.Card;

                var cardMinion = (CardMinion) Card;

                cardMinion.OnDefenseBreak += DefenseBreak;
                cardMinion.OnDie += Die;

                var battleTarget = _masterCard.gameObject.AddComponent<MinionAttack>();
                //battleTarget.Initialize(Player.opponent, dragArea, _rectTransform, this);

                
                Debug.Log("Intiialized for: " + this.gameObject.name);
                battleTarget.Initialize(new MinionAttackData()
                    {
                        Player = Player,
                        DragAreaTransform = dragArea,
                        OriginTransform = _rectTransform,
                        ThisSlot = this,
                    }
                );
            }
            else
            {
                Card = null;
                CanUse = false;
            }
        }

        private void DefenseBreak()
        {
            Instantiate(EffectsDB.Instance.GetItemById(5).effectPrefab, _masterCard.transform);
        }

        private void Die()
        {
            ((CardMinion) Card).OnDie -= Die;
            _masterCard.Explode();
            SetCard(null);
        }

        public void Highlight(bool enabled)
        {
            highlighted = enabled;
            
            if (enabled)
            {
                if (_colorTween != null && _colorTween.IsPlaying()) _colorTween.Kill();
                _colorTween = highlightImage.DOColor(Color.white, .5F);
            }
            else
            {
                if (_colorTween != null && _colorTween.IsPlaying()) _colorTween.Kill();
                _colorTween = highlightImage.DOColor(_originalColor, .5F);
            }
        }
    }
}