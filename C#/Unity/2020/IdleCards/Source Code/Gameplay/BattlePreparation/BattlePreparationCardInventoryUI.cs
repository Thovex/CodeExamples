using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Inventories;
using BaerAndHoggo.Utilities.Timeline;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.Serialization;
using UnityEngine.UI;

namespace BaerAndHoggo.UI
{
    /*
    public class BattlePreparationCardInventoryUI : CardInventoryUI
    {
         [SerializeField] private BattlePreperationUI battlePreperationUi;
        private bool _inventoryOpen;

        [Title("Opening UI")] [SerializeField] private Button inventorySetMinWidthButton;

        [SerializeField] private LayoutElement minHeightAdjustElement;
        [SerializeField] private CurveFloat minHeightCurve;

        private TimelineFloat _setMinHeightTl;

        protected override void Start()
        {
            base.Start();

            _setMinHeightTl = new TimelineFloat(this, minHeightCurve) {TimeScale = 2};

            inventorySetMinWidthButton.onClick.AddListener(OpenInventoryCall);

            OpenInventoryCall();
        }

        private void OpenInventoryCall()
        {
            if (_inventoryOpen)
            {
                _inventoryOpen = false;
                _setMinHeightTl.ReverseFrom(_setMinHeightTl.CurrentTime, value => OpenCloseInventoryMinHeight(ref value));
            }
            else
            {
                _inventoryOpen = true;
                _setMinHeightTl.PlayFrom(_setMinHeightTl.CurrentTime, value => OpenCloseInventoryMinHeight(ref value));
            }
        }

        private void OpenCloseInventoryMinHeight(ref float value)
        {
            minHeightAdjustElement.minHeight = value;
        }

        public override void UpdateUi(CardInventory cardInventory)
        {
            base.UpdateUi(cardInventory);

            foreach (var inventoryObject in inventoryObjects)
            {
                //GameObject inventoryGO = inventoryObject.Key.gameObject;
            }
        }
    }
    */
}