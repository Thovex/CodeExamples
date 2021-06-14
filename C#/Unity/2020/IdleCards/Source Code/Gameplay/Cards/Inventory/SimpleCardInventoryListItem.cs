using System;
using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Inventories;
using BaerAndHoggo.UI;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using Type = BaerAndHoggo.Gameplay.Cards.Type;

public class SimpleCardInventoryListItem : UpdateAbleItem<Card>
{
    public CardInventoryUI InventoryUI { get; set; }
    
    [SerializeField] private Image imageRef;
    [SerializeField] private SVGImage rarityRef;
    
    [SerializeField] private TMP_Text nameRef;
    [SerializeField] private TMP_Text manaRef;
    [SerializeField] private TMP_Text countRef;
    
    [SerializeField] private TMP_Text atkRef;
    [SerializeField] private TMP_Text hpRef;
    [SerializeField] private TMP_Text defRef;

    [SerializeField] private Button selectBtnRef;

    protected override void InitItem()
    {
        nameRef.text = Item.cardName;
        imageRef.sprite = Item.portrait;

        if (Item.type == Type.Minion)
        {
            var conversion = (CardMinion) Item;

            atkRef.text = conversion.damage.ToString();
            defRef.text = conversion.defense.ToString();
            hpRef.text = conversion.hp.ToString();
        }

        rarityRef.sprite = RarityDB.GetEmblemByRarity(Item.rarity);
        manaRef.text = Item.manaCost.ToString();

        countRef.text = GetCardCount().ToString();

        selectBtnRef.onClick.AddListener(delegate { InventoryUI.SimpleSelect(Item); });

    }

    public int GetCardCount()
    {
        CardInventory.Instance.Contains(Item, out var index);
        return CardInventory.Instance.inventory[index].CardCount;
    }
}
