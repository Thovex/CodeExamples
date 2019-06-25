using UnityEngine;
using System.Collections.Generic;
using UnityEditor;
using System.Linq;

public class GraphFieldFloat
{
    /// <summary>
    /// Variable containing the graph values.
    /// </summary>
    private List<float> m_Values;

    /// <summary>
    /// Property to access the graph values.
    /// </summary>
    public List<float> Values 
    {
        get 
        {
            return this.m_Values;
        }
        set
        {
            this.m_Values = value;
        }
    }
    private Vector3 m_NewGraphPlotPosition;
    private Vector3 m_OldGraphPlotPosition;

    /// <summary>
    /// Editor window UnityEngine.Rect(). Contains window width, height, position.
    /// </summary>    
    private Rect m_WindowSize;

    /// <summary>
    /// Base position to draw all values from.
    /// </summary>
    private Vector3 m_Position;

    /// <summary>
    /// Variable containing the graph height.
    /// </summary>
    private float m_Height;
    /// <summary>
    /// Property to access the graph height.
    /// </summary>
    public float Height 
    {
        get 
        {
            return this.m_Height;
        }
        set 
        {
            this.m_Height = value;
        }
    }

    /// <summary>
    /// Property to access graph field settings data. Contains all style information
    /// <param name="m_PlotSizes">Amount of Grid Cells in X and Y.</param>
    /// <param name="m_LineThickness">Size of the graph line.</param>
    /// <param name="m_HistoryCount">Value with the amount of history a graph can contain. The lower the more accurate your graph will be.</param>
    /// <param name="m_BorderColor">Color of the graphs border.</param>
    /// <param name="m_BackColor">Color of the graphs background.</param>
    /// <param name="m_LineColor">Color of the graph grid lines.</param>
    /// <param name="m_GraphLineColor">Color of the graph line.</param>
    /// </summary>
    private GraphFieldSettingsData m_GraphFieldSettingsData;
    public GraphFieldSettingsData GraphFieldSettingsData 
    {
        get
        {
            return m_GraphFieldSettingsData;
        }
        set 
        {
            m_GraphFieldSettingsData = value;
        }
    }

    private string m_Name;

    // Private variables
    private Material m_ShaderMat;
    private int m_GraphIndex;
    private int m_GraphFieldSettingsUpdate;

    // Toggle variables
    private bool m_ValueBreak = false;
    private bool m_ValueBreakPause = false;
    private float m_ValueBreakValue;
    private List<int> m_ValueBreakList = new List<int>();

    /// <summary>
    /// Constructor functionality to create a graph. Has a few required parameters.
    /// </summary>
    /// <param name="_index">The index this graph is. (if you feed your own data make sure you adjust the removal of data in the RemoveHistory function.</param>
    /// <param name="_values">List of values you want to stream into the graph. (can feed your own data or correspond with ChaserExtension.cs)</param>
    /// <param name="_position">The position it will display at in your editor window.</param>
    /// <param name="_height">The height of your graph.</param>
    /// <example> 
    /// Calling the <see cref="GraphFieldFloat"/> constructor.
    /// <code>
    /// using UnityEditor;
    /// using UnityEngine;
    /// using System.Collections.Generic;
    /// 
    /// class EditorWindowExample : EditorWindow 
    /// {
    ///     private List list;
    ///     private Vector3 pos;
    /// 
    ///     private GraphFieldFloat gF;
    /// 
    ///     private void OnGUI() 
    ///     {
    ///         if (gF == null)
    ///         {    
    ///             gF = new GraphFieldFloat(0, list, pos, 100);
    ///         }
    ///         gF.Update(position);
    ///     }
    /// }
    /// </code>
    /// </example>
    public GraphFieldFloat(int _index, List<float> _values, Vector3 _position, float _height) 
    {
        m_GraphIndex = _index;
        m_Values = _values;
        m_Position = _position;
        m_Height = _height;

        m_GraphFieldSettingsData = ChaserFloatSettings.m_ChasingFloatListSettings[m_GraphIndex];
    }

