// Created by Jesse J. van Vliet - Copyright 2019

using System;
using System.Collections.Generic;
using UnityEngine;

namespace BaerAndHoggo.Utilities.Timeline
{
    public class Timeline : AbstractTimeline<Action>
    {
        public Timeline()
        {
        }

        public Timeline(MonoBehaviour worldContext, float endTime)
        {
            this.WorldContext = worldContext;
            this.endTime = endTime;

            if (!this.WorldContext) throw new Exception("Invalid WorldContext in Timeline");
        }

        ~Timeline()
        {
            if (Active) InternalFinishTimeline();
        }


        public override void TimelineFunctionality(List<Action> methods)
        {
            foreach (var method in this.methods) method.Invoke();
        }
    }
}