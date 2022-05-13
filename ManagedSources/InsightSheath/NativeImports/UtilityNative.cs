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
        /// Retrieve a potentially null pointer containing a Unicode string containing the name of the hmodule that belongs to process handle. You will need to free it by using <see cref="FreeModuleNameViaHandleInternal(IntPtr)"/>
        /// </summary>
        /// <param name="ProcessHandle">Win32 Process Handle to use. It must have at least PROCESS_QUERY_INFROMATION or PROCESS_QUERY_LIMITED_INFORMATION access right.</param>
        /// <param name="ModuleHandle">HMODULe in question<param>
        /// <returns>returns an Unicode string containing the name of the module if the call worked and IntPtr.Zero if it failed</returns>
        [DllImport("InsightApi.Dll", CallingConvention=CallingConvention.Winapi, EntryPoint ="UtilGetModuleNameViaHandle")]
        public static extern IntPtr GetModuleNameViaHandleInternal(IntPtr ProcessHandle, IntPtr ModuleHandle);

        /// <summary>
        /// Free a memory pointer returned by <see cref="GetModuleNameViaHandleInternal(IntPtr, IntPtr)"/>
        /// </summary>
        /// <param name="NativeStringPtr">pointer in question. null is ignored.</param>
        /// <returns>Returns true if the call worked and false if it failed.</returns>
        /// <remarks>This imports UtilFreeModuleNameViaHandle() which itself points to a routine called RemoteRead_SimpleFree() which itself is a wrapper to free().  You are free to directly just call RemoteRead_SimpleFree() but if the implemention changes, UtilFreeModuleNameViaHandle may not always be a forwarder to RemoteRead_SimpleFree  </remarks>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilFreeModuleNameViaHandle")]
        public static extern bool FreeModuleNameViaHandleInternal(IntPtr NativeStringPtr);

        /// <summary>
        /// Get a Unicode string containing the name of the file/device pointed to by FileHandle. Caller will need to free it.
        /// </summary>
        /// <param name="FileHandle">Win32 Handle in question</param>
        /// <returns>Returns IntPtr containing a null terminated Unicode string or null if it failed.</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilGetFileNameViaHandle")]
        public static extern IntPtr GetFileNameViaHandle(IntPtr FileHandle);


        /// <summary>
        /// Free a memory pointer returned by <see cref="GetFileNameViaHandle(IntPtr)(IntPtr, IntPtr)"/>
        /// </summary>
        /// <param name="NativeStringPtr">pointer in question. null is ignored.</param>
        /// <returns>Returns true if the call worked and false if it failed.</returns>
        /// <remarks>This imports UtilFreeFileNameViaHandle() which itself points to a routine called RemoteRead_SimpleFree() which itself is a wrapper to free().  You are free to directly just call RemoteRead_SimpleFree() but if the implemention changes, UtilFreeFileNameViaHandle may not always be a forwarder to RemoteRead_SimpleFree  </remarks>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilFreeFileNameViaHandle")]
        public static extern bool FreeFileNameViaHandleInternal(IntPtr NativeStringPtr);

        /// <summary>
        /// return a 4 DWORD value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to read from</param>
        /// <returns>contents of that location OR 0 if you pass <see cref="IntPtr.Zero"/> as the <see cref="NativePtr"/> value</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPeek4")]
        public static extern uint Peek4(IntPtr NativePtr);
        //UtilDuplicateHandleIntoTarget

        /// <summary>
        /// Duplicate the handle into the passed process. Handle is assumed to be from the current process. Will still need a way to get to remote process.
        /// </summary>
        /// <param name="CurrentHandle"></param>
        /// <param name="Access"></param>
        /// <param name="CopyAccess"></param>
        /// <param name="TargetProcess"></param>
        /// <param name="Inherit"></param>
        /// <returns></returns>
        [DllImport("InsightApi.dll", EntryPoint = "UtilDuplicateHandleIntoTarget", CallingConvention= CallingConvention.Winapi,SetLastError =true)]
        public static extern IntPtr DuplicateHandleIntoTarget(IntPtr CurrentHandle, UInt32 Access, bool CopyAccess, IntPtr TargetProcess, bool Inherit);
        /// <summary>
        /// Write a 4 byte DWORD value to an unmanaged local memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to write to</param>
        /// <param name="NewValue">4 byte value to write</param>
        /// <returns>true if the value was set and false if it was not (or if <see cref="NativePtr"/> equals 0)</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPoke4")]
        public static extern IntPtr Poke4(IntPtr NativePtr, uint NewValue);


        /// <summary>
        /// return an 8 byte QWORD value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to read from</param>
        /// <returns>contents of that location OR 0 if you pass <see cref="IntPtr.Zero"/> as the <see cref="NativePtr"/> value</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPeek8")]
        public static extern ulong Peek8(IntPtr NativePtr);

        /// <summary>
        /// write a 8 byte QWORD value to a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to write to</param>
        /// <returns>true if the value was set and false if it was not (or if <see cref="NativePtr"/> equals 0)</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPoke8")]
        public static extern bool Poke8(IntPtr NativePtr, ulong NewValue);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilGetPEMachineTypeW")]
        public static extern MachineType GetPEMachineType([MarshalAs(UnmanagedType.LPWStr)]string Str);
        
        /// <summary>
        /// Short hand for asking for a native process handle that PROCESS_VM_READ | PROCESS_VM_WRITE access
        /// </summary>
        /// <param name="ProcessID">id of the target process to open.  You can get this with <see cref="InsightSheath.Wrappers.DebugEventCreateProcessInfo"/> event handling among other things</param>
        /// <returns>Returns Native handle with PROCESS_VM_READ + PROCESS_VM_WRITE access or null if you can't open it.</returns>

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilOpenProcessForMemory")]
        public static extern IntPtr OpenProcessForMemoryAccess(uint ProcessID);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilOpenProcesForQueryInformation")]
        public static extern IntPtr OpenProcesForQueryInformation(uint ProcessID);

        
        /// <summary>
        /// Open Process ALL_ACCESS
        /// </summary>
        /// <param name="ProcessID"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "OpenProcessNow")]
        public static extern IntPtr OpenProcessNow(uint ProcessID);
        /// <summary>
        /// Shorthand for asking for a native process handle that has PROCESS_DUP_HANDLE AND VM_PROCESS_WRITE (for WriteProcessMemory)
        /// </summary>
        /// <param name="ProcessID"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilOpenProcessForDuplicatingHandle")]
        public static extern IntPtr OpenProcessForDuplicatingHandle(uint ProcessID);

        /// <summary>
        /// Free a memory chuck that was allocated by the native DLL with a single call. It is recommended to NOT call this unless you know the underlying
        /// block or structure was allocated via C's malloc(). If multiple allocations happen, you'll be leaking memory by calling this.
        /// </summary>
        /// <param name="NativeStringPtr"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "RemoteRead_SimpleFree")]
        public static extern IntPtr SimpleFree(IntPtr NativeStringPtr);


        /// <summary>
        /// Wrapper for Kernel32.DLL's CloseHandle for user convenience 
        /// </summary>
        /// <param name="Handle"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", CallingConvention=CallingConvention.Winapi)]
        public static extern bool CloseHandle(IntPtr Handle);
    }
}
