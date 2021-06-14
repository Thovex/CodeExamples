using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Utilities;
using UnityEngine;
using UnityEngine.Serialization;

public class CubeSpawner : MonoBehaviour
{
     [SerializeField] private GameObject cube;

    private readonly List<Vector3> _positionsInUse = new List<Vector3>();

    private IEnumerator Start()
    {
        var count = 0;
        var maxCount = 10;

        while (count < maxCount)
        {
            var countVector = new Vector3Int(count, count, count);

            Utility.For3(countVector, (x, y, z) =>
            {
                var coord = new Vector3(x, y, z);

                if (!_positionsInUse.Contains(coord))
                {
                    SpawnCube(coord);
                    _positionsInUse.Add(coord);
                }
            });

            count++;
            yield return new WaitForSeconds(1);
        }
    }

    private void SpawnCube(Vector3 position)
    {
        Instantiate(cube, position, Quaternion.identity);
    }
}