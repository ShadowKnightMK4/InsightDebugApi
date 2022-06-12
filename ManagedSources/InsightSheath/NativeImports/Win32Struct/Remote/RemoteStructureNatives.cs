using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Win32Struct;
using InsightSheath.Win32Struct.Remote;
namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Internal Class. This imports the routines used in <see cref="RemoteStructure"/> class exports
    /// </summary>
    static class RemoteStructureInternal
    {
        /// <summary>
        /// Take case as the back end will promote x86 Unicode string to x64 bit to make sure we don't accidentally cut off a pointer.
        /// </summary>
        /// <param name="ProcessHandle">Process to read from</param>
        /// <param name="RemoteLocation">Location in the target to read from/</param>
        /// <param name="TargetIs32Bit">Indicate if we're reading from a <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/></param>
        /// <returns>Return local allocated Memory containing an <see cref="UnicodeString64"/></returns>
        [DllImport("InsightApi.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "RemoteReadUnicodeString")]
        public static extern IntPtr RemoteReadUnicodeString(IntPtr ProcessHandle, IntPtr RemoteLocation, bool TargetIs32Bit);

    
        /// <summary>
        /// We uses too care if the pointer was a unicode32-bit or unicode
        /// </summary>
        /// <param name="RemoteLocation"></param>
        /// <param name="TargetIs32Bit"></param>
        /// <returns></returns>
        [DllImport("InsightApi.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "RemoteFreeUnicodeString")]
        public static extern IntPtr RemoteFreeUnicodeString(IntPtr RemoteLocation, bool TargetIs32Bit);

        /// <summary>
        /// Calling <see cref="RemoteFreeUnicodeString(IntPtr, bool)"/> with the correct parameter.
        /// </summary>
        /// <param name="RemoteLocation"></param>
        /// <returns></returns>
        public static IntPtr RemoteFreeUnicodeString(IntPtr RemoteLocation)
        {
            return RemoteFreeUnicodeString(RemoteLocation, false);
        }


        [DllImport("InsightApi.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "RemoteReadObjectAttributes")]
        public static extern IntPtr RemoteReadObjectAttributes(IntPtr ProcessHandle, IntPtr RemoteLocation, bool TargetIs32Bit);

        [DllImport("InsightApi.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "RemoteFreeObjectAttributes")]
        public static extern bool RemoteFreeObjectAttributes(IntPtr RemoteLocation, bool TargetIs32Bit);




        /// <summary>
        /// Read a string from a remote process
        /// </summary>
        /// <param name="ProcessHandle">Handle for the remote process. It should be open for at least PROCESS_VM_READ rights.</param>
        /// <param name="RemoteLocation">virtual memory in the remote process to read from</param>
        /// <param name="char_count">how many chars in the string to read? </param>
        /// <returns></returns>
        [DllImport("InsightApi.dll",CallingConvention= CallingConvention.Winapi, CharSet= CharSet.Unicode, EntryPoint = "RemoteReadStringW", SetLastError =true)]
        public static extern IntPtr RemoteReadStringInternal(IntPtr ProcessHandle, IntPtr RemoteLocation, ulong char_count);
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
