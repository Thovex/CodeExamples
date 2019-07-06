// Created by Jesse J. van Vliet - Copyright 2019 - GPLv3 Licensed

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public abstract class Timeline<T>
{

    public List<T> Methods;
    protected MonoBehaviour worldContext;
    protected CurveBase curve;
    protected bool active = false;

    public Timeline() {}
    public Timeline(MonoBehaviour worldContext, float endTime)
    {
        this.worldContext = worldContext;
        this.endTime = endTime;

        if (!this.worldContext) throw new System.Exception("Invalid WorldContext in Timeline");
    }

    public Timeline(MonoBehaviour worldContext, CurveBase curve) : this(worldContext, 0)
    {
        this.curve = curve;
        this.endTime = curve.GetLength();
    }

    ~Timeline()
    {
        if (active) InternalFinishTimeline();
    }

    protected float currentTime = 0.0F;
    protected float endTime = 0.0F;
    protected float timeScale = 1.0F;

    public float CurrentTime { get => currentTime; }
    public float EndTime { get => endTime; set => endTime = value; }
    public float TimeScale { get => timeScale; set => timeScale = value; }


    public delegate void TimelinePlay();
    public event TimelinePlay OnTimelinePlay;

    public delegate void TimelineFinish();
    public event TimelineFinish OnTimelineFinish;

    public delegate void TimelineUpdate(float currentTime);
    public event TimelineUpdate OnTimelineUpdate;

    public void PlayTimeline(List<T> Methods)
    {
        this.Methods = Methods;
        this.worldContext.StartCoroutine(InternalTimeline(Methods));
    }

    public void PlayTimeline (T Method) {
        this.Methods = new List<T> (new T[] { Method } );
        this.worldContext.StartCoroutine(InternalTimeline(Methods));
    }

    public abstract void TimelineFunctionality(List<T> Methods);

    protected IEnumerator InternalTimeline(List<T> Methods)
    {
        yield return new WaitForEndOfFrame();

        InternalStartTimeline(Methods);

        while (CurrentTime < EndTime)
        {
            foreach (var Method in this.Methods)
            {
                TimelineFunctionality(Methods);
            }

            InternalUpdateTimeline();
            yield return new WaitForEndOfFrame();
        }
        InternalFinishTimeline();
    }

    protected void InternalUpdateTime(ref float inTime) => inTime += Time.deltaTime * TimeScale;

    protected virtual void InternalStartTimeline(List<T> Methods)
    {
        this.Methods = Methods;
        this.active = true;

        OnTimelinePlay?.Invoke();
    }

    protected void InternalUpdateTimeline()
    {
        InternalUpdateTime(ref this.currentTime);

        OnTimelineUpdate?.Invoke(CurrentTime);
    }

    protected void InternalFinishTimeline()
    {
        this.active = false;
        this.worldContext.StopCoroutine(InternalTimeline(this.Methods));

        OnTimelineFinish?.Invoke();
    }
}