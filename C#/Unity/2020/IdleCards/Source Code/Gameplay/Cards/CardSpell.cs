using System;
using BaerAndHoggo.SaveData;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{

    [Serializable]
    public class CardSpellSaveData : CardSaveData
    {
        // Don't forget to add new properties we need to save to the To & From Save Data functionality.
    }

    [CreateAssetMenu(fileName = "Spell Card", menuName = "Cards/Spell Card", order = 2)]
    [System.Serializable]
    public class CardSpell : Card, ISaveData<CardSpellSaveData>
    {
        public CardSpell()
        {
            this.Type = Type.Spell;
            this.Name = "Unnamed Spell Card";
        }

        public void LoadData(CardSpellSaveData loadData)
        {
            // CardSaveData
            this.Id = loadData.Id;
            this.Tier = loadData.Tier;
            this.Name = loadData.Name;
            this.Type = loadData.Type;
            this.Category = loadData.Category;

            // CardSpellSaveData
        }

        public CardSpellSaveData ToSaveData()
        {
            CardSpellSaveData saveData = new CardSpellSaveData();

            //CardSaveData
            saveData.Id = this.Id;
            saveData.Name = this.Name;
            saveData.Type = this.Type;
            saveData.Category = this.Category;

            //CardMinionSaveData

            return saveData;
        }
    }
}