    /// <summary>
    /// Updates the style of the graph.
    /// </summary>
    /// <param name="_plotSizes">Amount of Grid Cells in X and Y.</param>
    /// <param name="_lineThickness">Size of the graph line.</param>
    /// <param name="_historyCount">Value with the amount of history a graph can contain. The lower the less values your graph will contain and thus more clear.</param>
    public void CustomizeGraph(Vector2 _plotSizes, int _lineThickness, int _historyCount) 
    {
        m_GraphFieldSettingsData.m_LineThickness = _lineThickness;
        m_GraphFieldSettingsData.m_HistoryCount = _historyCount;

        // Making it user friendly to set plot sizes.
        _plotSizes.y = (int)m_Height / _plotSizes.y;
        m_GraphFieldSettingsData.m_AmountPlots = _plotSizes;
    }

    /// <summary>
    /// Updates the colors of the graph.
    /// </summary>
    /// <param name="_borderColor">Color of the graphs border.</param>
    /// <param name="_backColor">Color of the graphs background.</param>
    /// <param name="_lineColor">Color of the graph grid lines.</param>
    /// <param name="_graphLineColor">Color of the graph line.</param>
    /// <param name="_textColor">Color of the text inside the graph.</param>
    /// <param name="_breakColor">Color of break line.</param>

    public void CustomizeColors(Color _borderColor, Color _backColor, Color _lineColor, Color _graphLineColor, Color _textColor, Color _breakColor) 
    {
        m_GraphFieldSettingsData.m_BorderColor = _borderColor;
        m_GraphFieldSettingsData.m_BackColor = _backColor;
        m_GraphFieldSettingsData.m_LineColor = _lineColor;
        m_GraphFieldSettingsData.m_GraphLineColor = _graphLineColor;
        m_GraphFieldSettingsData.m_TextColor = _textColor;
        m_GraphFieldSettingsData.m_BreakLineColor = _breakColor;
    }

    /// <summary>
    /// Calls the drawing functionality.
    /// </summary>
    public void Update() 
    {
        DrawGraph();
    }

    /// <summary>
    /// Overloaded update which allows dynamic changes in position.
    /// </summary>
    public void Update(Rect _windowSize) 
    {
        m_WindowSize = _windowSize;
        DrawGraph();
    }

    /// <summary>
    /// Overloaded update which allows dynamic changes in position and height.
    /// </summary>
    public void Update(Rect _windowSize, float _graphHeight) 
    {
        m_WindowSize = _windowSize;
        m_Height = _graphHeight;
        DrawGraph();
    }

    /// <summary>
    /// Draw graph function. Calls all functionality in order to display graph.
    /// </summary>
    private void DrawGraph()
    {
        #if UNITY_EDITOR
        GraphFieldUpdateStyle();
        
        GraphFieldStartGL();
        GraphFieldDraw();
        GraphFieldDrawBorders();

        if (m_Values.Count > 2)
        {
            float _lowestValY, _valRangeY, _stepX;
            GraphFieldRemoveHistory();
            GraphFieldCalculateStepSize(out _lowestValY, out _valRangeY, out _stepX);
            GraphFieldCalculateLines(_lowestValY, _valRangeY, _stepX);
        }

        GraphFieldEndGL();
        GraphFieldSetColumnMargins();
        #endif
    }

    /// <summary>
    /// Update the style every 100 update ticks.
    /// </summary>
    private void GraphFieldUpdateStyle()
    {
        m_GraphFieldSettingsUpdate++;

        if (m_GraphFieldSettingsUpdate > 100)
        {
            m_GraphFieldSettingsData = ChaserFloatSettings.m_ChasingFloatListSettings[m_GraphIndex];
            m_Name = ChaserFloatSettings.m_ChasingFloatNames[m_GraphIndex];
            m_GraphFieldSettingsUpdate = 0;
        }
    }

    /// <summary>
    /// Set column margins based on GraphType in data.
    /// </summary>
    private void GraphFieldSetColumnMargins() 
    {
        if (m_GraphFieldSettingsData.m_GraphType != EGraphType.NoInfo) 
        {
            if (m_GraphFieldSettingsData.m_GraphType == EGraphType.SingleColumn) 
            {
                m_GraphFieldSettingsData.m_GraphColumnRightWidth = 0;
                GraphFieldDrawLeftColumn();
            }

            if (m_GraphFieldSettingsData.m_GraphType == EGraphType.DoubleColumn) 
            {
                GraphFieldDrawLeftColumn();
                GraphFieldDrawRightColumn();
            }
        } 
        else 
        {
            m_GraphFieldSettingsData.m_GraphColumnLeftWidth = 0;
            m_GraphFieldSettingsData.m_GraphColumnRightWidth = 0;
        }
    }

