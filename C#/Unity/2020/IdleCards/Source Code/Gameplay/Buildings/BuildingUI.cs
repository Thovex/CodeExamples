using System;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Gameplay.Towns;
using BaerAndHoggo.IO;
using BaerAndHoggo.Utilities;
using BaerAndHoggo.Utilities.Timeline;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

namespace BaerAndHoggo.UI
{
    public class BuildingUIUpdateData : UpdateAbleData
    {
        #region Variables
        public string BuildingName { get; set; }
        public long BuildingTier { get; set; }
        public Sprite BuildingImage { get; set; }
        public Color BuildingBackground { get; set; }
        public BuildingActionType BuildingAction { get; set; }
        #endregion
    }

    [Serializable]
    public class BuildingUIUpdateReferences : UpdateAbleReferences<BuildingUIUpdateData>
    {
        #region References
        [Title("References", "Building | Text")] 
        [SerializeField] private TMP_Text buildingActionRef;

        [SerializeField] private TMP_Text buildingActionTimerRef;
        [SerializeField] private TMP_Text buildingTierRef;

        [Title("References", "Building | Image")] 
        [SerializeField] private Image buildingBackgroundRef;

        [SerializeField] private Image buildingImageRef;

        [Title("References", "Building | Slider")] 
        [SerializeField] private Slider buildingActionTimerBarRef;

        [Title("References", "Building | Button")]
        [SerializeField] private Button nextButtonRef;
        [SerializeField] private Button previousButtonRef;
        [SerializeField] private Button upgradeTierButtonRef;
        
        #endregion References

        #region Timeline

        [Title("Data", "TimeLine")] 
        [SerializeField] private CurveFloat colorCurve;
        [SerializeField] private CurveFloat opacityCurve;

        private TimelineFloat _bgColorTl;
        private TimelineFloat _opacityTl;

        private Color _colorLerpFrom;
        private Color _colorLerpTo;

        public void InitializeTimelines(MonoBehaviour monoBehaviour)
        {
            this._bgColorTl = new TimelineFloat(monoBehaviour, colorCurve) {TimeScale = 2};
            this._opacityTl = new TimelineFloat(monoBehaviour, opacityCurve) {TimeScale = 2};
        }

        // Public Animations
        public void PopBuildingAnimation()
        {
            _opacityTl?.PlayFromStart(alpha => BuildingScaleLerp(ref alpha));
        }

        // Animation Implementations
        private void BackgroundColorLerp(ref float alpha) => buildingBackgroundRef.color
            = Color.Lerp(_colorLerpFrom, _colorLerpTo, alpha);

        private void BuildingScaleLerp(ref float alpha) => buildingImageRef.rectTransform.localScale
            = new Vector3(alpha, alpha, alpha);
        
        #endregion Timeline
        
        public void InitializeButtons(
            UnityAction nextButtonDelegate,
            UnityAction previousButtonDelegate,
            UnityAction upgradeTierButtonDelegate)
        {
            nextButtonRef.onClick.AddListener(nextButtonDelegate);
            previousButtonRef.onClick.AddListener(previousButtonDelegate);
            upgradeTierButtonRef.onClick.AddListener(upgradeTierButtonDelegate);
        }
        

        public override void Update(BuildingUIUpdateData data)
        {
            if (data.Name != default)
                this.nameRef.text = data.Name;

            if (data.BuildingAction != default)
                this.buildingActionRef.text = data.BuildingAction.ToString();

            if (data.BuildingImage != default)
                this.buildingImageRef.sprite = data.BuildingImage;

            if (data.BuildingTier != default)
                this.buildingTierRef.text = Utility.ToRoman(data.BuildingTier);

            if (data.BuildingBackground != default)
            {
                this._colorLerpFrom = this.buildingBackgroundRef.color;
                this._colorLerpTo = data.BuildingBackground;

                this._bgColorTl?.PlayFromStart(alpha => BackgroundColorLerp(ref alpha));
            }
        }

        public void InitTimer(ITimeEventCaller caller)
        {
            TimerUI.Instance.AddTimerElement(
                new TimerElement(
                    buildingActionTimerBarRef.GetComponent<RectTransform>(),
                    buildingActionTimerBarRef, 
                    buildingActionTimerRef),
                caller
            );
        }
    }
    
