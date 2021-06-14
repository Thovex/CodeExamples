using System;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Buildings
{
    [CreateAssetMenu(fileName = "New Fortress", menuName = "Idle Cards/Building/Fortress", order = 1)]
    [Serializable]
    public class BuildingFortress : Building
    {
        private BuildingFortress()
        {
            buildingType = BuildingType.Fortress;
        }
    }
}