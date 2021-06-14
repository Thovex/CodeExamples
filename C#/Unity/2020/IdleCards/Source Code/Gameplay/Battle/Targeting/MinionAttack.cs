using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Effects;
using DG.Tweening;
using Gameplay.Effects;
using UnityEngine;
using UnityEngine.EventSystems;

namespace BaerAndHoggo.Gameplay.Battle.Targeting
{
    [System.Serializable]
    public class MinionAttackData : TargetData
    {
        public BattlePlayer Player { get; set; }
        public RectTransform OriginTransform { get; set; }
        public Transform DragAreaTransform { get; set; }
        public BoardSlot ThisSlot { get; set; }

        public static MinionAttackData operator +(MinionAttackData a, MinionAttackData b)
        {
            var combinedData = a;

            if (!combinedData.Player) combinedData.Player = b.Player;
            if (!combinedData.OriginTransform) combinedData.OriginTransform = b.OriginTransform;
            if (!combinedData.DragAreaTransform) combinedData.DragAreaTransform = b.DragAreaTransform;
            if (!combinedData.ThisSlot) combinedData.ThisSlot = b.ThisSlot;

            return combinedData;
        }
    }

    public class MinionAttack : TargetBaseLR<MinionAttackData>
    {
        private TurnSlot[] _targets;
        private TurnSlot _target;

        private Camera _mainCamera;
        private Coroutine _updater;

        protected override void Awake()
        {
            _mainCamera = Camera.main;

            Data = new MinionAttackData();

            base.Awake();
        }

        private void SetTargets(MinionAttackData data)
        {
            var opponentBoardSlots = data.Player.opponent.boardSlots;

            // 1 extra for the Core.
            _targets = new TurnSlot[opponentBoardSlots.Length + 1];

            // Grab captain
            _targets[0] = data.Player.opponent.captain;

            // Grab other board slots
            for (var i = 0; i < opponentBoardSlots.Length; i++)
            {
                _targets[i + 1] = opponentBoardSlots[i];
            }
        }

        public override void Initialize(MinionAttackData data)
        {
            Data += data;

            TargetLineRendererStartColor = new Color(0.50F, 0.25F, 0.25F, 1);
            TargetLineRendererEndColor = Color.red;

            SetTargets(data);

            base.Initialize(data);
        }
        
        private bool Check_IsTurn()
        {
            return Data.Player.isTurn;
        }

        private bool Check_CanUse()
        {
            if (Data.ThisSlot.CanUse)
                return true;

            return false;
        }

        protected override bool Checks_Implementation(bool beginPlay = false)
        {
            if (IsBusy) return false;
            if (!Check_IsTurn()) return false;
            if (!Check_CanUse()) return false;

            return true;
        }

        protected override void OnBeginDrag_Implementation(PointerEventData eventData)
        {
            IsBusy = true;

            transform.SetParent(Data.DragAreaTransform, true);
            transform.SetAsLastSibling();

            Data.MasterCardUI.SetSize(new Vector2(1080 / 2, 1920 / 2));
            var moveTween = Data.DragTransform.DOMove(Data.DragAreaTransform.position, .25F);

            moveTween.onComplete += () =>
            {
                if (!IsDragging)
                {
                    _target = null;
                    OnEndDrag_Implementation(eventData);
                }
                else
                {
                    EnableLineRenderer();
                }

                IsBusy = false;
            };

            if (_updater != null) StopCoroutine(_updater);
            _updater = StartCoroutine(Updater());
        }

        protected override void OnDrag_Implementation(PointerEventData eventData)
        {
        }

        private TurnSlot GetClosestTargetToInput(Vector3 currentPosition, TurnSlot[] slots)
        {
            TurnSlot bestTarget = null;
            var closestDistanceSqr = Mathf.Infinity;

            foreach (var potentialTarget in slots)
            {
                var directionToTarget = potentialTarget.transform.position - currentPosition;
                directionToTarget.z = 25;

                var dSqrToTarget = directionToTarget.sqrMagnitude;

                //if (_enemyTargetsData[potentialTarget])
                //{
                //    if (!_enemyTargetsData[potentialTarget].Card) continue;
                //}

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


                    SetTarget(transform.position, _target.transform.position, new Vector3(0, 2.5F, 0));
                }
                else
                {
                    DisableLineRenderer();
                }

                yield return new WaitForFixedUpdate();
            }
        }

        private void ToDefault()
        {
            StopCoroutine(_updater);

            DisableLineRenderer();

            Data.MasterCardUI.SetSize(Data.OriginTransform.sizeDelta);
        }

        protected override void OnEndDrag_Implementation(PointerEventData eventData)
        {
            if (_target)
            {
                ValidTargetAction();
            }
            else
            {
                ToDefault();
                InvalidTargetAction();
            }
        }

        private void ValidTargetAction()
        {
            Data.ThisSlot.CanUse = false;
            
            var minionCard = (CardMinion) Data.MasterCardUI.Card;
            var slot = _target.GetComponent<TurnSlot>();
            
            switch (minionCard.minionType)
            {
                case MinionType.Melee:
                {
                    ToDefault();
                    DoMeleeAttack(minionCard, slot);
                } break;
                case MinionType.Magic:
                {
                    ToDefault();
                    DoMagicAttack(minionCard, slot);
                } break;
                case MinionType.Ranged:
                {
                    DoRangedAttack(minionCard, slot);
                } break;
            }
        }

