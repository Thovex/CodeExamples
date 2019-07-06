// Created by Jesse J. van Vliet - Copyright 2019

[System.Serializable]
public abstract class CurveBase
{
    private int curveCount;

    public CurveBase()
    {
    }

    public abstract float GetLength();

    protected int CurveCount { get => curveCount; set => curveCount = value; }
}