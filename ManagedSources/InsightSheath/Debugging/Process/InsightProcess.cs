using InsightSheath.NativeImports;
using InsightSheath.Win32Struct;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Abstract;
using InsightSheath.Debugging.SymbolEngine;

namespace InsightSheath.Debugging.Process
{
    /// <summary>
    /// Enum to control how to return control of a debugged process your debugger is respending too back to Windows. Used in several spots. <see cref="InsightProcess"/>, <see cref="DebugEventWorkerThreadSupport"/>
    /// </summary>
    public enum DebugContState : uint
    {
        /// <summary>
        /// The event was processed OK and program execution can continue safely (Use to continue all events and if an Exception Was Handled
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
    /// Flags to indicate to <see cref="InsightProcess"/> if you want the worker thread or not.
    /// </summary>
    [Flags]
    public enum DebugModeType
    {
        /// <summary>
        /// Tells Insight's native DLL to Not Spawn any worker threads for debug handling.  If you don't implement a debug loop, you will never see your process show up. Irrelevant if the process is not being debugged.
        /// You're going to want the WorkerThread as development features in this thread are not exported yet for use without the worker thread.
        /// </summary>
        NoWorkerThread = 0,
        /// <summary>
        /// When Spawning a process for debugger, InsightApi.DLL implements a debug loop and offloads the loop to a worker thread.  Put a call to <see cref="InsightProcess.PulseDebugEventThead"/> to continue after handling your debug event on a regular basis.  Skipping that means, your debugged process won't continue after the first event.
        /// </summary>
        WorkerThread = 1

    }


    /// <summary>
    /// choose an environment and spawn a process.
    /// This class is a wrapper for the C++ Class named "InsightProcess" implemented as a native DLL in the source PS_ProcessInformation.cpp
    /// and said class is the functionally the heart of the DLL.
    /// </summary>
    public class InsightProcess : NativeStaticContainer
    {
        #region Forewords
        /*
         * This area contains routines that are probably duplicated else were; however, they exist to make getting starting easier
         */

        #endregion
        public InsightProcess(IntPtr That): base(That)
        {
            if (That == IntPtr.Zero)
            {
                throw new ArgumentNullException(ConstructReceivedNullPointerOnConstructor_message("Argument", "PsProcessInformation.CreateInstance", nameof(Native)));
            }
        }

        public InsightProcess(IntPtr That, bool FreeOnCleanup): base(That, FreeOnCleanup)
        {
            if (That == IntPtr.Zero)
            {
                throw new ArgumentNullException(ConstructReceivedNullPointerOnConstructor_message("Argument", "PsProcessInformation.CreateInstance", nameof(Native)));
            }
        }
        public override int GetHashCode()
        {
            return Native.GetHashCode();
        }
        /// <summary>
        /// This delegate is used in the native worker thread to notify your .NET code when a debug event occurs.
        /// </summary>
        /// <param name="DebugEvent">Pointer to a DEBUG_EVENT struct. <see cref="DebugEvent"/> to make a wrapper for it. IMPORTANT: Your stub is receiving a pointer off the unmanaged heap and should make your wrapper with FreeOnCleanUp=false aka <see cref="DebugEvent(IntPtr)"/></param>
        /// <param name="ContinueState">Pointer to a 4 byte value where to write how your debugger callback responded to the routine. <see cref="SetDebugEventCallbackResponse(IntPtr, DebugContState)"/> or <see cref="LocalUnmanagedMemory.SetDebugEventCallbackResponse(IntPtr, DebugContState)"/> or <see cref="LocalUnmanagedMemory.Poke4(IntPtr, uint)"/> </param>
        /// <param name="WaitTimer">Pointer to a 4 byte value where to write how long to wait until receiving the next debug event. Note: If this timer expires, the worker thread is NOT going to call your callback. <see cref="LocalUnmanagedMemory.Poke4(IntPtr, uint)"/></param>
        /// <param name="CustomArg">Reserved. Always 0. </param>
        /// <returns>Your delegate should return 0 to keep going and non zero to quit. Value does not matter currently.</returns>
        /// <remarks> current C/C++ callback define for this is "typedef int(WINAPI* DebugEventCallBackRoutine)(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg);" </remarks>
        public delegate int DebugEventCallBackRoutine(IntPtr DebugEvent, IntPtr ContinueState, IntPtr WaitTimer, IntPtr CustomArg);

