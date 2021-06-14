using System.Collections;
using BaerAndHoggo.Utilities.Timeline;
using UnityEngine;

public class ScaleTest : MonoBehaviour
{
    public CurveFloat curveColor;
    public CurveVector3 curveScale;

    private Color _selectedColor;

    private IEnumerator Start()
    {
        _selectedColor = new Color(Random.Range(0, 2), Random.Range(0, 2), Random.Range(0, 2), 1);

        var timelineScale = new TimelineVector3(this, curveScale);
        timelineScale.PlayFromStart(size => SetScale(ref size));
        timelineScale.TimeScale = Random.Range(0.25F, 1.75F);

        var timelineColor = new TimelineFloat(this, curveColor);
        timelineColor.PlayFromStart(alpha => SetColor(ref alpha));

        yield return new WaitForSeconds(0.9F);

        timelineColor.Pause();


        yield return new WaitForSeconds(1.0F);

        timelineColor.Resume();
    }

    private void SetScale(ref Vector3 size)
    {
        transform.localScale = size;
    }

    private void SetColor(ref float alpha)
    {
        transform.GetComponent<MeshRenderer>().material.color = Color.Lerp(Color.white, _selectedColor, alpha);
    }
}