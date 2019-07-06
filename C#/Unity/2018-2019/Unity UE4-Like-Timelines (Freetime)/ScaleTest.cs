using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScaleTest : MonoBehaviour
{
    public CurveVector3 curveScale;
    public CurveFloat curveColor;

    private IEnumerator Start()
    {

        TimelineFloat timelineColor = new TimelineFloat(this, curveColor);
        timelineColor.PlayFromStart((alpha) => SetColor(ref alpha));

        yield return new WaitForSeconds(0.9F);

        timelineColor.Pause();

        TimelineVector3 timelineScale = new TimelineVector3(this, curveScale);
        timelineScale.PlayFromStart((size) => SetScale(ref size));

        yield return new WaitForSeconds(1.0F);

        timelineColor.Resume();

    }

    private void SetScale(ref Vector3 size) => transform.localScale = Vector3.one / 2 + size;

    private void SetColor(ref float alpha) => transform.GetComponent<MeshRenderer>().material.color = Color.Lerp(Color.white, Color.red, alpha);
}