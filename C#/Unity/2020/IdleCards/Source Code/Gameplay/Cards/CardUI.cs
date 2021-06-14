using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.UI
{
    public abstract class CardUI : SerializedMonoBehaviour
    {
        [SerializeField] protected TMP_Text costTextRef;

        [Title("Card UI References", "TMP_Text")] [SerializeField]
        protected TMP_Text nameTextRef;

        [Title("Card UI References", "Image")] [SerializeField]
        protected Image portraitImageRef;

        [SerializeField] protected List<SVGImage> rarityDark;

        [Title("Color Update Lists")] [SerializeField]
        protected List<SVGImage> rarityDefault;

        [Title("Other Data")] [SerializeField]
        protected Dictionary<Rarity, Sprite> rarityIcons = new Dictionary<Rarity, Sprite>();

        [Title("Card UI References", "SVGImage")] [SerializeField]
        protected SVGImage rarityImageRef;

        public virtual void UpdateCardData(Card card)
        {
            // Do textual changes
            nameTextRef.text = $"{card.cardName}";
            costTextRef.text = $"{card.manaCost}";

            // Do sprite changes
            portraitImageRef.sprite = card.portrait;

            if (rarityIcons.TryGetValue(card.rarity, out var rarityImage)) rarityImageRef.sprite = rarityImage;

            // Get Theme by Rarity
            var colorTheme = RarityDB.GetColorThemeByRarity(card.rarity);

            foreach (var rarityDefaultSvg in rarityDefault) rarityDefaultSvg.color = colorTheme.General;
            foreach (var rarityDarkSvg in rarityDark) rarityDarkSvg.color = colorTheme.Dark;
        }
    }
}