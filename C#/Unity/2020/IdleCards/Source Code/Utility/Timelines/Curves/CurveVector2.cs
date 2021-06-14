// Created by Jesse J. van Vliet - Copyright 2019

using System;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Timeline
{
    [Serializable]
    public class CurveVector2 : CurveBase
    {
        [SerializeField] private AnimationCurve curveX;
        [SerializeField] private AnimationCurve curveY;

        public CurveVector2()
        {
            CurveCount = 2;
        }

        public CurveVector2(AnimationCurve curveX, AnimationCurve curveY) : this()
        {
            this.CurveX = curveX;
            this.CurveY = curveY;
        }

        public AnimationCurve CurveX
        {
            get => curveX;
            set => curveX = value;
        }

        public AnimationCurve CurveY
        {
            get => curveY;
            set => curveY = value;
        }

        public override float GetLength()
        {
            return curveX.keys[CurveX.length - 1].time >= curveY.keys[CurveY.length - 1].time
                ? curveX.keys[CurveX.length - 1].time
                : curveY.keys[CurveY.length - 1].time;
        }
    }
}