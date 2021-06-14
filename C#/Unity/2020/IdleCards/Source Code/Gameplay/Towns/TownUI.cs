using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Gameplay.Towns;
using BaerAndHoggo.IO;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.UI
{
    public class TownUIUpdateData : UpdateAbleData
    {
        #region Variables

        #endregion
    }

    public class TownUIUpdateReferences : UpdateAbleReferences<TownUIUpdateData>
    {
        [Title("References", "Building | RectTransform")] [SerializeField]
        public RectTransform buildingScrollviewContainerRef;

        #region Update

        public override void Update(TownUIUpdateData data)
        {
            if (data.Name != default)
                this.nameRef.text = data.Name;
        }

        #endregion Update
    }

    public class TownUI :
        UpdateAbleScreenUI<TownDB, Town, TownUIUpdateReferences, TownUIUpdateData, TownIO[]>
    {
        [SerializeField] private BuildingUI buildingUI;

        [SerializeField] private Dictionary<Building, Button> buildingButton
            = new Dictionary<Building, Button>();

        [SerializeField] private Dictionary<GameObject, TownBuildingCard> buildingCards
            = new Dictionary<GameObject, TownBuildingCard>();

        [SerializeField] private Dictionary<BuildProgress.State, GameObject> progressPrefabs
            = new Dictionary<BuildProgress.State, GameObject>();

        protected override void Awake()
        {
            base.Awake();
            SaveManager.Instance.OnLoad += LoadInitialTown;
        }
        
        private void LoadInitialTown(IO.SaveData saveData)
        {
            SetItemFromIndex(0);
            InitializeReferences();
        }

        public override void InitializeReferences()
        {
            references.Update(
                new TownUIUpdateData()
                {
                    Id = item.id,
                    Name = item.townName,
                }
            );

            InitScrollViewChildren();
        }

        public void SelectBuilding(int buildingIndex)
        {
            MenuManager.Instance.NavigateMenu(buildingUI);

            buildingUI.UpdateFromTown(item, buildingIndex);
            buildingUI.SetItemFromId(item.buildings.ElementAt(buildingIndex).Key.id);
            buildingUI.InitializeReferences();
        }

        public void FinishConstruction(ConstructionEvent theEvent, ITimeEventCaller caller, Building building)
        {
            theEvent.OnCallFunctionality -= FinishConstruction;

            var selectedTown = TownDB.Instance.FindTownIdByBuildingId(building.id);

            TownDB.Instance.UpdateBuildingState(selectedTown.id, building, BuildProgress.State.Finished);

            var newBuildingTime = building.timeInSecondsPerAction.ElementAt(0);

            var actionEvent = new ActionEvent(building, building, newBuildingTime.Key);
            TimeEventManager.Instance.SetTimedEvent(building, actionEvent, newBuildingTime.Value);

            if (item.buildings.ContainsKey(building))
            {
                InitScrollViewChildren();
            }
        }

        public void PurchaseBuilding(int buildingIndex)
        {
            Building selectedBuilding = item.buildings.ElementAt(buildingIndex).Key;

            var constructionEvent = new ConstructionEvent(selectedBuilding, selectedBuilding);
            constructionEvent.OnCallFunctionality += FinishConstruction;

            TimeEventManager.Instance.SetTimedEvent(selectedBuilding, constructionEvent,
                selectedBuilding.constructionTime);

            TownDB.Instance.UpdateBuildingState
                (item.id, selectedBuilding, BuildProgress.State.UnderConstruction);

            InitScrollViewChildren();
        }

        [Button]
        private void InitScrollViewChildren()
        {
            if (references.buildingScrollviewContainerRef.childCount > 0)
            {
                DestroyScrollViewChildren();
            }

            for (int i = 0; i < item.buildings.Count; i++)
            {
                var buildingPair = item.buildings.ElementAt(i);

                var buildingCard =
                    Instantiate(progressPrefabs[buildingPair.Value.buildingState],
                        references.buildingScrollviewContainerRef);

                var townBuildingCard = buildingCard.GetComponent<TownBuildingCard>();
                townBuildingCard.SetBuilding(buildingPair.Key, buildingPair.Key);

                AppendButtonFunctionality(i, townBuildingCard, item.buildings.ElementAt(i).Value.buildingState);
                buildingCards.Add(buildingCard, townBuildingCard);
            }
        }

        private void AppendButtonFunctionality(int index, TownBuildingCard townBuildingCard, BuildProgress.State state)
        {
            switch (state)
            {
                case BuildProgress.State.Finished:
                {
                    townBuildingCard.GetUseButton().onClick.AddListener(delegate { SelectBuilding(index); });
                }
                    break;
                case BuildProgress.State.Purchasable:
                {
                    townBuildingCard.GetUseButton().onClick.AddListener(delegate { PurchaseBuilding(index); });
                }
                    break;

            }
        }

        private void DestroyScrollViewChildren()
        {
            for (int i = 0; i < references.buildingScrollviewContainerRef.childCount; i++)
            {
                Destroy(references.buildingScrollviewContainerRef.GetChild(i).gameObject);
            }

            buildingCards.Clear();
        }
    }
}