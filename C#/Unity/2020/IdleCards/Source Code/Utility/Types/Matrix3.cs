using System;
using BaerAndHoggo.Utilities;
using UnityEngine;

namespace BaerAndHoggo.Types
{
    [Serializable]
    public class Matrix3<T>
    {
        private T[,,] _matrixData;

        public Matrix3()
        {
        }

        public Matrix3(int matrixSize)
        {
            MatrixData = new T[matrixSize, matrixSize, matrixSize];
        }

        public Matrix3(Vector3Int matrixSize)
        {
            MatrixData = new T[matrixSize.x, matrixSize.y, matrixSize.z];
        }

        public T[,,] MatrixData
        {
            get => _matrixData;
            set
            {
                _matrixData = value;
                SetSize();
            }
        }

        public int SizeX { get; protected set; }
        public int SizeY { get; protected set; }
        public int SizeZ { get; protected set; }
        public Vector3Int Size { get; protected set; }

        protected void SetSize()
        {
            SizeX = _matrixData.GetLength(0);
            SizeY = _matrixData.GetLength(1);
            SizeZ = _matrixData.GetLength(2);

            Size = new Vector3Int(SizeX, SizeY, SizeZ);
        }

        public T GetDataAt(Vector3Int coordinate)
        {
            return MatrixData[coordinate.x, coordinate.y, coordinate.z];
        }

        public T GetDataAt(int x, int y, int z)
        {
            return GetDataAt(new Vector3Int(x, y, z));
        }

        public void SetDataAt(Vector3Int coordinate, T data)
        {
            SetDataAt(coordinate.x, coordinate.y, coordinate.z, data);
        }

        public void SetDataAt(int x, int y, int z, T data)
        {
            MatrixData[x, y, z] = data;
        }

        public bool Contains(T check, out Vector3Int coord)
        {
            var contains = false;

            coord = Vector3Int.zero;
            var containsCoord = Vector3Int.zero;

            Utility.For3(this, (x, y, z) =>
            {
                if (Equals(MatrixData[x, y, z], check))
                {
                    contains = true;
                    containsCoord = new Vector3Int(x, y, z);
                }
            });

            coord = containsCoord;
            return contains;
        }

        public bool Contains(T check)
        {
            var contains = false;
            Utility.For3(this, (x, y, z) =>
            {
                if (Equals(MatrixData[x, y, z], check)) contains = true;
            });
            return contains;
        }

        public void Clear()
        {
            MatrixData = new T[SizeX, SizeY, SizeZ];
        }

        public void Empty()
        {
            SizeX = 0;
            SizeY = 0;
            SizeZ = 0;

            Clear();
        }

        public void TransposeFlip()
        {
            var originalData = MatrixData;
            var copyMatrix = new T[SizeX, SizeY, SizeZ];

            for (var x = 0; x < SizeX; x++)
            for (var y = 0; y < SizeY; y++)
            for (var z = 0; z < SizeZ / 2 + 1; z++)
            {
                var elem = originalData[x, y, z];
                var neighbor = originalData[x, y, SizeZ - z - 1];
                copyMatrix[x, y, z] = neighbor;
                copyMatrix[x, y, SizeZ - z - 1] = elem;
            }

            MatrixData = copyMatrix;
        }

        public virtual void RotateCounterClockwise(int times)
        {
            var minX = 0;
            var maxX = SizeX - 1;

            var minZ = 0;
            var maxZ = SizeZ - 1;

            for (var i = 0; i < times; i++)
            for (var increment = 0; increment < SizeX / 2; increment++)
            for (var n = 0 + increment; n < maxX - increment; n++)
            {
                var originalData = MatrixData;
                var copyMatrix = new T[SizeX, SizeY, SizeZ];

                Utility.For3(this, (x, y, z) =>
                {
                    if (x >= minX + increment && x <= maxX - 1 - increment && z == minZ + increment)
                        copyMatrix[x + 1, y, z] = originalData[x, y, z];
                    else if (x == maxX - increment && z >= minZ + increment && z <= maxZ - 1 - increment)
                        copyMatrix[x, y, z + 1] = originalData[x, y, z];
                    else if (x >= minX + 1 + increment && x <= maxX - increment && z == maxZ - increment)
                        copyMatrix[x - 1, y, z] = originalData[x, y, z];
                    else if (x == minX + increment && z >= minZ + 1 + increment && z <= maxZ - increment)
                        copyMatrix[x, y, z - 1] = originalData[x, y, z];
                    else
                        copyMatrix[x, y, z] = originalData[x, y, z];
                });

                MatrixData = copyMatrix;
            }
        }

        protected T[,,] FlipRightLeft()
        {
            var originalData = MatrixData;
            var copyMatrix = new T[SizeX, SizeY, SizeZ];

            var x = SizeX - 1;
            var z = SizeZ - 1;

            for (var y = 0; y < SizeY; y++)
            {
                copyMatrix[0, y, 0] = originalData[x, y, 0];
                copyMatrix[x, y, 0] = originalData[0, y, 0];
                copyMatrix[0, y, z] = originalData[x, y, z];
                copyMatrix[x, y, z] = originalData[0, y, z];
            }

            return copyMatrix;
        }

        protected T[,,] FlipForwardBack()
        {
            var originalData = MatrixData;
            var copyMatrix = new T[SizeX, SizeY, SizeZ];

            var x = SizeX - 1;
            var z = SizeZ - 1;

            for (var y = 0; y < SizeY; y++)
            {
                copyMatrix[0, y, 0] = originalData[0, y, z];
                copyMatrix[0, y, z] = originalData[0, y, 0];
                copyMatrix[x, y, 0] = originalData[x, y, z];
                copyMatrix[x, y, z] = originalData[x, y, 0];
            }

            return copyMatrix;
        }

        protected T[,,] FlipUpDown()
        {
            var originalData = MatrixData;
            var copyMatrix = new T[SizeX, SizeY, SizeZ];

            var y = SizeY - 1;

            for (var x = 0; x < SizeX; x++)
            for (var z = 0; z < SizeZ; z++)
            {
                copyMatrix[x, 0, z] = originalData[x, y, z];
                copyMatrix[x, y, z] = originalData[x, 0, z];
            }

            return copyMatrix;
        }

        public virtual void PushData(Vector3Int direction)
        {
            var originalData = MatrixData;
            var copyMatrix = new T[SizeX, SizeY, SizeZ];


            Utility.For3(this, (x, y, z) =>
            {
                var sweepCoord = new Vector3Int(x + -direction.x, y + -direction.y, z + -direction.z);

                if (ValidCoordinate(sweepCoord))
                {
                    copyMatrix[x, y, z] = originalData[sweepCoord.x, sweepCoord.y, sweepCoord.z];
                }
                else
                {
                    if (typeof(T) == typeof(string))
                        copyMatrix[x, y, z] = (T) Convert.ChangeType("null", typeof(T));
                    else
                        copyMatrix[x, y, z] = default;
                }
            });

            MatrixData = copyMatrix;
        }

        public bool ValidCoordinate(Vector3Int coords)
        {
            if (coords.x < 0) return false;
            if (coords.x >= SizeX) return false;

            if (coords.y < 0) return false;
            if (coords.y >= SizeY) return false;

            if (coords.z < 0) return false;
            if (coords.z >= SizeZ) return false;

            return true;
        }
    }
}