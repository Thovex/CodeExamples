using Sirenix.OdinInspector;
using UnityEngine;

namespace GameDevManager.Employees {
	[System.Serializable]
	public class EmployeeSkillsWorker {
		[TabGroup ("GAR")][SerializeField] private EmployeeStatsGameArt employeeSkillsGameArt;
		[TabGroup ("GDS")][SerializeField] private EmployeeStatsGameDesign employeeSkillsGameDesign;
		[TabGroup ("GDV")][SerializeField] private EmployeeStatsGameDevelopment employeeSkillsGameDevelopment;
		[TabGroup ("IAD")][SerializeField] private EmployeeStatsInteractionDesign employeeSkillsInteractionDesign;
		[TabGroup ("AUD")][SerializeField] private EmployeeStatsAudioDesign employeeSkillsAudioDesign;

		public EmployeeStatsGameArt EmployeeSkillsGameArt {
			get {
				return employeeSkillsGameArt;
			}

			set {
				employeeSkillsGameArt = value;
			}
		}

		public EmployeeStatsGameDesign EmployeeSkillsGameDesign {
			get {
				return employeeSkillsGameDesign;
			}

			set {
				employeeSkillsGameDesign = value;
			}
		}

		public EmployeeStatsGameDevelopment EmployeeSkillsGameDevelopment {
			get {
				return employeeSkillsGameDevelopment;
			}

			set {
				employeeSkillsGameDevelopment = value;
			}
		}

		public EmployeeStatsInteractionDesign EmployeeSkillsInteractionDesign {
			get {
				return employeeSkillsInteractionDesign;
			}

			set {
				employeeSkillsInteractionDesign = value;
			}
		}

		public EmployeeStatsAudioDesign EmployeeSkillsAudioDesign {
			get {
				return employeeSkillsAudioDesign;
			}

			set {
				employeeSkillsAudioDesign = value;
			}
		}

	}

	[System.Serializable]
	public class EmployeeSkillsExtra {
		[TabGroup ("MRK")][SerializeField] private EmployeeStatsMarketing employeeSkillsMarketing;
		[TabGroup ("PRO")][SerializeField] private EmployeeStatsProducer employeeSkillsProducer;
		[TabGroup ("LEG")][SerializeField] private EmployeeStatsLegal employeeSkillsLegal;

		public EmployeeStatsMarketing EmployeeSkillsMarketing {
			get {
				return employeeSkillsMarketing;
			}

			set {
				employeeSkillsMarketing = value;
			}
		}

		public EmployeeStatsProducer EmployeeSkillsProducer {
			get {
				return employeeSkillsProducer;
			}

			set {
				employeeSkillsProducer = value;
			}
		}

		public EmployeeStatsLegal EmployeeSkillsLegal {
			get {
				return employeeSkillsLegal;
			}

			set {
				employeeSkillsLegal = value;
			}
		}
	}
}