using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

namespace Thovex.Utility
{
	public static class History 
	{
		public static List<string> m_LogFiles = new List<string>();
		public static List<DateTime> m_LogFilesTime = new List<DateTime>();
		public static int m_HistoryCount = 0;

		public static void Add(string _obj) 
		{
			m_LogFiles.Add(_obj);
			m_LogFilesTime.Add(DateTime.Now);
			m_HistoryCount++;
		}
	}
}