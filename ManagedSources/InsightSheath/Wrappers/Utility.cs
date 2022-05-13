﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using InsightSheath.NativeImports;

namespace InsightSheath
{
    /// <summary>
    /// Machine Type values extractable 
    /// </summary>
    public enum MachineType
    {
        /// <summary>
        /// Returned on error with <see cref="HelperRoutines.GetPEMachineType(string)"/> 
        /// </summary>
        MachineInvalid = 0,
        /// <summary>
        /// x86 based 
        /// </summary>
        MachineI386 = 0x014c,
        /// <summary>
        /// Intel Itanium based
        /// </summary>
        MachineIA64 = 0x0200,
        /// <summary>
        /// x64 based
        /// </summary>
        MachineAmd64 = 0x8664
    }
    

    /// <summary>
    /// Class contains various miscellaneous routines that while not tied directly to the library's purpose, they are used throughout the Sheath and wrappers.
    /// </summary>
    public static class HelperRoutines
    {

        public static MachineType GetPEMachineType(string TargetExe)
        {
            return NativeMethods.GetPEMachineType(TargetExe);
        }
        /// <summary>
        /// Read a 4 byte value from unmanaged memory
        /// </summary>
        /// <param name="Target">non null pointer to location to read from</param>
        /// <returns>return contains of passed memory location</returns>
        public static uint Peek4(IntPtr Target)
        {
            return NativeMethods.Peek4(Target);
        }

        /// <summary>
        /// Write 4 bytes to the memory location passed
        /// </summary>
        /// <param name="Target">non null pointer to location to write to </param>
        /// <param name="value">4 byte value to write</param>
        public static void Poke4(IntPtr Target, uint value)
        {
            NativeMethods.Poke4(Target, value);
        }

        /// <summary>
        /// read an 8 byte value from memory
        /// </summary>
        /// <param name="Target">non null local target pointer to read from</param>
        /// <returns>Returns memory contents OR 0 if you pass a null target</returns>
        public static ulong Peek8(IntPtr Target)
        {
            return NativeMethods.Peek8(Target);
        }

        /// <summary>
        ///  write a 8 byte value to memory
        /// </summary>
        /// <param name="Target">non null pointer to location to write to </param>
        /// <param name="val">8 byte value to right</param>
        /// <returns></returns>
        public static bool Poke8(IntPtr Target, ulong val)
        {
            return NativeMethods.Poke8(Target, val);
        }

        /// <summary>
        /// Duplicate a handle from the current process into the target process.
        /// </summary>
        /// <param name="CurrentHandle">handle located in current process</param>
        /// <param name="Access">Choose your access parameters</param>
        /// <param name="CopyAccess">set to true to ignore <see cref="Access"/> and take the current permssions from the handle for the new one too</param>
        /// <param name="TargetProcess">Target remote process to pick from</param>
        /// <param name="AllowInherit">Allow spawned processes to inherit</param>
        /// <returns></returns>
        public static IntPtr DuplicateHandleToRemote(IntPtr CurrentHandle, uint Access, bool CopyAccess, IntPtr TargetProcess, bool AllowInherit)
        {
            return NativeMethods.DuplicateHandleIntoTarget(CurrentHandle, Access, CopyAccess, TargetProcess, AllowInherit);
        }
        /// <summary>
        /// Get a Process's name via its handle.Handle needs PROCESSS_QUERY_INFOMRATION and VM_READ rights.
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
        /// <param name="ProcessHandle">Process Handle to retrieve name from</param>
        /// <returns></returns>
        public static string GetModuleNameViaHandle(IntPtr ProcessHandle)
        {
            return GetModuleNameViaHandle(ProcessHandle, IntPtr.Zero);
        }

        /// <summary>
        /// Get the module name (or the process name) of the process whose handle you have
        /// </summary>
        /// <param name="ProcessHandle">Process Handle to source data from</param>
        /// <param name="HModule">if IntPTr.zero then <see cref="GetProcessNameViaHandle(IntPtr)"/> otherwise retrieved the name for specified hmodule</param>
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
        /// Get a file name via a Native Handle. 
        /// </summary>
        /// <param name="FileHandle">Win32 Handle in question</param>
        /// <returns>returns either null or a string containing the name of fie file/IO device</returns>
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
                NativeMethods.FreeFileNameViaHandleInternal(retPtr);
                return ret;
            }
        }
        /// <summary>
        /// Open a process for Virtual Memory reading and writing
        /// </summary>
        /// <param name="ProcessId"></param>
        /// <returns>Returns a handle with PROCESS_VM_READ | PROCESS_VM_RIGHT access to the passed process (or 0)</returns>

        public static IntPtr OpenProcessForVirtualMemory(uint ProcessId)
        {
            return NativeMethods.OpenProcessForMemoryAccess(ProcessId);
        }

        /// <summary>
        /// Open a process for Querying Some information about it (requests  PROCESS_QUERY_INFORMATION )
        /// </summary>
        /// <param name="ProcessId"></param>
        /// <returns></returns>
        public static IntPtr OpenProcessForQueryInformation(uint ProcessId)
        {
            return NativeMethods.OpenProcesForQueryInformation(ProcessId);
        }

        /// <summary>
        /// Open a process for duplicating and inserting handles of resources you open into a process (PROCESS_DUP_HANDLE | PROCESS_VM_WRITE | PROCESS_VM_OPERATION) 
        /// </summary>
        /// <param name="ProcessID"></param>
        /// <returns></returns>
        public static IntPtr OpenProcessForHandleDuplicating(uint ProcessID)
        {
            return NativeMethods.OpenProcessForDuplicatingHandle(ProcessID);
        }


        /// <summary>
        /// Some of the routines here return Native Handles. This wraps 
        /// () from kernel32 for convenience  
        /// </summary>
        /// <param name="Handle"></param>
        /// <returns></returns>
        public static bool CloseHandle(IntPtr Handle)
        {
            return NativeMethods.CloseHandle(Handle);
        }
    }
}
