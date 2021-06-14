using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Utilities;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;
using static BaerAndHoggo.Colors.ColorTheme;

namespace BaerAndHoggo.UI
{
    public class CardMinionUI : CardUI
    {
        [Title("Card UI References", "TMP_Text")] 
        [SerializeField] protected TMP_Text atkTextRef;

        [SerializeField] protected TMP_Text defTextRef;
        [SerializeField] protected TMP_Text hpTextRef;

        [Title("Card UI References", "Transform")] 
        [SerializeField] protected Transform statusEffectParentRef;
        [SerializeField] protected TMP_Text statusEffectsTextRef;
        
        [Title("Other Data")] 
        [SerializeField] protected Dictionary<MinionType, Sprite> typeIcons = new Dictionary<MinionType, Sprite>();

        [Title("Card UI References", "SVGImage")] 
        [SerializeField] protected SVGImage typeImageRef;
        
        public override void UpdateCardData(Card card)
        {
            base.UpdateCardData(card);
            var cardConversion = (CardMinion) card;

            CardDB.Instance.GetBaseItem(cardConversion.id, out var baseCard);
            var baseCardConversion = (CardMinion) baseCard;

            var theme = RarityDB.GetColorThemeByRarity(card.rarity);

            // Do textual changes
            atkTextRef.text = cardConversion.damage.ToBetterWorseText(baseCardConversion.damage, 
                "#" + theme.HexFromVariation(Variation.Dark));
            
            defTextRef.text = cardConversion.defense.ToBetterWorseText(baseCardConversion.defense,
                "#" + theme.HexFromVariation(Variation.Dark));
            
            hpTextRef.text = cardConversion.hp.ToBetterWorseText(baseCardConversion.hp, 
                "#" + theme.HexFromVariation(Variation.Dark));

            // TryGet icons.
            if (typeIcons.TryGetValue(cardConversion.minionType, out var typeImage)) 
                typeImageRef.sprite = typeImage;

            if (statusEffectParentRef) 
                UpdateStatusEffects(cardConversion);
        }

        private void UpdateStatusEffects(CardMinion card)
        {
            var effectsDictionary = new Dictionary<string, List<CardStatusEffect>>();
            
            foreach (var statusEffect in card.statusEffects)
            {
                var name = statusEffect.GetName();
                
                if (effectsDictionary.ContainsKey(name))
                {
                    effectsDictionary[name].Add(statusEffect);
                }
                else
                {
                    effectsDictionary.Add(name, new List<CardStatusEffect> { statusEffect });
                }
            }

            var definedText = string.Empty;

            foreach (var pair in effectsDictionary)
            {
                var totalPower = pair.Value.Sum(effect => effect.GetPower);

                definedText += pair.Value[0].GetDescription(
                    totalPower, RarityDB.GetColorThemeByRarity(card.rarity).HexFromVariation(Variation.Dark));
                
                definedText += Environment.NewLine;
            }

            statusEffectsTextRef.text = definedText;
        }
    }
}