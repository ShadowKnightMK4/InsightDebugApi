using System;

namespace DllSaving
{
    class Program
    {
        static void Usage()
        {
            Console.WriteLine("DllSaving  -Location FolderPath  /Target \"Program\"  Arguments");
            Console.WriteLine("Whenever this receives a Create Process Event or Load Dll Event, we copy the targeted process and dll to a subfolder in the FolderPath ");
        }
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Usage();
                return;
            }
        }
    }
}