    /// <summary>
    /// Push a log of break result to Unity Editor console.
    /// </summary>
    private void GraphFieldBreakPause(float _val)
    {
        Debug.LogWarning("=== BEGIN VALUEBREAK ===");
        Debug.Log("--- Settings ---");
        Debug.Log("Graph Name: " + m_Name);
        Debug.Log("Graph Index: " + m_GraphIndex);
        Debug.Log("Game Time: " + Time.time);
        Debug.Log("Graph Count: " + m_Values.Count);
        Debug.Log("--- Values ---");
        Debug.Log("Absolute Difference Value: " + _val);
        Debug.Log("New Value: " + m_Values[m_Values.Count - 1]);
        Debug.Log("Old Value: " + m_Values[m_Values.Count - 2]);
        Debug.Log("=== END VALUEBREAK ===");

        EditorApplication.isPaused = true;
        m_ValueBreak = false;
    }

    /// <summary>
    /// Populate the values of the left side column and display.
    /// </summary>
    private void GraphFieldDrawLeftColumn() 
    {
        float _size = (m_Position.x * 2) - 5;
        Color _oldColor = GUI.contentColor;

        GUI.contentColor = m_GraphFieldSettingsData.m_TextColor;

        _size =  _size * ((m_WindowSize.width / 100) / 4);

        GUILayout.BeginArea(new Rect(m_Position.x, m_Position.y, _size, m_Height));

        GUILayout.BeginVertical();
        
        EditorGUILayout.LabelField("NAME: " + m_Name);
        m_GraphFieldSettingsData.m_HistoryCount = EditorGUILayout.IntField(m_GraphFieldSettingsData.m_HistoryCount);
        EditorGUILayout.LabelField("CURR: " + m_Values[m_Values.Count - 1]);
        EditorGUILayout.LabelField("AVG: " + m_Values.Average());
        EditorGUILayout.LabelField("MIN: " + m_Values.Min());
        EditorGUILayout.LabelField("MAX: " + m_Values.Max());
        
        GUILayout.EndVertical();

        GUILayout.EndArea();

        GUI.contentColor = _oldColor;
    }

    /// <summary>
    /// Create the right side column toggles and display.
    /// </summary>
    private void GraphFieldDrawRightColumn() 
    {
        float _size = (m_Position.x * 2) - 5;
        Color _oldColor = GUI.contentColor;

        GUI.contentColor = m_GraphFieldSettingsData.m_TextColor;

        _size =  _size * ((m_WindowSize.width / 100) / 4);

        GUILayout.BeginArea(new Rect(m_WindowSize.width - m_GraphFieldSettingsData.m_GraphColumnRightWidth, m_Position.y, _size, m_Height));

        GUILayout.BeginVertical();

        m_ValueBreak = EditorGUILayout.ToggleLeft(m_ValueBreak ? "Break ON" : "Break OFF", m_ValueBreak);
        m_ValueBreakPause = EditorGUILayout.ToggleLeft(m_ValueBreakPause ? "Pause ON" : "Pause OFF", m_ValueBreakPause);
        m_ValueBreakValue = EditorGUILayout.FloatField(m_ValueBreakValue);

        GUILayout.EndVertical();

        GUILayout.EndArea();

        GUI.contentColor = _oldColor;
    }

    /// <summary>
    /// Calculate the next graph point position based on stepsize, location and height.
    /// </summary>
    private void GraphFieldCalculateLines(float _lowestValY, float _valRangeY, float _stepX)
    {
        GraphFieldBreakLinePause();

        for (int i = 0; i < m_Values.Count; i++)
        {
            m_NewGraphPlotPosition.x = i * _stepX;
            float DifferenceYRange = ((m_Values[i] - _lowestValY) / _valRangeY) * m_Height;
            m_NewGraphPlotPosition.y = DifferenceYRange;

            m_NewGraphPlotPosition.x += m_Position.x + m_GraphFieldSettingsData.m_GraphColumnLeftWidth;
            m_NewGraphPlotPosition.y += m_Position.y + m_Height;

            if (i == 0)
            {
                m_OldGraphPlotPosition = m_NewGraphPlotPosition;
            }

            GraphFieldDrawLine();
            GraphFieldBreakLine(i);

            m_OldGraphPlotPosition = m_NewGraphPlotPosition;
        }
    }

