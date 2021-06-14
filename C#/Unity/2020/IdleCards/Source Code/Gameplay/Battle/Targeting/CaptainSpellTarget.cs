using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.UI;
using DG.Tweening;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Battle.Targeting
{
    [Serializable]
    public class CaptainSpellTargetData : TargetData
    {
        public BattlePlayer Player { get; set; }
        public Transform OriginParent { get; set; }
        public Vector3 OriginPosition { get; set; }
        public Transform DragAreaTransform { get; set; }
        public CardSpell Spell { get; set; }
        public bool IncludeCaptain { get; set; }
        
        public static CaptainSpellTargetData operator+ (CaptainSpellTargetData a, CaptainSpellTargetData b)
        {
            var combinedData = a;
            
            combinedData.Player = b.Player;
            combinedData.IncludeCaptain = b.IncludeCaptain;
            combinedData.OriginParent = b.OriginParent;
            combinedData.OriginPosition = b.OriginPosition;
            combinedData.DragAreaTransform = b.DragAreaTransform;
            combinedData.Spell = b.Spell;

            return combinedData;
        }
    }
    
    public class CaptainSpellTarget : TargetBaseLR<CaptainSpellTargetData>
    {
        private TurnSlot[] _targets;
        private TurnSlot _target;

        private Camera _mainCamera;
        private Coroutine _updater;

        private Image _image;
        
        protected override void Awake()
        {
            _mainCamera = Camera.main;
            _image = GetComponent<Image>();
            
            Data = new CaptainSpellTargetData();
            
            base.Awake();
        }

        private void SetTargets(CaptainSpellTargetData data)
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

        public override void Initialize(CaptainSpellTargetData data)
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
            if (Data.Player.deck.Captain.mana >= Data.Player.deck.Captain.activeManaCost)
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
            _image.enabled = false;
            
            transform.SetParent(Data.DragAreaTransform, true);
            transform.SetAsLastSibling();
            
            if (_updater != null) StopCoroutine(_updater);
            _updater = StartCoroutine(Updater());
        }

        private IEnumerator Updater()
        {
            while (true)
            {
                
                var mousePosition = _mainCamera.ScreenToWorldPoint(Input.mousePosition);
                mousePosition.z = 25;

                _target = GetClosestTargetToInput(Data.DragTransform.position, _targets);

                if (_target)
                {
                    EnableLineRenderer();
                    SetTarget(transform.position, _target.transform.position, Vector3.zero);
                }
                else
                {
                    DisableLineRenderer();
                }
                
                yield return new WaitForFixedUpdate();
            }
        }
        
        protected TurnSlot GetClosestTargetToInput (Vector3 currentPosition, TurnSlot[] transforms)
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
                if (currentPosition.y < -3) continue;
                
                if (!(dSqrToTarget < closestDistanceSqr)) continue;

                closestDistanceSqr = dSqrToTarget;
                bestTarget = potentialTarget;
                
            }

            return bestTarget;
        }

        protected override void OnDrag_Implementation(PointerEventData eventData)
        {
            eventData.selectedObject = Data.DragTransform.gameObject;
            
            var newLocation = _mainCamera.ScreenToWorldPoint(eventData.position);
            newLocation.z = Data.DragTransform.position.z;

            Data.DragTransform.position = newLocation;
        }
        
        protected override void OnEndDrag_Implementation(PointerEventData eventData)
        {
            DisableLineRenderer();
            
            StopCoroutine(_updater);

            if (_target)
            {
                Data.Player.deck.Captain.mana -= Data.Player.deck.Captain.activeManaCost;
                Data.Player.UpdateUI();
                
                ValidTargetAction();
            }
            else
            {
                InvalidTargetAction();
            }

            transform.SetParent(Data.OriginParent, true);
            transform.position = Data.OriginPosition;

            _image.enabled = true;
        }

        private void ValidTargetAction()
        {

            if (_target.GetType() == typeof(BoardSlot))
            { 
                Data.Spell.DoSpellMinion((CardMinion)((BoardSlot)_target).Card);
            } else if (_target.GetType() == typeof(CaptainSlot))
            {
                Data.Spell.DoSpellCaptain((CardCaptain)((CaptainSlot)_target).Card);
            }
        }

        private void InvalidTargetAction()
        {

        }
    }
}