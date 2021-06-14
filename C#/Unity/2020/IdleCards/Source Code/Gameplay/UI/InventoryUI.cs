using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Inventories;
using BaerAndHoggo.Gameplay.Cards;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

namespace BaerAndHoggo.UI
{
    public class InventoryUI : MonoBehaviour
    {
        [SerializeField] private RectTransform inventoryContentTransform;
        [SerializeField] private Button addRandomButton;
        [SerializeField] private Button removeRandomButton;
        [SerializeField] private List<TMP_Text> inventoryTexts = new List<TMP_Text>();

        private void Start()
        {
            CardInventory inventory = (CardInventory)CardInventory.Instance;

            inventory.inventoryUI = this;

            addRandomButton.onClick.AddListener(delegate { inventory.Debug_AddRandomCard(); });
            removeRandomButton.onClick.AddListener(delegate { inventory.Debug_RemoveRandomCard(); });
        }

        public void UpdateUI(CardInventory cardInventory)
        {
            int difference = inventoryTexts.Count - cardInventory.inventory.Count;

            if (difference > 0)
            {
                for (int i = inventoryTexts.Count - difference; i < inventoryTexts.Count + difference; i++)
                {
                    Destroy(inventoryTexts[i].gameObject);
                    inventoryTexts.RemoveAt(i);
                }
            }


            for (int i = 0; i < cardInventory.inventory.Count; i++)
            {
                CardStack cardStack = (CardStack)cardInventory.inventory[i];

                if (i + 1 > inventoryTexts.Count)
                {
                    GameObject newObject = new GameObject($"inventory-instance-{i}", typeof(RectTransform), typeof(TextMeshProUGUI));
                    newObject.transform.SetParent(inventoryContentTransform, false);
                    inventoryTexts.Add(newObject.GetComponent<TextMeshProUGUI>());
                }

                inventoryTexts[i].text = $"{cardStack.Name}: {cardStack.CardCount}";
            }
        }
    }
}