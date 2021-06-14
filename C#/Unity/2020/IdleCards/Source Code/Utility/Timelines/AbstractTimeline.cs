// Created by Jesse J. van Vliet - Copyright 2019

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Serialization;

namespace BaerAndHoggo.Utilities.Timeline
{
    [Serializable]
    public abstract class AbstractTimeline<T>
    {
        /* Public event on timeline Completion*/
        public delegate void TimelineFinish();

        /* Public event on timeline Play or Reverse */
        public delegate void TimelinePlay();

        /* Public event on timeline update with current time*/
        public delegate void TimelineUpdate(float currentTime);

        #region Definitions

        public enum ETimelineDirection
        {
            Forward,
            Reverse
        }

        #endregion

        /* Current timeline time it will use in the curves */
        [SerializeField] protected float currentTime;

        [SerializeField] private ETimelineDirection direction = ETimelineDirection.Forward;

        /* End time extracted from the curves */
        [SerializeField] protected float endTime;

        private int _internalLoopCount = 1;

        /* Maximum time a timeline can be active before it automatically stops */
        private float _internalMaxTimelineDuration = 120.0F;

        /* Internal timeline duration not affected by timescale */
        private float _internalTimelineDuration;

        /* Loop value */
        [SerializeField] protected bool loop;

        [SerializeField] protected int loopCount;

        /* Old timescale saved on a Pause call */
        private float _oldTimescale = 1.0F;

        /* Stop command */
        [SerializeField] protected bool stop;

        /* Scale at it will loop through the time */
        [SerializeField] protected float timeScale = 1.0F;

        /* Public property CurrentTime */
        public float CurrentTime
        {
            get => currentTime;
            set => currentTime = value >= 0 && value < EndTime ? value : EndTime;
        }

        /* Public property EndTime */
        public float EndTime
        {
            get => endTime;
            set => endTime = value;
        }

        /* Public property TimeScale - how fast does the timeline project */
        public float TimeScale
        {
            get => timeScale;
            set => timeScale = value;
        }

        /* Public property Loop - Does the timeline loop */
        public bool Loop
        {
            get => loop;
            set => loop = value;
        }

        /* Public property LoopCount - how often does it loop. 0 = infinite */
        public int LoopCount
        {
            get => loopCount;
            set => loopCount = value;
        }

        /* Public property timeline direction */
        public ETimelineDirection Direction
        {
            get => direction;
            set => direction = value;
        }

        public event TimelinePlay OnTimelinePlay;
        public event TimelineFinish OnTimelineFinish;
        public event TimelineUpdate OnTimelineUpdate;

        public void PlayFromStart(T method)
        {
            methods = new List<T>(new[] {method});

            direction = ETimelineDirection.Forward;
            currentTime = 0;

            Play();
        }

        public void PlayFromStart(T method, bool loop, int loopCount)
        {
            this.loop = loop;
            this.loopCount = loopCount;

            PlayFromStart(method);
        }

        public void PlayFromStart(List<T> methods)
        {
            if (methods.Count == 0) return;

            this.methods = methods;

            direction = ETimelineDirection.Forward;
            currentTime = 0;

            Play();
        }

        public void PlayFromStart(List<T> methods, bool loop, int loopCount)
        {
            this.loop = loop;
            this.loopCount = loopCount;

            PlayFromStart(methods);
        }

        public void PlayFrom(float startTime, T method)
        {
            methods = new List<T>(new[] {method});

            direction = ETimelineDirection.Forward;
            currentTime = startTime;

            Play();
        }

        public void PlayFrom(float startTime, List<T> methods)
        {
            this.methods = methods;

            direction = ETimelineDirection.Forward;
            currentTime = startTime;

            Play();
        }

        public void ReverseFromEnd(T method)
        {
            methods = new List<T>(new[] {method});

            direction = ETimelineDirection.Reverse;
            currentTime = endTime;

            Play();
        }

        public void ReverseFromEnd(T method, bool loop, int loopCount)
        {
            this.loop = loop;
            this.loopCount = loopCount;

            ReverseFromEnd(method);
        }

        public void ReverseFromEnd(List<T> methods)
        {
            this.methods = methods;

            direction = ETimelineDirection.Reverse;
            currentTime = endTime;

            Play();
        }

        public void ReverseFromEnd(List<T> methods, bool loop, int loopCount)
        {
            this.loop = loop;
            this.loopCount = loopCount;

            ReverseFromEnd(methods);
        }

