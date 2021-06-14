// Created by Jesse J. van Vliet - Copyright 2019

using System;
using System.Collections.Generic;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Timeline
{
    public class TimelineVector2 : AbstractTimeline<Action<Vector2>>
    {
        public TimelineVector2()
        {
        }

        public TimelineVector2(MonoBehaviour worldContext, float endTime)
        {
            this.WorldContext = worldContext;
            this.endTime = endTime;

            if (!this.WorldContext) throw new Exception("Invalid WorldContext in Timeline");
        }

        public TimelineVector2(MonoBehaviour worldContext, CurveVector2 vector2Curve) : this(worldContext, 0)
        {
            Curve = vector2Curve;
            endTime = vector2Curve.GetLength();
        }

        public void SetVector2Curve(CurveVector2 vector2Curve)
        {
            Curve = vector2Curve;
        }

        public override void TimelineFunctionality(List<Action<Vector2>> methods)
        {
            var curveVector2 = (CurveVector2) Curve;

            foreach (var method in this.methods)
                method.Invoke(new Vector3(
                    curveVector2.CurveX.Evaluate(currentTime),
                    curveVector2.CurveY.Evaluate(currentTime)
                ));
        }
    }
}