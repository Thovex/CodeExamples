private Vector3Int MinEntropyCoords()
{
    float minEntropy = 0;
    Vector3Int minEntropyCoords = new Vector3Int();

    System.Random random = new System.Random();

    For3(outputSize, (x, y, z) =>
    {
        Vector3Int currentCoordinates = new Vector3Int(x, y, z);

        if (wave.MatrixData[x, y, z].AllowedCount() > 1)
        {
            float entropy = ShannonEntropy(currentCoordinates);
            float entropyPlusNoise = entropy - (float)random.NextDouble() / 1000;

            if (minEntropy == 0 || entropyPlusNoise < minEntropy)
            {
                minEntropy = entropyPlusNoise;
                minEntropyCoords = new Vector3Int(x, y, z);
            }
        }
    });

    return minEntropyCoords;
}

public float ShannonEntropy(Vector3Int currentCoordinates)
{
    int sumOfWeights = 0;
    float sumOfWeightsLogWeights = 0;

    foreach (var pair in wave.GetDataAt(currentCoordinates).allowedPatterns)
    {
        training.Weights.TryGetValue(pair.Key, out int weight);

        sumOfWeights += weight;
        sumOfWeightsLogWeights += weight * (float)Math.Log(weight);
    }
    return (float)Math.Log(sumOfWeights) - (sumOfWeightsLogWeights / sumOfWeights);
}