using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Battle
{
    [Serializable]
    public class BattlePlayer : MonoBehaviour
    {
        [Title("References")]
        [SerializeField] public TMP_Text healthTextRef;
        [SerializeField] public TMP_Text manaTextRef;
        [SerializeField] public TMP_Text deckTextRef;
        [SerializeField] public Image captainPortraitRef;

        [SerializeField] private Transform handSlotParent;
        [SerializeField] private Transform boardSlotParent;

        [SerializeField] public HandSlot[] handSlots;
        [SerializeField] public BoardSlot[] boardSlots;

        [SerializeField] private CaptainSkill activeSkill;
        
        [SerializeField] public bool isAi;

        [SerializeField] public BattlePlayer opponent;
        
        [Title("Player State")]
        [SerializeField] public CaptainSlot captain;
        [SerializeField] public CardDeck deck;

        [SerializeField] public bool isTurn = false;
        [SerializeField] public bool isReady = false;

        public delegate void SetReadyDelegate();
        public event SetReadyDelegate OnReady;
        
        public void InitPlayer(CardDeck deck, BattlePlayer opponent)
        {
            RemoveAnyExistingChildren(handSlots);
            RemoveAnyExistingChildren(boardSlots);

            this.opponent = opponent;

            foreach (var handSlot in handSlots)
                handSlot.Player = this;
            
            foreach (var boardSlot in boardSlots)
                boardSlot.Player = this;
            
            this.deck = deck;
            this.deck.CopyDeckAndSetMods();
            
            activeSkill.Initialize(this);

            captain.Card = this.deck.Captain;
            this.deck.Captain.OnUpdate += UpdateUI;

            UpdateUI();
        }

        public void SetTurn(int turnIndex)
        {
            isTurn = true;

            deck.Captain.mana = 
                (turnIndex * deck.Captain.manaGrowth) >= deck.Captain.maxMana ? 
                    deck.Captain.maxMana : (turnIndex * deck.Captain.manaGrowth);
            
            foreach (var boardSlot in boardSlots)
                boardSlot.CanUse = true;
            
            foreach (var handSlot in handSlots)
                handSlot.CanUse = true;

            if (DrawCard())
            {
                // Maybe warn player no space in hand.
            }
            
            UpdateUI();

        }

        public bool DrawCard(bool forceCanUse = false)
        {
            foreach (var handSlot in handSlots)
            {
                if (!handSlot.Card)
                {
                    try
                    {
                        handSlot.AddCardToHand(deck.DrawCard());
                        handSlot.CanUse = forceCanUse;
                    
                        return true;
                    }
                    catch (InvalidOperationException)
                    {
                        return false;
                    }
                }
            }

            return false;
        }

        public void EndTurn()
        {
            isTurn = false;

            foreach (var boardSlot in boardSlots)
                boardSlot.CanUse = false;

            foreach (var handSlot in handSlots)
                handSlot.CanUse = false;
        }

        protected void RemoveAnyExistingChildren(IEnumerable<Component> array)
        {
            foreach (var handSlot in array)
                if (handSlot.transform.childCount > 0)
                    for (var i = handSlot.transform.childCount - 1; i > 0; i--)
                        Destroy(handSlot.transform.GetChild(i));
        }

        public void UpdateUI()
        {
            healthTextRef.text = deck.Captain.hp.ToBetterWorseText(deck.Captain.startHp, "#00ff00ff", "#ff0000ff");
            manaTextRef.text = $"{deck.Captain.mana.ToString()}/{deck.Captain.maxMana.ToString()}";
            deckTextRef.text = deck.GetDeckCount().ToString();

            captainPortraitRef.sprite = deck.Captain.portrait;
        }

        public virtual void OnReadyInvocator()
        {
            isReady = true;
            
            OnReady?.Invoke();
        }
    }
}