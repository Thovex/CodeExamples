using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Inventories;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;
using UnityEngine.Serialization;
using UnityEngine.UI;
using Type = BaerAndHoggo.Gameplay.Cards.Type;

namespace BaerAndHoggo.UI
{
    [Serializable] public enum CardInventoryFilter
    {
        Count,
        Attack,
        Health,
        Defense,
        Rarity,
        ManaCost,
    }

    [Serializable]  public enum CardInventoryFilterState
    {
        Default,
        Ascending,
        Descending,
    }

    [System.Serializable]
    public class FilterState
    {
        public CardInventoryFilterState State
        {
            get => _state;
            set
            {
                textRef.text = DetermineIcon(value);
                _state = value;
            }
        }

        [SerializeField] private CardInventoryFilterState _state;
        [SerializeField] public Button buttonRef;
        [SerializeField] public TMP_Text textRef;

        private string DetermineIcon(CardInventoryFilterState state)
        {
            switch (state)
            {
                case CardInventoryFilterState.Default: return "_";
                case CardInventoryFilterState.Ascending: return "^";
                case CardInventoryFilterState.Descending: return "v";
                default: throw new ArgumentOutOfRangeException(nameof(state), state, null);
            }
        }
    }
    
    public class CardInventoryUI : ManagedMenu
    {
 
        [Title("Transforms")] 
        [SerializeField] protected RectTransform inventoryContentTransform;

        [Title("Inventory")]
        [SerializeField] public Button selectCardButtonRef;
        [SerializeField] private TMP_Text selectCardTextRef;
        public int selectedStackIndex;

        [Title("Simple Inventory")]
        [SerializeField] private List<SimpleCardInventoryListItem> simpleListItems = new List<SimpleCardInventoryListItem>();
        [SerializeField] private GameObject simpleListItemPrefab;
        [SerializeField] private SimpleCardInventoryInspectionMinion simpleInspectMinion;
        [SerializeField] private SimpleCardInventoryInspectionSpell simpleInspectSpell;
        private bool _simpleMode = true;

        [Title("Search Settings")]
        [SerializeField] private TMP_InputField searchInputRef;
        [SerializeField] private string searchString = "";

        [Title("Filtering")]
        [SerializeField] private Dictionary<CardInventoryFilter, FilterState> filterStates
            = new Dictionary<CardInventoryFilter, FilterState>();
        private CardInventoryFilter _currentFilter;
        private bool _filterOn = false;
        
        protected virtual void Start()
        {
            CardInventory.Instance.RegisterInventoryUI(this);
            
            searchInputRef.
                onValueChanged.AddListener(TrySearch);
            
            filterStates[CardInventoryFilter.Attack].buttonRef.
                onClick.AddListener(delegate { SetFilterActivity(CardInventoryFilter.Attack); });
            
            filterStates[CardInventoryFilter.Count].buttonRef.
                onClick.AddListener(delegate { SetFilterActivity(CardInventoryFilter.Count); });
            
            filterStates[CardInventoryFilter.Defense].buttonRef.
                onClick.AddListener(delegate { SetFilterActivity(CardInventoryFilter.Defense); });
            
            filterStates[CardInventoryFilter.Health].buttonRef.
                onClick.AddListener(delegate { SetFilterActivity(CardInventoryFilter.Health); });
            
            filterStates[CardInventoryFilter.Rarity].buttonRef.
                onClick.AddListener(delegate { SetFilterActivity(CardInventoryFilter.Rarity); });
            
            filterStates[CardInventoryFilter.ManaCost].buttonRef.
                onClick.AddListener(delegate { SetFilterActivity(CardInventoryFilter.ManaCost); });
            
        }

        public virtual void UpdateUi()
        {
            foreach (var listItem in simpleListItems)
                Destroy(listItem.gameObject);
            
            simpleListItems.Clear();

            foreach (var stack in CardInventory.Instance.inventory)
            {
                if (!string.IsNullOrWhiteSpace(searchString))
                {
                    if (!Regex.IsMatch(stack.name, searchString, RegexOptions.IgnoreCase)) 
                    {
                        continue;
                    }
                }

                var listItem = Instantiate(simpleListItemPrefab, inventoryContentTransform);
                var simpleComponent = listItem.GetComponent<SimpleCardInventoryListItem>();

                simpleListItems.Add(simpleComponent);
                
                CardDB.Instance.GetBaseItem(stack.id, out var baseCard);

                simpleComponent.InventoryUI = this;
                simpleComponent.Item = baseCard;
            }

            if (_filterOn)
                UpdateWithFilter();
        }

        private void UpdateWithFilter()
        {
            var sortOrder = new List<int>();
            
            switch (filterStates[_currentFilter].State)
            {
                case CardInventoryFilterState.Ascending:
                { 
                    sortOrder = SetFilter(simpleListItems, _currentFilter, true);
                } break;
                case CardInventoryFilterState.Descending:
                {
                    sortOrder = SetFilter(simpleListItems, _currentFilter, false);
                } break;
            }
            
            foreach (var item in simpleListItems)
            {
                item.transform.SetParent(null, true);
            }

            foreach (var sortItem in sortOrder)
            {
                simpleListItems[sortItem].transform.SetParent(inventoryContentTransform, true);
            }
        }

