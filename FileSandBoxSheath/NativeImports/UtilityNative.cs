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
        [DllImport("FileSandBoxApi.DLL", CallingConvention=CallingConvention.Winapi, EntryPoint ="UtilGetModuleNameViaHandle")]
        public static extern IntPtr GetModuleNameViaHandleInternal(IntPtr ProcessHandle, IntPtr ModuleHandle);
        
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilFreeModuleNameViaHandle")]
        public static extern IntPtr FreeModuleNameViaHandleInternal(IntPtr NativeStringPtr);

        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilGetFileNameViaHandle")]
        public static extern IntPtr GetFileNameViaHandle(IntPtr FileHandle);
        /// <summary>
        /// return a 4 DWORD value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to read from</param>
        /// <returns>contents of that location OR 0 if you pass <see cref="IntPtr.Zero"/> as the <see cref="NativePtr"/> value</returns>
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPeek4")]
        public static extern uint Peek4(IntPtr NativePtr);

        /// <summary>
        /// Write a 4 byte DWORD value to an unmanaged local memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to write to</param>
        /// <param name="NewValue">4 byte value to write</param>
        /// <returns>true if the value was set and false if it was not (or if <see cref="NativePtr"/> equals 0)</returns>
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPoke4")]
        public static extern IntPtr Poke4(IntPtr NativePtr, uint NewValue);


        /// <summary>
        /// return an 8 byte QWORD value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to read from</param>
        /// <returns>contents of that location OR 0 if you pass <see cref="IntPtr.Zero"/> as the <see cref="NativePtr"/> value</returns>
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPeek8")]
        public static extern ulong Peek8(IntPtr NativePtr);

        /// <summary>
        /// write a 8 byte QWORD value to a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to write to</param>
        /// <returns>true if the value was set and false if it was not (or if <see cref="NativePtr"/> equals 0)</returns>
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPoke9")]
        public static extern bool Poke8(IntPtr NativePtr, ulong NewValue);


        /// <summary>
        /// Short hand for asking for a native process handle that PROCESS_VM_READ | PROCESS_VM_WRITE access
        /// </summary>
        /// <param name="ProcessID">id of the target process to open.  You can get this with <see cref="InsightSheath.Wrappers.DebugEventCreateProcessInfo"/> event handling among other things</param>
        /// <returns>Returns Native handle with PROCESS_VM_READ + PROCESS_VM_WRITE access or null if you can't open it.</returns>

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
