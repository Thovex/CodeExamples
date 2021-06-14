using System;
using BaerAndHoggo.SaveData;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public enum MinionType { NULL, Melee, Magic, Ranged }

    [Serializable]
    public class CardMinionSaveData : CardSaveData
    {
        public int hp { get; set; }
        public int damage { get; set; }
        public int defense { get; set; }
        public MinionType minionType { get; set; }

        // Don't forget to add new properties we need to save to the To & From Save Data functionality.
    }

    [CreateAssetMenu(fileName = "Minion Card", menuName = "Cards/Minion Card", order = 1)]
    [System.Serializable]
    public class CardMinion : Card, ISaveData<CardMinionSaveData>
    {
        [Header("Stats")]
        public int hp = 1;
        public int damage = 0;
        public int defense = 0;

        [Header("Minion Type")]
        public MinionType minionType = MinionType.NULL;

        public CardMinion()
        {
            this.Type = Type.Minion;
            this.Name = "Unnamed Minion Card";
        }

        public void LoadData(CardMinionSaveData loadData)
        {
            // CardSaveData
            this.Id = loadData.Id;
            this.Tier = loadData.Tier;
            this.Name = loadData.Name;
            this.Type = loadData.Type;
            this.Category = loadData.Category;

            // CardMinionSaveData
            this.hp = loadData.hp;
            this.damage = loadData.damage;
            this.defense = loadData.defense;
            this.minionType = loadData.minionType;
        }

        public CardMinionSaveData ToSaveData()
        {
            CardMinionSaveData saveData = new CardMinionSaveData();

            //CardSaveData
            saveData.Id = this.Id;
            saveData.Name = this.Name;
            saveData.Type = this.Type;
            saveData.Category = this.Category;

            //CardMinionSaveData
            saveData.hp = this.hp;
            saveData.damage = this.damage;
            saveData.defense = this.defense;
            saveData.minionType = this.minionType;

            return saveData;
        }
    }
}