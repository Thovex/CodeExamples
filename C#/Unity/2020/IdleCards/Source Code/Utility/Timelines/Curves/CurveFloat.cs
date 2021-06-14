// Created by Jesse J. van Vliet - Copyright 2019

using System;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Timeline
{
    [Serializable]
    public class CurveFloat : CurveBase
    {
        [SerializeField] private AnimationCurve curveX;

        public CurveFloat()
        {
            CurveCount = 1;
        }

        public CurveFloat(AnimationCurve curveX) : this()
        {
            this.CurveX = curveX;
        }

        public AnimationCurve CurveX
        {
            get => curveX;
            set => curveX = value;
        }

        public override float GetLength()
        {
            return CurveX.keys[CurveX.length - 1].time;
        }
    }
}