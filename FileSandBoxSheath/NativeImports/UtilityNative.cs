using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBoxSheath.NativeImports
{
    internal static partial class NativeMethods
    {
        [DllImport("FileSandBoxApi.DLL", CallingConvention=CallingConvention.Winapi, EntryPoint ="UtilGetModuleNameViaHandle")]
        public static extern IntPtr GetModuleNameViaHandleInternal(IntPtr ProcessHandle, IntPtr ModuleHandle);
        
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilFreeModuleNameViaHandle")]
        public static extern IntPtr FreeModuleNameViaHandleInternal(IntPtr NativeStringPtr);

        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilGetFileNameViaHandle")]
        public static extern IntPtr GetFileNameViaHandle(IntPtr FileHandle);
        /// <summary>
        /// return a 4 dword value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "Peek4")]
        public static extern IntPtr Peek4(IntPtr NativePtr);

        /// <summary>
        /// return a 4 dword value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPoke4")]
        public static extern IntPtr Poke4(IntPtr NativePtr, uint NewValue);

        [DllImport("FileSandBoxApi.dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilOpenProcessForMemory")]
        public static extern IntPtr OpenProcessForMemoryAccess(uint ProcessID);

        [DllImport("FileSandBoxApi.dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilOpenProcesForQueryInformation")]
        public static extern IntPtr OpenProcesForQueryInformation(uint ProcessID);

        /// <summary>
        /// Free a memory chuck that was allocated by the native dll with a single call. It is recommanded to NOT call this unless you know the underlying
        /// block or structure has a single allocation. If multiple allocations happen, you'll be leaking memory.
        /// </summary>
        /// <param name="NativeStringPtr"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "RemoteRead_SimpleFree")]
        public static extern IntPtr SimpleFree(IntPtr NativeStringPtr);


        /// <summary>
        /// Wrapper for Kernel32.DLL's CloseHandle
        /// </summary>
        /// <param name="Handle"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", CallingConvention=CallingConvention.Winapi)]
        public static extern bool CloseHandle(IntPtr Handle);
    }
}
