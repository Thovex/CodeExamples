using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay;
using BaerAndHoggo.IO;
using BaerAndHoggo.SaveData;
using BaerAndHoggo.Utilities.Curve;
using UnityEngine;


public class CurveIO : IIO
{
    
}

public class CurveDB : DB<CurveObject, CurveIO>
{
    // Start is called before the first frame update
    protected override void InitializeDB()
    {
        IsInitialized = true;
    }

    protected override void StartLoad(SaveData loadData)
    {
        // Ignored
    }

    public override CurveIO OnSave_Implementation()
    {
        return new CurveIO();
    }

    public override void OnLoad_Implementation(CurveIO loadData)
    {
        // Ignored
    }
}
