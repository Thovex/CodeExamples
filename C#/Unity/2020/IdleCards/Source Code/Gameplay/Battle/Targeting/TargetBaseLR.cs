using BaerAndHoggo.Gameplay.Battle.Targeting;
using DG.Tweening;
using UnityEngine;

namespace BaerAndHoggo.Gameplay.Battle.Targeting
{
    public abstract class TargetBaseLR<T> : TargetBase<T> where T : TargetData
    {

        protected LineRenderer TargetLineRenderer;

        protected Color TargetLineRendererStartColor = new Color(0, 0, 0, .5F);
        protected Color TargetLineRendererEndColor = new Color(0, 0, 0, .5F);
        
        private bool _fastState;

        private Vector3 _currentTarget;
        private Vector3 _previousTarget;

        public override void Initialize(T data)
        {
            GetOrCreateLineRenderer();
            base.Initialize(data);
        }

        private void GetOrCreateLineRenderer()
        {
            TargetLineRenderer = gameObject.GetComponent<LineRenderer>();

            if (!TargetLineRenderer)
            {
                TargetLineRenderer = gameObject.AddComponent<LineRenderer>();
                TargetLineRenderer.material = new Material(Shader.Find("Sprites/Default"));
                TargetLineRenderer.widthCurve = CurveDB.Instance.GetItemById(1).curve;
            }

            TargetLineRenderer.numCapVertices = 10;
            TargetLineRenderer.positionCount = 5;
            
            TargetLineRenderer.startColor = TargetLineRendererStartColor;
            TargetLineRenderer.endColor = TargetLineRendererEndColor;
        }

        protected void SetTarget(Vector3 origin, Vector3 target, Vector3 offset)
        {
            if (_previousTarget == target) return;
            
            origin.x += offset.x;
            origin.y += offset.y;

            origin.z = 25;
            target.z = 25;

            _currentTarget = _previousTarget;

            var tween = DOTween.To(()=> _currentTarget, x=> _currentTarget = x, target, .25F);
            tween.onUpdate += () =>
            {
                SetTarget_Tween(origin, _currentTarget);
            };

            _previousTarget = target;
        }

        private void SetTarget_Tween(Vector3 origin, Vector3 target)
        {
            var tweenPosition1 = (origin + target) / 2F;
            var tweenPosition2 = (tweenPosition1 + target) / 2F;
            var tweenPosition3 = (tweenPosition2 + target) / 2F;
            
            TargetLineRenderer.SetPositions(new[]
                {origin, tweenPosition1, tweenPosition2, tweenPosition3, target});
        }

        protected void EnableLineRenderer()
        {
            if (_fastState)
                return;

            _fastState = true;
            TargetLineRenderer.enabled = true;

            var colorTween = TargetLineRenderer.DOColor(
                new Color2(
                    new Color(0, 0, 0, 0), 
                    new Color(0, 0, 0, 0)
                ), 
                new Color2(
                    TargetLineRendererStartColor, 
                    TargetLineRendererEndColor
                ), .5F);
        }
        
        protected void DisableLineRenderer()
        {
            if (!_fastState)
                return;

            _fastState = false;

            var colorTween = TargetLineRenderer.DOColor(
                new Color2(
                    TargetLineRenderer.startColor,
                    TargetLineRenderer.endColor
                ),
                new Color2(
                    new Color(0, 0, 0, 0),
                    new Color(0, 0, 0, 0)
                ), .5F);

            colorTween.onComplete += () =>
            {
                if (!_fastState)
                    TargetLineRenderer.enabled = false;
            };
        }
    }
}