        private void DoMeleeAttack(CardMinion thisCard, TurnSlot turnSlot)
        {
            // Start Melee Attack Tween: A move from CurrentPos to EnemyTransform.
            
            var tweenAttackEnemy = Data.DragTransform.DOMove(_target.transform.position, .25F);
            tweenAttackEnemy.SetEase(Ease.InFlash);
            tweenAttackEnemy.onComplete += ()  =>
            {
                
                // On Completion scratch the enemy.
                Instantiate(EffectsDB.Instance.GetItemById(1).effectPrefab, _target.transform);

                
                // If the enemy is a slot on the board
                if (turnSlot)
                {
                    if (turnSlot.GetType() == typeof(BoardSlot))
                    {
                        // Scratch on this melee unit as well because both take damage.
                        Instantiate(EffectsDB.Instance.GetItemById(1).effectPrefab, Data.DragTransform);
                        ((CardMinion) turnSlot.Card).TakeDamage(thisCard);

                    } else if (turnSlot.GetType() == typeof(CaptainSlot))
                    {
                        Data.Player.opponent.deck.Captain.TakeDamage(thisCard.damage);
                    }
                }
                
                // Shake current Menu
                MenuManager.Instance.GetCurrentMenu().MenuShake(1, 15);


                var tweenBackToSlot = Data.DragTransform.DOMove(Data.OriginTransform.position, .25F);
                tweenBackToSlot.SetEase(Ease.OutCirc);
                tweenBackToSlot.onComplete += () =>
                {
                    Data.MasterCardUI.transform.SetParent(Data.OriginTransform, true);
                    IsBusy = false;
                };
                
            };
        }

        private void DoMagicAttack(CardMinion thisCard, TurnSlot turnSlot)
        {
            var fireball = Instantiate(EffectsDB.Instance.GetItemById(3).effectPrefab, Data.DragAreaTransform.transform);
            var fireballShootEffect = fireball.GetComponent<FireballShootEffect>();
            fireballShootEffect.Shoot(_target.transform.position, _target.transform);

            var tweenBackToSlot = Data.DragTransform.DOMove(Data.OriginTransform.position, .25F);
            tweenBackToSlot.SetEase(Ease.OutCirc);
            tweenBackToSlot.onComplete += () =>
            {
                if (turnSlot)
                {
                    if (turnSlot.GetType() == typeof(BoardSlot))
                    {
                        ((CardMinion) turnSlot.Card).TakeDamage(thisCard);
                    } else if (turnSlot.GetType() == typeof(CaptainSlot))
                    {
                        Data.Player.opponent.deck.Captain.TakeDamage(thisCard.damage);
                    }
                }

                MenuManager.Instance.GetCurrentMenu().MenuShake(1, 10);

                Data.MasterCardUI.transform.SetParent(Data.OriginTransform, true);
                IsBusy = false;
            };
        }

        private void DoRangedAttack(CardMinion thisCard, TurnSlot turnSlot)
        {

            var dragTransformPositionZMod = Data.DragTransform.position;
            dragTransformPositionZMod.z = 0;

            var targetTransformZMod = _target.transform.position;
            targetTransformZMod.z = 0;

            var tweenAttackEnemy = Data.DragTransform.DOLocalMove(
                ((dragTransformPositionZMod - targetTransformZMod).normalized * 200F), .5F);
            tweenAttackEnemy.onComplete += () =>
            {
                ToDefault();
                
                var arrow = Instantiate(EffectsDB.Instance.GetItemById(2).effectPrefab,
                    Data.DragAreaTransform.transform);
                var arrowShootEffect = arrow.GetComponent<ArrowShootEffect>();
                arrowShootEffect.Shoot(_target.transform.position, _target.transform);


                MenuManager.Instance.GetCurrentMenu().MenuPunch(.50F, new Vector3(0, 10, 0));
                
                var tweenBackToSlot = Data.DragTransform.DOMove(Data.OriginTransform.position, .25F);
                tweenBackToSlot.SetEase(Ease.OutCirc);
                tweenBackToSlot.onComplete += () =>
                {
                    if (turnSlot)
                    {
                        if (turnSlot.GetType() == typeof(BoardSlot))
                        {
                            ((CardMinion) turnSlot.Card).TakeDamage(thisCard);
                        } else if (turnSlot.GetType() == typeof(CaptainSlot))
                        {
                            Data.Player.opponent.deck.Captain.TakeDamage(thisCard.damage);
                        }
                    }

                    Data.MasterCardUI.transform.SetParent(Data.OriginTransform, true);
                    IsBusy = false;
                };
            };
        }

        private void InvalidTargetAction()
        {
            Data.MasterCardUI.SetSize(Data.OriginTransform.sizeDelta);

            var tween = Data.DragTransform.DOMove(Data.OriginTransform.position, .25F);
            tween.onComplete += () =>
            {
                Data.MasterCardUI.transform.SetParent(Data.OriginTransform, true);
                IsBusy = false;
            };
        }
    }
}