
using System;
using System.Collections.Generic;
using UnityEngine;

public class TimelineVector2 : AbstractTimeline<Action<Vector2>>
{
    public TimelineVector2() { }
    public TimelineVector2(MonoBehaviour worldContext, float endTime)
    {
        this.worldContext = worldContext;
        this.endTime = endTime;

        if (!this.worldContext) throw new System.Exception("Invalid WorldContext in Timeline");
    }

    public TimelineVector2(MonoBehaviour worldContext, CurveVector2 vector2Curve) : this(worldContext, 0)
    {
        this.curve = vector2Curve;
        this.endTime = vector2Curve.GetLength();
    }

    public override void TimelineFunctionality(List<Action<Vector2>> Methods)
    {
        CurveVector2 curveVector2 = (CurveVector2)curve;

        foreach (var Method in this.Methods)
        {
            Method.Invoke(new Vector3(
                curveVector2.CurveX.Evaluate(currentTime),
                curveVector2.CurveY.Evaluate(currentTime)
            ));
        }
    }
}