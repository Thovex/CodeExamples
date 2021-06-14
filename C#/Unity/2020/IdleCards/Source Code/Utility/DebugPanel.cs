using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.IO;
using Sirenix.OdinInspector;
using UnityEngine;

public class DebugPanel : MonoBehaviour
{
    [Button]
    public void KillSave()
    {
        Save.RemoveSaveFileIo("IC_Save_Data");
    }
}
