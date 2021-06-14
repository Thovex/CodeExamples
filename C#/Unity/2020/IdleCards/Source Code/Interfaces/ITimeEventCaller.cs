using UnityEngine;

public enum CallerEventType
{
    Building,
    Town,
    CardPack,
}

public interface ITimeEventCaller
{
    CallerEventType GetEventType();
    long GetID();
}