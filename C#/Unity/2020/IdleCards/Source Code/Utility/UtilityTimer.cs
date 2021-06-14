public static partial class Utility
{
    public static string DefineTimer(int secs)
    {
        int hours = secs / 3600;
        int mins = (secs % 3600) / 60;
        secs = secs % 60;
        return string.Format("{0:D2}:{1:D2}:{2:D2}", hours, mins, secs);
    }
}