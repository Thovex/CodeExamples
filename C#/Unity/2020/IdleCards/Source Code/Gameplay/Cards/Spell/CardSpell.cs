using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.SaveData;
using Sirenix.OdinInspector;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    public enum SpellType
    {
        Target,
        TargetWithHero,
        AllyBoard,
        EnemyBoard,
        AllyHero,
        EnemyHero,
    }

    [Serializable]
    public class SpellIdWithPower
    {
        public long spellId;
        public int spellPower;
    }
    
    [CreateAssetMenu(fileName = "Spell Card", menuName = "Idle Cards/Cards/Spell Card", order = 2)]
    [Serializable]
    public class CardSpell : Card, ISaveData<CardSpellIO>
    {
        
        [Title("Spell Type")]
        public SpellType spellType = SpellType.Target;

        public bool spellOneShot = true;

        [Title("Spells", "requires a long (key) to retrieve from EffectsDB with an int (value) power.")]
        public List<SpellIdWithPower> spells = new List<SpellIdWithPower>();

        public CardSpell()
        {
            type = Type.Spell;
        }
        
        public virtual void DoSpellMinion(CardMinion targetMinion)
        {
            foreach (var spell in spells)
            {
                var effectObject = CardStatusEffectDB.Instance.GetItemById(spell.spellId);
                
                effectObject.effect.SetTargetMinion(targetMinion);
                effectObject.effect.SetOneShot(spellOneShot);
                effectObject.effect.SetPower(spell.spellPower);
                effectObject.effect.Activate_Minion();
            }
        }

        public virtual void DoSpellCaptain(CardCaptain targetCaptain)
        {
            foreach (var spell in spells)
            {
                var effectObject = CardStatusEffectDB.Instance.GetItemById(spell.spellId);
                
                effectObject.effect.SetTargetCaptain(targetCaptain);
                effectObject.effect.SetOneShot(spellOneShot);
                effectObject.effect.SetPower(spell.spellPower);
                effectObject.effect.Activate_Captain();
            }
        }

        public CardSpellIO OnSave_Implementation()
        {
            return new CardSpellIO
            {
                Id = id,
                Name = cardName,
                Type = type,
                PlayCost = manaCost,
                Category = category,
                SpellType = spellType,
                Spells = spells,
                SpellOneShot = spellOneShot,
            };
        }

        public void OnLoad_Implementation(CardSpellIO loadData)
        {
            id = loadData.Id;
            cardName = loadData.Name;
            type = loadData.Type;
            manaCost = loadData.PlayCost;
            category = loadData.Category;
            spellType = loadData.SpellType;
            spells = loadData.Spells;
            spellOneShot = loadData.SpellOneShot;

            CardDB.Instance.GetBaseItem(id, out var baseCard);
            portrait = baseCard.portrait;
        }
        
#if UNITY_EDITOR

        [Title("Debug")] 
        [ReadOnly] [SerializeField] [TextArea]
        private string spellNames;
        
        protected override void OnValidate()
        {
            base.OnValidate();

            spellNames = string.Empty;
            var effects = Resources.LoadAll<CardStatusEffectObject>("StatusEffects");

            foreach (var effect in effects)
            {
                foreach (var spell in spells.Where(spell => spell.spellId == effect.id))
                    spellNames += $"{effect.GetName()} [{spell.spellPower}]{Environment.NewLine}";
            }
        }
#endif
    }
}