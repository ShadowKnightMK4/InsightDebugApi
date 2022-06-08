using InsightSheath.Debugging;
using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.NativeImports
{

    /// <summary>
    /// Internal Class. This imports the routines used in <see cref="DebugEvent"/> class
    /// </summary>
    static partial class DebugEventNative
    {
        /// <summary>
        /// Return the Process ID contained within the Native DEBUG_EVENT struct
        /// </summary>
        /// <param name="LPDebugEvent"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_GetProcessId", ExactSpelling = false)]
        public static extern uint DebugEvent_GetProcessID(IntPtr LPDebugEvent);

        /// <summary>
        /// Return the Thread ID contained within the Native DEBUG_EVENT struct
        /// </summary>
        /// <param name="LPDebugEvent"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_GetThreadId", ExactSpelling = false)]
        public static extern uint DebugEvent_GetThreadID(IntPtr LPDebugEvent);

        /// <summary>
        /// Return the Event Tuype Contrained within the DEBUG_EVENT struct
        /// </summary>
        /// <param name="LPDebugEvent"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_GetEventType", ExactSpelling = false)]
        public static extern DebugEventType DebugEvent_GetEventType(IntPtr LPDebugEvent);


        /// <summary>
        /// If the Event type is a RIP_EVENT ( <see cref="DebugEvent_GetEventType(IntPtr)"/> returns <see cref="DebugEventType.RipEvent"/>, returns the Type of Rip Event
        /// </summary>
        /// <param name="LPDebugEvent"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_RipGetErrorType", ExactSpelling = false)]
        public static extern RipErrorType DebugEvent_RipGetErrorType(IntPtr LPDebugEvent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_RipGetError", ExactSpelling = false)]
        public static extern uint DebugEvent_RipGetError(IntPtr LPDebugEvent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_CreateProcessGetHFile", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_CreateProcessGetFileHandle(IntPtr LPDebugEvent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExceptionInfo_GetFirstChance", ExactSpelling = false)]
        public static extern UInt32 DebugEvent_ExceptionInfo_GetFirstChance(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExceptionInfo_GetExceptionInformation", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_ExceptionInfo_GetExceptionInformation(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExceptionInfo_GetExceptionArgumentCount", ExactSpelling = false)]
        public static extern UInt32 DebugEvent_ExceptionInfo_GetExceptionArgumentCount(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExceptionInfo_GetExceptionAddress", ExactSpelling = false)]
        public static extern UInt64 DebugEvent_ExceptionInfo_GetExceptionAddress(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExceptionInfo_GetExceptionRecord", ExactSpelling = false)]
        public static extern UInt32 DebugEvent_ExceptionInfo_GetExceptionRecord(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExceptionInfo_GetExceptionFlags", ExactSpelling = false)]
        public static extern UInt32 DebugEvent_ExceptionInfo_GetExceptionFlags(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExceptionInfo_GetExceptionCode", ExactSpelling = false)]
        public static extern UInt32 DebugEvent_ExceptionInfo_GetExceptionCode(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_LoadDllInfo_GetHFile", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_LoadDllInfo_GetHFile(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_LoadDllInfo_GetBaseOfDll", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_LoadDllInfo_GetBaseOfDll(IntPtr LPDebugEevent);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_LoadDllINfo_GetDebugInfoOffset", ExactSpelling = false)]
        public static extern UInt32 DebugEvent_LoadDllINfo_GetDebugInfoOffset(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_LoadDllINfo_GetDebugInfoSize", ExactSpelling = false)]
        public static extern UInt32 DebugEvent_LoadDllINfo_GetDebugInfoSize(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_LoadDllInfo_GetImageNamge", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_LoadDllInfo_GetImageNamge(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_LoadDllInfo_GetUnicodeFlag", ExactSpelling = false)]
        public static extern int DebugEvent_LoadDllInfo_GetUnicodeFlag(IntPtr LPDebugEevent);


        
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_UnloadDllInfo_GetBaseAddress", ExactSpelling = false)]
        public static extern ulong DebugEvent_UnloadDllInfo_GetBaseAddress(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_CreateThreadInfo_GetThreadHandle", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_CreateThreadInfo_GetThreadHandle(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_CreateThreadInfo_GetThreadStartAddress", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_CreateThreadInfo_GetThreadStartAddress(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_CreateThreadInfo_GetThreadLocalBase", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_CreateThreadInfo_GetThreadLocalBase(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExitThreadInfo_GetExitCode", ExactSpelling = false)]
        public static extern uint DebugEvent_ExitThreadInfo_GetExitCode(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_ExitProcessInfo_GetExitCode", ExactSpelling = false)]
        public static extern uint DebugEvent_ExitProcessInfo_GetExitCode(IntPtr LPDebugEevent);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_IsEventFrom32Bit", ExactSpelling = false)]
        public static extern bool DebugEvent_IsEventFrom32Bit(IntPtr LPDebugEevent);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugEvent_AllocateStructure", ExactSpelling = false)]
        public static extern IntPtr DebugEvent_AllocateStructure();








        /*
		
		*/
        //DWORD WINAPI DebugEvent_ExceptionInfo_GetFirstChance(LPDEBUG_EVENT Ptr)
        //DWORD WINAPI Debug_Event_ExceptionInfo_GetFirstChange(LPDEBUG_EVENT Ptr)
    }
}
