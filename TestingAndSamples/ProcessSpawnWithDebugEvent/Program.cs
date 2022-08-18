using System;
using InsightSheath.Debugging;
using InsightSheath.Debugging.Process;


namespace ProcessSpawnWithDebugEvent
{
    class Program
    {
        
        /// <summary>
        /// example callback for the worker thread.
        /// </summary>
        /// <param name="DebugEvent"></param>
        /// <param name="ContState"></param>
        /// <param name="WaitTimer"></param>
        /// <param name="CustomArg"></param>
        /// <returns></returns>
        static int debug_event_callback(IntPtr DebugEventPtr, IntPtr ContState, IntPtr WaitTimer, IntPtr CustomArg)
        {
            DebugEvent DebugEvent = new DebugEvent(DebugEventPtr);
            switch (DebugEvent.EventType)
            {
                case DebugEventType.ExceptionEvent:
                    {
                        InsightProcess.SetDebugEventCallbackResponse(ContState, DebugContState.DebugExceptionNotHandled);
                        break;
                    }
                case DebugEventType.OutputDebugString:
                    {
                        Console.WriteLine("Process OutputDebugString: \"" + DebugEvent.GetDebugEventStringInfo().OutputString +"\"");
                        break;
                    }
                default:
                    {
                        Console.WriteLine("Debug Event :" + Enum.GetName(typeof(DebugEventType), DebugEvent.EventType));
                        break;
                    }
            }
            return 0;
        }

        static InsightProcess.DebugEventCallBackRoutine Callback;
        static void Main(string[] args)
        {
            Callback = new InsightProcess.DebugEventCallBackRoutine(debug_event_callback);
                /// This adds 
                Environment.SetEnvironmentVariable("PATH", Environment.GetEnvironmentVariable("PATH") + ";C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x64\\program");
                InsightProcess LaunchCmd; 
                LaunchCmd = InsightProcess.CreateInstance();
                LaunchCmd.CreationFlags = InsightProcess.CreationFlagValues.CreateNewConsole | InsightProcess.CreationFlagValues.DebugOnlyThisProcess;
                LaunchCmd.ProcessArguments = "/K \"echo Hello World from Process Spawn with debug processing example Example\"\r\n";
                LaunchCmd.ProcessName = Environment.GetFolderPath(Environment.SpecialFolder.System) + "\\cmd.exe";

                /*
                 * The worker thread spawns the process and pumps the Native routines WaitForDebugEvent() and ContinueDebugEvent()
                 * You don't have to use the worker thread if you don't want; however, most of the development works under the assumation
                 * that the worker thread is active.
                 * */
                LaunchCmd.DebugMode = DebugModeType.WorkerThread;
              LaunchCmd.UserDebugCallRoutine = new InsightProcess.DebugEventCallBackRoutine(debug_event_callback);
                

                LaunchCmd.SpawnProcess();
            //InsightProcess.WaitForProcessExit();
            }
        }
    }


