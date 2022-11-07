using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using InsightSheath;
using InsightSheath.Debugging.Process;

namespace InsightSheath.Debugging
{
    /// <summary>
    /// If you are wanting to control your debug message pump on the .NET side rather than native (why?), then these routines will help you 
    /// glue everything together.  This class exists only in the sheath and is not duplicated in Native. Some routines here may be echoed in other classes in the sheath
    /// Some of the routines here exist as extensions to <see cref="DebugEvent"/>
    /// </summary>
    public static class DebugEventWorkerThreadSupport
    {
        [DllImport("kernel32.dll",SetLastError =true)]
        internal static extern bool WaitForDebugEventEx(IntPtr DebugEvent, uint Timeout);

        [DllImport("kernel32.dll", SetLastError = true, EntryPoint ="ContinueDebugEvent")]
        internal static extern bool ContinueDebugEventInternal(uint ProcessId, uint ThreadID, uint ContinueState);

        /// <summary>
        /// Wait for Debug Event to occur and get information regarding it. This Wraps MSDN's WaitForDebugEventEx() (which optpts you into UNICODE) Call it and pass <see cref="DebugEvent"/>'s Native Pointer.
        /// </summary>
        /// <param name="Output">Allocate a native block correctly sized for debug event or just make an instance with existing routines</param>
        /// <param name="WaitTime">How long to wait between times. (uint)-1 aka <see cref="uint.MaxValue"/></param> means wait until event triggers
        /// <returns></returns>
        public static bool WaitForDebugEvent(this DebugEvent Output, uint WaitTime)
        {
            return WaitForDebugEventEx(Output.NativePointer, WaitTime);
        }

        /// <summary>
        /// Return Control of the debugged Software back to Windows and specify how to resume. This wraps MSDN's ContinueDebugEvent
        /// </summary>
        /// <param name="that">Exists to let this routine to be an extension to <see cref="DebugEvent"/> </param>
        /// <param name="ProcessId">Process ID of the debug event that was generated</param>
        /// <param name="ThreadId">thread id of the debug event that was generated</param>
        /// <param name="ContinueState">how to return control back to Windows</param>
        /// <returns></returns>
        public static bool ContinueDebugEvent(this DebugEvent that,uint ProcessId, uint ThreadId, DebugContState ContinueState)
        {
            return ContinueDebugEventInternal(ProcessId, ThreadId, (uint)ContinueState);
        }
    }
}
