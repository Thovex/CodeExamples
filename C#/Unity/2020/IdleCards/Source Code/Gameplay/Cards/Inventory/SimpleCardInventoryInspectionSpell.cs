using System;
using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.UI;
using JetBrains.Annotations;
using TMPro;
using UnityEngine;

public class SimpleCardInventoryInspectionSpell: UpdateAbleItem<CardSpell>
{
    
    [SerializeField] private TMP_Text manaRef;
    [SerializeField] private TMP_Text descriptionRef;
    protected override void InitItem()
    {
        descriptionRef.text = $"Description {Environment.NewLine}Not implemented yet.";
        manaRef.text = $"Mana {Environment.NewLine}Not implemented yet.";
    }
}