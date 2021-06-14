// Created by Jesse J. van Vliet - Copyright 2019

using System;
using System.Collections.Generic;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Timeline
{
    [Serializable]
    public class TimelineFloat : AbstractTimeline<Action<float>>
    {
        public TimelineFloat()
        {
        }

        public TimelineFloat(MonoBehaviour worldContext, float endTime)
        {
            this.WorldContext = worldContext;
            this.endTime = endTime;

            if (!this.WorldContext) throw new Exception("Invalid WorldContext in Timeline");
        }

        public TimelineFloat(MonoBehaviour worldContext, CurveFloat floatCurve) : this(worldContext, 0)
        {
            Curve = floatCurve;
            endTime = floatCurve.GetLength();
        }

        public void SetFloatCurve(CurveFloat floatCurve)
        {
            Curve = floatCurve;
        }

        public override void TimelineFunctionality(List<Action<float>> methods)
        {
            var curveFloat = (CurveFloat) Curve;

            foreach (var method in this.methods) method.Invoke(curveFloat.CurveX.Evaluate(currentTime));
        }
    }
}