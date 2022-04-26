using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Wrappers;
using InsightSheath.Remote;
using InsightSheath.Misc;
using System.Runtime.InteropServices;

namespace InsightSheath.Telemetry
{
    public static class TelemetryExceptionExtensionsCheckers
    {
        /// <summary>
        /// Extension to <see cref="DebugEventExceptionInfo"/>, returns true if the exception is from the iodevicetelemetry dll (and able to be cast to it)
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        public static bool IsIoDeviceTelemetryException(this DebugEventExceptionInfo that)
        {
            if (that.ExceptionCode == IoDeviceTelemetryReaderExtensions.FixedExceptionCode)
            {
                return true;
            }
            return false;
        }
        /// <summary>
        /// Extension for <see cref="DebugEvent"/> returns true if event is an exception from iodevicetelemtry dll and able to be cast too it.
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        public static bool IsIoDeviceTelemetryException(this DebugEvent that)
        {
            if (that.EventType == DebugEventType.ExceptionEvent)
            {
                if (that.GetDebugEventExceptionInfo().ExceptionCode == IoDeviceTelemetryReaderExtensions.FixedExceptionCode)
                {
                    return true;
                }
            }
            return false;
        }

    }

    public struct IoDeviceTelemetyCreateFIle
    {
        public readonly uint dwProcessId;
        public readonly uint dwThreadId;
        /// <summary>
        /// Name of the file the debugged process is trying to open
        /// </summary>
        public string FileName;
        /// <summary>
        /// Desired access for the target file
        /// </summary>
        public uint DesiredAccess;
        /// <summary>
        /// Will the debugged app open for sharing?
        /// </summary>
        public uint SharedMode;
        /// <summary>
        /// Debugged app's requested security attributes (IMPORTANT this points to virtual memory in *that* process, not yours.)
        /// </summary>
        public IntPtr SecurityAttrib;
        /// <summary>
        /// Creation disposition
        /// </summary>
        public uint CreateDisposition;
        /// <summary>
        /// Flags and attributes
        /// </summary>
        public uint FlagsAndAttributes;
        /// <summary>
        /// Template file that's always null
        /// </summary>
        public uint TemplateFile;
        /// <summary>
        /// Pointer to a 4 byte block of memory that can be overwritten with a handle value (it will need to be duplicated into the debugged process) to force use of a different value. Default is 0.
        /// </summary>
        public IntPtr ForceHandle;

        /// <summary>
        /// Duplicate this handle back to the process (uses) <see cref="ForceHandle"/>
        /// </summary>
        /// <param name="ReplacementHandle"></param>
        public void SetForceHandle(IntPtr ReplacementHandle)
        {
            IntPtr handle = HelperRoutines.OpenProcessForVirtualMemory(dwProcessId);
            {
                RemoteStructure.RemotePoke4(handle, (uint)ReplacementHandle.ToInt32(), this.ForceHandle);
            }
            HelperRoutines.CloseHandle(handle);
        }

        
        /// <summary>
        /// contains pointer in the <see cref="dwProcessId"/> context pointing to memory on the stack that contra ins a value to set last error too one execution is resumed.
        /// </summary>
        public  IntPtr ForceLastError;

        public void SetLastErroValue(uint NewValue)
        {
            IntPtr Handle = HelperRoutines.OpenProcessForVirtualMemory(dwProcessId);
            {
                RemoteStructure.RemotePoke4(Handle, NewValue, ForceLastError);
            }
            HelperRoutines.CloseHandle(Handle);

        }

    }


    /// <summary>
    /// An Exception from the debug event struct that was generated via the IoException Telemetry DLL. This class should contain code to read exceptions generated from telemetry dll IoDeviceTelemetry
    /// </summary>
    public static class IoDeviceTelemetryReaderExtensions
    {
        static readonly uint ExceptionArgType = 0;
        static readonly uint ExceptionSubType = 0;
        static readonly uint CreateFile_LastErrorPtr = 1;
        /* if exception is NotificationType.CreateFile */
        static readonly uint CreateFile_FilenamePtr = 2;
        static readonly uint CreateFile_FileNameCharLen = 3;
        static readonly uint CreateFile_DesiredAccess = 4;
        static readonly uint CreateFile_ShareMode = 5;
        static readonly uint CreateFile_SecurityPtr = 6;
        static readonly uint CreateFile_CreationDisposition = 7;
        static readonly uint CreateFile_FlagsAndAttribs = 8;
        static readonly uint CreateFile_TemplateFile = 9;
        static readonly uint CreateFile_OvrridePtr = 10;
        

        

        /* if ExceptionSubtype == CreateFile*/ 
        //static readonly uint 
       /// <summary>
       /// This class in question will deal with exceptions of this value from IoDeviceTelemetry 
       /// </summary>
        public static readonly uint FixedExceptionCode = 0x68ACB7A9;
        public enum NotificationType
        {
            /// <summary>
            /// Exception is from either a CreateFileA or CreateFileW call
            /// </summary>
            CreateFile = 1,
            /// <summary>
            /// Exception is either from a CreateFIleTransactedA or CreateFIleTransactedW call
            /// </summary>
            CreateFileTransacted = 2,
            /// <summary>
            /// Exception from a CloseHandle cal
            /// </summary>
            CloseHandle = 3,
            /// <summary>
            /// A lower level NtCreateFIle call
            /// </summary>
            NtCreateFile = 4,
            /// <summary>
            /// A Lower Level NtOpen file call.
            /// </summary>
            NtOpenFile =5
        }

        public static NotificationType GetIoDeviceExceptionType(this DebugEventExceptionInfo that)
        {
            return (NotificationType)that.ExceptionParameter64[ExceptionSubType];
        }

        /// <summary>
        /// Only valid if <see cref="GetIoDeviceExceptionType(DebugEventExceptionInfo)"/> returns <see cref="NotificationType.CreateFile"/>
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        public static IoDeviceTelemetyCreateFIle GetCreateFileSettings(this DebugEventExceptionInfo that)
        {
            var ret = new IoDeviceTelemetyCreateFIle();
            var Arguments = that.ExceptionParameter32;
            var Handle = HelperRoutines.OpenProcessForVirtualMemory(that.ProcessID);
            {
                ret.FileName = RemoteStructure.RemoteReadString(Handle, new IntPtr(Arguments[CreateFile_FilenamePtr]), Arguments[CreateFile_FileNameCharLen]);
                ret.DesiredAccess = Arguments[CreateFile_DesiredAccess];
                ret.SharedMode = Arguments[CreateFile_ShareMode];
                ret.SecurityAttrib = new IntPtr(Arguments[CreateFile_SecurityPtr]);
                ret.CreateDisposition = Arguments[CreateFile_CreationDisposition];
                ret.FlagsAndAttributes = Arguments[CreateFile_FlagsAndAttribs];
                ret.TemplateFile = Arguments[CreateFile_TemplateFile];
                ret.ForceHandle = new IntPtr(Arguments[CreateFile_OvrridePtr]);
                ret.ForceLastError = new IntPtr(Arguments[CreateFile_LastErrorPtr]);

            }
            HelperRoutines.CloseHandle(Handle);
            return ret;
        }

    }
}
