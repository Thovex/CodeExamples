
using System;
using System.Collections.Generic;
using UnityEngine;

public class TimelineVector3 : Timeline<Action<Vector3>>
{
    public TimelineVector3() { }
    public TimelineVector3(MonoBehaviour worldContext, float endTime)
    {
        this.worldContext = worldContext;
        this.endTime = endTime;

        if (!this.worldContext) throw new System.Exception("Invalid WorldContext in Timeline");
    }

    public TimelineVector3(MonoBehaviour worldContext, CurveVector3 vector3Curve) : this(worldContext, 0)
    {
        this.curve = vector3Curve;
        this.endTime = vector3Curve.GetLength();
    }

    public override void TimelineFunctionality(List<Action<Vector3>> Methods)
    {
        CurveVector3 curveVector3 = (CurveVector3)curve;

        foreach (var Method in this.Methods)
        {
            Method.Invoke(new Vector3(
                curveVector3.CurveX.Evaluate(currentTime),
                curveVector3.CurveY.Evaluate(currentTime),
                curveVector3.CurveZ.Evaluate(currentTime)
            ));
        }
    }
}