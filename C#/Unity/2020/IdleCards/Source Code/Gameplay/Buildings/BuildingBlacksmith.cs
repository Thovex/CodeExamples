using System;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Buildings
{
    [CreateAssetMenu(fileName = "New Blacksmith", menuName = "Idle Cards/Building/Blacksmith", order = 2)]
    [Serializable]
    public class BuildingBlacksmith : Building
    {
        public BuildingBlacksmith()
        {
            buildingType = BuildingType.Blacksmith;
        }
    }
}