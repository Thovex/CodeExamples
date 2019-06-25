using System.ComponentModel;

namespace GameDevManager.Employees.Enums {
    public enum EEmployeeSkills { 
        [Description("Game Artist")]GameArt, 
        [Description("Game Designer")]GameDesign, 
        [Description("Game Developer")]GameDevelopment, 
        [Description("Interaction Designer")]InteractionDesign, 
        [Description("Audio Designer")]AudioDesign, 
        [Description("Marketing")]Marketing, 
        [Description("Producer")]Producer 
    }
}