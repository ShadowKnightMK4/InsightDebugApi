using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Debugging;
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

        /// <summary>
        /// Maps the passed DLL/EXE file into our memory and fetches the IMAGE_NT_HEADER->FileHeader.Machine value before finishing. Recommended to call once and save results.
        /// </summary>
        /// <param name="Target">Unicode String pointing to the file to check.</param>
        /// <returns>Returns the Machine entry for the struct as told at https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-image_file_header. If you get something other than those or get zero, something went wrong.  </returns>
        /// <remarks>This does involve calling the ImageNtHeader DebugHelp() without taking care to sync calls. TODO: Add that. </remarks>
        public static MachineType GetPEMachineType(string TargetExe)
        {
            return NativeMethods.GetPEMachineType(TargetExe);
        }

        /// <summary>
        /// Duplicate a handle from the current process into the target process.
        /// </summary>
        /// <param name="CurrentHandle">handle located in current process</param>
        /// <param name="Access">Choose your access parameters</param>
        /// <param name="CopyAccess">set to true to ignore <see cref="Access"/> and take the current permissions from the handle for the new one too</param>
        /// <param name="TargetProcess">Target remote process to pick from</param>
        /// <param name="AllowInherit">Allow spawned processes to inherit</param>
        /// <returns></returns>
        public static IntPtr DuplicateHandleToRemote(IntPtr CurrentHandle, uint Access, bool CopyAccess, IntPtr TargetProcess, bool AllowInherit)
        {
            return NativeMethods.DuplicateHandleIntoTarget(CurrentHandle, Access, CopyAccess, TargetProcess, AllowInherit);
        }

        /// <summary>
        /// Copy a handle
        /// </summary>
        /// <param name="ProcessID"></param>
        /// <param name="SourceHandle"></param>
        /// <returns></returns>
        public static IntPtr CopyHandleFromRemote(uint ProcessID, IntPtr SourceHandle)
        {
            IntPtr HandleOfProcess = IntPtr.Zero;
            IntPtr ret = IntPtr.Zero;
            try
            {
                HandleOfProcess = HelperRoutines.OpenProcessForHandleDuplicating(ProcessID);
                if (HandleOfProcess !=  IntPtr.Zero)
                {
                    ret = CopyHandleFromRemote(HandleOfProcess, SourceHandle, 0, true, false);
                    
                }
            }
            finally
            {
                if (HandleOfProcess != IntPtr.Zero)
                    NativeMethods.CloseHandle(HandleOfProcess);
            }
            return ret;
        }
        /// <summary>
        /// Copy a Handle from a remote process to you. Don't forget to close it.
        /// </summary>
        /// <param name="SourceProcess">Process to copy from</param>
        /// <param name="TargetHandle">by value handle to copy to your process to access the same resource</param>
        /// <param name="Access">Choose your access parameters</param>
        /// <param name="CopyAccess">set to true to ignore <see cref="Access"/> and take the current permissions from the handle for the new one too</param>
        /// <param name="AllowInherit">Allow spawned processes to inherit</param>
        /// <returns></returns>
        public static IntPtr CopyHandleFromRemote(IntPtr SourceProcess, IntPtr TargetHandle, uint Access, bool CopyAccess, bool AllowInherit)
        {
            return NativeMethods.DuplicateHandleFromRemote(SourceProcess, TargetHandle, Access, CopyAccess, AllowInherit);
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
        /// Some of the routines here return Native Handles. This wraps <see href="https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle"/> aka CloseHandle() 
        ///  from kernel32 for convenience  
        /// </summary>
        /// <param name="Handle">Handle to close.</param>
        /// <returns>True if it worked and false if it did not.</returns>
        /// <remarks>When running under an attached debugger, an exception can trigger if the debugged process tries closing an invalid handle. <see cref="DebugExceptionTypes.InvalidHandleCloseException"/></remarks>
        /// <exception cref="DebugExceptionTypes.InvalidHandleCloseException"> This may trigger CloseHandle is asked to close an invalid handle in software attached to a debugger</exception>
        public static bool CloseHandle(IntPtr Handle)
        {
            return NativeMethods.CloseHandle(Handle);
        }
    }
}
