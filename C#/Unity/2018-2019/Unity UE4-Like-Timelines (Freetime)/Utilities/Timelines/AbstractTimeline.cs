// Created by Jesse J. van Vliet - Copyright 2019

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public abstract class AbstractTimeline<T>
{
    #region Definitions
    public enum ETimelineDirection { FORWARD, REVERSE };
    #endregion

    #region Requirements
    public List<T> Methods;
    protected MonoBehaviour worldContext;
    protected CurveBase curve;
    protected bool active = false;

    #endregion

    #region Constructor & Destructor
    public AbstractTimeline() { }

    /* Constructor without Curve */
    public AbstractTimeline(MonoBehaviour worldContext, float endTime)
    {
        this.worldContext = worldContext;
        this.endTime = endTime;

        if (!this.worldContext) throw new System.Exception("Invalid WorldContext in Timeline");
    }

    /* Constructor with Curve */
    public AbstractTimeline(MonoBehaviour worldContext, CurveBase curve) : this(worldContext, 0)
    {
        this.curve = curve;
        this.endTime = curve.GetLength();
    }

    /* Destructor ends the Coroutine */
    ~AbstractTimeline()
    {
        if (active) InternalFinishTimeline();
    }

    #endregion

    /* Current timeline time it will use in the curves */
    protected float currentTime = 0.0F;

    /* End time extracted from the curves */
    protected float endTime = 0.0F;

    /* Scale at it will loop through the time */
    protected float timeScale = 1.0F;

    /* Old timescale saved on a Pause call */
    private float oldTimescale = 1.0F;

    private ETimelineDirection direction = ETimelineDirection.FORWARD;

    /* Internal timeline duration not affected by timescale */
    private float internalTimelineDuration = 0.0F;

    /* Maximum time a timeline can be active before it automatically stops */
    private float internalMaxTimelineDuration = 120.0F;

    /* Stop command */
    protected bool stop = false;

    /* Public property CurrentTime */
    public float CurrentTime
    {
        get => currentTime; set
        {
            currentTime = value >= 0 && value < EndTime ? value : EndTime;
        }
    }

    /* Public property EndTime */
    public float EndTime { get => endTime; set => endTime = value; }

    /* Public property TimeScale - how fast does the timeline project */
    public float TimeScale { get => timeScale; set => timeScale = value; }

    /* Public property timeline direction */
    public ETimelineDirection Direction { get => direction; set => direction = value; }

    /* Public event on timeline Play or Reverse */
    public delegate void TimelinePlay();
    public event TimelinePlay OnTimelinePlay;

    /* Public event on timeline Completion*/
    public delegate void TimelineFinish();
    public event TimelineFinish OnTimelineFinish;

    /* Public event on timeline update with current time*/
    public delegate void TimelineUpdate(float currentTime);
    public event TimelineUpdate OnTimelineUpdate;

    public void PlayFromStart(T Method)
    {
        this.Methods = new List<T>(new T[] { Method });

        this.direction = ETimelineDirection.FORWARD;
        this.currentTime = 0;

        Play();
    }

    public void PlayFromStart(List<T> Methods)
    {
        this.Methods = Methods;

        this.direction = ETimelineDirection.FORWARD;
        this.currentTime = 0;

        Play();
    }

    public void PlayFrom(float startTime, T Method)
    {
        this.Methods = new List<T>(new T[] { Method });

        this.direction = ETimelineDirection.FORWARD;
        this.currentTime = startTime;

        Play();
    }

    public void PlayFrom(float startTime, List<T> Methods)
    {
        this.Methods = Methods;

        this.direction = ETimelineDirection.FORWARD;
        this.currentTime = startTime;

        Play();
    }

    public void ReverseFromEnd(T Method)
    {
        this.Methods = new List<T>(new T[] { Method });

        this.direction = ETimelineDirection.REVERSE;
        this.currentTime = endTime;

        Play();
    }

    public void ReverseFromEnd(List<T> Methods)
    {
        this.Methods = Methods;

        this.direction = ETimelineDirection.REVERSE;
        this.currentTime = endTime;

        Play();
    }

    public void ReverseFrom(float startTime, T Method)
    {
        this.Methods = new List<T>(new T[] { Method });

        this.direction = ETimelineDirection.REVERSE;
        this.currentTime = startTime;

        Play();
    }

    public void ReverseFrom(float startTime, List<T> Methods)
    {
        this.Methods = Methods;

        this.direction = ETimelineDirection.REVERSE;
        this.currentTime = startTime;

        Play();
    }

    public void InvertDirection() => Direction = (Direction == (ETimelineDirection)0 ? (ETimelineDirection)1 : (ETimelineDirection)0);

    public void Play()
    {
        if (worldContext) this.worldContext.StartCoroutine(InternalTimeline(Methods));
    }

    public void Stop() => stop = true;

    public void Pause()
    {
        oldTimescale = timeScale;
        timeScale = 0;
    }

    public void Resume()
    {
        timeScale = oldTimescale;
    }

    public bool IsPlaying() => this.active;
    public abstract void TimelineFunctionality(List<T> Methods);

    protected IEnumerator InternalTimeline(List<T> Methods)
    {
        yield return new WaitForEndOfFrame();

        InternalStartTimeline(Methods);

        while (!InternalTimelineFinishCheck())
        {
            foreach (var Method in this.Methods)
            {
                TimelineFunctionality(Methods);
            }

            InternalUpdateTimeline();
            InternalAutoCleanup();
            yield return new WaitForEndOfFrame();
        }
        InternalFinishTimeline();
    }

    private bool InternalTimelineFinishCheck()
    {
        if (stop) return true;

        switch (Direction)
        {
            case ETimelineDirection.FORWARD:
                {
                    return !(CurrentTime < EndTime);
                }
            case ETimelineDirection.REVERSE:
                {
                    return !(CurrentTime > 0);
                }
        }

        throw new Exception("Timeline direction is NULL");
    }

    protected void InternalUpdateTime(ref float inTime)
    {
        inTime += Time.deltaTime * (Direction == ETimelineDirection.FORWARD ? TimeScale : -TimeScale);
    }

    protected virtual void InternalStartTimeline(List<T> Methods)
    {
        this.Methods = Methods;
        this.active = true;

        OnTimelinePlay?.Invoke();
    }

    private void InternalAutoCleanup()
    {
        if (internalTimelineDuration >= internalMaxTimelineDuration)
        {
            stop = true;
        }
    }

    protected void InternalUpdateTimeline()
    {
        InternalUpdateTime(ref this.currentTime);

        internalTimelineDuration += Time.deltaTime;

        OnTimelineUpdate?.Invoke(CurrentTime);
    }

    protected void InternalFinishTimeline()
    {
        this.active = false;
        this.worldContext.StopCoroutine(InternalTimeline(this.Methods));

        OnTimelineFinish?.Invoke();
    }
}