        [Obsolete("Not supported anymore")]
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
            /// report success BUT do not actually open the file for reading
            /// </summary>
            PositiveDenyFileReadRequests = 2,
            /// <summary>
            /// Report access denied and do not open file 
            /// </summary>
            NegativeDenyFileReadRequest = 3,

            /// <summary>
            /// Strip generic write from NtOpen/ NtCreate
            /// </summary>
            DropFileWriteRequests = 4,
            /// <summary>
            /// report success BUT do not actually open file for writing
            /// </summary>
            PositiveDenyFileWriteRequest = 5,
            /// <summary>
            /// report access denied and don't open file
            /// </summary>
            NegativeDenyFileWriteRequests = 6,


            /// <summary>
            /// report failure and do not spawn Processes
            /// </summary>
            NegativeDenyProcessSpawn = 7,

            /// <summary>
            /// force the process to load the helper DLL.
            /// </summary>
            CommandProcessProperate = 8,


            /// <summary>
            ///  A theoretical cap for the values. 
            /// </summary>
            CommadMaxValue = 255
        }



        /// <summary>
        /// I've read that the GC will sometimes collect function pointers passed to native routines that will need to call back.
        /// This gets an assignment with calling <see cref="UserDebugCallRoutine"/> while sending the pointer into native land. This
        /// should in theory let the GC know that the pointer needs to be kept in memory.
        /// </summary>
        DebugEventCallBackRoutine BackUpCopy;
        /// <summary>
        /// Used to make 'typical' flags like ResumeThread, DebugProcess and DebugOnlyThisProcess without needing to look values up. You also can just look the values up.
        /// </summary>
        [Flags]
        public enum SpecialCaseFlags
        {
            /// <summary>
            /// nothing needed
            /// </summary>
            None = 0,
            /// <summary>
            /// Add  the "DEBUG_ONLY_THIS_PROCESS" value when spawning the process
            /// </summary>
            DebugOnlyThis = 1,
            /// <summary>
            /// Add  the "DEBUG_PROCESS" value when spawning the process
            /// </summary>
            DebugChild = 2,
            /// <summary>
            /// Add the CREATE_SUSPENDED value when spawning the process
            /// </summary>
            CreateSuspended = 4,
        }

        /// <summary>
        /// Create an instance of PsProcessInformation on  the native side and return a DotNot size class ready to use.
        /// </summary>
        /// <returns></returns>
        public static InsightProcess CreateInstance()
        {
            IntPtr ret = InsightProcessInternal.CreateInsightProcessNativeClass();
            if (ret == IntPtr.Zero)
            {
                throw new InvalidOperationException("Native DLL InsightApi Failed to instance a copy of its InsightProcess class via the exported routine");
            }
            else
            {
                return new InsightProcess(ret);
            }
        }


        #region Exported Routines
        /// <summary>
        /// Spawn the process contained within the underlying class.  Return Value is the spawned process's ID on OK and 0 on failure.
        /// </summary>
        /// <returns>process id </returns>
        public int SpawnProcess()
        {
            if (ExtraFlags != SpecialCaseFlags.None)
            {
                // the code that applies the special case flags as in the get property code for CreationFlags.

                uint flags = CreationFlags;
                CreationFlags = flags;
            }
            return InsightProcessInternal.InsightProcess_Spawn(Native).ToInt32();
        }

