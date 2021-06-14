using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Effects;
using BaerAndHoggo.SaveData;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{

    [CreateAssetMenu(fileName = "Captain Card", menuName = "Idle Cards/Cards/Captain Card", order = 0)]
    [Serializable]
    public class CardCaptain : Card, ISaveData<CardCaptainIO>
    {

        [Header("Stats HP")] 
        public int startHp = 1;
        public int hp = 1;

        [Header("Stats Mana")]
        public int startMana = 1;
        public int manaGrowth = 1;
        public int mana = 1;
        public int maxMana = 1;

        [Header("Active")] 
        public int activeManaCost = 1;
        public long activeSpellId;
        
        [Header("Passive")]
        public long passiveSpellId;
        
        [Title("Buffs & Debuffs", "Only classes inherited from CardStatusEffect")]
        public List<CardStatusEffect> statusEffects = new List<CardStatusEffect>();
        
        public delegate void Die();
        public event Die OnDie;
        
        public CardCaptain()
        {
            type = Type.Captain;
            cardName = "Unnamed Captain Card";
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
            hp -= value;

            if (hp <= 0)
            {
                OnDie?.Invoke();
            }

            CallUpdate();
        }

        public CardCaptainIO OnSave_Implementation()
        {
            return new CardCaptainIO()
            {
                Id = id,
                Name = cardName,
                Type = type,
                PlayCost = manaCost,
                Category = category,
                StartHp = startHp,
                Hp = hp,
                StartMana = startMana,
                ManaGrowth = manaGrowth,
                MaxMana = maxMana,
                ActiveManaCost = activeManaCost,
                ActiveSpell = activeSpellId,
                PassiveSpell = passiveSpellId,
            };
        }

        public void OnLoad_Implementation(CardCaptainIO loadData)
        {
            id = loadData.Id;
            cardName = loadData.Name;
            type = loadData.Type;
            category = loadData.Category;
            startHp = loadData.StartHp;
            hp = loadData.Hp;
            startMana = loadData.StartMana;
            manaGrowth = loadData.ManaGrowth;
            maxMana = loadData.MaxMana;
            activeManaCost = loadData.ActiveManaCost;

            activeSpellId = loadData.ActiveSpell;
            passiveSpellId = loadData.PassiveSpell;

            CardDB.Instance.GetBaseItem(id, out var baseCard);
            portrait = baseCard.portrait;
        }
        
#if UNITY_EDITOR

        [Title("Debug")]
        [ReadOnly] [SerializeField] private string activeSpellName;
        [ReadOnly] [SerializeField] private string passiveSpellName;
        protected override void OnValidate()
        {
            base.OnValidate();

            var cards = Resources.LoadAll<Card>("Cards");

            foreach (var card in cards)
            {
                if (card.id == activeSpellId)
                {
                    activeSpellName = (card.type == Type.Spell) ? card.cardName : "Not a spell!";
                }
                
                if (card.id == passiveSpellId)
                {
                    passiveSpellName = (card.type == Type.Spell) ? card.cardName : "Not a spell!";
                }
            }
        }
#endif
    }
}