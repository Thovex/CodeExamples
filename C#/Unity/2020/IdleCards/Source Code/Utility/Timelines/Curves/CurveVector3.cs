// Created by Jesse J. van Vliet - Copyright 2019

using System;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Timeline
{
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

        public CurveVector3(AnimationCurve curveX, AnimationCurve curveY, AnimationCurve curveZ) : this()
        {
            this.CurveX = curveX;
            this.CurveY = curveY;
            this.CurveZ = curveZ;
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

        public AnimationCurve CurveZ
        {
            get => curveZ;
            set => curveZ = value;
        }

        public override float GetLength()
        {
            return Utility.Max(CurveX.keys[CurveX.length - 1].time, CurveY.keys[CurveY.length - 1].time,
                CurveZ.keys[CurveZ.length - 1].time);
        }
    }
}