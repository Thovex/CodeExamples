using UnityEngine;
using System.Collections.Generic;
using System.Linq;

namespace BaerAndHoggo.Gameplay.Cards
{
    public struct ChanceData
    {
        public Dictionary<Rarity, float> Chances;
        private float ChanceMax;
        public ChanceData(float common, float uncommon, float rare, float epic, float legendary)
        {

            this.Chances = new Dictionary<Rarity, float>() {
                { Rarity.Common, common },
                { Rarity.Uncommon, uncommon },
                { Rarity.Rare, rare },
                { Rarity.Epic, epic },
                { Rarity.Legendary, legendary }
            };

            this.ChanceMax = common + uncommon + rare + epic + legendary;
        }

        public float GetMax(float multiplier = 0) => this.ChanceMax * multiplier;
        public readonly static ChanceData DEFAULT_PACK = new ChanceData(64.0F, 32.0F, 16.0F, 8.0F, 4.0F);
    }

    public static class CardUnpack
    {

        public static Dictionary<Rarity, int> TotalCardRaritiesCount = new Dictionary<Rarity, int>();
        public static int TotalPacksOpened;

        static CardUnpack()
        {
            // Initialize counter for Total Packs Opened. TODO: Retrieve data from online db.
            TotalPacksOpened = 0;

            // Initialize counter for Total Cards Opened per Rarity. TODO: Retrieve data from online db.
            foreach (Rarity rarity in (Rarity[])Rarity.GetValues(typeof(Rarity)))
            {
                TotalCardRaritiesCount.Add(rarity, 0);
            }
        }


        public static CardPack CreateCardPack(ChanceData chanceData, int packCardCount = 5, int multiplier = 1)
        {
            // Track the cards we have
            Dictionary<Rarity, int> CardPackRaritiesCount = new Dictionary<Rarity, int>();

            // Randomize a new card for the amount of cards this pack contains.
            for (int currentCardIndex = 0; currentCardIndex < packCardCount; currentCardIndex++)
            {
                // create a number in between 0 and the Maximum values of the chances.
                float RNG = Random.Range(0, chanceData.GetMax(multiplier));

                // Track index of current rarity
                byte idx = 0;

                while (RNG >= 0 && idx < (byte)Rarity.Legendary)
                {
                    // Remove the chance from the random number.
                    RNG -= chanceData.Chances[(Rarity)(++idx)];
                }

                // Check if value exists in pack, if so increment, else add 1.
                if (CardPackRaritiesCount.TryGetValue((Rarity)idx, out var value))
                {
                    CardPackRaritiesCount[(Rarity)idx]++;
                }
                else
                {
                    CardPackRaritiesCount.Add((Rarity)idx, 1);
                }
            }

            // Track for total count
            foreach (var RarityCount in CardPackRaritiesCount)
            {
                TotalCardRaritiesCount[RarityCount.Key] += RarityCount.Value;
            }

            TotalPacksOpened++;

            //Debug.Log(string.Join(", ", CardPackRaritiesCount.Select(a => $"{a.Key}: {a.Value}")));

            return new CardPack();
        }
    }
}