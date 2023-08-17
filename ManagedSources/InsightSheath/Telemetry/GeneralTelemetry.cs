using InsightSheath.Debugging;
using InsightSheath.Win32Struct.Remote;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Telemetry
{
    /// <summary>
    /// holds some common defines between the included telemetry projects.
    /// </summary>
    internal static class GeneralTelemetry
    {
        /// <summary>
        /// The exception that the telemetry generated should use the first arg to indicate what routine it has packeged for the debugger
        /// </summary>
        public const uint ExceptionSubType = 0;
        /// <summary>
        /// The exception that the telemetry generated should also place a DWORD* int this location to let the debugger overwrite the last error (or for certrain nt functions, the return value)
        /// </summary>
        public const uint LastError_Ptr = 1;
    }

    /// <summary>
    /// General common values/code between the telemetry reader code.
    /// </summary>
    public abstract class GeneralTelemtryHelperStruct
    {
        public GeneralTelemtryHelperStruct(uint ProcessId, uint ThreadID, IntPtr ForceHandle, IntPtr LastError, MachineType Type)
        {
            this.ProcessId = ProcessId;
            this.ThreadId = ThreadID;
            this.ForceHandlePtr = ForceHandle;
            this.LastErrorPtr = LastError;
            this.Type = Type;
        }
        /// <summary>
        /// Depends on what <see cref="DebugEvent.IsEventFrom32BitProcess"/> returns. Intended for determining pointer size only. If that's true, Value is <see cref="MachineType.MachineI386"/> otherwise value is <see cref="MachineType.MachineAmd64"/>  i386 pointer sizes are 4 bytes long and Amd64 pointer sizes are 8 bytes.    This may not actually be the exact machine type as stored in the debugged process's physical PE/EXE file.
        /// </summary>
        public readonly MachineType Type;

        /// <summary>
        /// Process that the exception originated from.
        /// </summary>
        public readonly uint ProcessId;
        /// <summary>
        /// Thread that the exception originated from.
        /// </summary>
        public readonly uint ThreadId;
        /// <summary>
        /// Pointer to the handle that is returned back to the target process after handling the exception.
        /// </summary>
        public readonly IntPtr ForceHandlePtr;
        /// <summary>
        /// Depending on Context of target detoured routine, may be an NTSTATUS return value or an actual last error pointer.
        /// </summary>
        public readonly IntPtr LastErrorPtr;


        /// <summary>
        /// Duplicate the handle you provide into the process the exception was generated from and write it to the memory location specified by <see cref="ForceHandle"/> 
        /// </summary>
        /// <param name="ReplacementHandle"></param>
        public void SetForceHandle(IntPtr ReplacementHandle)
        {
            //IntPtr handle = NativeImports.NativeMethods.OpenProcessNow(dwProcessId);
            IntPtr handle = HelperRoutines.OpenProcessForHandleDuplicating(ProcessId);
            try
            {
                /* Caution. This code is size Dependant on knowing the target's handle size*/
                if (Type == MachineType.MachineI386) /* 4 byte pointer / handle size*/
                {
                    //if ((uint)ReplacementHandle.ToInt32() != InvalidHandleValue32)
                    if ((uint)ReplacementHandle.ToInt32() != Int32.MaxValue)
                    {
                        IntPtr duphandle = NativeImports.NativeMethods.DuplicateHandleIntoTarget(ReplacementHandle, 0, true, handle, true);
                        MemoryNative.RemotePoke4(handle, (uint)duphandle.ToInt32(), ForceHandlePtr);
                    }
                    else
                    {
                        //MemoryNative.RemotePoke4(handle, InvalidHandleValue32, ForceHandlePtr);
                        MemoryNative.RemotePoke4(handle, Int32.MaxValue, ForceHandlePtr);
                    }
                }
                else /* 8 byte pointer / handle size */
                {
                    //if ((ulong)ReplacementHandle.ToInt64() != InvalidHandleValue64)
                    if ((ulong)ReplacementHandle.ToInt64() != Int64.MaxValue)
                    {
                        IntPtr duphandle = NativeImports.NativeMethods.DuplicateHandleIntoTarget(ReplacementHandle, 0, true, handle, true);
                        MemoryNative.RemotePoke8(handle, (ulong)duphandle.ToInt64(), ForceHandlePtr);
                    }
                    else
                    {
                        //MemoryNative.RemotePoke8(handle, InvalidHandleValue64, ForceHandlePtr);
                        MemoryNative.RemotePoke8(handle, int.MaxValue, ForceHandlePtr);
                    }
                }


            }
            finally
            {
                HelperRoutines.CloseHandle(handle);
            }
        }


        /// <summary>
        /// Set the handle to the appropriate InvalidHandleValue based on the <see cref="Type"/> value in this struct.
        /// </summary>
        public void SetForceHandle()
        {
            if (Type == MachineType.MachineI386)
            {
                SetForceHandle(0xffffffff);
            }
            else
            {
                SetForceHandle(0xffffffffffffffff);
            }
        }
        /// <summary>
        /// Set the 64-bit handle value to something other than an <see cref="IntPtr"/> - for example <see cref="Telemetry.InvalidHandleValue32"/>
        /// </summary>
        /// <param name="HandleValue">Set what HANDLE value will be passed back to the debugged app via the telemetry dll</param>
        /// <remarks>Note HANDLE Values in windows are 4 bytes for x86 software and 8 bytes for x64 bit software</remarks>
        public void SetForceHandle(ulong HandleValue)
        {
            IntPtr handle = HelperRoutines.OpenProcessForHandleDuplicating(ProcessId);
            try
            {
                //if (HandleValue != InvalidHandleValue64)
                if (HandleValue != ulong.MaxValue)
                {
                    IntPtr duphandle = NativeImports.NativeMethods.DuplicateHandleIntoTarget(new IntPtr((long)HandleValue), 0, true, handle, true);
                    MemoryNative.RemotePoke8(handle, (ulong)duphandle.ToInt64(), ForceHandlePtr);
                }
                else
                {
                    //MemoryNative.RemotePoke8(handle, InvalidHandleValue64, ForceHandlePtr);
                    MemoryNative.RemotePoke8(handle, ulong.MaxValue, ForceHandlePtr);
                }


            }
            finally
            {
                HelperRoutines.CloseHandle(handle);
            }
        }


        /// <summary>
        /// Set the 32-bit handle value to something other than an <see cref="IntPtr"/> - for example <see cref="Telemetry.InvalidHandleValue32"/>
        /// </summary>
        /// <param name="HandleValue">Set what HANDLE value will be passed back to the debugged app via the telemetry dll</param>
        /// <remarks>Note HANDLE Values in windows are 4 bytes for x86 software and 8 bytes for x64 bit software</remarks>
        public void SetForceHandle(uint HandleValue)
        {
            IntPtr handle = HelperRoutines.OpenProcessForHandleDuplicating(ProcessId);
            try
            {
                //if (HandleValue != InvalidHandleValue32)
                if (HandleValue != Int32.MaxValue)
                {
                    IntPtr duphandle = NativeImports.NativeMethods.DuplicateHandleIntoTarget(new IntPtr(HandleValue), 0, true, handle, true);
                    MemoryNative.RemotePoke4(handle, (uint)duphandle.ToInt32(), ForceHandlePtr);
                }
                else
                {
                    //MemoryNative.RemotePoke4(handle, InvalidHandleValue32, ForceHandlePtr);
                    MemoryNative.RemotePoke4(handle, Int32.MaxValue, ForceHandlePtr);
                }


            }
            finally
            {
                HelperRoutines.CloseHandle(handle);
            }
        }




        /// <summary>
        /// Set the last error value that will be set by the detouring routine when returning control to the debugged process.
        /// </summary>
        /// <param name="NewValue">Set what value the telemetry dll will pass to <see href="https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-setlasterror"/> </param>
        public void SetLastErrorValue(uint NewValue)
        {
            IntPtr handle = NativeImports.NativeMethods.OpenProcessForMemoryAccess(ProcessId);
            try
            {
                MemoryNative.RemotePoke4(handle, NewValue, LastErrorPtr);
            }
            finally
            {
                HelperRoutines.CloseHandle(handle);
            }
        }


    }

}
