using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.IO;
using BaerAndHoggo.UI;
using Sirenix.OdinInspector;
using UnityEngine;
using Random = UnityEngine.Random;

namespace BaerAndHoggo.Gameplay.Inventories
{
    [Serializable]
    public class CardInventory : Inventory<Card, CardStack, CardStackIO[]>
    {
         public new static CardInventory Instance => (CardInventory)GetInstance();

        public override void RegisterInventoryUI(CardInventoryUI cardInventory)
        {
            if (!inventoryUi.Contains(cardInventory))
            {
                inventoryUi.Add(cardInventory);
                cardInventory.UpdateUi();
            }
        }

        private void Awake()
        {
            SaveManager.Instance.OnLoad += LoadInventory;
        }

        public void UnSelectPeekAll()
        {
            foreach (var stack in inventory)
                stack.RemoveAllPeek();
        }

        public override void LoadInventory(IO.SaveData saveData)
        {
            OnLoad_Implementation(saveData.CardStackIO);
        }
        
        public override CardStackIO[] OnSave_Implementation()
        {
            var inventoryData = new CardStackIO[inventory.Count];
            
            for (int i = 0; i < inventory.Count; i++)
            {
                inventoryData[i] = inventory[i].OnSave_Implementation();
            }

            return inventoryData;
        }

        public override void OnLoad_Implementation(CardStackIO[] loadData)
        {
            Debug.Log("Loading inventory!");

            inventory = new List<CardStack>();

            foreach (var data in loadData)
                inventory.Add(new CardStack(data));

            UpdateUi();
            Debug.Log("Finished loading inventory!");
        }

        public bool Contains(Card obj, out int index)
        {
            index = -1;

            for (var i = 0; i < inventory.Count; i++)
                if (inventory[i].id == obj.id)
                {
                    index = i;
                    break;
                }

            return index != -1;
        }

        public override void Add(Card obj, bool forceUpdate = true)
        {
            var contains = Contains(obj, out var index);

            if (!contains)
            {
                var newStack = new CardStack(obj);

                inventory.Add(newStack);
                Debug.Log($"Created new Card Stack {newStack.id} and added Card {obj.cardName} to it.");
            }
            else
            {
                inventory[index].AddCard(obj);
                Debug.Log($"Added Card {obj.cardName} to Card Stack {inventory[index].id}");
            }

            if (forceUpdate) UpdateUi();
        }

        public override void Add(Card[] obj)
        {
            foreach (var card in obj) Add(card, false);

            UpdateUi();
        }

        protected override void UpdateUi()
        {
            for (var i = inventory.Count - 1; i >= 0; i--)
                if (inventory[i].CardCount == 0)
                    inventory.RemoveAt(i);

            foreach (var inventory in inventoryUi) inventory.UpdateUi();
        }


        [Button]
        public void Debug_AddRandomCard()
        {
            if (CardDB.Instance.GetRandomCard(out var card)) Add(card);
        }


        [Button]
        public void Debug_RemoveRandomCard()
        {
            var availableStacks = inventory.Where(cardStack => cardStack.CardCount > 0).ToList();

            if (availableStacks.Count <= 0) return;
            
            availableStacks[Random.Range(0, availableStacks.Count)].RemoveCard();
            UpdateUi();
        }

        public override void Remove(Card obj, int quantity = 1)
        {
            if (Contains(obj, out var index)) inventory[index].RemoveCard(quantity);

            UpdateUi();
        }
    }
}