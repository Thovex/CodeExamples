using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Inventories;
using BaerAndHoggo.UI;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Inventories
{

    [System.Serializable]
    public class CardInventory : Inventory<Card, CardStack, CardStackSaveData>
    {
        public InventoryUI inventoryUI;

        private bool Contains(Card obj, out int index)
        {
            index = -1;

            for (int i = 0; i < inventory.Count; i++)
            {
                if (inventory[i].Id == obj.Id)
                {
                    index = i;
                    break;
                }
            }

            return index != -1;
        }

        public override void Add(Card obj, bool forceUpdate = true)
        {

            bool contains = this.Contains(obj, out int index);

            if (!contains)
            {
                CardStack newStack = new CardStack(obj);

                inventory.Add(newStack);
                ConsoleProDebug.LogToFilter($"Created new Card Stack {newStack.Id} and added Card {obj.Name} to it.", "Inventory");
            }
            else
            {
                inventory[index].AddCard(obj);
                ConsoleProDebug.LogToFilter($"Added Card {obj.Name} to Card Stack {inventory[index].Id}", "Inventory");
            }

            if (forceUpdate)
            {
                UpdateUI();
            }
        }

        public override void Add(Card[] obj)
        {
            foreach (Card card in obj)
            {
                this.Add(card, false);
            }

            UpdateUI();
        }

        private void UpdateUI()
        {
            for (int i = inventory.Count - 1; i >= 0; i--)
            {
                if (inventory[i].CardCount == 0)
                {
                    inventory.RemoveAt(i);
                }
            }

            inventoryUI.UpdateUI(this);
        }


        public void Debug_AddRandomCard()
        {
            if (CardDB.Instance.GetRandomCard(out Card card))
            {
                this.Add(card);
            }
        }

        public void Debug_RemoveRandomCard()
        {
            List<CardStack> availableStacks = new List<CardStack>();

            foreach (CardStack cardStack in inventory)
            {
                if (cardStack.CardCount > 0)
                {
                    availableStacks.Add(cardStack);
                }
            }

            if (availableStacks.Count > 0)
            {
                availableStacks[Random.Range(0, availableStacks.Count)].RemoveCard();
                UpdateUI();
            }
        }

        public override void Remove(Card obj, int quantity = 1)
        {
            if (Contains(obj, out int index))
            {
                inventory[index].RemoveCard(quantity);
            }

            UpdateUI();
        }

        public override void LoadInventory(CardStackSaveData[] loadData)
        {
            this.inventory = new List<CardStack>();

            foreach (CardStackSaveData data in loadData)
            {
                this.inventory.Add(new CardStack(data));
            }

            UpdateUI();
        }
    }
}