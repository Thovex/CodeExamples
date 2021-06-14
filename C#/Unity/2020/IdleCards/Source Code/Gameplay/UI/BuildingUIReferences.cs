using Sirenix.OdinInspector;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using UnityEngine.Events;
using BaerAndHoggo.Gameplay.Time;
using System.Collections;

namespace BaerAndHoggo.UI
{
    public struct BuildingUIUpdateData
    {
        public string BuildingName { get; set; }
        public BuildingActionType BuildingAction { get; set; }
        public string BuildingActionTimer { get; set; }
        public float BuildingActionTimerBar { get; set; }
        public Sprite BuildingImage { get; set; }
    }

    public class BuildingUIReferences : SerializedMonoBehaviour
    {
        [Title("Required references for functionality")]
        [Required] [SerializeField] private TMP_Text buildingName;
        [Required] [SerializeField] private TMP_Text buildingAction;
        [Required] [SerializeField] private TMP_Text buildingActionTimer;
        [Required] [SerializeField] private Slider buildingActionTimerBar;
        [Required] [SerializeField] private Image buildingImage;


        [Title("Preserved Button Functionality")]
        [Required] [SerializeField] public Button nextButton;
        [Required] [SerializeField] public Button previousButton;

        private BuildingUIUpdateData data;
        private Coroutine barUpdateCoroutine;


        public void InitializeButtons(UnityAction nextButtonDelegate, UnityAction previousButtonDelegate)
        {
            nextButton.onClick.AddListener(nextButtonDelegate);
            previousButton.onClick.AddListener(previousButtonDelegate);
        }

        public void UpdateUI(BuildingUIUpdateData newData)
        {
            this.data = newData;

            if (this.data.BuildingName != default)
            {
                buildingName.text = this.data.BuildingName;
            }

            if (this.data.BuildingAction != default)
            {
                buildingAction.text = this.data.BuildingAction.ToString();
            }

            if (this.data.BuildingActionTimer != default)
            {
                buildingActionTimer.text = this.data.BuildingActionTimer;

                
                if (barUpdateCoroutine != null) StopCoroutine(barUpdateCoroutine);

                barUpdateCoroutine = StartCoroutine(LerpBar(this.data.BuildingActionTimerBar));
            }

            if (this.data.BuildingImage != default)
            {
                buildingImage.sprite = this.data.BuildingImage;
            }
        }
        
        public IEnumerator LerpBar(float to)
        {

            while (!Mathf.Approximately(buildingActionTimerBar.value, this.data.BuildingActionTimerBar))
            {
                buildingActionTimerBar.value = Mathf.Lerp(buildingActionTimerBar.value, to, Time.deltaTime * 5);

                yield return new WaitForEndOfFrame();
            }

            StopCoroutine(LerpBar(to));
        }
    }
}