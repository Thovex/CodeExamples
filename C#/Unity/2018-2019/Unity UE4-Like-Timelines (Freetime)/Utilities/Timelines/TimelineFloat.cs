
using System;
using System.Collections.Generic;
using UnityEngine;

public class TimelineFloat : AbstractTimeline<Action<float>>
{
    public TimelineFloat() { }
    public TimelineFloat(MonoBehaviour worldContext, float endTime)
    {
        this.worldContext = worldContext;
        this.endTime = endTime;

        if (!this.worldContext) throw new System.Exception("Invalid WorldContext in Timeline");
    }

    public TimelineFloat(MonoBehaviour worldContext, CurveFloat floatCurve) : this(worldContext, 0)
    {
        this.curve = floatCurve;
        this.endTime = floatCurve.GetLength();
    }
    
    public override void TimelineFunctionality(List<Action<float>> Methods)
    {
        CurveFloat curveFloat = (CurveFloat)curve;

        foreach (var Method in this.Methods)
        {
            Method.Invoke(curveFloat.CurveX.Evaluate(currentTime));
        }
    }
}