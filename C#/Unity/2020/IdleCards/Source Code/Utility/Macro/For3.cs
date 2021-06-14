using System;
using BaerAndHoggo.Types;
using UnityEngine;

namespace BaerAndHoggo.Utilities
{
    public static partial class Utility
    {
        public static void For3B(Vector3Int data, bool breakout, Action<int, int, int, bool> iterator)
        {
            for (var x = 0; x < data.x; x++)
            {
                if (breakout) goto end;
                for (var y = 0; y < data.y; y++)
                {
                    if (breakout) goto end;
                    for (var z = 0; z < data.z; z++)
                    {
                        if (breakout) goto end;
                        iterator(x, y, z, breakout);
                    }
                }
            }

            end: ;
        }

        public static void For3B<T>(Matrix3<T> data, bool breakout, Action<int, int, int, bool> iterator)
        {
            if (data != null) For3B(new Vector3Int(data.SizeX, data.SizeY, data.SizeZ), breakout, iterator);
        }

        public static void For3(Vector3Int data, Action<int, int, int> iterator)
        {
            for (var x = 0; x < data.x; x++)
            for (var y = 0; y < data.y; y++)
            for (var z = 0; z < data.z; z++)
                iterator(x, y, z);
        }

        public static void For3(Vector3Int data, int iteratorJumpSize, Action<int, int, int> iterator)
        {
            for (var x = 0; x < data.x; x += iteratorJumpSize)
            for (var y = 0; y < data.y; y += iteratorJumpSize)
            for (var z = 0; z < data.z; z += iteratorJumpSize)
                iterator(x, y, z);
        }

        public static void For3<T>(Matrix3<T> data, Action<int, int, int> iterator)
        {
            if (data != null) For3(new Vector3Int(data.SizeX, data.SizeY, data.SizeZ), iterator);
        }

        public static void For3<T>(Matrix3<T> data, int iteratorJumpSize, Action<int, int, int> iterator)
        {
            if (data != null) For3(new Vector3Int(data.SizeX, data.SizeY, data.SizeZ), iteratorJumpSize, iterator);
        }
    }
}