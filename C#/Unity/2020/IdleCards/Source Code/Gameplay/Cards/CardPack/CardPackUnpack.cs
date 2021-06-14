using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using Random = UnityEngine.Random;

namespace BaerAndHoggo.Gameplay.Cards
{
    public struct ChanceData
    {
        public Dictionary<Rarity, float> Chances;
        private readonly float _chanceMax;

        public ChanceData(float common, float uncommon, float rare, float epic, float legendary)
        {
            Chances = new Dictionary<Rarity, float>
            {
                {Rarity.Common, common},
                {Rarity.Uncommon, uncommon},
                {Rarity.Rare, rare},
                {Rarity.Epic, epic},
                {Rarity.Legendary, legendary}
            };

            _chanceMax = common + uncommon + rare + epic + legendary;
        }

        public float GetMax(float multiplier = 0) => _chanceMax * multiplier;

        public static readonly ChanceData DefaultPack = new ChanceData(128.0F, 64.0F, 16.0F, 8.0F, 4.0F);
    }

    public static class CardPackUnpack
    {
        public static Dictionary<Rarity, int> TotalCardRaritiesCount = new Dictionary<Rarity, int>();
        public static int TotalPacksOpened { get; set; }

        static CardPackUnpack()
        {
            // Initialize counter for Total Packs Opened. TODO: Retrieve data from online db.
            TotalPacksOpened = 0;

            // Initialize counter for Total Cards Opened per Rarity. TODO: Retrieve data from online db.
            foreach (var rarity in (Rarity[]) Enum.GetValues(typeof(Rarity))) TotalCardRaritiesCount.Add(rarity, 0);
        }


        public static Dictionary<Rarity, int> DetermineQualities(this CardPack cardPack)
        {
            // Track the cards we have
            var cardPackRaritiesCount = new Dictionary<Rarity, int>();

            
            // TODO: What the fuck is going on over here lmao, somethign funky with rng value
            // probably just remake
            
            
            // Randomize a new card for the amount of cards this pack contains.
            for (var currentCardIndex = 0; currentCardIndex < cardPack.cardCount; currentCardIndex++)
            {
                // create a number in between 0 and the Maximum values of the chances.
                var rng = Random.Range(0, ChanceData.DefaultPack.GetMax(1));

                // Track index of current rarity
                byte idx = 0;

                while (rng >= 0 && idx < (byte) Rarity.Legendary)
                    // Remove the chance from the random number.
                    rng -= cardPack.chanceData.Chances[(Rarity) (++idx)];

                // Check if value exists in pack, if so increment, else add 1.
                if (cardPackRaritiesCount.TryGetValue((Rarity) idx, out var value))
                    cardPackRaritiesCount[(Rarity) idx]++;
                else
                    cardPackRaritiesCount.Add((Rarity) idx, 1);
            }

            // Track for total count
            foreach (var rarityCount in cardPackRaritiesCount)
                TotalCardRaritiesCount[rarityCount.Key] += rarityCount.Value;

            TotalPacksOpened++;

            Debug.Log($"{cardPack.packName} opened! " + string.Join(", ", 
                cardPackRaritiesCount.Select(a => $"{a.Key}: {a.Value}")));

            return cardPackRaritiesCount;
        }
    }
}