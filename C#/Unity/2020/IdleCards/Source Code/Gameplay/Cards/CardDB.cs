using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;

namespace BaerAndHoggo.Gameplay.Cards
{
    public class CardDB : Singleton<CardDB>
    {
        [SerializeField] [ReadOnly] private Dictionary<long, Card> cardDB = new Dictionary<long, Card>();

        private void Awake()
        {
            // Grab cards we have defined in the Resources/Cards folder.
            Card[] cards = Resources.LoadAll<Card>("Cards");

            foreach (Card card in cards)
            {
                // Add an instantiated clone from the existing card. 
                // We do not ever want to make changes on the files by programming here.
                cardDB.Add(card.Id, Instantiate(card));
            }
        }

        public bool GetRandomCard(out Card card)
        {
            card = null;

            if (cardDB.Count == 0)
            {
                new System.Exception($"CardDB is Empty.");
                return false;
            }

            card = Instantiate(cardDB.ElementAt(Random.Range(0, cardDB.Count)).Value);

            return true;
        }

        // Returns a nullptr on false.
        public bool GetCard(long id, out Card card)
        {
            card = null;

            // If DB contains this id.
            if (cardDB.ContainsKey(id))
            {
                // Create an instantiated clone from this card to give to the player.
                // We do not want the player to directly adjust the "main card" where we clone from.
                // However in future we can "permanently" upgrade the cards.
                card = Instantiate(cardDB[id]);
                return true;
            }

            new System.Exception($"Invalid card requested of id {id}. But not available in CardDB.");
            return false;
        }

        public long GetIdFromName(string Name)
        {
            foreach (var card in cardDB)
            {
                if (card.Value.Name == Name)
                {
                    return card.Value.Id;
                }
            }

            Debug.LogError($"GetIdFromName Failed! No such Id found for Name {Name}");
            return -1;
        }

        public string GetNameFromId(long Id)
        {
            foreach (var card in cardDB)
            {
                if (card.Value.Id == Id)
                {
                    return card.Value.Name;
                }
            }

            Debug.LogError($"GetNameFromId Failed! No such Name found for Id {Id}");
            return "NULL";
        }
    }
}
