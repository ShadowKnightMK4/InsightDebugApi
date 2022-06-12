using System;
using InsightSheath;
using InsightSheath.Debugging;
/// <summary>
/// External project is simply a test of what InsightAPI/Sheath looks like
/// </summary>
namespace ExternalProject
{
    class Program
    {
        static void Main(string[] args)
        {
            DebugEvent test = null;
            
            test.GetDebugEventCreateProcessInfo();
            Console.WriteLine("Hello World!");
            
        }
    }
}