    private void GraphFieldBreakLinePause() 
    {
        float _val = Mathf.Abs(m_Values[m_Values.Count - 1] - m_Values[m_Values.Count  - 2]);

        if (m_ValueBreakValue > 0 || m_ValueBreakValue < 0)
        {
            if (_val > m_ValueBreakValue)
            {
                if (m_ValueBreakPause)
                {
                    GraphFieldBreakPause(_val);
                }
            }
        }
    }

    /// <summary>
    /// Check if indexed value is lower or higher than breaking point. Draw a breakline if it is.
    /// </summary>
    private void GraphFieldBreakLine(int i)
    {
        if (m_ValueBreak)
        {
            if (!m_ValueBreakPause) 
            {
                if (i > 1)
                {
                    float _val = Mathf.Abs(m_Values[i - 1] - m_Values[i - 2]);

                    if (m_ValueBreakValue > 0 || m_ValueBreakValue < 0)
                    {
                        if (_val > m_ValueBreakValue)
                        {
                                GL.Color(m_GraphFieldSettingsData.m_BreakLineColor);

                                GL.Vertex3(m_NewGraphPlotPosition.x, m_Position.y, m_NewGraphPlotPosition.z);
                                GL.Vertex3(m_NewGraphPlotPosition.x, m_Position.y + m_Height, m_NewGraphPlotPosition.z);                        
                        }
                    }
                }
            }
        }
    }

    /// <summary>
    /// Draw a vertex line based on newly calculated positions.
    /// </summary>
    private void GraphFieldDrawLine()
    {
        GL.Color(m_GraphFieldSettingsData.m_GraphLineColor);

        if (m_GraphFieldSettingsData.m_LineThickness <= 0)
        {
            GL.Vertex3(m_OldGraphPlotPosition.x, m_OldGraphPlotPosition.y, m_OldGraphPlotPosition.z);
            GL.Vertex3(m_NewGraphPlotPosition.x, m_NewGraphPlotPosition.y, m_NewGraphPlotPosition.z);
        }
        else
        {
            for (int j = -m_GraphFieldSettingsData.m_LineThickness; j < m_GraphFieldSettingsData.m_LineThickness; j++)
            {
                GL.Vertex3(m_OldGraphPlotPosition.x + j, m_OldGraphPlotPosition.y, m_OldGraphPlotPosition.z);
                GL.Vertex3(m_NewGraphPlotPosition.x + j, m_NewGraphPlotPosition.y, m_NewGraphPlotPosition.z);
                GL.Vertex3(m_OldGraphPlotPosition.x, m_OldGraphPlotPosition.y + j, m_OldGraphPlotPosition.z);
                GL.Vertex3(m_NewGraphPlotPosition.x, m_NewGraphPlotPosition.y + j, m_NewGraphPlotPosition.z);
            }
        }
    }

    /// <summary>
    /// Calculate X and Y stepsize based on the Highest and Lowest value in m_Values. 
    ///  Adds a tiny offset from the top Y to reduce spacing issues and not divide by itself.
    /// </summary>
    /// <returns>Outs three float values.</returns>
    private void GraphFieldCalculateStepSize(out float _lowestValY, out float _valRangeY, out float _stepX)
    {
        float _highestValY = m_Values.Max() + 0.05f;
        _lowestValY = m_Values.Min() - 0.05f;

        _valRangeY = (_lowestValY - _highestValY);
        _stepX = (m_WindowSize.width - (m_GraphFieldSettingsData.m_GraphColumnLeftWidth + m_GraphFieldSettingsData.m_GraphColumnRightWidth) - (m_Position.x * 2)) / (m_Values.Count - 1);
    }

    /// <summary>
    /// Remove the history of GraphFieldFloat[m_GraphIndex] in ChaserExtension class.
    /// </summary>
    private void GraphFieldRemoveHistory()
    {
        if (m_Values.Count > m_GraphFieldSettingsData.m_HistoryCount)
        {
            ChaserFloatSettings.m_ChasingFloatList[m_GraphIndex].RemoveAt(0);
        }
    }

