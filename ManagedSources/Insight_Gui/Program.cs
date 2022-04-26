using InsightSheath;
using InsightSheath.Structs;
using InsightSheath.Telemetry;
//using FileSandBoxSheath.NativeImports is intended for general use.;
using InsightSheath.Wrappers;
using System;
//using System.Windows.Forms;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using static InsightSheath.Wrappers.InsightHunter;

namespace FileSandBox_GUI
{
    static class Program
    {
        //typedef int(WINAPI* DebugEventCallBackRoutine)(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg);

        static List<string> DesourcesTouched = new List<string>();
        static bool Quit = false;
        static InsightHunter Insight;
        //public delegate bool SymbolSearchCallBackRoutine(IntPtr SymbolInfo);
        static SymbolSearchCallBackRoutine Tmp;
        static SymbolSourceCallbackRoutine Tmp2;
        static bool SymbolSearchCallBackRoutineHandle(IntPtr Target)
        {
            var Symbol = new SymbolInfo(Target);
            return true;
        }

        static bool SymbolSourceCallBackHandler(IntPtr Data)
        {
            var Info = Marshal.PtrToStructure<SourceFile>(Data);
            Console.WriteLine(Info.FileName + " is loaded at " + Info.ModBase);
            return true;
        }
        static int StubCallback(IntPtr DebugEvent, IntPtr ContStat, IntPtr WaitTime, IntPtr Custom)
        {
            DebugEvent Debug = new DebugEvent(DebugEvent);
            InsightProcess.Poke4(ContStat, (int)InsightProcess.DebugContState.DebugContinueState);

            
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

         //       Console.WriteLine("BeginningEnumeration of loaded symbols");
   


            }

            if (Debug.EventType == DebugEventType.LoadDllEvent)
            {
                SymbolSearchCallBackRoutine test = Callback;
                SymbolSourceCallbackRoutine test2 = SymbolSourceCallBackHandler;

                Insight.EnumerateSourceFiles("*!*", test2);

            //    Insight.EnumerateSymbols("*!*", test);
            }
            
            if (Debug.EventType == InsightSheath.Wrappers.DebugEventType.ExceptionEvent)
            {
                if (Debug.IsIoDeviceTelemetryException())
                {
                    Console.WriteLine("Receive IoTelemetryException");
                    using (var ExceptionData = Debug.GetDebugEventExceptionInfo())
                    {
                        if (ExceptionData.GetIoDeviceExceptionType() == IoDeviceTelemetryReaderExtensions.NotificationType.CreateFile)
                        {
                            var Info = ExceptionData.GetCreateFileSettings();
                            DesourcesTouched.Add(Info.FileName + "for " + Enum.GetName(typeof(AccessMasks), Info.DesiredAccess));
                        }
                     
                    }
                    InsightProcess.Poke4(ContStat, unchecked((int)InsightProcess.DebugContState.DebugContinueState));
                }
                else 
                {
                    var Data = Debug.GetDebugEventExceptionInfo();
                    var ArgList = Data.ExceptionParameter64;
                    InsightProcess.Poke4(ContStat, unchecked((int)InsightProcess.DebugContState.DebugExceptionNotHandled));
                }

                
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


  


            }
            

            if (Debug.EventType == InsightSheath.Wrappers.DebugEventType.LoadDllEvent)
            {
                Console.WriteLine(Debug.ProcessID + " Loaded Dll From \"" + Debug.GetDebugEventLoadDll().ImageName + "\"");


            }

            InsightProcess.Poke4(WaitTime, int.MaxValue);
            
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
             InsightProcess TestRun = InsightProcess.CreateInstance();
            TestRun.ExtraFlags = InsightProcess.SpecialCaseFlags.DebugOnlyThis;
            TestRun.WorkingDirectory = "C:\\Windows\\";
            TestRun.ProcessName = "C:\\Windows\\system32\\notepad.exe";
            //TestRun.ProcessName = "C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x86\\program\\HelloWorld.exe";
            TestRun.AddDetoursDll("C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x86\\program\\Telemetry\\IoDeviceTracking.dll");
            Tmp = new SymbolSearchCallBackRoutine(Callback);

       
            
            TestRun.UserDebugCallRoutine = new InsightProcess.DebugEventCallBackRoutine(StubCallback);
            TestRun.EnableSymbolEngine = true;
            TestRun.DebugMode = DebugModeType.WorkerThread;
            TestRun.ExtraFlags = InsightProcess.SpecialCaseFlags.DebugOnlyThis;
            TestRun.CreationFlags = 2;

            Insight = TestRun.GetSymbolHandler();

      
            

            var ProcessId = TestRun.SpawnProcess();


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
                            foreach (string file in DesourcesTouched)
                            {
                                Console.WriteLine(file);
                            }
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
