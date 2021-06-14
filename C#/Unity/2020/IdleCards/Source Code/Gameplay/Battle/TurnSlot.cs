using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.UI;
using Sirenix.OdinInspector;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Battle
{
    public abstract class TurnSlot : SerializedMonoBehaviour
    {        
        [SerializeField] protected Transform dragArea;
        
        private void Start()
        {
            _rectTransform = GetComponent<RectTransform>();
        }

        public BattlePlayer Player { get; set; }
        public bool CanUse { get; set; }
        public Card Card { get; set; }
        protected RectTransform _rectTransform;

    }
}