using System;
using System.Collections.Generic;
using UnityEngine;
using static Thovex.Utility;

[Serializable]
public class Pattern : Matrix3<Module>
{
    [SerializeField] private Dictionary<Vector3Int, List<Pattern>> _propagator;

    public Dictionary<Vector3Int, List<Pattern>> Propagator
    {
        get => _propagator;
        set => _propagator = value;
    }

    public int id;

    public int N
    {
        get => n;
        set => n = value;
    }

    private int n = 2;


    public Pattern(int patternSize)
    {
        MatrixData = new Module[patternSize, patternSize, patternSize];
        N = patternSize;
    }

    public Pattern(Vector3Int patternSize)
    {
        MatrixData = new Module[patternSize.x, patternSize.y, patternSize.z];
        N = patternSize.x;

    }

    public Pattern(Module[,,] patternData)
    {
        MatrixData = patternData;
        N = MatrixData.GetLength(0);
    }

    public override void RotateCounterClockwise(int times)
    {
        base.RotateCounterClockwise(times);

        for (int i = 0; i < times; i++)
        {
            For3(this, (x, y, z) => { MatrixData[x, y, z].RotationEuler += new Vector3Int(0, -90, 0); });
        }
    }

    public static bool operator ==(Pattern lhs, Pattern rhs)
    {
        bool bIsEqual = true;

        if (lhs == null || rhs == null) return false;
        if (lhs.Size != rhs.Size) return false;

        For3(lhs, (x, y, z) =>
        {
            if (lhs.GetDataAt(x, y, z) != rhs.GetDataAt(x, y, z))
            {
                bIsEqual = false;
            }
        });
        return bIsEqual;
    }

    public static bool operator !=(Pattern lhs, Pattern rhs)
    {
        return !(lhs == rhs);
    }

}