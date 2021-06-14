// Created by Jesse J. van Vliet - Copyright 2019

using System;

namespace BaerAndHoggo.Utilities.Timeline
{
    [Serializable]
    public abstract class CurveBase
    {
        private int _curveCount;

        protected int CurveCount
        {
            get => _curveCount;
            set => _curveCount = value;
        }

        public abstract float GetLength();
    }
}