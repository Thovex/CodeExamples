using System.Globalization;

namespace Gameplay.Currency
{
    public enum Currencies
    {
        Orrons
    }

    public static class CurrencyUtility
    {
        public static string AsCurrency(this int value)
        {
            return value.AsCurrency(CultureInfo.GetCultureInfo("nl-NL"));
        }

        public static string AsCurrency(this int value, CultureInfo culture)
        {
            return (value / 100m).ToString("N", culture);
        }
    }
}