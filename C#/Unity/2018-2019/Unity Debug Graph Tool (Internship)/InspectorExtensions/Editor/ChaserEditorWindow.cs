using UnityEngine;
using System.Collections.Generic;
using UnityEditor;

public class ChaserEditorWindow : EditorWindow
{
    private bool m_IsRecording = false;
    private List<GraphFieldFloat> m_GraphFieldList = new List<GraphFieldFloat>();
    private Vector2 m_ScrollPosition = Vector2.zero;

    [MenuItem("Debug/Chase Variables %g")]
    private static void ShowWindow()
    {
        ChaserEditorWindow window = (ChaserEditorWindow)EditorWindow.GetWindow(typeof(ChaserEditorWindow), false, "Chase Variables");
        window.Show();

    }

    private void Update()
    {
        if (m_IsRecording)
        {
            Repaint();
        }
    }

    private void OnEnable()
    {
        ChaserExtension.m_IsRecording = true;
        m_IsRecording = true;
    }

    private void OnDisable()
    {

    }

    private void OnGUI()
    {
        if (GUILayout.Button("Record Data: " + m_IsRecording.ToString()))
        {
            m_IsRecording = !m_IsRecording;
            ChaserExtension.m_IsRecording = m_IsRecording;
        }

        if (GUILayout.Button("Clear Lists"))
        {
            ClearData();
        }

        if (m_IsRecording) 
        {
            for (int i = 0; i < ChaserFloatSettings.m_ChasingFloatList.Count; i++) 
            {
                if (m_GraphFieldList.Count == 0 || m_GraphFieldList.Count < ChaserFloatSettings.m_ChasingFloatList.Count) 
                {
                    GraphFieldFloat _graphField = new GraphFieldFloat(i, ChaserFloatSettings.m_ChasingFloatList[i], new Vector3(25, 75 + (i * 150) + (i*25), 0), 150);
                    m_GraphFieldList.Add(_graphField);
                }
                m_GraphFieldList[i].Update(position);
            }
        }    
        else 
        {
            for (int i = 0; i < ChaserFloatSettings.m_ChasingFloatList.Count; i++) 
            {
                if (m_GraphFieldList.Count > 0) 
                {
                    m_GraphFieldList[i].Update(position);
                }
            }
        }

    }

    private void ClearData()
    {
        for (int i = 0; i < ChaserFloatSettings.m_ChasingFloatList.Count; i++)
        {
            ChaserFloatSettings.m_ChasingFloatList[i].Clear();
        }
    }
}