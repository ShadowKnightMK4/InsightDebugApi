using System;
using InsightSheath.Debugging.Process;

namespace ProcessSpawn
{
    class Program
    {
        static void Main(string[] args)
        {
            Environment.SetEnvironmentVariable("PATH", Environment.GetEnvironmentVariable("PATH") + ";C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x64\\program");
            InsightProcess LaunchCmd = InsightProcess.CreateInstance();
            LaunchCmd.CreationFlags = InsightProcess.CreationFlagValues.CreateNewConsole;
            LaunchCmd.ProcessArguments = "/K \"echo Hello World from Process Spawn Example\"\r\n";
            LaunchCmd.ProcessName = Environment.GetFolderPath(Environment.SpecialFolder.System) + "\\cmd.exe";


            LaunchCmd.SpawnProcess();

        }
    }
}
