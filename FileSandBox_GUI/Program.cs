using FileSandBoxSheath;
//using FileSandBoxSheath.NativeImports is intended for general use.;
using FileSandBoxSheath.NativeImports;
using FileSandBoxSheath.Wrappers;
using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
//using System.Windows.Forms;
using FileSandBox_GUI;
using System.Collections;
using System.Collections.Generic;
using static FileSandBoxSheath.Wrappers.InsightHunter;

namespace FileSandBox_GUI
{
    static class Program
    {
        //typedef int(WINAPI* DebugEventCallBackRoutine)(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg);
        static InsightHunter Insight;
        static bool Quit = false;

        //public delegate bool SymbolSearchCallBackRoutine(IntPtr SymbolInfo);
        static SymbolSearchCallBackRoutine Tmp;
        static int StubCallback(IntPtr DebugEvent, IntPtr ContStat, IntPtr WaitTime, IntPtr Custom)
        {
            DebugEvent Debug = new DebugEvent(DebugEvent);
            PsProcessInformation.Poke4(ContStat, (int)PsProcessInformation.DebugContState.DebugContinueState);

            
            if (Debug.EventType == FileSandBoxSheath.Wrappers.DebugEventType.OutputDebugString)
            {
                Console.WriteLine(Debug.ProcessID + "Says: " + Debug.GetDebugEventStringInfo().OutputString);
            }
            if (Debug.EventType == FileSandBoxSheath.Wrappers.DebugEventType.CreateProcessEvent)
            {
           
                Console.WriteLine(Debug.ProcessID + " Spawned from " + Debug.GetDebugEventCreateProcessInfo().ImageName);
                Console.Write(Debug.ProcessID + " should be ");
                switch (Debug.IsEventFrom32BitProcess)
                {
                    case false:
                        Console.Write("a 64-bit ");
                        break;
                    default:
                        Console.Write("a 32-bit (or WOW64) ");
                        break;
                }
                Console.WriteLine("Process");

                Console.WriteLine("BeginningEnumeration of loaded symbols");
                Insight.EnumerateSymbols("*!*", Tmp);
            }
            if (Debug.EventType == FileSandBoxSheath.Wrappers.DebugEventType.ExceptionEvent)
            {
                Console.Write(Debug.ProcessID + " Exception \"" + Debug.GetDebugEventExceptionInfo().ExceptionCode + "\" happend");
                if (Debug.GetDebugEventExceptionInfo().IsFirstChanceException)
                {
                    Console.WriteLine("\t and is a first chance\r\n");
                }
                else
                {
                    Console.WriteLine("\t and was seen before\r\n");
                }

                Console.WriteLine("\t Exception Type" + Enum.GetName(typeof(DebugExceptionTypes), Debug.GetDebugEventExceptionInfo().ExceptionCode));
                PsProcessInformation.Poke4(ContStat, unchecked ( (int)PsProcessInformation.DebugContState.DebugExceptionNotHandled) );
            }
            

            if (Debug.EventType == FileSandBoxSheath.Wrappers.DebugEventType.LoadDllEvent)
            {
                Console.WriteLine(Debug.ProcessID + " Loaded Dll From \"" + Debug.GetDebugEventLoadDll().ImageName + "\"");
                Console.WriteLine("BeginningEnumeration of loaded symbols");
                Insight.EnumerateSymbols("*!*", Tmp);
            }

            PsProcessInformation.Poke4(WaitTime, int.MaxValue);
            
            if (Debug.EventType == FileSandBoxSheath.Wrappers.DebugEventType.ExitProcessEvent)
            {

                List<IntPtr> input = new List<IntPtr>();


                IntPtr ProcessHandle = IntPtr.Zero;
                try
                {
                    IntPtr HModule = IntPtr.Zero;
                    ProcessHandle   = HelperRoutines.OpenProcessForVirtualMemory(Debug.ProcessID);
                    Console.WriteLine("There were ");
                    while (true)
                    {
                        HModule = Detours.DetourEnumerateModulesEx(ProcessHandle, HModule);
                        input.Add(HModule);
                        if (HModule == IntPtr.Zero)
                        {
                            break;
                        }
                    }
                    foreach (IntPtr I in input) 
                    {
                        Console.Write(HelperRoutines.GetModuleNameViaHandle(ProcessHandle, I) + "\r\n");
                    }
                    HModule = IntPtr.Zero;
                }
                finally
                {
                    HelperRoutines.CloseHandle(ProcessHandle);
                }
            }
            return 0;
        }

