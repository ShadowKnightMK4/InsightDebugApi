using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Wrappers;

namespace InsightSheath.NativeImports
{
    internal enum ThreadConext_Arguments: uint
    {
        UseHandle = 0,
        UseThreadId = 1
    }
    internal static partial class NativeMethods
    {
        /// <summary>
        /// Import for the C level wrapper that calls ThreadContext's SetThreadDescriptionW
        /// </summary>
        /// <param name="That">The C++ this pointer</param>
        /// <param name="NewDescription">New Description for the thread</param>
        /// <returns>True if it worked and false if it did not.</returns>
        /// <remarks>Use of this or <see cref="ThreadContext_GetTheadDescriptionW(IntPtr, string)"/> will trigger LoadLibrary (MSDN) calls on the first use</remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetThreadDescriptionW")]
        public static extern bool ThreadContext_SetTheadDescriptionW(IntPtr That, [MarshalAs(UnmanagedType.LPWStr)] string NewDescription);
        /// <summary>
        /// Gets the Current Thread Description (if defined and system supported).
        /// </summary>
        /// <param name="that">Pointer to the native instance of <see cref="ThreadContext"/> </param>
        /// <returns>Returns a string if defined or null if unsupported/not defined. </returns>
        /// <remarks>Use of this or <see cref="ThreadContext_SetTheadDescriptionW(IntPtr, string)"/> will trigger LoadLibrary (MSDN) calls on the first use</remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadDescriptionW")]
        public static extern string ThreadContext_GetThreadDescriptionW(IntPtr that);

        /// <summary>
        /// Create a new instance of the  native side <see cref="ThreadContext"/> class. Returns a Native pointer that can be passed to <see cref="ThreadContext.ThreadContext(IntPtr)"/> to get a managed wrapper for that class
        /// </summary>
        /// <param name="HandleOrThreadId">4 byte value that can be a thread id or a handle</param>
        /// <param name="Flags">0 = HandleOrThreadId is a Handle to the thread. It's unneeded afterwards. Non-zero means argument is a thread id. </param>
        /// <returns>Returns a pointer to a C++ native side that implements this class on OK. and <see cref="IntPtr.Zero"/> on error. </returns>
        /// <remarks>If a Thread ID, the SECURITY_DESCRIPTOR must allow these access rights (THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION | THREAD_SET_INFORMATION)</remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_CreateInstance")]
        public static extern IntPtr ThreadContext_CreateInstance(ulong HandleOrThreadId, uint Flags);

        /// <summary>
        /// Dispose of the C++ side of <see cref="ThreadContext"/> class. Any pointer to this class is worthless after calling this. 
        /// </summary>
        /// <param name="That">Use the <see cref="NativeStaticContainer.NativePointer"/></param> belonging to a ThreadClass
        /// <returns>Returns true if it worked and false if not. </returns>
        /// <remarks>Passing <see cref="IntPtr.Zero"/> is simply dropped</remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_KillInstance")]
        public static extern bool ThreadContext_KillInstance(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetTargetThread")]
        public static extern bool ThreadContext_SetTargetThread(IntPtr That, IntPtr HandleOfThread, ThreadConext_Arguments Flags);

        /// <summary>
        /// Call the Native Side's routine to get the Priority of the thread. 
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetPriority")]
        public static extern int ThreadContext_GetPriority(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetPriority")]
        public static extern bool ThreadContext_SetPriority(IntPtr That, int NewPriority);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_DupInstance")]
        public static extern IntPtr ThreadContext_DupInstance(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeCreationTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeCreationTime(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeExitTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeExitTime(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeUserTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeUserTime(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeKernelTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeKernelTime(IntPtr That);

        /// <summary>
        /// C Level wrapper to call the C++ class part of ThreadContext to get the thread if this is paired with.
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetTargetThreadId")]
        public static extern uint ThreadContext_GetTargetThreadId(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetIdealProcessor")]
        public static extern uint ThreadContext_GetIdealProcessor(IntPtr That);

        
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetIdealProcessor")]
        public static extern uint ThreadContext_SetIdealProcessor(IntPtr That, uint NewIdealProcessor);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadPriorityBoostSetting")]
        public static extern bool ThreadContext_GetThreadPriorityBoostSetting(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetThreadPriorityBoostSetting")]
        public static extern bool ThreadContext_SetThreadPriorityBoostSetting(IntPtr That, bool EnablePriorityBoost);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadProcessAffinityMask")]
        public static extern uint ThreadContext_GetThreadProcessAffinityMask(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetThreadProcessAffinityMask")]
        public static extern uint ThreadContext_SetThreadProcessAffinityMask(IntPtr That, uint NewIdealProcessor);
        
        



    }
}
