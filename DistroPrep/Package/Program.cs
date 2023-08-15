using System.Collections.Generic;
using System.Reflection;
using System.Reflection.Metadata;
using System.Reflection.PortableExecutable;
using System.Runtime.Serialization;

namespace Package
{
    internal class Program
    {
        static string CodeTarget;
        static string CodeFolder;
        static CodeType Tele = CodeType.None;
        static CodeType Dlls;
        static bool ReleaseMode=false;
        static bool WantDoc=false;
        enum CodeType
        {
            None=0,
            x86 = 1,
            x64 = 2,
            Both = 3
        }

        

        static void ModeSetting()
        {
            switch (Tele)
            {
                case CodeType.x64:
                    Console.WriteLine("You want only x64 Telemetry.");
                    break;
                case CodeType.x86:
                    Console.WriteLine("You want only x86 Telemetry.");
                    break;
                case CodeType.Both:
                    Console.WriteLine("You want both x64 and x86 Telemetry. Remember name32.dll is x86 and name64.dll is x64");
                    break; 
            }

            switch (Dlls)
            {
                case CodeType.x64:
                    Console.WriteLine("You opted for x64 bit Sheath and Native Dll.");
                    break;
                case CodeType.x86:
                    Console.WriteLine("You opted for x86 bit Sheath and Native Dll.");
                    break;
            }

            if (ReleaseMode)
            {
                Console.WriteLine("Copied Telemetry and Sheath/Native Dll WON'T include debug information");
            }
            else
            {
                Console.WriteLine("Copied Telemetry and Sheath/Native Dll WILL include debug information");
            }

            if (WantDoc)
            {
                Console.WriteLine("You opted for the xml doc generated for the Sheath");
            }
        }

        static void CopySpecs(string source, string[] Wildcard, string target)
        {
            string[] entries;
            string target_loc;
            if (Directory.Exists(target)==false)
            {
                Directory.CreateDirectory(target);
            }
            foreach (string W in Wildcard)
            {
                entries = Directory.GetFiles(source, W, SearchOption.TopDirectoryOnly);
                if (entries != null)
                {
                    foreach (string entry in entries)
                    {
                        var Item = new FileInfo(entry);
                        target_loc = Path.Combine(target, Item.Name);
                        File.Copy(Item.FullName, target_loc, true);
                    }
                }
            }
        }
        static string BaseTelemetry = "{0}\\Native\\TelemetryDlls\\{1}\\{2}";
        static void CopyTelemetry()
        {
            throw new NotImplementedException();
            string MakeTelemetrySource(CodeType BaseType)
            {
                string CodeTypeMode = "xERROR";
                string BuildMode;
                if (ReleaseMode)
                {
                    BuildMode = "Release";
                }
                else
                {
                    BuildMode = "Debug";
                }

                switch (Dlls)
                {
                    case CodeType.x64:
                        CodeTypeMode = "x64";
                        break;
                    case CodeType.x86:
                        CodeTypeMode = "x86";
                        break;
                }
                throw new NotImplementedException();
             //   return ret;
            }
            
            string x86source = MakeTelemetrySource(CodeType.x86);
            string x64source = MakeTelemetrySource(CodeType.x64);

            switch (Tele)
            {
                case CodeType.x64:
                    {
                        x86source = null;
                        break;
                    }
                case CodeType.x86:
                    {
                        x64source = null;
                        break;
                    }
                case CodeType.None:
                    {
                        x64source = x86source = null; 
                        break;
                    }
            }


            if (x64source != null)
            {
                CopySpecs(x64source, new string[] { "*.DLL", "*.PDB" }, CodeTarget);
            }

            if (x86source != null)
            {
                CopySpecs(x86source, new string[] { "*.DLL", "*.PDB" }, CodeTarget);
            }
        }


        static void DoTheThing() // June Lee ....  Avacare Kora Reference
        {
            Console.WriteLine("Copying Requested Stuff to " + CodeTarget );
            if (Tele != CodeType.None )
            {
                CopyTelemetry();
            }
        }
        static void Main(string[] args)
        {
           
            Console.WriteLine("Package is intended to wrap everything up from InsightApi into a neat binary bow.");
            Console.WriteLine("It's not intended to be fancy or multi-featured but to just work.");
            if (args.Length == 0)
            {
                Console.WriteLine("Please specific where the InsightApi repo is on your machine as the first argument");
                return;
            }
            else
            {
                if (Directory.Exists(args[0]))
                {
                    CodeFolder = args[0];
                }
                if (!Directory.Exists(args[1]))
                {
                    Directory.CreateDirectory(args[1]);
                }
                CodeTarget = args[1];
                for (int step =2; step < args.Length; step++)
                {
                    switch (args[step].ToLower())
                    {
                        case "/d":
                            {
                                ReleaseMode = false;
                                break;
                            }
                        case "/r":
                            {
                                ReleaseMode = true;
                                break;
                            }
                        case "/x86":
                            {
                                Dlls = CodeType.x86;
                                break;
                            }
                        case "/x64":
                            {
                                Dlls = CodeType.x64;
                                break;
                            }
                        case "/tele86":
                        {
                                Tele = CodeType.x86;
                            break;
                        }
                        case "/tele64":
                        {
                                Tele = CodeType.x64;
                                break;
                        }
                        case "/tele":
                        {
                                Tele = CodeType.Both;
                                break;
                            }
                        case "/doc":
                        {
                                WantDoc = true;
                                break;        
                        }
                    }

                }

                CopyTelemetry();
            }
        }
    }
}