        private static List<int> 
            SetFilter(List<SimpleCardInventoryListItem> existingList, CardInventoryFilter type, bool ascending)
        {
            var sortedList = existingList;
            
            switch (type)
            {
                case CardInventoryFilter.Count:
                    sortedList = (ascending
                        ? sortedList.OrderBy(sort => sort.GetCardCount())
                        : sortedList.OrderByDescending(sort => sort.GetCardCount())).ToList();
                    break;
                case CardInventoryFilter.Attack:
                    sortedList = (ascending
                        ? sortedList.OrderBy(sort => sort.Item.type == Type.Minion ? ((CardMinion)sort.Item).damage : 0)
                        : sortedList.OrderByDescending(sort => sort.Item.type == Type.Minion ? ((CardMinion)sort.Item).damage : 0)).ToList();
                    break;
                case CardInventoryFilter.Health:
                    sortedList = (ascending
                        ? sortedList.OrderBy(sort => sort.Item.type == Type.Minion ? ((CardMinion)sort.Item).hp : 0)
                        : sortedList.OrderByDescending(sort => sort.Item.type == Type.Minion ? ((CardMinion)sort.Item).hp : 0)).ToList();
                    break;
                case CardInventoryFilter.Defense:
                    sortedList = (ascending
                    ? sortedList.OrderBy(sort => sort.Item.type == Type.Minion ? ((CardMinion)sort.Item).defense : 0)
                    : sortedList.OrderByDescending(sort => sort.Item.type == Type.Minion ? ((CardMinion)sort.Item).defense : 0)).ToList();
                    break;
                case CardInventoryFilter.Rarity:
                    sortedList = (ascending
                        ? sortedList.OrderBy(sort => sort.Item.rarity)
                        : sortedList.OrderByDescending(sort => sort.Item.rarity)).ToList();
                    break;
                case CardInventoryFilter.ManaCost:
                    sortedList = (ascending
                        ? sortedList.OrderBy(sort => sort.Item.manaCost)
                        : sortedList.OrderByDescending(sort => sort.Item.manaCost)).ToList();
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(type), type, null);
            }

            return sortedList.Select(sortedItem => existingList.IndexOf(sortedItem)).ToList();
        }

        public void SimpleSelect(Card card)
        {
            print($"Selected card: {card.cardName}");

            CardInventory.Instance.Contains(card, out int stackIndex);
            selectedStackIndex = stackIndex;

            var inventoryCard = CardInventory.Instance.inventory[selectedStackIndex].PeekCard();
            UpdateInspection(inventoryCard);
        }

        private void UpdateInspection(Card card)
        {
            switch (card.type)
            {
                case Type.Minion:
                    simpleInspectMinion.gameObject.SetActive(true);
                    simpleInspectSpell.gameObject.SetActive(false);

                    simpleInspectMinion.Item = (CardMinion) card;

                    break;
                case Type.Spell:
                    simpleInspectMinion.gameObject.SetActive(false);
                    simpleInspectSpell.gameObject.SetActive(true);

                    simpleInspectSpell.Item = (CardSpell) card;
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }


            if (CardInventory.Instance.inventory[selectedStackIndex].IsPeekIndexAvailable())
            {
                selectCardButtonRef.interactable = true;
                selectCardTextRef.text =
                    $"Select Card " +
                    $"{CardInventory.Instance.inventory[selectedStackIndex].currentPeekIndex+1}/" +
                    $"{CardInventory.Instance.inventory[selectedStackIndex].CardCount}";
            }
            else
            {
                selectCardButtonRef.interactable = false;
                selectCardTextRef.text = $"Already in-use";
            }
        }

        public void SimpleNextPeek()
        {
            var inventoryCard = CardInventory.Instance.inventory[selectedStackIndex].PeekCard(false, true);
            UpdateInspection(inventoryCard);
        }
        
        public void SimplePreviousPeek()
        {
            var inventoryCard = CardInventory.Instance.inventory[selectedStackIndex].PeekCard(false, false);
            UpdateInspection(inventoryCard);
        }

        public void TrySearch(string searchArg)
        {
            searchString = searchArg;
            
            UpdateUi();
        }

        public void SetFilterActivity(CardInventoryFilter filterType)
        {
            // Reset all but this filter type to default.
            foreach (var filter in filterStates)
            {
                if (filter.Key != filterType)
                    filter.Value.State = CardInventoryFilterState.Default;
            }
            
            if ((int)filterStates[filterType].State + 1 > 
                Enum.GetValues(typeof(CardInventoryFilterState)).Cast<int>().Max())
            {
                filterStates[filterType].State = (CardInventoryFilterState) Enum.GetValues(typeof(CardInventoryFilterState)).Cast<int>().Min();
            } else {
                filterStates[filterType].State++;
            }

            if (filterStates[filterType].State != CardInventoryFilterState.Default)
            {
                _filterOn = true;
                _currentFilter = filterType;
            } else {
                _filterOn = false;
            }

            UpdateUi();
        }

        public void ClearSearch()
        {
            searchString = string.Empty;
            searchInputRef.text = string.Empty;
        }
    }
}