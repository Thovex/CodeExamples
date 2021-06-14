using System.Collections;
using System.Data;
using BaerAndHoggo.Gameplay.Battle;
using BaerAndHoggo.Gameplay.Battle.Targeting;
using BaerAndHoggo.Gameplay.Cards;
using DG.Tweening;
using UnityEngine;
using UnityEngine.EventSystems;

namespace Gameplay.Battle.Targetting
{
    [System.Serializable]
    public class MinionSpawnData : TargetData
    {
        public BattlePlayer Player { get; set; }
        public RectTransform OriginTransform { get; set; }
        public Transform DragAreaTransform { get; set; }
        
        public static MinionSpawnData operator+ (MinionSpawnData a, MinionSpawnData b)
        {
            var combinedData = a;
            
            if (!combinedData.Player) combinedData.Player = b.Player;
            if (!combinedData.OriginTransform) combinedData.OriginTransform = b.OriginTransform;
            if (!combinedData.DragAreaTransform) combinedData.DragAreaTransform = b.DragAreaTransform;

            return combinedData;
        }
    }
    
    public class MinionSpawn : TargetBaseLR<MinionSpawnData>
    {
        
        private Camera _mainCamera;
        private BoardSlot _targetSlot;
        private Coroutine _updater;
        
        protected override void Awake()
        {
            _mainCamera = Camera.main;
            
            Data = new MinionSpawnData();
            
            base.Awake();
        }
        
        public override void Initialize(MinionSpawnData data)
        {
            Data += data;

            TargetLineRendererEndColor = Color.cyan;
            
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
        
        private BoardSlot GetClosestSocket (BoardSlot[] slots )
        {
            BoardSlot bestTarget = null;
            var closestDistanceSqr = Mathf.Infinity;
            var currentPosition = transform.position;
            
            foreach(var potentialTarget in slots)
            {
                if (potentialTarget.Card) continue;
                
                var directionToTarget = potentialTarget.transform.position - currentPosition;
                var dSqrToTarget = directionToTarget.sqrMagnitude;

                // Highly debatable if this is a good solution lol
                if (currentPosition.y < -1) continue;

                if (!(dSqrToTarget < closestDistanceSqr)) continue;

                closestDistanceSqr = dSqrToTarget;
                bestTarget = potentialTarget;
                
                bestTarget.Highlight(true);

            }

            foreach (var potentialTarget in slots)
                potentialTarget.Highlight(potentialTarget == bestTarget);
            
            return bestTarget;
        }
        
        
        private IEnumerator Updater()
        {
            while (true)
            {
                _targetSlot = GetClosestSocket(Data.Player.boardSlots);

                if (_targetSlot)
                {
                    EnableLineRenderer();
                    SetTarget(transform.position, _targetSlot.transform.position, new Vector3(0, -2.5F, 0));
                }
                else
                {
                    DisableLineRenderer();
                }
                
                yield return new WaitForFixedUpdate();
            }
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

            if (_targetSlot)
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
            var selectedSocketTransform = _targetSlot.GetComponent<RectTransform>();

            DisableLineRenderer();

            var tween = Data.DragTransform.DOMove(selectedSocketTransform.position, .5F); 
            Data.MasterCardUI.SetSize(selectedSocketTransform.sizeDelta);
            
            tween.onComplete += () =>
            {
                Data.MasterCardUI.transform.SetParent(selectedSocketTransform, true);
                _targetSlot.SetCard(Data.MasterCardUI);

                CallOnPlay();
                
                Destroy(this);
            };
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