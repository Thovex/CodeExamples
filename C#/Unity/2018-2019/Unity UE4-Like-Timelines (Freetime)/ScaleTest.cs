using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScaleTest : MonoBehaviour
{
    public CurveVector3 curve;

    private TimelineVector3 _timelineRef;

    private void OnEnable()
    {
        _timelineRef.OnTimelinePlay += LogPlay;
        _timelineRef.OnTimelineUpdate += LogUpdate;
        _timelineRef.OnTimelineFinish += LogFinish;
    }

    private void OnDisable()
    {
        _timelineRef.OnTimelinePlay -= LogPlay;
        _timelineRef.OnTimelineUpdate -= LogUpdate;
        _timelineRef.OnTimelineFinish -= LogFinish;
    }

    void Awake()
    {
        _timelineRef = new TimelineVector3(this, curve);
        _timelineRef.PlayTimeline((size) => SetScale(ref size));

    }

    void SetScale(ref Vector3 size)
    {
        transform.localScale = size;
    }

    void LogPlay()
    {
        Debug.Log("Timeline: play!");

    }

    void LogUpdate(float time)
    {
        Debug.Log("Timeline updated: " + time);

    }

    void LogFinish()
    {
        Debug.Log("Timeline: finished!");

    }
}