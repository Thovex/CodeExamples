using UnityEngine;
using System.Collections;

namespace BaerAndHoggo.SaveData
{
    public interface ISaveData<T>
    {
        T ToSaveData();
    }
}