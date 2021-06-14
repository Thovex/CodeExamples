using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.SaveData;
using UnityEngine;
using UnityEngine.Serialization;
using Type = BaerAndHoggo.Gameplay.Cards.Type;

namespace BaerAndHoggo.Gameplay.Inventories
{
    public enum CardStackUpdateType
    {
        Add,
        Remove,
        Pop,
    }

    [Serializable]
    public class CardAvailability
    {
        [SerializeField] private Card card;
        [SerializeField] private bool availability;

        public CardAvailability(Card card, bool availability)
        {
            this.Card = card;
            this.availability = availability;
        }

        public Card Card
        {
            get => this.card;
            set => card = value;
        }

        public bool Availability
        {
            get => this.availability;
            set => availability = value;
        }

    }

    [Serializable]
    public class CardStack : ISaveData<CardStackIO>
    {
        public delegate void UpdatedStack(CardStackUpdateType type);

         [SerializeField] public long id;
         [SerializeField] public string name;
         [SerializeField] public List<CardAvailability> stack = new List<CardAvailability>();
         [SerializeField] public Type type;

         public int currentPeekIndex = -1;

        public CardStack(CardStackIO loadData)
        {
            OnLoad_Implementation(loadData);
        }
        
        public CardStackIO OnSave_Implementation()
        {
            var saveData = new CardStackIO
            {
                Id = id, 
                Name = name, 
                Type = type
            };

            var saveStack = new CardIO[stack.Count];

            for (var i = 0; i < stack.Count; i++)
            {
                var card = stack[i].Card;
                
                switch (card.type)
                {
                    case Type.Minion: saveStack[i] = ((CardMinion) card).OnSave_Implementation(); break;
                    case Type.Spell: saveStack[i] = ((CardSpell) card).OnSave_Implementation(); break;
                }
            }
            
            saveData.Stack = saveStack;
            return saveData;
        }

        public void OnLoad_Implementation(CardStackIO loadData)
        {
            id = loadData.Id;
            name = loadData.Name;
            type = loadData.Type;

            stack = new List<CardAvailability>();

            foreach (var cardSaveData in loadData.Stack)
            {
                switch (loadData.Type)
                {
                    case Type.Minion:
                    {
                        var newCardMinion = ScriptableObject.CreateInstance<CardMinion>();
                        newCardMinion.OnLoad_Implementation((CardMinionIO) cardSaveData);
                        stack.Add(new CardAvailability(newCardMinion, true));
                    } break;
                    case Type.Spell:
                    {
                        var newCardSpell = ScriptableObject.CreateInstance<CardSpell>();
                        newCardSpell.OnLoad_Implementation((CardSpellIO) cardSaveData);
                        stack.Add(new CardAvailability(newCardSpell, true));
                    } break;
                }
            }
        }

        public CardStack(Card card)
        {
            id = card.id;
            name = card.cardName;
            stack = new List<CardAvailability>();
            type = card.type;

            stack.Add(new CardAvailability(card, true));
        }
        
        public int CardCount => stack.Count;
        public event UpdatedStack OnUpdatedStack;

        public void AddCard(Card card)
        {
            if (id != card.id) Debug.LogError($"Stack id ({id}) not equal to incoming card id ({card.id})");

            stack.Add(new CardAvailability(card, true));
            OnUpdatedStack?.Invoke(CardStackUpdateType.Add);
        }

        public void RemoveCard(int quantity = 1)
        {
            if (CardCount <= quantity)
            {
                Debug.LogWarning($"Removing more cards {quantity} than available in stack {id}");
                stack.Clear();
            }
            else
            {
                for (var i = 0; i < quantity; i++) stack.RemoveAt(CardCount - 1);
            }

            OnUpdatedStack?.Invoke(CardStackUpdateType.Remove);
        }

        public Card PopCard()
        {
            if (CardCount == 0)
            {
                throw new Exception($"Trying to pop a card from stack {id}. But none are available.");
            }

            var card = stack[0].Card;
            stack.RemoveAt(0);

            OnUpdatedStack?.Invoke(CardStackUpdateType.Pop);

            return card;
        }

        public bool IsPeekIndexAvailable() => stack[currentPeekIndex].Availability;

        public void SelectPeek()
        {
            stack[currentPeekIndex].Availability = false;
        }

        public void RemoveAllPeek()
        {
            foreach (var pair in stack.Where(pair => !pair.Availability))
                pair.Availability = true;
        }

        public Card PeekCard(bool first = true, bool next = true, bool onPeek = false)
        {
            if (onPeek)
                return stack[currentPeekIndex].Card;
            
            if (first)
            {
                currentPeekIndex = 0;
                return CardCount == 0 ? null : stack[0].Card;
            }

            if (next)
            {
                if (currentPeekIndex + 1 < stack.Count)
                    currentPeekIndex++;
                else
                    currentPeekIndex = 0;
            }
            else
            {
                if (currentPeekIndex - 1 > 0)
                    currentPeekIndex--;
                else
                    currentPeekIndex = stack.Count - 1;
            }

            return CardCount == 0 ? null : stack[currentPeekIndex].Card;
        }
    }
}