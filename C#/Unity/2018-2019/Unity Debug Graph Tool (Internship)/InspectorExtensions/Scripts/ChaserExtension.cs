using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public static class ChaserFloatSettings {
	public static List<List<float>> m_ChasingFloatList = new List<List<float>>();
	public static List<GraphFieldSettingsData> m_ChasingFloatListSettings = new List<GraphFieldSettingsData>();
	public static List<int> m_ChasingFloatListAddCount = new List<int>();
	public static List<string> m_ChasingFloatNames = new List<string>();
}

public static class ChaserExtension {

	public static bool m_IsRecording = false;
	public static float m_TickTime = .01f;

	public static void GraphLog(this float _obj, string _graphName, int _graphIndex, GraphFieldSettingsData _graphData) 
	{
		GraphLogExecute<float>(_obj, _graphIndex);
		ChaserFloatSettings.m_ChasingFloatNames[_graphIndex] = _graphName;
		ChaserFloatSettings.m_ChasingFloatListSettings[_graphIndex] = _graphData;
	}

	public static void GraphLog(this float _obj, string _graphName, int _graphIndex) 
	{
		GraphLogExecute<float>(_obj, _graphIndex);
		ChaserFloatSettings.m_ChasingFloatNames[_graphIndex] = _graphName;

	}

	private static void GraphLogExecute<T>(T _obj, int _graphIndex) 
	{
		if (m_IsRecording) 
		{
			var _thisType = typeof(T);
			
			if (_thisType == typeof(float))
			{
				if (ChaserFloatSettings.m_ChasingFloatList.Count <= _graphIndex + 1) 
				{
					for (int i = ChaserFloatSettings.m_ChasingFloatList.Count; i <= _graphIndex; i++) 
					{
						List<float> newList = new List<float>();
						ChaserFloatSettings.m_ChasingFloatList.Add(newList);
						ChaserFloatSettings.m_ChasingFloatListSettings.Add(new GraphFieldSettingsData());
						ChaserFloatSettings.m_ChasingFloatListAddCount.Add(0);
						ChaserFloatSettings.m_ChasingFloatNames.Add("New Graph");
					}
				}

				float _floatValue = (float)(object)_obj;
				ChaserFloatSettings.m_ChasingFloatList[_graphIndex].Add(_floatValue);
				ChaserFloatSettings.m_ChasingFloatListAddCount[_graphIndex]++;
			}
		}
	}

	public static void GraphLog(this int _obj) {
		Debug.Log(_obj);
	}
}