        /// <summary>
        /// When  Spawning a process with a DEBUG option and  <see cref="DebugMode"/> is set to <see cref="DebugModeType.WorkerThread"/>, the native DLL spawns a thread to handle consuming events.
        /// The thread also using an native Event object to sync giving the events back to the caller so that the caller's GUI is not interrupted. Does not actually pulse event.
        /// </summary>
        public void PulseDebugEventThead()
        {
            InsightProcessInternal.InsightProcess_PulseDebugEvent(Native);
        }


        /// <summary>
        /// get an instance of the symbol handler uses by this process context.  
        /// </summary>
        /// <returns></returns>
        public InsightHunter GetSymbolHandler()
        {
            IntPtr ret = InsightProcessInternal.InsightProcess_GetSymbolEngineClassPtr(Native);
            if (ret != IntPtr.Zero)
            {
                return new InsightHunter(ret, false);
            }
            return null;
        }

        /// <summary>
        /// If you don't want the worker thread, you'll need to call this routine to update symbol engine with new data when you get process debug events
        /// </summary>
        /// <param name="DebugEnvent"></param>
        /// <returns></returns>
        public bool UpdateSymbolEngine(IntPtr DebugEnvent)
        {
         

            throw new NotImplementedException(nameof(UpdateSymbolEngine) + " is not implemented in PSProcessInformation.cs, the native version at PS_ProcessInformation.cpp AND the C linking code at PS_ProcessInformation.CCall.cpp.  You'll need to add code at all three spots. ");

        }

        #endregion

        #region Multi Process info
        /// <summary>
        /// Return a list of process ids the class has received via CREATE_PROCESS_DEBUG_EVENT
        /// </summary>
        /// <returns></returns>
   
        /// <summary>
        /// Get a list of thread ids the class has received info about (CREATE_THREAD_DEBUG_EVENT) and return it. If there are know threads, you'll get nullinstead
        /// </summary>
        /// <param name="ProcessID"></param>
      

        #endregion

        #region Process Statistics

        #endregion
        #region Member Access 



