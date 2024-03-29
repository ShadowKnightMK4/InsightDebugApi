using InsightSheath;
using InsightSheath.Telemetry;
//using FileSandBoxSheath.NativeImports is intended for general use.;
using InsightSheath.Wrappers;
using System;
//using System.Windows.Forms;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using InsightSheath.Debugging;
using InsightSheath.Debugging.Process;
using InsightSheath.Debugging.Thread;
using InsightSheath.Debugging.SymbolEngine;
using static InsightSheath.Debugging.SymbolEngine.InsightHunter;
using InsightSheath.Win32Struct;
using InsightSheath.Resource;
using InsightSheath.Detours;
using System.IO;
using System.Drawing;

namespace FileSandBox_GUI
{
    static class Program
    {
        //typedef int(WINAPI* DebugEventCallBackRoutine)(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg);

        static List<string> DesourcesTouched = new List<string>();
        static bool Quit = false;
        static InsightHunter Insight;
        //public delegate bool SymbolSearchCallBackRoutine(IntPtr SymbolInfo);
        static InsightHunter_SymbolSearchCallBackRoutine Tmp;
        static InsightHunter_SymbolSourceCallbackRoutine Tmp2;
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
            
            //MemoryNative.SetDebugEventCallbackResponse(ContStat, DebugContState.DebugExceptionNotHandled);

            //InsightProcess.Poke4(ContStat, unchecked ((int)DebugContState.DebugExceptionNotHandled));
            InsightProcess.SetDebugEventCallbackResponse(ContStat, DebugContState.DebugExceptionNotHandled);
            MemoryNative.Poke4(WaitTime, 0xffffffff);
            switch (Debug.EventType)
            {
                case DebugEventType.CreateProcessEvent:
                    {
                        var test = Debug.GetDebugEventCreateProcessInfo();
                        Console.WriteLine(test.ImageName);
                        break;
                    }
                case DebugEventType.CreateTheadEvent:
                    {
                        var test = Debug.GetDebugEventCreateThreadInfo();

                        if (Debug.IsEventFrom32BitProcess)
                        {
                            var thread = ThreadContext.CreateInstance(test.ThreadHandle);
                            if (thread != null)
                            {
                                var context = thread.Context;
                                var wow = thread.Wow64Context;

                                var testclass = new Wow64Context(wow, false);

                                testclass.Dr0 = testclass.Dr0;

                                thread.Context = testclass.NativePointer;

                            }
                            
                        }
                        Console.WriteLine(test.ThreadStartAddress);
                        break;
                    }
                case DebugEventType.ExitProcessEvent:
                    {
                        var test = Debug.GetEventExitProcessInfo();
                        
                        Console.WriteLine("Exiting with " + test.ExitCode);
                        break;
                    }
                case DebugEventType.ExitThreadEvent:
                    {
                        var test = Debug.GetEventExitThreadInfo();
                        Console.WriteLine("Thread exit: " + test.ExitCode);
                        break;
                    }
                case DebugEventType.LoadDllEvent:
                    {
                        var test = Debug.GetDebugEventLoadDll();
                        Console.WriteLine(test.ImageName);
                        break;
                    }
                case DebugEventType.OutputDebugString:
                    {
                        var test = Debug.GetDebugEventStringInfo();

                        Console.WriteLine(test.OutputString);

                        break;
                    }
                case DebugEventType.RipEvent:
                    {
                        var test = Debug.GetDebugEventRipInfo();
                        Console.WriteLine(test.Error);
                        break;
                    }
                case DebugEventType.UnloadDllEvent:
                    {
                        
                        var test = Debug.GetDebugEventUnloadDllInfo();
                        Console.WriteLine(test.BaseOfDll + " Unloaded");
                        break;
                    }
                case DebugEventType.ExceptionEvent:
                    {
                        
                        DebugEventExceptionInfo test = Debug.GetDebugEventExceptionInfo();

                       
                        if (test.IsIoDeviceTelemetryException())
                        {
                            var data = test.GetIoDeviceExceptionType();
                            switch (data)
                            {
                                case IoDeviceTelemetryReaderExtensions.NotificationType.CreateFile:
                                    {
                                        var Info = test.GetCreateFileSettings();
                                        if (Info.DesiredAccess.HasFlag(AccessMasks.FileReadData))
                                        {
                                            Info.SetForceHandle(Telemetry.InvalidHandleValue64);
                                            Info.SetLastErrorValue(5);
                                        }
                                        InsightProcess.SetDebugEventCallbackResponse(ContStat, DebugContState.DebugContinueState);
                                        break;
                                    }
                                case IoDeviceTelemetryReaderExtensions.NotificationType.NtCreateFile:
                                    {
                                        var Info2 = test.GetNtCreateFileSettings();

                                        break;
                                    }
                                default:
                                    InsightProcess.SetDebugEventCallbackResponse(ContStat, DebugContState.DebugExceptionNotHandled);
                                    break;
                            }
                        }
                        else
                        {
                            MemoryNative.SetDebugEventCallbackResponse(ContStat, DebugContState.DebugExceptionNotHandled);
                        }
                        
    
                        
                        break;
                    }

            }
            
            /*
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
                        if (ExceptionData.GetIoDeviceExceptionType() == IoDeviceTelemetryReaderExtensions.NotificationType.NtCreateFile)
                        {
                            var Data = ExceptionData.GetNtCreateFileSettings();


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
            */
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


