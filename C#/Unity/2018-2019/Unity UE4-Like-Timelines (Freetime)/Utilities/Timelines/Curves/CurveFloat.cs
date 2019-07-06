// Created by Jesse J. van Vliet - Copyright 2019

using System;
using UnityEngine;

[Serializable]
public class CurveFloat : CurveBase
{
    [SerializeField] private AnimationCurve curveX;

    public CurveFloat()
    {
        CurveCount = 1;
    }

    public CurveFloat(AnimationCurve CurveX) : this()
    {
        this.CurveX = CurveX;
    }

    public AnimationCurve CurveX { get => curveX; set => curveX = value; }

    public override float GetLength() => CurveX.keys[CurveX.length - 1].time;
}