        /// <summary>
        /// Specify if you want the native DLL to handling spawning the process or if you're code will.  Should you make it so your code will. You'll need to include WaitForDebugEvent() and ContinueDebugEvent() calls in a debugger loop
        /// </summary>
        public DebugModeType DebugMode
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetDebugMode(Native);
            }
            set
            {
                InsightProcessInternal.InsightProcess_SetDebugMode(Native, value);
            }
        }

        /// <summary>
        /// Ask for the "SeDebugPriv" if spawning things to Debug PROCESS_DEBUG and PROCESS_DEBUG_ONLY_THIS
        /// </summary>
        public bool RequestDebugPriv
        {
            set
            {
                InsightProcessInternal.InsightProcess_RequestDebugPriv(Native, value);
            }
            get
            {
                bool tmp = InsightProcessInternal.InsightProcess_RequestDebugPriv(Native, false);
                if (tmp != false)
                {
                    InsightProcessInternal.InsightProcess_RequestDebugPriv(Native, tmp);
                }
                return tmp;
            }
        }


        /// <summary>
        /// Get (or set) the routine that the debug worker thread will be calling.
        /// null (or unassigned) means use the default one which does nothing with events and exceptions, does not handle exceptions, and continues until it gets a single exit process debug event
        /// </summary>
        public DebugEventCallBackRoutine UserDebugCallRoutine
        {
            get
            {
                return (DebugEventCallBackRoutine)InsightProcessInternal.InsightProcess_GetDebugCallbackRoutine(Native);
            }
            set
            {
                InsightProcessInternal.InsightProcess_SetDebugCallbackRoutine(Native, value);
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
                ulong size = InsightProcessInternal.InsightProcess_GetDetourListSize(Native);
                if (size == 0)
                {
                    return null;
                }
                else
                {
                    List<string> ret = new List<string>();
                    for (uint step = 0; step < size; step++)
                    {
                        IntPtr ListPtrEntry = (IntPtr)InsightProcessInternal.PSProcessInformation_GetDetourListIndex(Native, step);
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
                return Marshal.PtrToStringUni(InsightProcessInternal.InsightProcess_GetProcessName(Native));
            }
            set
            {
                InsightProcessInternal.InsightProcess_SetProcessName(Native, value);
            }
        }

        /// <summary>
        /// Contains the process arguments that will be passed to the process when it is launched.
        /// </summary>
        public string ProcessArguments
        {
            get
            {
                return Marshal.PtrToStringUni(InsightProcessInternal.InsightProcess_GetProcessArgument(Native));
            }
            set
            {
                InsightProcessInternal.InsightProcess_SetProcessArgument(Native, value);
            }
        }

        /// <summary>
        ///  Set the starting directory for the process to be launched
        /// </summary>
        public string WorkingDirectory
        {
            get
            {
                return Marshal.PtrToStringUni(InsightProcessInternal.PsProcessInformation_GetWorkingDirectory(Native));
            }
            set
            {
                InsightProcessInternal.InsightProcess_SetWorkingDirectory(Native, value);
            }
        }


        /// <summary>
        /// Default is TRUE.  Should the code be unable to detour, process is killed and failure is returned.
        /// </summary>
        public bool DetourMustSucceed
        {
            get
            {
                return false;
            }
            set
            {

            }
        }

        /// <summary>
        /// Default is false.  If false, only environment variables you explicitly define will be in the spawned process.  If true, the spawned process will inherit your debugger environment variables BUT the explicit variables you define will override the inherited ones.
        /// </summary>
        public bool InheritDefaultEnviroment
        {
            set
            {
                InsightProcessInternal.InsightProcess_SetInheritDefaultEnviroment(Native, value);
            }
        }

        /// <summary>
        /// Enable or Disable the symbol engine. Symbol engine is active once first process starts.  Disabling the engine means it will no longer get updates until enabled again.  <see cref="DebugMode"/>  should be set to <see cref="DebugModeType.WorkerThread"/>
        /// </summary>
        public bool EnableSymbolEngine
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetSymbolHandling(Native);
            }
            set
            {
                InsightProcessInternal.InsightProcess_SetSymbolHandling(Native, value);
            }
        }



        /// <summary>
        /// Specify process creation flags (or get them).  Look at the CreateProcessA/W <see href="https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa"/> flags online for more info. 
        /// </summary>
        public uint CreationFlags
        {
            get
            {
                uint Val = InsightProcessInternal.InsightProcess_GetCreationFlags(Native);
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
                InsightProcessInternal.InsightProcess_SetCreationFlags(Native, Val);
            }
        }

        /// <summary>
        /// Specify some specific case flags. Or you can just directly Set CreationFlags itself.
        /// Implemented only in the C# wrapper. NOT IMPLMENTING in the C++ code.
        /// </summary>
        public SpecialCaseFlags ExtraFlags
        {
            get
            {
                uint Flags = CreationFlags;
                return _ContainerFlag;
            }
            set
            {
                _ContainerFlag = value;
                uint Flags = CreationFlags;
            }
        }
        private SpecialCaseFlags _ContainerFlag;


        /// <summary>
        /// Specify an explicit environmental value.  Values that match the default environment will overwrite the default onces for the process
        /// </summary>
        /// <param name="Name">This is the name of the environment value to define. For example "PATH"</param>
        /// <param name="Value">This is the value of the environment value to define. For example "C:\Windows;C:\Windows\system32;</param>
        public void SetExplicitEnviromentValue(string Name, string Value)
        {
            InsightProcessInternal.InsightProcess_SetExplicitEnviromentValue(Native, Name, Value);
        }

        /// <summary>
        /// Get an explicit environmental value from a previous call to <see cref="SetExplicitEnviromentValue(string, string)"/>
        /// </summary>
        /// <param name="Name">This is the name of the environment value to define. For example "PATH"</param>
        /// <returns>Returns a string if the value exists or null if it does NOT</returns>
        public string GetExplicitEnviromentValue(string Name)
        {
            return Marshal.PtrToStringUni(InsightProcessInternal.InsightProcess_GetExplicitEnviromentValue(Native, Name));
        }

        /// <summary>
        /// Retrieve instance of the class that handles the startup info management. 
        /// </summary>
        /// <remarks>Should the Native size for this instance of StartupInfo change to be allocated, this routine will need to be updated to prevent a memeory leak.</remarks>
        /// <returns>This routines an instance to a <see cref="StartupInfoExW"/> class that you can use to customize startup settings. This instance is part of the underlying instance of <see cref="InsightProcess"/> and should not be freed/deleted on clean up if duplicated</returns>
        public StartupInfoExW GetStartupInfoClass()
        {
            /* MEMORY MANAGEMENT IMPORTANCE***** 
             * Should the Native implementation for InsightProcess's instance of StartupInfo change to
             * be allocated on demand, 
             * the false in this statement will need to be changed to true*/
            return new StartupInfoExW(InsightProcessInternal.InsightProcess_GetStartupInfoClass(Native), false);
        }

        /// <summary>
        /// Add a DLL to list of Detours DLLs to force the target process to load on spawn.
        /// DOES NOT WORK if process is running 
        /// </summary>
        /// <param name="NewDllToForceLoad"></param>
        /// <remarks>Currently detouring from parent to child with matching bitness works; however, x64 bit code is limited currently to detouring x64 processes. x86 can do both.</remarks>
        public void AddDetoursDll(string NewDllToForceLoad)
        {
            InsightProcessInternal.PsProcessInformation_AddDetourDllToLoad(Native, NewDllToForceLoad);
        }

        /// <summary>
        /// Clear the detours list back to empty.
        /// </summary>
        public void ResetDetoursDllList()
        {
            InsightProcessInternal.InsightProcess_ClearDetourList(Native);
        }


        /// <summary>
        /// Add an entry to the list of paths that the LoadLibraryXXXX() routines will check first before assuming normal search path
        /// </summary>
        /// <param name="NewLocation"></param>
        public bool HelperDll_AddLoadLibraryPath(string NewLocation)
        {
            return InsightProcessInternal.InsightProcess_AddPriorityLoadLibraryPath(Native, NewLocation);
        }

        /// <summary>
        /// Remove all entries for the HelperDll's priority LoadLibrary search path
        /// </summary>
        public void HelperDll_ClearLoadLibraryPath()
        {
            InsightProcessInternal.InsightProcess_ClearPriorityDllPath(Native);
        }

        /// <summary>
        /// return how many entries are in the helperdll's priority LoadLibrary search path
        /// 
        /// </summary>
        /// <returns></returns>
        public uint HelperDll_GetLoadLibraryPathCount()
        {
            return InsightProcessInternal.InsightProcess_GetProrityLoadLibraryPath_NumberOf(Native);
        }

        /// <summary>
        /// return a string index pointed to the helper DLL's search path (or null) if index out of bounds
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public string HelperDll_IndexLoadLibraryPath(int index)
        {
            IntPtr ret = InsightProcessInternal.InsightProcess_IndexPriorityDllPath(Native, index);
            if (ret != IntPtr.Zero)
            {
                return Marshal.PtrToStringUni(ret);
            }
            return null;
            
        }



        #endregion
        #region Getting Memory information about your main running process
        /// <summary>
        /// Get all the memory stats in once call and return a struct containing them. May return null if it can't fetch it.
        /// </summary>
        public ProcessMemoryCount32? GetMemoryStatsBulk
        {
            get
            {
                IntPtr ret = InsightProcessInternal.InsightProcess_GetMemoryStatsBulkPtr(Native);
                if (ret != IntPtr.Zero)
                {
                    return Marshal.PtrToStructure<ProcessMemoryCount32>(ret);
                }
                return null;
            }
        }
        /// <summary>
        /// Number of Page Faults for the main process.
        /// </summary>
        /// <remarks>Why is this hidden? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong PageFaultCount
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetPageFaultCount(Native);
            }
        }
        /// <summary>
        /// Peaked working set size, in bytes
        /// </summary>
        /// <remarks>Why is this hidden in Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong PeakWorkingSet
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetPeakWorkingSet(Native);
            }
        }
        /// <summary>
        /// Working set size, in bytes
        /// </summary>
        /// <remarks>Why is this hidden in Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong WorkingSet
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetWorkingSetSize(Native);
            }
        }
        /// <summary>
        /// peaked page pool usage, in bytes
        /// </summary>
        /// <remarks>Why is this hidden in Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong QuotaPeakPagePoolUsage
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetQuotaPeakPagePoolUsage(Native);
            }
        }

        /// <summary>
        /// current page pool usage, in bytes
        /// </summary>
        /// <remarks>Why is this hidden in Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong QuotaPagePoolUsage
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetQuotaPagePoolUsage(Native);
            }
        }

        /// <summary>
        /// peaked nonpaged pool usage, in bytes
        /// </summary>
        /// <remarks>Why is this hidden in Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong QuotaPeakNonPagePoolUsage
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetQuotaPeakNonPagePoolUsage(Native);
            }
        }
        /// <summary>
        /// current non page pool usage, in bytes
        /// </summary>
        /// <remarks>Why is this hidden in Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong QuotaNonPagePoolUsage
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetQuotaPeakNonPagePoolUsage(Native);
            }
        }
        /// <summary>
        /// Commit change value for the process in bytes.  Note: MSDN Windows 7/ Server 2008 says check <see cref="PrivateUsage"/> instead
        /// </summary>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong PageFileUsage
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetPageFileUsage(Native);
            }
        }

        /// <summary>
        /// Peak value of committed change during process lifetime
        /// </summary>
        /// <remarks>Why is this hidden in Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong PeakPageFileUsage
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetPeakPageFileUsage(Native);
            }
        }
        /// <summary>
        /// Private memory usage.
        /// </summary>
        /// <remarks>Why is this hidden Visual Studio's debug class Visualizer? Performance reasons while viewing in visual studio's debugger visualizer and the data can be gotten with <see cref="GetMemoryStatsBulk"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public ulong PrivateUsage
        {
            get
            {
                return InsightProcessInternal.InsightProcess_GetPrivateUsage(Native);
            }
        }





        #endregion
        #region Deconstructors / Cleanup

        ~InsightProcess()
        {
            Dispose(false);
        }

        

        protected override void Dispose(bool Managed)
        {
            if (IsCleanned)
            {
                throw new ObjectDisposedException(nameof(InsightProcess));
            }
            if (Managed)
            {

            }
            if (Native != IntPtr.Zero)
            {
                InsightProcessInternal.InsightProcess_KillInstance(Native);
            }
            IsCleanned = true;
        }

        /// <summary>
        /// For the <see cref="InsightProcess.DebugEventCallBackRoutine"/>.  Use the <see cref="IntPtr"/> passed containing the location of where to write the continue state.
        /// </summary>
        /// <param name="contStat">Pointer receiving in the <see cref="InsightProcess.DebugEventCallBackRoutine"/> to process events</param> 
        /// <param name="Response">Indicate your response to Windows using <see cref="DebugContState"/></param>
        public static void SetDebugEventCallbackResponse(IntPtr contStat, DebugContState Response)
        {
            LocalUnmanagedMemory.SetDebugEventCallbackResponse(contStat, Response);
        }

        /// <summary>
        /// Once the dispose()/ finalize() is ran, this is set to true
        /// </summary>
        bool IsCleanned = false;
        #endregion
    }
}
