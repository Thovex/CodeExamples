
using System;
using System.Collections.Generic;

[Serializable]
public abstract class CurveBase
{
    private int curveCount;

    public CurveBase()
    {
    }

    public abstract float GetLength();

    protected int CurveCount { get => curveCount; set => curveCount = value; }
}