// Created by Jesse J. van Vliet - Copyright 2019

using System;
using System.Collections.Generic;
using UnityEngine;

public class Timeline : AbstractTimeline<Action>
{
    public Timeline() { }
    public Timeline(MonoBehaviour worldContext, float endTime)
    {
        this.worldContext = worldContext;
        this.endTime = endTime;

        if (!this.worldContext) throw new System.Exception("Invalid WorldContext in Timeline");
    }

    ~Timeline()
    {
        if (active) InternalFinishTimeline();
    }


    public override void TimelineFunctionality(List<Action> Methods)
    {
        foreach (var Method in this.Methods)
        {
            Method.Invoke();
        }
    }
}