using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.NativeImports
{
    internal static partial class NativeMethods
    {
        /// <summary>
        /// WWraps RemoteReadDebugString.  Return value is a string that needs to be freed afterwares.
        /// </summary>
        /// <param name="ProcessHandle"></param>
        /// <param name="DebugEventPtr"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteReadDebugString")]
        public static extern IntPtr RemoteReadDebugStringInternal(IntPtr ProcessHandle, IntPtr DebugEventPtr);

        /// <summary>
        /// Imports RemoteFreeDEbugString() from the dll. Currently that forwards to "RemoteRead_SimpleFree" in the code. 
        /// If the implementation of RemoteReadDebugString changes, RemoteRead_SimpleFree may no longer free properly. USE RemoteFreeDebugString()
        /// <param name="Memory">Pointer to a IntPtr returned by<see cref=">RemoteReadDebugStringInternl"/></param>
        [DllImport("FileSandBoxApi", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteFreeDebugString")]
        public static extern void RemoteFreeDebugStringInternal(IntPtr Memory);

        // 	VOID* WINAPI RemoteReadPointer(HANDLE Process, VOID* Target, DWORD pointerSize)

        /// <summary>
        /// Read a pointer (4 or 8 bytes) from the target process's virtual memory location/
        /// </summary>
        /// <param name="TargetProcess">0 = self.  </param>
        /// <param name="TargetLocation">Virtual memory to read from</param>
        /// <param name="PointerSize">must be 4 or 8 otherwise call will be rejected.</param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteReadPointer")]
        public static extern IntPtr RemoteReadPointer(IntPtr TargetProcess, IntPtr TargetLocation, uint PointerSize);
    }
}