        public void ReverseFrom(float startTime, T method)
        {
            methods = new List<T>(new[] {method});

            direction = ETimelineDirection.Reverse;
            currentTime = startTime;

            Play();
        }

        public void ReverseFrom(float startTime, List<T> methods)
        {
            this.methods = methods;

            direction = ETimelineDirection.Reverse;
            currentTime = startTime;

            Play();
        }

        public void InvertDirection()
        {
            Direction = Direction == (ETimelineDirection) 0 ? (ETimelineDirection) 1 : 0;
        }

        public void Play()
        {
            if (WorldContext) WorldContext.StartCoroutine(InternalTimeline(methods));
        }

        public void Stop()
        {
            stop = true;
        }

        public void Pause()
        {
            _oldTimescale = timeScale;
            timeScale = 0;
        }

        public void Resume()
        {
            timeScale = _oldTimescale;
        }

        public bool IsPlaying()
        {
            return Active;
        }

        public abstract void TimelineFunctionality(List<T> methods);

        protected IEnumerator InternalTimeline(List<T> methods)
        {
            yield return new WaitForEndOfFrame();

            InternalStartTimeline(methods);

            while (!InternalTimelineFinishCheck())
            {
                foreach (var method in this.methods) TimelineFunctionality(methods);

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
                case ETimelineDirection.Forward:
                {
                    return !(CurrentTime < EndTime);
                }
                case ETimelineDirection.Reverse:
                {
                    return !(CurrentTime > 0);
                }
            }

            throw new Exception("Timeline direction is NULL");
        }

        protected void InternalUpdateTime(ref float inTime)
        {
            inTime += Time.deltaTime * (Direction == ETimelineDirection.Forward ? TimeScale : -TimeScale);
        }

        protected virtual void InternalStartTimeline(List<T> methods)
        {
            this.methods = methods;
            Active = true;

            OnTimelinePlay?.Invoke();
        }

        private void InternalAutoCleanup()
        {
            if (_internalTimelineDuration >= _internalMaxTimelineDuration) stop = true;
        }

        protected void InternalUpdateTimeline()
        {
            InternalUpdateTime(ref currentTime);

            _internalTimelineDuration += Time.deltaTime;

            OnTimelineUpdate?.Invoke(CurrentTime);
        }

        protected void InternalFinishTimeline()
        {
            Active = false;
            WorldContext.StopCoroutine(InternalTimeline(methods));

            if (loop)
            {
                if (loopCount == 0)
                {
                    switch (direction)
                    {
                        case ETimelineDirection.Forward:
                        {
                            PlayFromStart(methods, true, loopCount);
                        }
                            break;
                        case ETimelineDirection.Reverse:
                        {
                            ReverseFromEnd(methods, true, loopCount);
                        }
                            break;
                    }

                    return;
                }

                if (loopCount > 0 && _internalLoopCount < loopCount)
                {
                    Debug.Log($"{_internalLoopCount}/{loopCount}");

                    switch (direction)
                    {
                        case ETimelineDirection.Forward:
                        {
                            PlayFromStart(methods, true, loopCount);
                        }
                            break;
                        case ETimelineDirection.Reverse:
                        {
                            ReverseFromEnd(methods, true, loopCount);
                        }
                            break;
                    }

                    _internalLoopCount++;
                    return;
                }
            }

            OnTimelineFinish?.Invoke();
        }

        #region Requirements

         public List<T> methods;
        protected MonoBehaviour WorldContext;
        protected CurveBase Curve;

        protected bool Active;

        #endregion

        #region Constructor & Destructor

        public AbstractTimeline()
        {
        }

        /* Constructor without Curve */
        public AbstractTimeline(MonoBehaviour worldContext, float endTime)
        {
            this.WorldContext = worldContext;
            this.endTime = endTime;

            if (!this.WorldContext) throw new Exception("Invalid WorldContext in Timeline");
        }

        /* Constructor with Curve */
        public AbstractTimeline(MonoBehaviour worldContext, CurveBase curve) : this(worldContext, 0)
        {
            this.Curve = curve;
            endTime = curve.GetLength();
        }

        /* Destructor ends the Coroutine */
        ~AbstractTimeline()
        {
            if (Active) InternalFinishTimeline();
        }

        #endregion
    }
}