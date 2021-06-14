// Created by Jesse J. van Vliet - Copyright 2019

using System;
using System.Collections.Generic;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Timeline
{
    public class TimelineVector3 : AbstractTimeline<Action<Vector3>>
    {
        public TimelineVector3()
        {
        }

        public TimelineVector3(MonoBehaviour worldContext, float endTime)
        {
            this.WorldContext = worldContext;
            this.endTime = endTime;

            if (!this.WorldContext) throw new Exception("Invalid WorldContext in Timeline");
        }

        public TimelineVector3(MonoBehaviour worldContext, CurveVector3 vector3Curve) : this(worldContext, 0)
        {
            Curve = vector3Curve;
            endTime = vector3Curve.GetLength();
        }

        public void SetVector3Curve(CurveVector3 vector3Curve)
        {
            Curve = vector3Curve;
        }

        public override void TimelineFunctionality(List<Action<Vector3>> methods)
        {
            var curveVector3 = (CurveVector3) Curve;

            foreach (var method in this.methods)
                method.Invoke(new Vector3(
                    curveVector3.CurveX.Evaluate(currentTime),
                    curveVector3.CurveY.Evaluate(currentTime),
                    curveVector3.CurveZ.Evaluate(currentTime)
                ));
        }
    }
}