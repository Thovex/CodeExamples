using System;
using UnityEngine;

[Serializable]
public class CurveVector2 : CurveBase
{
    [SerializeField] private AnimationCurve curveX;
    [SerializeField] private AnimationCurve curveY;

    public CurveVector2()
    {
        CurveCount = 2;
    }

    public CurveVector2(AnimationCurve CurveX, AnimationCurve CurveY) : this()
    {
        this.CurveX = CurveX;
        this.CurveY = CurveY;
    }

    public AnimationCurve CurveX { get => curveX; set => curveX = value; }
    public AnimationCurve CurveY { get => curveY; set => curveY = value; }

    public override float GetLength() => curveX.keys[CurveX.length- 1].time >= curveY.keys[CurveY.length- 1].time ? curveX.keys[CurveX.length- 1].time : curveY.keys[CurveY.length- 1].time;
}