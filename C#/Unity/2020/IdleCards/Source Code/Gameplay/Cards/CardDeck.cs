using System;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Battle;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Cards
{
    [Serializable]
    public abstract class CardModification
    {
        [SerializeField] public int manaMod = 0;
    }

    [Serializable]
    public class CardMinionModification : CardModification
    {
        [SerializeField] public int damageMod = 0;
        [SerializeField] public int defenseMod = 0;
        [SerializeField] public int hpMod = 0;
    }

    [Serializable]
    public class CardSpellModification : CardModification
    {
        
    }

    [Serializable]
    public class CardMinionWithMod
    {
        [SerializeField] public CardMinion card;
        [SerializeField] public CardMinionModification mod;
        
        public CardMinionWithMod(CardMinion card)
        {
            this.card = card;
            this.mod = new CardMinionModification();
        }
        
        public CardMinionWithMod(CardMinion card, CardMinionModification mod)
        {
            this.card = card;
            this.mod = mod;
        }
    }
    
    [Serializable]
    public class CardSpellWithMod
    {
        [SerializeField] public CardSpell card;
        [SerializeField] public CardSpellModification mod;
        
        public CardSpellWithMod(CardSpell card)
        {
            this.card = card;
            this.mod = new CardSpellModification();
        }
        
        public CardSpellWithMod(CardSpell card, CardSpellModification mod)
        {
            this.card = card;
            this.mod = mod;
        }
    }
    
    [Serializable]
    public class CardDeck : ICloneable
    {
        [SerializeField] private CardCaptain captain;
        [SerializeField] private List<CardMinionWithMod> minions;
        [SerializeField] private List<CardSpellWithMod> spells;

        [SerializeField] private Stack<Card> cards;

        public CardCaptain Captain => captain;
        public int GetDeckCount() => cards.Count;

        public CardDeck(CardCaptain captain, List<Card> cards)
        {
            this.captain = captain;
            minions = new List<CardMinionWithMod>();
            spells = new List<CardSpellWithMod>();

            foreach (var card in cards)
            {
                switch (card.type)
                {
                    case Type.Minion:
                        minions.Add(new CardMinionWithMod((CardMinion)card));
                        break;
                    case Type.Spell:
                        spells.Add(new CardSpellWithMod((CardSpell)card)); 
                        break;
                }
            }
        }

        public Card DrawCard()
        {
            return cards.Pop();
        }

        public void CopyDeckAndSetMods()
        {
            var copyCaptain = ScriptableObject.CreateInstance<CardCaptain>();
            copyCaptain.OnLoad_Implementation(this.captain.OnSave_Implementation());

            this.captain = copyCaptain;

            cards = new Stack<Card>();

            if (minions.Count > 0)
            {
                var copyMinions = new List<CardMinionWithMod>();

                foreach (var minion in minions)
                {
                    var copyMinion = ScriptableObject.CreateInstance<CardMinion>();
                    
                    copyMinion.OnLoad_Implementation(minion.card.OnSave_Implementation());

                    copyMinion.manaCost += minion.mod.manaMod;
                    copyMinion.damage += minion.mod.damageMod;
                    copyMinion.defense += minion.mod.defenseMod;
                    copyMinion.hp += minion.mod.hpMod;
                    
                    copyMinions.Add(new CardMinionWithMod(copyMinion, minion.mod));
                    cards.Push(copyMinion);

                }

                minions = copyMinions;
            }
            
            if (spells.Count > 0)
            {
                var copySpells = new List<CardSpellWithMod>();

                foreach (var spell in spells)
                {
                    var copySpell = ScriptableObject.CreateInstance<CardSpell>();
                    copySpell.OnLoad_Implementation(spell.card.OnSave_Implementation());

                    copySpell.manaCost += spell.mod.manaMod;
                    
                    copySpells.Add(new CardSpellWithMod(copySpell, spell.mod));
                    cards.Push(copySpell);

                }

                spells = copySpells;
            }
            
            cards.Shuffle();
        }

        public object Clone()
        {
            return this.MemberwiseClone();
        }
    }
}