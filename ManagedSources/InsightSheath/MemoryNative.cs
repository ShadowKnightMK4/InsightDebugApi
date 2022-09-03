using InsightSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Debugging.Process;
using InsightSheath.Debugging;
namespace InsightSheath
{
    /// <summary>
    /// Read and write to native memory via Peek and Poke routines.
    /// </summary>
    /// <remarks>IMPORTANT! This is accessing unmanaged memory outside of C#'s GC. The InsightAPI.dll does deal with given  and receiving native pointers ect and there will be times you'll need to do that with a separate process. </remarks>
    public static class MemoryNative
    {
        /// <summary>
        /// Read a 4 byte value from unmanaged memory
        /// </summary>
        /// <param name="Target">non null pointer to location to read from</param>
        /// <returns>return contains of passed memory location</returns>
        public static uint Peek4(IntPtr Target)
        {
            return MemoryNativeInternal.Peek4(Target);
        }

        /// <summary>
        /// Write a 4 byte value to the memory location passed
        /// </summary>
        /// <param name="Target">non null pointer to location to write to </param>
        /// <param name="value">4 byte value to write</param>
        public static void Poke4(IntPtr Target, uint value)
        {
            MemoryNativeInternal.Poke4(Target, value);
        }

        /// <summary>
        /// For the <see cref="InsightProcess.DebugEventCallBackRoutine"/>.  Use the <see cref="IntPtr"/> passed containing the location of where to write the continue state.
        /// </summary>
        /// <param name="ContState">Pointer received as the ContState value in <seealso cref="InsightProcess.DebugEventCallBackRoutine"/> callback</param>
        /// <param name="Response">Indicate what your debugger did to handle it or not <see cref="DebugContState"/></param>
        /// <remarks>This is a thin wrapper round <see cref="Poke4"/></remarks> to be used in the <see cref="InsightProcess.DebugEventCallBackRoutine"/> so one does not need to typecast and use the unchecked keyboard
        public static void SetDebugEventCallbackResponse(IntPtr ContState, DebugContState Response)
        {
            MemoryNativeInternal.Poke4(ContState, (uint)Response);
        }

        /// <summary>
        /// read an 8 byte value from memory
        /// </summary>
        /// <param name="Target">non null local target pointer to read from</param>
        /// <returns>Returns memory contents OR 0 if you pass a null target</returns>
        public static ulong Peek8(IntPtr Target)
        {
            return MemoryNativeInternal.Peek8(Target);
        }

        /// <summary>
        ///  write a 8 byte value to memory
        /// </summary>
        /// <param name="Target">non null pointer to location to write to </param>
        /// <param name="val">8 byte value to right</param>
        /// <returns></returns>
        public static bool Poke8(IntPtr Target, ulong val)
        {
            return MemoryNativeInternal.Poke8(Target, val);
        }

        /// <summary>
        /// Write a 4 byte value to the process you indicate.
        /// </summary>
        /// <param name="ProcessHandle">  Native Handle to a process with at least  PROCESS_VM_WRITE and maybe PROCESS_VM_OPERATION </param>
        /// <param name="val">value to write</param>
        /// <param name="RemoteLocation">Virtual Memory in the remote process to write too. This is not a pointer to memory in your process.</param>
        /// <returns>returns true it if worked and false if it did not</returns>
        public static bool RemotePoke4(IntPtr ProcessHandle, uint val, IntPtr RemoteLocation)
        {
            return MemoryNativeInternal.RemotePoke4(ProcessHandle, val, RemoteLocation);
        }

        /// <summary>
        /// Write an 8 byte value to a process you specify
        /// </summary>
        /// <param name="ProcessHandle">Handle to the target</param>
        /// <param name="val">value to write</param>
        /// <param name="RemoteLocation">local in virtual memory of the target to write too</param>
        /// <returns>true if it worked and false if it did not</returns>
        public static bool RemotePoke8(IntPtr ProcessHandle, ulong val, IntPtr RemoteLocation)
        {
            return MemoryNativeInternal.RemotePoke8(ProcessHandle, val, RemoteLocation);
        }
    }
}
