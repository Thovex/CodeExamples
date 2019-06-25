using UnityEngine;

public enum EGraphType {NoInfo, SingleColumn, DoubleColumn};

[System.Serializable]
public class GraphFieldSettingsData 
{
    public Vector2 m_AmountPlots = new Vector2(12, 40);
    public int m_HistoryCount = 500;
    public int m_LineThickness = 1;
    public int m_GraphColumnRightWidth = 150;
    public int m_GraphColumnLeftWidth = 150;
    public Color m_BorderColor = new Color(.1f, .1f, .1f);
    public Color m_BackColor = new Color(.3f, .3f, .3f);
    public Color m_LineColor = new Color(.5f, .5f, .5f);
    public Color m_GraphLineColor = new Color (.9f, .9f, .9f);
    public Color m_TextColor = new Color (2, 2, 2, 1f);
    public Color m_BreakLineColor = Color.red;
    public EGraphType m_GraphType = EGraphType.DoubleColumn;

    public void Randomize() {
        m_AmountPlots = new Vector2(Random.Range (0, 50), Random.Range(0,50));
        m_BackColor = Random.ColorHSV();
        m_BorderColor = Random.ColorHSV();
        m_GraphColumnLeftWidth = Random.Range(150, 250);
        m_GraphColumnRightWidth = Random.Range(150, 250);
        m_GraphType = (EGraphType)Random.Range(0, EGraphType.GetNames(typeof(EGraphType)).Length);
        m_LineColor = Random.ColorHSV();
        m_LineThickness = Random.Range(0,5);
        m_TextColor = Random.ColorHSV();
        m_GraphLineColor = Random.ColorHSV();
        m_HistoryCount = Random.Range(0, 1000);
    }
}