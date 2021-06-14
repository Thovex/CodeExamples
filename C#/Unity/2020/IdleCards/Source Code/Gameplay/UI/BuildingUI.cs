using System;
using UnityEngine;

using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.Utilities;

using Sirenix.OdinInspector;

namespace BaerAndHoggo.UI
{
    public class BuildingUI : SerializedMonoBehaviour
    {
        [SerializeField] public Building Building;
        [SerializeField] private BuildingActionType currentAction;
        public bool isActiveUI = false;
        private BuildingUIReferences references;
        private Animator animator;

        private void Start()
        {
            this.animator = GetComponent<Animator>();
            this.references = GetComponent<BuildingUIReferences>();

            // Set initial building
            SetBuildingFromIndex(0);

            // Set initial action
            currentAction = BuildingActionType.CardPack;
            // Add ^ event to the event manager
            SetBuildingEvent(Building, currentAction);

            // Call when want to initialize the base data for when we swap Building
            InitializeReferences();

            // initialize button functionality
            references.InitializeButtons(delegate { SelectNextBuilding(); }, delegate { SelectPreviousBuilding(); });

            // This should be true if user is looking at this current panel.
            isActiveUI = true;

        }


        private void InitializeReferences()
        {
            var updateData = new BuildingUIUpdateData();
            updateData.BuildingName = Building.Name;
            updateData.BuildingAction = currentAction;
            updateData.BuildingImage = Building.Image;

            UpdateTimer();

            // Calls UpdateUI with a specific UpdateData struct.
            references.UpdateUI(updateData);

        }

        public void SetBuildingEvent(Building building, BuildingActionType action)
        {
            if (building.TimeInSecondsPerAction[action] <= 0)
            {
                new SystemException($"Invalid time on Entity {this.ToString()}");
            }

            // Set Event in the TimeEventManager
            TimeEventManager.Instance.SetEvent(building, action, building.TimeInSecondsPerAction[action]);

            // Update our Timer
            UpdateTimer();
        }

        public void SetBuildingFromIndex(int index)
        {
            if (BuildingDB.Instance.GetBuilding(index, out Building building))
            {
                this.Building = building;

                // Set Action to whatever was running before.
                this.currentAction = TimeEventManager.Instance.GetActiveType(Building);
            }
        }

        public void SelectNextBuilding()
        {
            // if building is last in list, then set first building
            if (this.Building.Id == BuildingDB.Instance.GetIdFromIndex(BuildingDB.Instance.GetBuildingCount - 1))
            {
                SetBuildingFromIndex(0);
            }
            else
            {
                // Set next
                SetBuildingFromIndex(BuildingDB.Instance.GetIndexFromId(this.Building.Id) + 1);
            }

            SetBuildingEvent(Building, currentAction);

            // Animator will call the "Update"
            animator.SetTrigger("SwitchBuilding");
            Debug.Log("Selecting next building");

        }

        public void SelectPreviousBuilding()
        {
            // if building is first in list, then set last building
            if (this.Building.Id == BuildingDB.Instance.GetIdFromIndex(0))
            {
                SetBuildingFromIndex(BuildingDB.Instance.GetBuildingCount - 1);
            }
            else
            {
                // Set previous
                SetBuildingFromIndex(BuildingDB.Instance.GetIndexFromId(this.Building.Id) - 1);
            }

            SetBuildingEvent(Building, currentAction);

            animator.SetTrigger("SwitchBuilding");
            Debug.Log("Selecting previous building");
        }

        public void CycleAction()
        {
            // Cycle through the various actions in enum BuildingActionType
            if ((byte)currentAction + 1 >= Enum.GetValues(typeof(BuildingActionType)).Length)
            {
                currentAction = (BuildingActionType)1;
            }
            else
            {
                currentAction++;
            }

            // When we Cycle, set this currentAction as the new "active" event.
            SetBuildingEvent(Building, currentAction);

            // Get the current times from the TimeEventManager 
            UpdateTimer();

            var updateData = new BuildingUIUpdateData();
            updateData.BuildingAction = currentAction;
            references.UpdateUI(updateData);

        }

        public void UpdateTimer()
        {
            var updateData = new BuildingUIUpdateData();

            int timeLeft = TimeEventManager.Instance.GetTimeRemaining(Building, currentAction);

            updateData.BuildingActionTimer = Utility.DefineTimer(timeLeft);

            float timeProceeded = TimeEventManager.Instance.GetTimeProceeded(Building, currentAction);
            updateData.BuildingActionTimerBar = timeProceeded / Building.TimeInSecondsPerAction[currentAction];

            references.UpdateUI(updateData);
        }

        // Called when any active timer finishes with it's designated action type which finished. Also restarts that timer.
        public void ActionEventCall(Building building, BuildingActionType type)
        {
            Debug.Log($"{building.ToString()}'s event {type.ToString()} just finished!");
            SetBuildingEvent(building, type);
        }
    }
}