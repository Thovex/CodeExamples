using System.ComponentModel;

namespace GameDevManager.Employees.Enums {
    public enum EEmployeeLookingDirection { 
        [Description("Looking at the top Left")]TopLeft, 
        [Description("Looking at the top Right")]TopRight, 
        [Description("Looking at the bottom Left")]BottomLeft, 
        [Description("Looking at the bottom Right")]BottomRight, 
        [Description("Looking forward")]Forward, 
        [Description("Looking backwards")]Backward,
        [Description("Looking as avatar")] Avatar
    }
}