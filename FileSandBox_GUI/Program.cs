using InsightSheath;
//using FileSandBoxSheath.NativeImports is intended for general use.;
using InsightSheath.NativeImports;
using InsightSheath.Wrappers;
using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
//using System.Windows.Forms;
using FileSandBox_GUI;
using System.Collections;
using System.Collections.Generic;
using static InsightSheath.Wrappers.InsightHunter;
using InsightSheath.Structs;

namespace FileSandBox_GUI
{
    static class Program
    {
        //typedef int(WINAPI* DebugEventCallBackRoutine)(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg);
        
        static bool Quit = false;
        static InsightHunter Insight;
        //public delegate bool SymbolSearchCallBackRoutine(IntPtr SymbolInfo);
        static SymbolSearchCallBackRoutine Tmp;
        static int StubCallback(IntPtr DebugEvent, IntPtr ContStat, IntPtr WaitTime, IntPtr Custom)
        {
            DebugEvent Debug = new DebugEvent(DebugEvent);
            PsProcessInformation.Poke4(ContStat, (int)PsProcessInformation.DebugContState.DebugContinueState);

            
            if (Debug.EventType == InsightSheath.Wrappers.DebugEventType.OutputDebugString)
            {
                Console.WriteLine(Debug.ProcessID + "Says: " + Debug.GetDebugEventStringInfo().OutputString);
            }
            if (Debug.EventType == InsightSheath.Wrappers.DebugEventType.CreateProcessEvent)
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
            
            if (Debug.EventType == InsightSheath.Wrappers.DebugEventType.ExceptionEvent)
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

            if (Debug.EventType == DebugEventType.ExitThreadEvent)
            {
                using (var Tester = ThreadContext.CreateInstance(Debug.ThreadID))
                {
                    Console.Write("threadexit here");
                }
            }
            if (Debug.EventType == DebugEventType.CreateTheadEvent)
            {
                using (var Tester = ThreadContext.CreateInstance(Debug.GetDebugEventCreateThreadInfo().ThreadID))
                {

                    Tester.PriorityBoost = true;
                    Tester.PriorityBoost = false;
                    Tester.PriorityBoost = true;

                    Tester.IdealProcessor = 1;
                    Tester.IdealProcessor = 4;
                    Tester.ProcessorAffinity = 1;
                    Tester.ProcessorAffinity = 4;

                    Tester.ThreadPriority = ThreadPriorityLevel.Highest;

                    Console.Write("DebugHere");
                    

                }


            }
            

            if (Debug.EventType == InsightSheath.Wrappers.DebugEventType.LoadDllEvent)
            {
                Console.WriteLine(Debug.ProcessID + " Loaded Dll From \"" + Debug.GetDebugEventLoadDll().ImageName + "\"");


            }

            PsProcessInformation.Poke4(WaitTime, int.MaxValue);
            
            if (Debug.EventType == DebugEventType.ExitProcessEvent)
            {

                List<IntPtr> input = new();


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
             PsProcessInformation TestRun = PsProcessInformation.CreateInstance();
            TestRun.ExtraFlags = PsProcessInformation.SpecialCaseFlags.DebugOnlyThis;
            TestRun.WorkingDirectory = "C:\\Windows\\";
            TestRun.ProcessName = "C:\\Windows\\system32\\notepad.exe";
            Tmp = new SymbolSearchCallBackRoutine(Callback);

       
            
            TestRun.UserDebugCallRoutine = new PsProcessInformation.DebugEventCallBackRoutine(StubCallback);
            TestRun.EnableSymbolEngine = true;
            TestRun.DebugMode = DebugModeType.WorkerThread;
            TestRun.ExtraFlags = PsProcessInformation.SpecialCaseFlags.DebugOnlyThis;
            TestRun.CreationFlags = 2;

            Insight = TestRun.GetSymbolHandler();
            var ProcessId= TestRun.SpawnProcess();

            TestRun.CreationFlags = 2; 



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
