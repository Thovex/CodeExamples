using System;
using UnityEngine;

[Serializable]
public class CurveVector3 : CurveBase
{
    [SerializeField] private AnimationCurve curveX;
    [SerializeField] private AnimationCurve curveY;
    [SerializeField] private AnimationCurve curveZ;


    public CurveVector3()
    {
        CurveCount = 3;
    }

    public CurveVector3(AnimationCurve CurveX, AnimationCurve CurveY, AnimationCurve CurveZ) : this()
    {
        this.CurveX = CurveX;
        this.CurveY = CurveY;
        this.CurveZ = CurveZ;
    }

    public AnimationCurve CurveX { get => curveX; set => curveX = value; }
    public AnimationCurve CurveY { get => curveY; set => curveY = value; }
    public AnimationCurve CurveZ { get => curveZ; set => curveZ = value; }

    public override float GetLength() => UtilitiesMath.Max(CurveX.keys[CurveX.length - 1].time, CurveY.keys[CurveY.length - 1].time, CurveZ.keys[CurveZ.length - 1].time);
}