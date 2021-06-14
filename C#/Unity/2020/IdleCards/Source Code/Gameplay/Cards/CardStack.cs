using System;
using System.Linq;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using UnityEngine;
using BaerAndHoggo.SaveData;

namespace BaerAndHoggo.Gameplay.Inventories
{
    [Serializable]
    public class CardStackSaveData
    {
        public long Id { get; set; }
        public string Name { get; set; }
        public Cards.Type Type { get; set; }
        public CardSaveData[] Stack { get; set; }

    }

    [System.Serializable]
    public class CardStack : ISaveData<CardStackSaveData>
    {
        [SerializeField] public long Id;
        [SerializeField] public string Name;
        [SerializeField] public Cards.Type Type;
        [SerializeField] public List<Card> Stack;

        public CardStackSaveData ToSaveData()
        {
            CardStackSaveData saveData = new CardStackSaveData();
            saveData.Id = this.Id;
            saveData.Name = this.Name;
            saveData.Type = this.Type;

            CardSaveData[] stack = new CardSaveData[Stack.Count];

            for (int i = 0; i < Stack.Count; i++)
            {
                Card card = (Card)Stack[i];
                switch (card.Type)
                {
                    case Cards.Type.Minion:
                        {
                            CardMinion minion = (CardMinion)card;
                            stack[i] = minion.ToSaveData();
                        }
                        break;
                    case Cards.Type.Spell:
                        {
                            CardSpell spell = (CardSpell)card;
                            stack[i] = spell.ToSaveData();
                        }
                        break;
                    default:
                        {
                            Debug.LogError($"Invalid SaveData Configuration for Card Type {card.Type}");
                        }
                        break;
                }
            }

            saveData.Stack = stack;
            return saveData;
        }

        public CardStack(CardStackSaveData loadData)
        {
            this.Id = loadData.Id;
            this.Name = loadData.Name;
            this.Type = loadData.Type;

            this.Stack = new List<Card>();

            for (int i = 0; i < loadData.Stack.Length; i++)
            {
                CardSaveData cardSaveData = (CardSaveData)loadData.Stack[i];
                switch (loadData.Type)
                {
                    case Cards.Type.Minion:
                        {
                            CardMinion newCardMinion = ScriptableObject.CreateInstance<CardMinion>();
                            newCardMinion.LoadData((CardMinionSaveData)cardSaveData);
                            this.Stack.Add(newCardMinion);
                        }
                        break;
                    case Cards.Type.Spell:
                        {
                            CardSpell newCardSpell = ScriptableObject.CreateInstance<CardSpell>();
                            newCardSpell.LoadData((CardSpellSaveData)cardSaveData);
                            this.Stack.Add(newCardSpell);
                        }
                        break;
                    default:
                        {
                            Debug.LogError($"Invalid SaveData Configuration for Card Type {loadData.Type}");
                        }
                        break;
                }
            }
        }

        public CardStack(Card card)
        {
            this.Id = card.Id;
            this.Name = card.Name;
            this.Stack = new List<Card>();
            this.Type = card.Type;

            this.Stack.Add(card);
        }

        public void AddCard(Card card)
        {
            if (Id != card.Id)
            {
                Debug.LogError($"Stack id ({Id}) not equal to incoming card id ({card.Id})");
            }

            this.Stack.Add(card);
        }

        public int CardCount => Stack.Count;

        public void RemoveCard(int quantity = 1)
        {
            if (this.CardCount <= quantity)
            {
                Debug.LogWarning($"Removing more cards {quantity} than available in stack {this.Id}");
                this.Stack.Clear();
            }
            else
            {
                for (int i = 0; i < quantity; i++)
                {
                    this.Stack.RemoveAt(this.CardCount - 1);
                }
            }
        }

        public Card PopCard()
        {
            if (this.CardCount == 0)
            {
                new System.Exception($"Trying to pop a card from stack {this.Id}. But none are available.");
                return null;
            }

            Card card = this.Stack[this.CardCount];
            this.Stack.RemoveAt(this.CardCount);
            return card;
        }
    }
}