using System.Reflection.PortableExecutable;
using System.Runtime.Serialization;

namespace Package
{
    internal class Program
    {
        static string CodeFolder;
        enum CodeType
        {
            x86 = 0,
            x64 = 1,
            Both = 2
        }
        
        

        static CodeType MachineCode;
        static CodeType TeleDlls;
        static void Main(string[] args)
        {
            string PromptResult;
            CodeFolder = args[0];

            try1:
            Console.Write("x[8]6 or x[6]4 bit Sheath and Native? Use 8 or 6");
            PromptResult = Console.ReadLine();
            if (PromptResult != null)
            {
                PromptResult = PromptResult.Trim();
                if (PromptResult.StartsWith("8"))
                {
                    MachineCode = CodeType.x86;
                    goto yat;
                }

                if (PromptResult.StartsWith("6"))
                {
                    MachineCode = CodeType.x64;
                    goto yat;
                }
                Console.WriteLine("Must pick either x86 or x64.");
                goto try1;

            yat:
                goto outside;
            }

        outside:

            Console.WriteLine("use [8] for only x86 telemetry dlls and [6] for x64 dlls. Press Enter for both.");
            PromptResult = Console.ReadLine().Trim();
            
            if (PromptResult != null)
            {
                if (PromptResult.StartsWith("8"))
                {
                    TeleDlls = CodeType.x86;
                    Console.WriteLine("Selecting x86 Telemetry Dlls. ");
                }
                if (PromptResult.StartsWith("6"))
                {
                    TeleDlls = CodeType.x64;
                    Console.WriteLine("Selecting x64 Telemetry Dlls. ");
                }
                if (PromptResult.StartsWith("B") || (PromptResult.Length == 0))
                {
                    TeleDlls = CodeType.Both;
                    Console.WriteLine("Selecting both x86 and x64 Telemetry Dlls.");
                }
            }

            Console.WriteLine("Debug or Release Version?");

        }
    }
}