        static bool Callback(IntPtr SymbolHandler)
        {
            SymbolInfo Symbol = new SymbolInfo(SymbolHandler, false);
            Console.WriteLine("\t" + Symbol.SymbolName);
            
            return true;
        }

        

        /// <summary>
        ///  The main entry point for the application.
        /// </summary>


        static void Main()
        {
            /*            FileSandBox_Forms.InsightDebuggerMainView test;
            test = new FileSandBox_Forms.InsightDebuggerMainView();

            test.ShowDialog();

            return;            */
             PsProcessInformation TestRun = new PsProcessInformation();
            //TestRun.ProcessName = Environment.GetFolderPath(Environment.SpecialFolder.System) + "\\cmd.exe";
            TestRun.ProcessName = Environment.GetFolderPath(Environment.SpecialFolder.System) + "\\notepad.exe";
            //TestRun.ProcessName = "C:\\Users\\Thoma\\source\\repos\\FileSandbox\\x64\\Debug\\HelloWorld.exe";
            //estRun.ProcessName = "C:\\Users\\Thoma\\source\\repos\\FileSandbox\\x64\\Debug\\HelloWorld.exe";
            TestRun.WorkingDirectory = "C:\\Users\\Thoma";
            TestRun.CreationFlags = 0;
            
            TestRun.InheritDefaultEnviroment = true;
             //TestRun.AddDetoursDll(Directory.GetCurrentDirectory() + "\\FileSandboxHelperDll.dll");
            TestRun.SetExplicitEnviromentValue("BORG", "NO");
            TestRun.EnableSymbolEngine = true;
            
            Console.WriteLine("Smybol status = " + TestRun.EnableSymbolEngine);

            
            
            string[] Data = TestRun.DetourList;
            TestRun.SetExplicitEnviromentValue("PROMPT", "WHAT MEASURE OF A KEYBOARD IS A PROMPT:>");
            TestRun.DebugMode = DebugModeType.WorkerThread;
            var FlagTest = TestRun.CreationFlags;
            TestRun.CreationFlags = 2;
            var Test = TestRun.ProcessName;
            var TestWorkDir = TestRun.WorkingDirectory;
            var ExpliEnv = TestRun.GetExplicitEnviromentValue("BORG");

            
            var DM = TestRun.DebugMode;
            Tmp = new SymbolSearchCallBackRoutine(Callback);


            
            TestRun.UserDebugCallRoutine = new PsProcessInformation.DebugEventCallBackRoutine(StubCallback);

          
            

            var ProcessId= TestRun.SpawnProcess();
            

            using (System.Diagnostics.Process Target = Process.GetProcessById(ProcessId))
            {
                while (true)
                {

                    TestRun.PulseDebugEventThead();
                    try
                    {
                        if (Target.HasExited == true)
                        {
                            Console.WriteLine("Process Has Quit during kick loop");
                            break;
                        }
                        else
                        {
                         //   Console.WriteLine("Process is alive");
                        }
                    }
                    catch (System.ComponentModel.Win32Exception e)
                    {
                        Console.WriteLine(e.Message + "<- triggered when seeing if runnig");
                    }
                }
            }
            TestRun.Dispose();

            //Application.Run(new MainWindow());
            //Application.Exit();
        }
    }
}
