using System;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Battle;
using BaerAndHoggo.Gameplay.Inventories;
using BaerAndHoggo.IO;
using BaerAndHoggo.UI;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Cards
{
    public class BattlePreparationUI : ManagedMenu
    {
        [SerializeField] private CardInventoryUI inventoryUI;
        [SerializeField] private BattleUI battleUI;
        
        [SerializeField] private CardSlot selectedSlot;
        [SerializeField] private List<CardSlot> slots = new List<CardSlot>();

        protected override void Awake()
        {
            base.Awake();

            var cardSlots = GetComponentsInChildren<CardSlot>();

            foreach (var slot in cardSlots)
            {
                slots.Add(slot);
                slot.gameObject.GetComponent<Button>().onClick.AddListener(delegate { ClickCardSlot(slot); });
            }
        }

        public void Reinitialize()
        {
            selectedSlot = null;

            // Remove all card children on reinitialization.
            foreach (var slot in slots)
            {
                if (slot.transform.childCount <= 0) continue;
                
                for (var i = 0; i < slot.transform.childCount; i++)
                    Destroy(slot.transform.GetChild(i).gameObject);
            }
        }

        public void ClickCardSlot(CardSlot slot)
        {
            // Check if slot already has a card under it's belt.
            if (slot.inUse)
                return;
            
            // Select this slot.
            selectedSlot = slot;
            
            // Open Inventory
            MenuManager.Instance.NavigateMenu(inventoryUI, true);
            
            // Remove any existing listeners on select and add this Selection
            inventoryUI.selectCardButtonRef.onClick.RemoveAllListeners();
            inventoryUI.selectCardButtonRef.onClick.AddListener(SelectCard);
        }

        private void SelectCard()
        {
            inventoryUI.selectCardButtonRef.onClick.RemoveAllListeners();
            var card = CardInventory.Instance.inventory[inventoryUI.selectedStackIndex].PeekCard(false, false, true);

            CardInventory.Instance.inventory[inventoryUI.selectedStackIndex].SelectPeek();

            selectedSlot.SetCard(card);
            

            MenuManager.Instance.Return();
        }

        public void StartBattle()
        {
            LoadingManager.Instance.StartLoading("Preparing Battle");
            MenuManager.Instance.NavigateMenu(battleUI);
            
            CardCaptain captain = null;
            var cards = new List<Card>();

            foreach (var slot in slots)
            {
                if (slot.currentCard == null)
                    continue;
                if (slot.captainSlot)
                    captain = (CardCaptain) slot.currentCard;
                else
                    cards.Add(slot.currentCard);
            }

            //if (captain == null) return;
            
            var playerDeck = new CardDeck(captain, cards);

            battleUI.InitializeNewBattle(new BattleData
            {
                PlayerDeck = playerDeck,
                EnemyAI = AiDB.Instance.GetItemById(1),
            });
        }
    }
}