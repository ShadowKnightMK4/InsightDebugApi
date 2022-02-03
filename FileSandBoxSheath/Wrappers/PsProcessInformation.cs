using FileSandBoxSheath.NativeImports;
using FileSandBoxSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBoxSheath
{

    [Flags]
    public enum DebugModeType
    {
        /// <summary>
        /// Tells FIleSandbox's native DLL to Not Spwn any worker threads for debug handling.  If you don't implement a debug loop, you will never see your process show up. Irrevelant if the process is not being debugged
        /// </summary>
        NoWorkerThread = 0,
        /// <summary>
        /// When Spawning a process for debugger, FileSandBox.DLL implements a debug loop and offloads the loop to a worker thread.  Put a call to <see cref="PsProcessInformation.PulseDebugEventThead"/> to continue after handling your debug event on a regular basis.  Skipping that means, you're debugged process won't continue after the first event.
        /// </summary>
        WorkerThread = 1

    }


    /// <summary>
    /// choose an enviroment and spawn a process.
    /// This class is a wrapper for the C++ Class named "PS_ProcessInformation" implemented as a native dll in the source PS_ProcessInformation.cpp
    /// </summary>
    public class PsProcessInformation : IDisposable
    {
        // typedef int(WINAPI* DebugEventCallBackRoutine)(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg);
        public delegate int DebugEventCallBackRoutine(IntPtr DebugEvent, IntPtr ContinueState, IntPtr WaitTimer, IntPtr CustomArg);

        /// <summary>
        /// The various commandment flags to enable. (These are constants defined is PS_ProcessInformation.h and should be kept synced
        /// </summary>
        public enum ProcessRestriction : uint
        {
            /// <summary>
            /// Strip generic read from NtOpen/ NtCreate
            /// </summary>
            DropFileReadRequests = 1,
            /// <summary>
            /// report sucess BUT do not actually open the file for reading
            /// </summary>
            PositiveDenyFileReadRequests = 2,
            /// <summary>
            /// Report access denied and do not open file 
            /// </summary>
            NegativeDenyFileReadRequest = 3,

            /// <summary>
            /// Strip generiv write from NtOpen/ NtCreate
            /// </summary>
            DropFileWriteRequests = 4,
            /// <summary>
            /// report sucess BUT do not actually open file for writing
            /// </summary>
            PositiveDenyFileWriteRequest = 5,
            /// <summary>
            /// report acces sdenied and don't open file
            /// </summary>
            NegativeDenyFileWriteRequests = 6,


            /// <summary>
            /// report failure and do not spawn Processes
            /// </summary>
            NegativeDenyProcessSpawn = 7,

            /// <summary>
            /// force the process to load the helper dll.
            /// </summary>
            CommandProcessProperate = 8,


            /// <summary>
            ///  A theoritcal cap for the values. 
            /// </summary>
            CommadMaxValue = 255
        }

        public enum DebugContState : uint
        {
            /// <summary>
            /// The event was processed ok and program execution can continue safely (Use to continue all events and if an Exception Was Handled
            /// </summary
            DebugContinueState = 0x00010002,
            /// <summary>
            /// The Exception Was not handled at all by the your debugger,  pass it back to the debugged program
            /// </summary>
            DebugExceptionNotHandled = 0x80010001,
            /// <summary>
            /// I'm sorry, I'm not in right now to deal with this exception.  Tells Windows to reply the event later.
            /// </summary>
            DebugExceptionReplyLater = 0x40010001

        }

        /// <summary>
        /// Read a dword from local unmanaged memory and return it
        /// </summary>
        /// <param name="TargetLocalMemory">local virtual memory to read from</param>
        /// <returns>returns the dword cast as an int</returns>
        public static uint Peek4(IntPtr TargetLocalMemory)
        {
            return (uint)NativeImports.NativeMethods.Peek4(TargetLocalMemory);
        }

        /// <summary>
        /// Write a 4 byte uint to the target local unmanaged memory.
        /// </summary>
        /// <param name="TargetLocalMemory"></param>
        /// <param name="value"></param>
        public static void Poke4(IntPtr TargetLocalMemory, int value)
        {
            NativeMethods.Poke4(TargetLocalMemory, (uint)value);
        }
        /// <summary>
        /// I've read that the GC will sometimes collect function pointers passed to native routines that will need to call back.
        /// This gets an assigmenet with calling <see cref="UserDebugCallRoutine"/> while sending the pointer into native land. This
        /// should in theory let the GC know that the pointer needs to be kept in memory.
        /// </summary>
        DebugEventCallBackRoutine BackUpCopy;
        /// <summary>
        /// Used to make 'typeical' flags like ResumeThread, DebugProcess nd DebugOnlyThisProcess without needing to look values up
        /// </summary>
        [Flags]
        public enum SpecialCaseFlags
        {
            /// <summary>
            /// nothing needed
            /// </summary>
            None = 0,
            /// <summary>
            /// DEBUG_ONLY_THIS_PROCESS eded
            /// </summary>
            DebugOnlyThis = 1,
            DebugChild = 2,
            CreateSuspended = 3,
        }

        public PsProcessInformation()
        {
            That = NativeMethods.CreatePsProcessInformation();
            if (That == IntPtr.Zero)
            {
                throw new InvalidOperationException("Native DLL FileSandbox Failed to instance a copy of its PS_ProcessCreation via the exported routine");
            }
        }


        #region Exported Routines
        /// <summary>
        /// Spawn the process contained within the udneryling class.  Return Value is the spawned process's ID on ok and 0 on falure.
        /// </summary>
        /// <returns>process id </returns>
        public Int32 SpawnProcess()
        {
            return NativeMethods.PsProcessInformation_Spawn(That).ToInt32();

        }

        /// <summary>
        /// When  Spawning a process with a DEBUG option and nd <see cref="DebugMode"/> is set to <see cref="DebugModeType.WorkerThread"/>, the native dll spawns a thread to handle consuming events.
        /// The thread also using an native Event object to sync giving the events back to the caller so that the caller's gui is not interuptted. Does not actually pulse event.
        /// </summary>
        public void PulseDebugEventThead()
        {
            NativeMethods.PsProcessInformation_PulseDebugEvent(That);
        }


        /// <summary>
        /// get an instance of the symbol handler uses by this process context.  
        /// </summary>
        /// <returns></returns>
        public InsightHunter GetSymbolHandler()
        {
            IntPtr ret = NativeMethods.PsProcessInformation_GetSymbolEngineClassPtr(That);
            if (ret != IntPtr.Zero)
            {
                return new InsightHunter(ret, false);
            }
            return null;
        }

        /// <summary>
        /// If you don't want the worker thred, you'll need to call this routien to update symbol engine eith new data when you get process debug events
        /// </summary>
        /// <param name="DebugEnvent"></param>
        /// <returns></returns>
        public bool UpdateSymbolEngine(IntPtr DebugEnvent)
        {
            throw new NotImplementedException(nameof(UpdateSymbolEngine) + " is not implemented in PSProcessInformation.cs, the native version at PS_ProcessInformation.cpp AND the C linking code at PS_ProcessInformation.CCall.cpp.  You'll need to add code at all three spots. ");

        }

        /// <summary>
        /// Modify a commandment that will effect the spawned process (requires helper dll)
        /// </summary>
        /// <param name="Cmd">pick one from <see cref="ProcessRestriction"/></param>
        /// <param name="Val">true means the effect is active, false if not</param>
        /// <returns>true if command was sec ok (probably ALWAYS true)</returns>
        public bool SetCommandment(ProcessRestriction Cmd, bool Val)
        {
            return NativeImports.NativeMethods.PsProcessInformation_SetCommandment(That, (uint)Cmd, Val);
        }

        /// <summary>
        /// return the status of the passed command (if it exists) or false
        /// </summary>
        /// <param name="Cmd">pick one from <see cref="ProcessRestriction"/></param>
        /// <returns>ether the command's setting or false</returns>
        public bool GetCommandment(ProcessRestriction Cmd)
        {
            return NativeMethods.PsProcesInformation_GetCommandment(That, (uint)Cmd);
        }
        #endregion

        #region Member Access 



        /// <summary>
        /// Specifiy if you want the native dll to handling spaning the process or if you're code will.  Should you make it so your code will. You'll need to include WaitForDebugEvent() and ContinueDebugEvent() calls in a debugger loop
        /// </summary>
        public DebugModeType DebugMode
        {
            get
            {
                return NativeMethods.PsProcessInformation_GetDebugMode(That);
            }
            set
            {
                NativeMethods.PsProcessInformation_SetDebugMode(That, value);
            }
        }


        /// <summary>
        /// Get (or set) the routine that the debug worker thread will be calling.
        /// null (or unasssinged) means use the default one which does nother with events, does not handle exceptions, and continues until it gets a single exit process debug event
        /// </summary>
        public DebugEventCallBackRoutine UserDebugCallRoutine
        {
            get
            {
                return (DebugEventCallBackRoutine)NativeMethods.PsProcessInformation_GetDebugCallbackRoutine(That);
            }
            set
            {
                NativeMethods.PsProcessInformation_SetDebugCallbackRoutine(That, value);
                BackUpCopy = value;
            }
        }





        /// <summary>
        /// Return an unmodifiable list of the entries in the Current Detours List. Please use other routines to change this entry.
        /// </summary>
        public string[] DetourList
        {
            get
            {
                uint size = NativeMethods.PSProcessInformation_GetDetourListSize(That);
                if (size == 0)
                {
                    return null;
                }
                else
                {
                    List<string> ret = new List<string>();
                    for (uint step = 0; step < size; step++)
                    {
                        IntPtr ListPtrEntry = (IntPtr)NativeMethods.PSProcessInformation_GetDetourListIndex(That, step);
                        ret.Add(Marshal.PtrToStringAnsi(ListPtrEntry));
                    }
                    return ret.ToArray();
                }
            }
        }



        /// <summary>
        /// Contains the process that was / will be launched.
        /// </summary>
        public string ProcessName
        {
            get
            {
                return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetProcessName(That));
            }
            set
            {
                NativeMethods.PsProcessInformation_SetProcessName(That, value);
            }
        }

        /// <summary>
        /// Contains the process argumentst that was / will be launched
        /// </summary>
        public string ProcessArguments
        {
            get
            {
                return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetProcessArgument(That));
            }
            set
            {
                NativeMethods.PsProcessInformation_SetProcessArgument(That, value);
            }
        }

        /// <summary>
        ///  Set the starting directory
        /// </summary>
        public string WorkingDirectory
        {
            get
            {
                return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetWorkingDirectory(That));
            }
            set
            {
                NativeMethods.PsProcessInformation_SetWorkingDirectory(That, value);
            }
        }


        /// <summary>
        /// Default is false.  If false, only enviromeent variables you explicity define will be in the spawned process.  If true, the spawned process will enherit your debugger enviroment variables BUT the explicit variables will overread the inheirited ones.
        /// </summary>
        public bool InheritDefaultEnviroment
        {
            set
            {
                NativeMethods.PsProcessInformation_SetInheritDefaultEnviroment(That, value);
            }
        }

        /// <summary>
        /// Enable or Disable the symbol engine. Symbol engine is active once first process starts.  Disabling the negine meains it will no longer get updates until renabling.  <see cref="DebugMode"/>  should be set to <see cref="DebugModeType.WorkerThread"/>
        /// </summary>
        public bool EnableSymbolEngine
        {
            get
            {
                return NativeMethods.PsProcessInformation_GetSymbolHandling(That);
            }
            set
            {
                NativeMethods.PsProcessInformation_SetSymbolHandling(That, value);
            }
        }



        /// <summary>
        /// Specify process creation flags (or get them).  Look at the CreateProcessA/W flags online for more info. 
        /// </summary>
        public uint CreationFlags
        {
            get
            {
                uint Val = NativeMethods.PsProcessInformation_GetCreationFlags(That);
                if (ExtraFlags != SpecialCaseFlags.None)
                {
                    if (ExtraFlags.HasFlag(SpecialCaseFlags.DebugOnlyThis))
                    {
                        // DEBUG_PROCESS ONLY
                        Val |= 1;
                    }
                    else
                    {
                        if (ExtraFlags.HasFlag(SpecialCaseFlags.DebugChild))
                        {
                            //  DEBUG THIS PROCESS and kids.
                            Val |= 2;
                        }
                    }

                    if (ExtraFlags.HasFlag(SpecialCaseFlags.CreateSuspended))
                    {
                        /// CREATE_SUSPSENDED
                        Val |= 4;
                    }
                }
                return Val;
            }
            set
            {
                uint Val = value;
                if (ExtraFlags != SpecialCaseFlags.None)
                {
                    if (ExtraFlags.HasFlag(SpecialCaseFlags.DebugOnlyThis))
                    {
                        // DEBUG_PROCESS ONLY
                        Val |= 1;
                    }
                    else
                    {
                        if (ExtraFlags.HasFlag(SpecialCaseFlags.DebugChild))
                        {
                            //  DEBUG THIS PROCESS and kids.
                            Val |= 2;
                        }
                    }

                    if (ExtraFlags.HasFlag(SpecialCaseFlags.CreateSuspended))
                    {
                        /// CREATE_SUSPSENDED
                        Val |= 4;
                    }
                }
                NativeMethods.PsProcessInformation_SetCreationFlags(That, Val);
            }
        }

        /// <summary>
        /// Specify some specicase case flags. Or you can just directly Set CreationFlags itself.
        /// Implemented only in the C# wrapper. NOT IMPLMENTING in the C++ code.
        /// </summary>
        public SpecialCaseFlags ExtraFlags;


        /// <summary>
        /// Specify an explicit environmental value.  values that match the default envornment will overwrite it for the process
        /// </summary>
        /// <param name="Name"></param>
        /// <param name="Value"></param>
        public void SetExplicitEnviromentValue(string Name, string Value)
        {
            NativeMethods.PsProcessInformation_SetExplicitEnviromentValue(That, Name, Value);
        }

        /// <summary>
        /// Get an explicit environmental value from a previous call to SetExplicitEnviromentValue()
        /// </summary>
        /// <param name="Name"></param>
        /// <returns>returns a string if the value existed or null if it does NOT</returns>
        public string GetExplicitEnviromentValue(string Name)
        {
            return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetExplicitEnviromentValue(That, Name));
        }


        /// <summary>
        /// Add a dll to list of Detours Dlls to force the target process to load on spawn.
        /// DOES NOT WORK if process is already running.
        /// </summary>
        /// <param name="NewDllToForceLoad"></param>
        public void AddDetoursDll(string NewDllToForceLoad)
        {
            NativeMethods.PsProcessInformation_AddDetourDllToLoad(That, NewDllToForceLoad);
        }

        /// <summary>
        /// Clear the detours list
        /// </summary>
        public void ResetDetoursDllList()
        {
            NativeMethods.PsProcessInformation_ClearDetourList(That);
        }


        /// <summary>
        /// Add an entry to the list of paths that the LoadLibraryXXXX() routines will check first before assuming normal search path
        /// </summary>
        /// <param name="NewLocation"></param>
        public bool HelperDll_AddLoadLibraryPath(string NewLocation)
        {
            return NativeMethods.PsProcessInformation_AddPriorityLoadLibraryPath(That, NewLocation);
        }

        /// <summary>
        /// Remove all entries for the HelperDll's priority LoadLibrary search path
        /// </summary>
        public void HelperDll_ClearLoadLibraryPath()
        {
             NativeMethods.PsProcessInformation_ClearPriorityDllPath(That);
        }

        /// <summary>
        /// return how many entries are in the helperdll's priority loadlibrary searwch path
        /// 
        /// </summary>
        /// <returns></returns>
        public uint HelperDll_GetLoadLibraryPathCount()
        {
            return NativeMethods.PsProcessInformation_GetProrityLoadLibraryPath_NumberOf(That);
        }

        /// <summary>
        /// return a string index pointed to the helper dfll's search path (or null) if index out of bounds
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public string HelperDll_IndexLoadLibraryPath(int index)
        {
            IntPtr ret = NativeMethods.PsProcessInformation_IndexPriorityDllPath(That, index);
            if (ret != IntPtr.Zero)
            {
                return Marshal.PtrToStringUni(ret);
            }
            return null;
            
        }

       

        #endregion

        #region Descontructors

        ~PsProcessInformation()
        {
            Dispose(false);
        }

        public virtual void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool Managed)
        {
            if (IsCleanned)
            {
                throw new ObjectDisposedException(nameof(PsProcessInformation));
            }
            if (Managed)
            {

            }
            if (That != IntPtr.Zero)
            {
                NativeMethods.KillPSProcessInformation(That);
            }
            IsCleanned = true;
        }
        IntPtr That = IntPtr.Zero;
        /// <summary>
        /// Once the dipose()/ finalize() is ran, this is set to true
        /// </summary>
        bool IsCleanned = false;
        #endregion
    }
}