    public class BuildingUI :
        UpdateAbleScreenUI<BuildingDB, Building, BuildingUIUpdateReferences, BuildingUIUpdateData, 
            BuildingIO[]>
    {
        [SerializeField] private Town owningTown;
        [SerializeField] private BuildingActionType currentAction;
        [SerializeField] private int buildingIndex = 0;

        protected override void Awake()
        {
            base.Awake();
            
            SaveManager.Instance.OnLoad += LoadInitialBuilding;

            references.InitializeTimelines(this);
            references.InitializeButtons(SelectNextBuilding, SelectPreviousBuilding, UpgradeTierBuilding);
        }

        private void LoadInitialBuilding(IO.SaveData saveData)
        {
            SetItemFromIndex(0);
            SetActionToCurrentActive();
            SetBuildingEvent(item, currentAction);
            InitializeReferences();
        }

        public override void InitializeReferences()
        {
            references.Update(
                new BuildingUIUpdateData()
                {
                    Id = item.id,
                    Name = item.buildingName,
                    BuildingAction = currentAction,
                    BuildingImage = item.image,
                    BuildingTier = item.tier,
                    BuildingBackground = item.backgroundColor
                }
            );
            
            references.InitTimer(item);
            references.PopBuildingAnimation();
        }
        
        public void UpdateFromTown(Town town, int index)
        {
            owningTown = town;
            buildingIndex = index;
        }

        public void SetBuildingEvent(Building originBuilding, BuildingActionType action)
        {
            if (originBuilding.timeInSecondsPerAction[action] <= 0)
                throw new SystemException($"Invalid time on Entity {ToString()}");

            // base action time
            BuildingDB.Instance.GetBaseItem(originBuilding.id, out var baseBuilding);
            var baseSeconds = baseBuilding.timeInSecondsPerAction[action];

            // calculate upgrade time, affected by tier
            if (action == BuildingActionType.Upgrade)
                // time = base_time * tier^2
                originBuilding.timeInSecondsPerAction[action] =
                    baseSeconds * Convert.ToInt32(Math.Pow(originBuilding.tier, 2));

            // Set Event in the TimeEventManager

            var actionEvent = new ActionEvent(originBuilding, originBuilding, action);
            actionEvent.OnCallFunctionality += ActionEventCall;

            TimeEventManager.Instance.SetTimedEvent(
                originBuilding,
                actionEvent,
                originBuilding.timeInSecondsPerAction[action]
            );

        }

        public void SelectNextBuilding()
        {
            int oldBuildingIndex = buildingIndex;
            int retries = 0;
            
            while (true)
            {
                buildingIndex = (buildingIndex + 1) > owningTown.buildings.Count - 1 ? 0 : buildingIndex + 1;

                if (owningTown.buildings.ElementAt(buildingIndex).Value.buildingState == BuildProgress.State.Finished)
                {
                    if (buildingIndex == oldBuildingIndex) break;

                    SwitchBuilding();
                    Debug.Log("Selecting next building");
                }
                else
                {
                    retries++;

                    if (retries > 100) break;
                    continue;
                }

                break;
            }
        }

        public void SelectPreviousBuilding()
        {
            int oldBuildingIndex = buildingIndex;
            int retries = 0;
            
            while (true)
            {
                buildingIndex = (buildingIndex - 1) > -1 ? buildingIndex - 1 : owningTown.buildings.Count - 1;

                if (owningTown.buildings.ElementAt(buildingIndex).Value.buildingState == BuildProgress.State.Finished)
                {
                    if (buildingIndex == oldBuildingIndex) break;
                    
                    SwitchBuilding();
                    Debug.Log("Selecting previous building");
                }
                else
                {
                    retries++;

                    if (retries > 100) break;
                    continue;
                }

                break;
            }
        }

        public override void SetItemFromId(long id)
        {
            base.SetItemFromId(id);
            
            references.InitTimer(item);
            TimerUI.Instance.ForceUpdate(item);
            
            SetActionToCurrentActive();

            SetBuildingEvent(item, currentAction);
            InitializeReferences();

        }

        private void SetActionToCurrentActive()
        {
            if (TimeEventManager.Instance.GetCallerCurrentActiveEvent(item, out var activeEvent))
            {
                var actionEvent = (ActionEvent) activeEvent;
                currentAction = actionEvent.actionType;

                Debug.Log(currentAction);
            }
            else
            {
                // Pick first whatever we can find.
                currentAction = item.timeInSecondsPerAction.Keys.ElementAt(0);
            }
        }

        private void SwitchBuilding()
        {
            SetItemFromId(owningTown.buildings.ElementAt(buildingIndex).Key.id);
        }

        public void UpgradeTierBuilding()
        {
            Debug.Log("Upgrading building");

            // sets the timer to upgrade a building
            currentAction = BuildingActionType.Upgrade;
            SetBuildingEvent(item, currentAction);

            references.Update(new BuildingUIUpdateData
            {
                BuildingAction = currentAction
            });

            Debug.Log("Setting upgrade building timer");
        }

        public void CycleAction()
        {
            // Cycle through the various actions in enum BuildingActionType

            var existingKeys = item.timeInSecondsPerAction.Keys.ToList();
            var currentIndex = existingKeys.IndexOf(currentAction);

            currentAction = currentIndex + 1 >= existingKeys.Count ? existingKeys[0] : existingKeys[currentIndex + 1];

            // When we Cycle, set this currentAction as the new "active" event.
            SetBuildingEvent(item, currentAction);

            references.Update(new BuildingUIUpdateData()
            {
                BuildingAction = currentAction
            });

            references.InitTimer(item);
            TimerUI.Instance.ForceUpdate(item);
        }

        // Called when any active timer finishes with it's designated action type which finished. Also restarts that timer.
        public void ActionEventCall(ActionEvent theEvent, ITimeEventCaller caller, Building originBuilding, BuildingActionType type)
        {
            theEvent.OnCallFunctionality -= ActionEventCall;
            
            Debug.Log($"{originBuilding}'s event {type.ToString()} just finished!");

            if (type == BuildingActionType.Upgrade) UpgradeTier(originBuilding);

            SetBuildingEvent(originBuilding, type);
        }

        private void UpgradeTier(Building originBuilding)
        {
            // Increase the building tier by 1
            originBuilding.tier++;

            Debug.Log($"{originBuilding.buildingName}'s tier is now {originBuilding.tier.ToString()}!");

            if (originBuilding != item) return;

            references.Update(new BuildingUIUpdateData()
            {
                BuildingTier = item.tier
            });

            references.PopBuildingAnimation();
        }
    }
}