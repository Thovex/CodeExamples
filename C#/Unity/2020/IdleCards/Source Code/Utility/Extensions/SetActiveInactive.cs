using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetActiveInactive : MonoBehaviour
{
    [SerializeField] private GameObject target;
    public void SwapActive() => target.SetActive(!target.activeInHierarchy);
}
