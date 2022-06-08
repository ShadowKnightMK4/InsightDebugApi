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
    ///  Read and Write to local unmanaged memory. A lot of the wrapper routines use this and you will be needing this for your Debug Event processor callback. <see cref="InsightProcess.DebugEventCallBackRoutine"/> unless you cut out the middle man and use the <see cref="DebugEventWorkerThreadSupport"> extensions.
    /// </summary>
    public static class LocalUnmanagedMemory
    {
        /// <summary>
        /// Read a 4 byte value from unmanaged memory
        /// </summary>
        /// <param name="Target">non null pointer to location to read from</param>
        /// <returns>return contains of passed memory location</returns>
        public static uint Peek4(IntPtr Target)
        {
            return MemoryNative.Peek4(Target);
        }

        /// <summary>
        /// Write 4 bytes to the memory location passed
        /// </summary>
        /// <param name="Target">non null pointer to location to write to </param>
        /// <param name="value">4 byte value to write</param>
        public static void Poke4(IntPtr Target, uint value)
        {
            MemoryNative.Poke4(Target, value);
        }

        /// <summary>
        /// For the <see cref="InsightProcess.DebugEventCallBackRoutine"/>.  Use the <see cref="IntPtr"/> passed containing the location of where to write the continue state.
        /// </summary>
        /// <param name="ContState">Pointer received as the ContState value in <seealso cref="InsightProcess.DebugEventCallBackRoutine"/> callback</param>
        /// <param name="Response">Indicate what your debugger did to handle it or not <see cref="DebugContState"/></param>
        /// <remarks>This is a thin wrapper round <see cref="Poke4"/></remarks> to be used in the <see cref="InsightProcess.DebugEventCallBackRoutine"/> so one does not need to typecast and use the unchecked keyboard
        public static void SetDebugEventCallbackResponse(IntPtr ContState, DebugContState Response)
        {
            MemoryNative.Poke4(ContState, (uint)Response);
        }

        /// <summary>
        /// read an 8 byte value from memory
        /// </summary>
        /// <param name="Target">non null local target pointer to read from</param>
        /// <returns>Returns memory contents OR 0 if you pass a null target</returns>
        public static ulong Peek8(IntPtr Target)
        {
            return MemoryNative.Peek8(Target);
        }

        /// <summary>
        ///  write a 8 byte value to memory
        /// </summary>
        /// <param name="Target">non null pointer to location to write to </param>
        /// <param name="val">8 byte value to right</param>
        /// <returns></returns>
        public static bool Poke8(IntPtr Target, ulong val)
        {
            return MemoryNative.Poke8(Target, val);
        }
    }
}
