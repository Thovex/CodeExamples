using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Cards;
using UnityEngine;

public class CardPackUI : MonoBehaviour
{
    [SerializeField] private GameObject noPackContainer;
    [SerializeField] private GameObject anyPackContainer;

    [SerializeField] private int currentIndex = -1;

    /*
    private void Awake()
    {
        CardPackInventory.Instance.OnPackAdd += UpdatePackScreen;

        CheckForPacks();
    }

    private void UpdatePackScreen()
    {
        if (CheckForPacks(out var packCount))
        {
            if (currentIndex == -1 ) currentIndex = GetFirstAvailableIndex();
        }
    }

    private int GetFirstAvailableIndex()
    {
        int index = 0;
        
        foreach (var pair in CardPackInventory.Instance.inventory)
        {
            if (pair.Value > 0)
            {
                return index;
            }

            index++;
        }

        return -1;
    }

    private bool CheckForPacks(out int packCount)
    {
        packCount = CardPackInventory.Instance.inventory.Where(cardPackPair => cardPackPair.Value > 0)
            .Sum(cardPackPair => cardPackPair.Value);

        if (packCount > 0)
        {
            anyPackContainer.SetActive(true);
            noPackContainer.SetActive(false);
        }
        else
        {
            anyPackContainer.SetActive(false);
            noPackContainer.SetActive(true);

            currentIndex = -1;
        }

        return packCount > 0;
    }

    public void SelectNextPack()
    {
    }

    public void SelectPreviousPack()
    {
    }
    */
}