        static IntPtr TestPtr;  
        static bool FileCallaback (IntPtr Context, string original, string current, IntPtr Replacement)
        {

            if (original.CompareTo("KERNEL32.dll") == 0)
            {
                if (TestPtr == IntPtr.Zero)
                {
                    TestPtr = Marshal.StringToHGlobalAnsi("KERNEL64.DLL");
                }
                
                {
                    if (IntPtr.Size == 4)
                    {
                        MemoryNative.Poke4(Replacement, (uint)TestPtr);
                    }
                    else
                    {
                        MemoryNative.Poke8(Replacement, (ulong)TestPtr);
                    }
                }

            }
                return true;
        }

        static void Main()
        {
           
             InsightProcess TestRun = InsightProcess.CreateInstance();
            TestRun.ExtraFlags = InsightProcess.SpecialCaseFlags.DebugOnlyThis;
            TestRun.WorkingDirectory = "C:\\Windows\\";
            TestRun.ProcessName = "C:\\Windows\\system32\\notepad.exe";
            //TestRun.ProcessName = "C:\\Windows\\system32\\cmd.exe";
            //TestRun.ProcessName = "C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x86\\program\\HelloWorld.exe";
            //TestRun.ProcessName = "C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x64\\program\\HelloWorld.exe";
            //TestRun.AddDetoursDll("C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x64\\program\\Telemetry\\IoDeviceTracking.dll");


            Console.WriteLine("Target is a " + Enum.GetName(typeof(MachineType), HelperRoutines.GetPEMachineType(TestRun.ProcessName)));
            var typ = HelperRoutines.GetPEMachineType(TestRun.ProcessName) ;
            if (typ ==  MachineType.MachineI386)
            {
                Console.WriteLine("Picking the x86 version of the detours");
                TestRun.AddDetoursDll("C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x86\\program\\Telemetry\\IoDeviceTracking32.dll");
            }
            else
            {
                Console.WriteLine("Picking the x64 version of the detours");
                TestRun.AddDetoursDll("C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x64\\program\\Telemetry\\IoDeviceTracking64.dll");
            }
            Tmp = new InsightHunter_SymbolSearchCallBackRoutine(Callback);

       
            
            TestRun.UserDebugCallRoutine = new InsightProcess.DebugEventCallBackRoutine(StubCallback);
            TestRun.UserDebugCallRoutine = StubCallback;
            TestRun.EnableSymbolEngine = true;
            TestRun.DebugMode = DebugModeType.EnableWorkerThread | DebugModeType.WorkerDropCallbackForNoEvents;
            
            TestRun.CreationFlags = InsightProcess.CreationFlagValues.DebugProcess;
            TestRun.GetStartupInfoClass().FlagSetterHelper = true;
            TestRun.GetStartupInfoClass().Flags = StartupInfoExW_Flags.Startf_UseShowWindow;
            TestRun.GetStartupInfoClass().ShowWindow = StartupInfoExW_ShowWindow.Maximize;
            //TestRun.ExtraFlags = InsightProcess.SpecialCaseFlags.DebugOnlyThis;
            
            TestRun.RequestDebugPriv = true;
            Insight = TestRun.GetSymbolHandler();

        

            
            var ProcessId = TestRun.SpawnProcess();


            try
            {
                using (System.Diagnostics.Process Target = Process.GetProcessById(ProcessId))
                {
                    while (true)
                    {

                        TestRun.PulseDebugEventThead();
                        try
                        {
                            if (!true)
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
                        catch (AccessViolationException e)
                          {
                            Console.WriteLine(e.Message + "<- triggered when seeing if runnig");
                        }
                        catch (System.ComponentModel.Win32Exception e)
                        {
                            Console.WriteLine(e.Message + "<- triggered when seeing if runnig");
                        }
                    }
                }
            }
            finally
            {

            }

            TestRun.Dispose();

            //Application.Run(new MainWindow());
            //Application.Exit();
        }

        private static bool typeme(IntPtr hModule, IntPtr LpType, IntPtr CustomArg)
        {
            string type = null;
            ResourceTypes ttyupe = 0;

            if (ResourceWalker.IsIntResource(LpType))
            {
                ttyupe = (ResourceTypes)LpType;
                Console.WriteLine(Enum.GetName(typeof(ResourceTypes), ttyupe));
            }
            else
            {
                type = Marshal.PtrToStringUni(LpType);
                Console.WriteLine(type);
            }
            

            return true;
        }

        private static bool GetResource(IntPtr hModule, IntPtr lpType, IntPtr Name, IntPtr CustomArg)
        {
            ResourceTypes ttype, tname;
            string type=null, name=null;
            if (ResourceWalker.IsIntResource(lpType))
            {
                ttype = (ResourceTypes)lpType;
            }
            else
            {
                type = Marshal.PtrToStringUni(lpType);
                ttype = 0;

            }

            if (ResourceWalker.IsIntResource(Name))
            {
                tname = (ResourceTypes)Name;
            }
            else
            {
                name= Marshal.PtrToStringUni(Name);
                tname = 0;
            }

            Console.Write(" Resource of type: (");
            if (type == null)
            {
                Console.Write(Enum.GetName(typeof(ResourceTypes), ttype));
            }
            else
            {
                Console.Write(type);
            }

            Console.Write(") and its name is (");
            if (type == null)
            {
                Console.Write(Enum.GetName(typeof(ResourceTypes), tname));
            }
            else
            {
                Console.Write(name);
            }

            Console.WriteLine();
            return true;
        }
    }
}
