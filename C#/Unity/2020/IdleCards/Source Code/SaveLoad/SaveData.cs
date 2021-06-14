using System;
using BaerAndHoggo.Gameplay.Buildings;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.Gameplay.Inventories;
using BaerAndHoggo.Gameplay.Time;
using BaerAndHoggo.Gameplay.Towns;
using BaerAndHoggo.Utilities;
using UnityEngine.Serialization;

namespace BaerAndHoggo.IO
{
    [Serializable]
    public class SaveData
    {
        public DateTime saveDate;

        public TimeEventIO TimeEventIO { get; set; }
        public TownIO[] TownIO { get; set; }
        public CardIO[] CardIO { get; set; }
        public CardPackIO[] CardPackIO { get; set; }
        public CardStackIO[] CardStackIO { get; set; }
        public BuildingIO[] BuildingIO { get; set; }

        public override string ToString()
        {
            return $"<SAVE DETAILS>" +
                   $"<Save Section TownIO>{Environment.NewLine}{Environment.NewLine}" +
                   $"{TownIO.ToDebugString()}" +
                   $"</Save Section TownIO>{Environment.NewLine}{Environment.NewLine}" +

                   $"<Save Section CardIO>{Environment.NewLine}{Environment.NewLine}" +
                   $"{CardIO.ToDebugString()}" +
                   $"</Save Section CardIO>{Environment.NewLine}{Environment.NewLine}" +

                   $"<Save Section CardStackIO>{Environment.NewLine}{Environment.NewLine}" +
                   $"{CardStackIO.ToDebugString()}" +
                   $"</Save Section CardStackIO>{Environment.NewLine}{Environment.NewLine}" +

                   $"<Save Section BuildingIO>{Environment.NewLine}{Environment.NewLine}" +
                   $"{BuildingIO.ToDebugString()}" +
                   $"</Save Section BuildingIO>{Environment.NewLine}{Environment.NewLine}";
        }
    }
}