    /// <summary>
    /// Draws the GraphField background and plots the lines in between based on given Vector2 AmountPlots.
    /// </summary>
    /// <param name="GraphFieldSettingsData.m_AmountPlots">Required AmountPlots Vector2 parameter</param>
    private void GraphFieldDraw()
    {
        GL.Color(m_GraphFieldSettingsData.m_BorderColor);
        for (int i = 1; i < m_Height; i++)
        {
            GL.Color(m_GraphFieldSettingsData.m_BackColor);

            if (i % m_GraphFieldSettingsData.m_AmountPlots.y == 0)
            {
                GL.Color(m_GraphFieldSettingsData.m_LineColor);
            }

            GL.Vertex3(m_Position.x + m_GraphFieldSettingsData.m_GraphColumnLeftWidth, m_Position.y + i, m_Position.z);
            GL.Vertex3(m_WindowSize.width - m_Position.x - m_GraphFieldSettingsData.m_GraphColumnRightWidth, m_Position.y + i, m_Position.z);
        }

        GL.Color(m_GraphFieldSettingsData.m_LineColor);
        for (int j = 0; j < m_GraphFieldSettingsData.m_AmountPlots.x; j++)
        {
            GL.Vertex3((m_WindowSize.width - (m_GraphFieldSettingsData.m_GraphColumnLeftWidth + m_GraphFieldSettingsData.m_GraphColumnRightWidth) - (m_Position.x * 2)) / m_GraphFieldSettingsData.m_AmountPlots.x * j + m_Position.x + m_GraphFieldSettingsData.m_GraphColumnLeftWidth, m_Position.y, m_Position.z);
            GL.Vertex3((m_WindowSize.width - (m_GraphFieldSettingsData.m_GraphColumnLeftWidth + m_GraphFieldSettingsData.m_GraphColumnRightWidth) - (m_Position.x * 2)) / m_GraphFieldSettingsData.m_AmountPlots.x * j + m_Position.x + m_GraphFieldSettingsData.m_GraphColumnLeftWidth, m_Position.y - 1 + m_Height, m_Position.z);
        }
    }

    /// <summary>
    /// Draws the borders of a GraphField.
    /// </summary>
    private void GraphFieldDrawBorders()
    {
        GL.Color(m_GraphFieldSettingsData.m_BorderColor);
        
        // Left
         GL.Vertex3(m_Position.x + m_GraphFieldSettingsData.m_GraphColumnLeftWidth, m_Position.y, m_Position.z);
        GL.Vertex3(m_Position.x + m_GraphFieldSettingsData.m_GraphColumnLeftWidth, m_Position.y + m_Height, m_Position.z);

        // Right
        GL.Vertex3(m_WindowSize.width - m_Position.x - m_GraphFieldSettingsData.m_GraphColumnRightWidth, m_Position.y, m_Position.z);
        GL.Vertex3(m_WindowSize.width - m_Position.x - m_GraphFieldSettingsData.m_GraphColumnRightWidth, m_Position.y + m_Height, m_Position.z);

        // Top
        GL.Vertex3(m_Position.x - 1 + m_GraphFieldSettingsData.m_GraphColumnLeftWidth, m_Position.y, m_Position.z);
        GL.Vertex3(m_WindowSize.width - m_Position.x - m_GraphFieldSettingsData.m_GraphColumnRightWidth, m_Position.y, m_Position.z);

        // Bottom
        GL.Vertex3(m_Position.x - 1 + m_GraphFieldSettingsData.m_GraphColumnLeftWidth, m_Position.y + m_Height, m_Position.z);
        GL.Vertex3(m_WindowSize.width - m_Position.x - m_GraphFieldSettingsData.m_GraphColumnRightWidth, m_Position.y + m_Height, m_Position.z);
    }

    /// <summary>
    /// Starts using the Unity low level graphics library.
    /// </summary>
    private void GraphFieldStartGL()
    {
        GL.PushMatrix();

        if (m_ShaderMat == null)
        {
            m_ShaderMat = GetShader();
        }

        m_ShaderMat.SetPass(0);
        GL.LoadPixelMatrix();
        GL.Begin(GL.LINES);
    }

    /// <summary>
    /// Ends using the Unity low level graphics library.
    /// </summary>
    private static void GraphFieldEndGL()
    {
        GL.End();
        GL.PopMatrix();
    }

    /// <summary>
    /// Required to draw Vertices with the Unity low level graphics library.
    /// </summary>
    /// <returns>Returns a GL drawing material.</returns>
    private Material GetShader()
    {
        var shader = Shader.Find("Hidden/Internal-Colored");
        return new Material(shader);
    }
}