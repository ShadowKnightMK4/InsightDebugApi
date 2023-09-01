using System;
using System.Windows.Forms;
using InsightSheath;
namespace InsightLogger
{
    internal static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            InsightApiNativeDllSettings.SetDllDirectory(@"C:\Users\Thoma\source\repos\InsightAPI\Code\Native\InsightApi\Debug\x64");
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();
            Application.Run(new LoggerRunDialog());
        }
    }
}