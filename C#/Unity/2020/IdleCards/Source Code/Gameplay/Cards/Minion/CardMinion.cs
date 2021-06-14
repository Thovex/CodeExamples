using System;
using System.Collections.Generic;
using BaerAndHoggo.SaveData;
using Sirenix.OdinInspector;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public enum MinionType
    {
        Melee,
        Magic,
        Ranged
    }

    [CreateAssetMenu(fileName = "Minion Card", menuName = "Idle Cards/Cards/Minion Card", order = 1)]
    [Serializable]
    public class CardMinion : Card, ISaveData<CardMinionIO>
    {

        [Header("Stats")] 
        public int hp = 1;
        public int damage;
        public int defense;

        [Header("Minion Type")] public MinionType minionType = MinionType.Melee;
        
        [Title("Buffs & Debuffs", "Only classes inherited from CardStatusEffect")]
        public List<CardStatusEffect> statusEffects = new List<CardStatusEffect>();
        
        public delegate void DefenseBreak();
        public event DefenseBreak OnDefenseBreak;
        
        public delegate void Die();
        public event Die OnDie;
        

        public CardMinion()
        {
            type = Type.Minion;
            cardName = "Unnamed Minion Card";
        }

        public void Heal(int value)
        {
            hp += value;
            
            if (hp <= 0)
            {
                OnDie?.Invoke();
            }
            
            CallUpdate();
        }

        public void TakeDamage(int value)
        {
            var penetrateDamage = value - defense;
            
            if (penetrateDamage > 0)
                hp -= penetrateDamage;
            else if (defense > 0)
            {
                OnDefenseBreak?.Invoke();
                defense -= 1;
            }

            if (hp <= 0)
            {
                OnDie?.Invoke();
            }

            CallUpdate();
        }
        
        public void TakeDamage(CardMinion minion, bool retaliate = false)
        {
            var penetrateDamage = minion.damage - defense;

            if (penetrateDamage > 0)
                hp -= penetrateDamage;
            else if (defense > 0)
            {
                OnDefenseBreak?.Invoke();
                defense -= 1;
            }

            if (hp <= 0)
            {
                OnDie?.Invoke();
            }

            if (minion.minionType == MinionType.Melee && !retaliate)
                minion.TakeDamage(this, true);

            CallUpdate();
        }

        public CardMinionIO OnSave_Implementation()
        {
            return new CardMinionIO
            {
                Id = id,
                Name = cardName,
                Type = type,
                PlayCost = manaCost,
                Category = category,
                Hp = hp,
                Damage = damage,
                Defense = defense,
                MinionType = minionType
            };
        }

        public void OnLoad_Implementation(CardMinionIO loadData)
        {
            id = loadData.Id;
            cardName = loadData.Name;
            type = loadData.Type;
            category = loadData.Category;
            manaCost = loadData.PlayCost;
            hp = loadData.Hp;
            damage = loadData.Damage;
            defense = loadData.Defense;
            minionType = loadData.MinionType;

            CardDB.Instance.GetBaseItem(id, out var baseCard);
            portrait = baseCard.portrait;
        }
    }
}