using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using FileSandBoxSheath.NativeImports;

namespace FileSandBoxSheath
{
    public static class HelperRoutines
    {

        /// <summary>
        /// Forward of NativeMethods.peek4
        /// </summary>
        /// <param name="Target"></param>
        /// <returns></returns>
        public static IntPtr Peek4(IntPtr Target)
        {
            return NativeMethods.Peek4(Target);
        }

        /// <summary>
        /// Forwrd of NativeMethods.poke4
        /// </summary>
        /// <param name="Target"></param>
        /// <param name="value"></param>
        public static void Poke4(IntPtr Target, UInt32 value)
        {
            NativeMethods.Poke4(Target, value);
        }

        
        /// <summary>
        /// Get a Process's name via its handle.   Handle needs PROCESSS_QUERY_INFOMRATION and VM_READ rights.
        /// </summary>
        /// <param name="ProcessHandle"></param>
        /// <returns></returns>
        public static string GetProcessNameViaHandle(IntPtr ProcessHandle)
        {
            return GetModuleNameViaHandle(ProcessHandle, IntPtr.Zero);
        }
        /// <summary>
        /// Same as <see cref="GetProcessNameViaHandle(IntPtr)"/>
        /// </summary>
        /// <param name="ProcessHandle"></param>
        /// <returns></returns>
        public static string GetModuleNameViaHandle(IntPtr ProcessHandle)
        {
            return GetModuleNameViaHandle(ProcessHandle, IntPtr.Zero);
        }

        /// <summary>
        /// Get the module name (or the process name) of the process whose handle you have
        /// </summary>
        /// <param name="ProcessHandle"></param>
        /// <param name="HModule">uf IntPTr.zero then <see cref="GetProcessNameViaHandle(IntPtr)"/> otherwise retrieved the name for specified hmodule</param>
        /// <returns></returns>
        public static string GetModuleNameViaHandle(IntPtr ProcessHandle, IntPtr HModule)
        {
            IntPtr retPtr = NativeMethods.GetModuleNameViaHandleInternal(ProcessHandle, HModule );
            if (retPtr == IntPtr.Zero)
            {
                return null;
            }
            else
            {
                string ret = Marshal.PtrToStringUni(retPtr);
                NativeMethods.FreeModuleNameViaHandleInternal(retPtr);
                return ret;
            }
        }

        /// <summary>
        /// Get a file name via a Handle. 
        /// </summary>
        /// <param name="FileHandle"></param>
        /// <returns></returns>
        public static string GetFileNameViaHandle(IntPtr FileHandle)
        {
            IntPtr retPtr = NativeMethods.GetFileNameViaHandle(FileHandle);
            if (retPtr == IntPtr.Zero)
            {
                return null;
            }
            else
            {
                string ret = Marshal.PtrToStringUni(retPtr);
                NativeMethods.FreeModuleNameViaHandleInternal(retPtr);
                return ret;
            }
        }
        /// <summary>
        /// Open a process for Virtual Memmory reading and writing
        /// </summary>
        /// <param name="ProcessId"></param>
        /// <returns>Returns a hadnle with VM_READ | VM_RIGHT access to the passed process (or 0)</returns>

        public static IntPtr OpenProcessForVirtualMemory(uint ProcessId)
        {
            return NativeMethods.OpenProcessForMemoryAccess(ProcessId);
        }

        /// <summary>
        /// Open a process for Querying Some Indformeciton about it.
        /// </summary>
        /// <param name="ProcessId"></param>
        /// <returns></returns>
        public static IntPtr OpenProcessForQueryInformation(uint ProcessId)
        {
            return NativeMethods.OpenProcesForQueryInformation(ProcessId);
        }


        /// <summary>
        /// Some of the routines here return Native Handles.    This wrapps CloseHandle() for kernel 32 for covinenece 
        /// </summary>
        /// <param name="Handle"></param>
        /// <returns></returns>
        public static bool CloseHandle(IntPtr Handle)
        {
            return NativeMethods.CloseHandle(Handle);
        }
    }
}
