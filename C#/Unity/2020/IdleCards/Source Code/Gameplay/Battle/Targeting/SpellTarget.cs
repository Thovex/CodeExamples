using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.UI;
using DG.Tweening;
using UnityEngine;
using UnityEngine.EventSystems;

namespace BaerAndHoggo.Gameplay.Battle.Targeting
{
    [Serializable]
    public class SpellTargetData : TargetData
    {
        public BattlePlayer Player { get; set; }
        public RectTransform OriginTransform { get; set; }
        public Transform DragAreaTransform { get; set; }
        public bool IncludeCaptain { get; set; }

        public static SpellTargetData operator+ (SpellTargetData a, SpellTargetData b)
        {
            var combinedData = a;
            
            if (!combinedData.Player) combinedData.Player = b.Player;
            if (!combinedData.OriginTransform) combinedData.OriginTransform = b.OriginTransform;
            if (!combinedData.DragAreaTransform) combinedData.DragAreaTransform = b.DragAreaTransform;
            if (!combinedData.MasterCardUI) combinedData.MasterCardUI = b.MasterCardUI;
            
            combinedData.IncludeCaptain = b.IncludeCaptain;

            return combinedData;
        }
    }
    
    public class SpellTarget : TargetBaseLR<SpellTargetData>
    {
        private TurnSlot[] _targets;
        private TurnSlot _target;

        private Camera _mainCamera;
        private Coroutine _updater;
        
        protected override void Awake()
        {
            _mainCamera = Camera.main;
            
            Data = new SpellTargetData();
            
            base.Awake();
        }
        
        private void SetTargets(SpellTargetData data)
        {
            var playerBoardSlots = data.Player.boardSlots;
            var opponentBoardSlots = data.Player.opponent.boardSlots;
            var combinedSlots = playerBoardSlots.Concat(opponentBoardSlots).ToArray();
            
            var targetsList = combinedSlots.Cast<TurnSlot>().ToList();
            
            if (data.IncludeCaptain)
            {
                targetsList.Add(data.Player.captain);
                targetsList.Add(data.Player.opponent.captain);
            }

            _targets = targetsList.ToArray();

        }

        public override void Initialize(SpellTargetData data)
        {
            Data += data;

            TargetLineRendererStartColor = new Color(0.25F, 0.25F, 0.50F, 1);
            TargetLineRendererEndColor = Color.blue;

            SetTargets(data);
            
            base.Initialize(data);
        }
        
        private bool Check_IsTurn()
        {
            return Data.Player.isTurn;
        }

        private bool Check_EnoughMana(bool beginPlay = false)
        {
            if (Data.Player.deck.Captain.mana >= Data.MasterCardUI.Card.manaCost)
                return true;

            if (beginPlay)
            {
                var tweenManaText = Data.Player.manaTextRef.transform.DOPunchScale(Vector3.one * 1.01F, .25F, 5, 0.5F);
                tweenManaText.onComplete += () =>
                {
                    Data.Player.manaTextRef.transform.localScale = Vector3.one;
                };
            }

            return false;
        }
        
        protected override bool Checks_Implementation(bool beginPlay = false)
        {
            if (!Check_IsTurn()) return false;
            if (!Check_EnoughMana(beginPlay)) return false;
            return true;
        }

        protected override void OnBeginDrag_Implementation(PointerEventData eventData)
        {
            transform.SetParent(Data.DragAreaTransform, true);
            transform.SetAsLastSibling();

            Data.MasterCardUI.SetSize(new Vector2(1080 / 2, 1920 / 2));
            
            if (_updater != null) StopCoroutine(_updater);
            _updater = StartCoroutine(Updater());
        }

        private IEnumerator Updater()
        {
            while (true)
            {
                
                var mousePosition = _mainCamera.ScreenToWorldPoint(Input.mousePosition);
                mousePosition.z = 25;

                _target = GetClosestTargetToInput(mousePosition, _targets);

                if (_target)
                {
                    EnableLineRenderer();
                    SetTarget(transform.position, _target.transform.position, new Vector3(0, -2.5F, 0));
                }
                else
                {
                    DisableLineRenderer();
                }
                
                yield return new WaitForFixedUpdate();
            }
        }
        
        private TurnSlot GetClosestTargetToInput (Vector3 currentPosition, TurnSlot[] transforms)
        {
            TurnSlot bestTarget = null;
            var closestDistanceSqr = Mathf.Infinity;
            
            foreach(var potentialTarget in transforms)
            {
                var directionToTarget = potentialTarget.transform.position - currentPosition;
                directionToTarget.z = 25;
                
                var dSqrToTarget = directionToTarget.sqrMagnitude;

                if (potentialTarget.GetType() == typeof(BoardSlot))
                {
                    var slot = (BoardSlot) potentialTarget;
                    if (!slot.Card) continue;
                }
                
                // Highly debatable if this is a good solution lol
                if (currentPosition.y < -1) continue;
                
                if (!(dSqrToTarget < closestDistanceSqr)) continue;

                closestDistanceSqr = dSqrToTarget;
                bestTarget = potentialTarget;
                
            }

            return bestTarget;
        }

        protected override void OnDrag_Implementation(PointerEventData eventData)
        {
            var newLocation = _mainCamera.ScreenToWorldPoint(eventData.position);
            newLocation.z = Data.DragTransform.position.z;
            
            Data.DragTransform.position = Vector3.Lerp(Data.DragTransform.position, newLocation, 0.25F);
            Data.DragTransform.localPosition = new Vector3(Data.DragTransform.localPosition.x, Data.DragTransform.localPosition.y + 120, 0);
        }
        
        protected override void OnEndDrag_Implementation(PointerEventData eventData)
        {
            DisableLineRenderer();
            
            StopCoroutine(_updater);

            if (_target)
            {
                Data.Player.deck.Captain.mana -= Data.MasterCardUI.Card.manaCost;
                Data.Player.UpdateUI();

                ValidTargetAction();
            }
            else
            {
                InvalidTargetAction();
            }
        }

        private void ValidTargetAction()
        {

            if (_target.GetType() == typeof(BoardSlot))
            {
                ((CardSpell) Data.MasterCardUI.Card).DoSpellMinion((CardMinion)((BoardSlot)_target).Card);
            } else if (_target.GetType() == typeof(CaptainSlot))
            {
                ((CardSpell) Data.MasterCardUI.Card).DoSpellCaptain((CardCaptain)((CaptainSlot)_target).Card);
            }

            Data.MasterCardUI.SetSize(Vector3.zero);
            Data.MasterCardUI.Explode();

            CallOnPlay();
        }

        private void InvalidTargetAction()
        {
            Data.MasterCardUI.SetSize(Data.OriginTransform.sizeDelta);
                
            var tween = Data.DragTransform.DOMove(Data.OriginTransform.position, .25F);
            tween.onComplete += () =>
            {
                Data.MasterCardUI.transform.SetParent(Data.OriginTransform, true);
            };
        }
    }
}