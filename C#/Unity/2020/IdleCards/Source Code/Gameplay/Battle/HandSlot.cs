using System;
using BaerAndHoggo.Gameplay.Battle.Targeting;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.UI;
using Gameplay.Battle.Targetting;
using Sirenix.OdinInspector;
using UnityEngine;
using Type = BaerAndHoggo.Gameplay.Cards.Type;

namespace BaerAndHoggo.Gameplay.Battle
{
    public class HandSlot : TurnSlot
    {
        private MasterCardUI _masterCard;

        public void AddCardToHand(Card card)
        {
            this.Card = card;
            
            var masterCardObject = Instantiate(CardDB.Instance.GetMasterCardPrefab(), transform);
            _masterCard = masterCardObject.GetComponent<MasterCardUI>();
            _masterCard.SetNewCard(card);
            
            _masterCard.SetSize(_rectTransform.rect.size);


            switch (_masterCard.Card.type)
            {
                case Type.Minion:
                {
                    var minionCardDrag = masterCardObject.AddComponent<MinionSpawn>();

                    minionCardDrag.Initialize(new MinionSpawnData()
                    {
                        Player = Player,
                        DragAreaTransform =  dragArea,
                        OriginTransform = _rectTransform,
                    });
                    
                    minionCardDrag.OnPlay += CardPlay;
                    
                } break;
                case Type.Spell:
                {
                    switch (((CardSpell) _masterCard.Card).spellType)
                    {
                        case SpellType.Target:
                        {
                            var spellTargetCardDrag = masterCardObject.AddComponent<SpellTarget>();
                            
                            spellTargetCardDrag.Initialize(new SpellTargetData()
                            {
                                Player = Player,
                                DragAreaTransform =  dragArea,
                                OriginTransform = _rectTransform,
                                IncludeCaptain = false,
                            });

                            spellTargetCardDrag.OnPlay += CardPlay;

                        } break;
                        case SpellType.TargetWithHero:
                        {
                            var spellTargetWithHeroCardDrag = masterCardObject.AddComponent<SpellTarget>();
                            
                            spellTargetWithHeroCardDrag.Initialize(new SpellTargetData()
                            {
                                Player = Player,
                                DragAreaTransform =  dragArea,
                                OriginTransform = _rectTransform,
                                IncludeCaptain = true,
                            });

                            spellTargetWithHeroCardDrag.OnPlay += CardPlay;
                            
                        } break;
                        case SpellType.AllyBoard:
                        {
                            
                        } break;
                        case SpellType.EnemyBoard:
                        {
                            
                        } break;
                        case SpellType.AllyHero:
                        {
                            
                        } break;
                        case SpellType.EnemyHero:
                        {
                            
                        } break;
                    }
                } break;
            }
        }

        private void CardPlay()
        {
            this.Card = null;
            this.CanUse = false;
        }
    }
}