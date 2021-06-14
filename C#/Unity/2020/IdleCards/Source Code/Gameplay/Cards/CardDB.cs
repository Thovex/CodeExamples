using System;
using System.Linq;
using BaerAndHoggo.Gameplay.Inventories;
using Sirenix.OdinInspector;
using UnityEngine;
using Random = UnityEngine.Random;

namespace BaerAndHoggo.Gameplay.Cards
{
    public class CardDB : DB<Card, CardIO[]>
    {
        [SerializeField] [Required] private GameObject cardStackPrefab;
        [SerializeField] [Required] private GameObject masterCardPrefab;
        public new static CardDB Instance => (CardDB)GetInstance();
        
        public GameObject GetMasterCardPrefab()=>masterCardPrefab;
        public GameObject GetCardStackPrefab()=>cardStackPrefab;

        public bool GetRandomCard(out Card card)
        {
            card = null;
            
            if (db.Count == 0)
            {
                throw new System.Exception("CardDB is Empty.");
            }

            card = Instantiate(db.ElementAt(Random.Range(0, db.Count)).Value);

            return true;
        }

        protected override void InitializeDB()
        {
            IsInitialized = true;
        }
        
        protected override void StartLoad(IO.SaveData loadData)
        {
            OnLoad_Implementation(loadData.CardIO);
        }

        public override CardIO[] OnSave_Implementation()
        {
            var data = new CardIO[db.Count];
            
            for (int i = 0; i < db.Count; i++)
            {
                var card = db.ElementAt(i).Value;
                
                switch (card.type)
                {
                    case Type.Minion:data[i] = ((CardMinion) card).OnSave_Implementation(); break;
                    case Type.Spell:data[i] = ((CardSpell) card).OnSave_Implementation(); break;
                }
            }
            
            return data;
        }

        public override void OnLoad_Implementation(CardIO[] loadData)
        {
            if (loadData == null) return; 
            if (loadData.Length == 0) return;
            
            // Foreach saved building IO, we go through the IDs and load with the new data.
            foreach (var cardIO in loadData)
            {
                var card = db[cardIO.Id];

                switch (card.type)
                {
                    case Type.Minion:
                        ((CardMinion)card).OnLoad_Implementation((CardMinionIO)cardIO); 
                        break;
                    case Type.Spell: 
                        ((CardSpell)card).OnLoad_Implementation((CardSpellIO)cardIO); 
                        break;
                }
            }
            
        }
    }
}