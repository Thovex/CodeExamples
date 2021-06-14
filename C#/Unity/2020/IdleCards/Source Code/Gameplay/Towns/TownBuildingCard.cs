using BaerAndHoggo.Gameplay.Buildings;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Towns
{
    public abstract class TownBuildingCard : MonoBehaviour
    {
        [SerializeField] protected TMP_Text buildingName;
        
        protected Building Building { get; set; }
        protected ITimeEventCaller EventCaller { get; set; }

        public void SetBuilding(ITimeEventCaller eventCaller, Building building)
        {
            this.EventCaller = eventCaller;
            this.Building = building;
            this.buildingName.text = building.buildingName;
            
            this.InitializeCard();
        }
        
        public abstract Button GetUseButton();

        protected abstract void InitializeCard();
    }
}