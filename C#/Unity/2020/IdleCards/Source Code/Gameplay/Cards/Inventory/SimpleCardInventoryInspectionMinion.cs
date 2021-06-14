using System;
using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.UI;
using BaerAndHoggo.Utilities;
using TMPro;
using UnityEngine;

public class SimpleCardInventoryInspectionMinion : UpdateAbleItem<CardMinion>
{
    
    [SerializeField] private TMP_Text manaRef;
    [SerializeField] private TMP_Text atkRef;
    [SerializeField] private TMP_Text hpRef;
    [SerializeField] private TMP_Text defRef;

    protected override void InitItem()
    {
        CardDB.Instance.GetBaseItem(Item.id, out var baseCard);

        var baseMinion = (CardMinion) baseCard;
        
        var atkText = Item.damage.ToBetterWorseText(baseMinion.damage);
        var defText = Item.defense.ToBetterWorseText(baseMinion.defense);
        var hpText = Item.hp.ToBetterWorseText(baseMinion.hp);
        var manaText = Item.manaCost.ToBetterWorseText(baseMinion.manaCost, false);

        atkRef.text = $"Attack {Environment.NewLine}{atkText}";
        defRef.text = $"Defense {Environment.NewLine}{defText}";
        hpRef.text = $"HP {Environment.NewLine}{hpText}";
        manaRef.text = $"Mana {Environment.NewLine}{manaText}";
    }
}
