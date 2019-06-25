using System.ComponentModel;

namespace GameDevManager.Employees.Enums {
    public enum EEmployeeSpecialization {
        /* Starting */
         [Description("")] None,
        /* Dev*/
         [Description("Lead Developer")] LeadDevelopment,
         [Description("Engine Developer")] EngineDevelopment,
        /* Design */
         [Description("Lead Designer")] LeadDesign,
        [Description("World Builder")] WorldBuilding,
         [Description("Level Designer")]LevelDesign,
        /* Art */
         [Description("Lead Artist")]LeadArt,
         [Description("Concept Art")]ConceptArt,
        /* Audio */
         [Description("Voice Actor")]VoiceActing,
    }
}