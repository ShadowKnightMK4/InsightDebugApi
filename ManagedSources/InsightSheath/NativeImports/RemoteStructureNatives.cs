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

        //RemoteReadStringW
        


        /// <summary>
        /// Read a string from a remote process
        /// </summary>
        /// <param name="ProcessHandle">Handle for the remote process. It should be open for at least PROCESS_VM_READ rights.</param>
        /// <param name="RemoteLocation">virtual memory in the remote process to read from</param>
        /// <param name="char_count">how many chars in the string to read? </param>
        /// <returns></returns>
        [DllImport("InsightApi.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError =true, EntryPoint = "RemotePoke4")]
        public static extern bool RemotePoke4(IntPtr ProcessHandle, uint value , ulong remote_location);

        /// <summary>
        /// Read a string from a remote process
        /// </summary>
        /// <param name="ProcessHandle">Handle for the remote process. It should be open for at least PROCESS_VM_READ rights.</param>
        /// <param name="RemoteLocation">virtual memory in the remote process to read from</param>
        /// <param name="char_count">how many chars in the string to read? </param>
        /// <returns></returns>
        [DllImport("InsightApi.dll",CallingConvention= CallingConvention.Winapi, CharSet= CharSet.Unicode, EntryPoint = "RemoteReadStringW")]
        public static extern IntPtr RemoteReadStringInternal(IntPtr ProcessHandle, IntPtr RemoteLocation, uint char_count);
        /// <summary>
        /// WWraps RemoteReadDebugString.  Return value is a string that needs to be freed afterwares.
        /// </summary>
        /// <param name="ProcessHandle">Handle to the remote process. It should be open for at lead PROCESS_VM_READ rights</param>
        /// <param name="DebugEventPtr"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteReadDebugString")]
        public static extern IntPtr RemoteReadDebugStringInternal(IntPtr ProcessHandle, IntPtr DebugEventPtr);

        /// <summary>
        /// Imports RemoteFreeDEbugString() from the dll. Currently that forwards to "RemoteRead_SimpleFree" in the code. 
        /// If the implementation of RemoteReadDebugString changes, RemoteRead_SimpleFree may no longer free properly. USE RemoteFreeDebugString()
        /// <param name="Memory">Pointer to a IntPtr returned by<see cref=">RemoteReadDebugStringInternl"/></param>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteFreeDebugString")]
        public static extern void RemoteFreeDebugStringInternal(IntPtr Memory);

        // 	VOID* WINAPI RemoteReadPointer(HANDLE Process, VOID* Target, DWORD pointerSize)

        /// <summary>
        /// Read a pointer (4 or 8 bytes) from the target process's virtual memory location/
        /// </summary>
        /// <param name="TargetProcess">0 = self.  </param>
        /// <param name="TargetLocation">Virtual memory to read from</param>
        /// <param name="PointerSize">must be 4 or 8 otherwise call will be rejected.</param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteReadPointer")]
        public static extern IntPtr RemoteReadPointer(IntPtr TargetProcess, IntPtr TargetLocation, uint PointerSize);
    }
}
