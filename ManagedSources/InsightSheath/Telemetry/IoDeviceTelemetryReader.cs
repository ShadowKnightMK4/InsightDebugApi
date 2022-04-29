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

    /// <summary>
    /// Action to be taken by CreateFileA/W
    /// </summary>
    public enum CreationDisposition: uint
    { 
        /// <summary>
        /// Makes a new file if said target does not already exists
        /// </summary>
        CreateNew = 1,
        /// <summary>
        /// Creates fil and truncates if file exists already, createfilea/w sets last error if truncating
        /// </summary>
        CreateAlways = 2,
        /// <summary>
        /// open an existing file / device and fail if not found
        /// </summary>
        OpenExisting = 3,
        /// <summary>
        /// open existing file / - attempts to create if not found
        /// </summary>
        OpenAlways = 4,
        /// <summary>
        /// Open a file and truncate it to zero bytes long
        /// </summary>
        TruncateExisting = 5
    }

    [Flags]
    /// <summary>
    /// from <see cref="https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-samr/262970b7-cd4a-41f4-8c4d-5a27f0092aaa"/>, these are the accessible flags
    /// </summary>
    public enum AccessMasks: uint
    {
        /// <summary>
        /// MSDN CreateFileA/W suggests meta data can be queried with opening for no access, hence the pair of these.
        /// </summary>
        NoAccess = 0,
        /// <summary>
        /// MSDN CreateFileA/W suggests meta data can be queried with opening for no access, hence the pair of these.
        /// </summary>
        MetaData = NoAccess,
        /// <summary>
        /// GENERIC_READ / for read access
        /// </summary>
        GenericRead = 0x80000000,
        /// <summary>
        /// GENERIC_WRITE / for write access
        /// </summary>
        GenericWrite = 0x40000000,
        /// <summary>
        /// GENERIC_EXECUTE / for execute access
        /// </summary>
        GenericExecute = 0x20000000,
        /// <summary>
        /// Generic all / for all possible access
        /// </summary>
        GenericAll = 0x10000000
    }

    /// <summary>
    /// Share mode for CreateFileA/W
    /// </summary>
    [Flags]
    public enum ShareMasks: uint
    {
        /// <summary>
        /// Don't wanna share
        /// </summary>
        NoShare = 0,
        /// <summary>
        /// Share for delete requests only
        /// </summary>
        ShareDelete = 0x00000004,
        /// <summary>
        /// Share for read access only
        /// </summary>
        ShareRead = 0x00000001,
        /// <summary>
        /// Share for write access only
        /// </summary>
        ShareWrite = 0x00000002
    }

    public static class TelemetryExceptionExtensionsCheckers
    {
        /// <summary>
        /// Extension to <see cref="DebugEventExceptionInfo"/>, Returns if the contained exception <see cref="DebugEventExceptionInfo"/> is one generated via IoDeviceTelemetry DLL
        /// </summary>
        /// <param name="that">the this parameter or class to test</param>
        /// <returns> Returns if the contained exception <see cref="DebugEventExceptionInfo"/> is one generated via IoDeviceTelemetry DLL</returns>
        public static bool IsIoDeviceTelemetryException(this DebugEventExceptionInfo that)
        {
            if (that.ExceptionCode == IoDeviceTelemetryReaderExtensions.FixedExceptionCode)
            {
                return true;
            }
            return false;
        }
        /// <summary>
        /// Extension to <see cref="DebugEvent"/>, Returns if the contained exception <see cref="DebugEvent"/> is one generated via IoDeviceTelemetry DLL
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
        public IoDeviceTelemetyCreateFIle(uint dwProcess, uint dwThread, IntPtr HandlePtr, IntPtr ErrorPtr)
        {
            FileName = null;
            DesiredAccess = AccessMasks.NoAccess;
            SharedMode = ShareMasks.NoShare;
            SecurityAttrib = IntPtr.Zero;
            CreateDisposition = 0;
            FlagsAndAttributes = 0;
            TemplateFile = 0;
            ForceHandle = HandlePtr;
            ForceLastError = ErrorPtr;
            ProcessId = dwProcess;
            ThreadId = dwThread;
        }
        public readonly uint ProcessId;
        public readonly uint ThreadId;
        /// <summary>
        /// Name of the file the debugged process is trying to open
        /// </summary>
        public string FileName;
        /// <summary>
        /// Desired access for the target file
        /// </summary>
        public AccessMasks DesiredAccess;
        /// <summary>
        /// Will the debugged app open for sharing?
        /// </summary>
        public ShareMasks SharedMode;
        /// <summary>
        /// Debugged app's requested security attributes (IMPORTANT this points to virtual memory in *that* process, not yours.)
        /// </summary>
        public IntPtr SecurityAttrib;
        /// <summary>
        /// Creation disposition
        /// </summary>
        public CreationDisposition CreateDisposition;
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
        public readonly IntPtr ForceHandle;


         static readonly uint InvalidHandleValue = (0xffffffff);
        /// <summary>
        /// Duplicate this handle back to the process (uses) <see cref="ForceHandle"/>
        /// </summary>
        /// <param name="ReplacementHandle"></param>
        public void SetForceHandle(IntPtr ReplacementHandle)
        {
            //IntPtr handle = NativeImports.NativeMethods.OpenProcessNow(dwProcessId);
            IntPtr handle = HelperRoutines.OpenProcessForHandleDuplicating(ProcessId);
            {
                if ((uint)ReplacementHandle.ToInt32() != InvalidHandleValue)
                {
                    IntPtr duphandle = NativeImports.NativeMethods.DuplicateHandleIntoTarget(ReplacementHandle, 0, true, handle, true);
                    RemoteStructure.RemotePoke4(handle, (uint)duphandle.ToInt32(), this.ForceHandle);
                }
                else
                {
                    RemoteStructure.RemotePoke4(handle, (uint)InvalidHandleValue, this.ForceHandle);
                }
                
                
            }
            HelperRoutines.CloseHandle(handle);
        }

        
        /// <summary>
        /// contains pointer in the <see cref="dwProcessId"/> context pointing to memory on the stack that contra ins a value to set last error too one execution is resumed.
        /// </summary>
        public  readonly IntPtr ForceLastError;

        public void SetLastErroValue(uint NewValue)
        {
            IntPtr handle = NativeImports.NativeMethods.OpenProcessNow(ProcessId);
            //IntPtr Handle = HelperRoutines.OpenProcessForVirtualMemory(dwProcessId);
            {
                RemoteStructure.RemotePoke4(handle, NewValue, ForceLastError);
            }
            HelperRoutines.CloseHandle(handle);

        }

    }


    /// <summary>
    /// An Exception from the debug event struct that was generated via the IoException Telemetry DLL. This class should contain code to read exceptions generated from telemetry dll IoDeviceTelemetry
    /// </summary>
    public static class IoDeviceTelemetryReaderExtensions
    {
        static readonly uint ExceptionArgType = 0;
        const uint ExceptionSubType = 0;
        const uint CreateFile_LastErrorPtr = 1;


        /* if exception is NotificationType.CreateFile */
        const uint CreateFile_FilenamePtr = 2;
        const uint CreateFile_FileNameCharLen = 3;
        const uint CreateFile_DesiredAccess = 4;
        const uint CreateFile_ShareMode = 5;
        const uint CreateFile_SecurityPtr = 6;
        const uint CreateFile_CreationDisposition = 7;
        const uint CreateFile_FlagsAndAttribs = 8;
        const uint CreateFile_TemplateFile = 9;
        const uint CreateFile_OvrridePtr = 10;
        

        

       /// <summary>
       /// This class in question will deal with exceptions of this value from Debugged apps that have had IoDeviceTelemetry loaded into to them.
       /// </summary>
        public static readonly uint FixedExceptionCode = 0x68ACB7A9;
        public enum NotificationType
        {
            /// <summary>
            /// Exception is from either a CreateFileA or CreateFileW call. Use the <see cref="GetCreateFileSettings"/> routine to retrieve formatted specifics
            /// </summary>
            CreateFile = 1,
            /// <summary>
            /// Exception is either from a CreateFIleTransactedA or CreateFIleTransactedW call.
            /// </summary>
            CreateFileTransacted = 2,
            /// <summary>
            /// Exception from a CloseHandle call. 
            /// </summary>
            CloseHandle = 3,
            /// <summary>
            /// TODO:: A lower level NtCreateFIle call
            /// </summary>
            NtCreateFile = 4,
            /// <summary>
            /// TODO:: A Lower Level NtOpen file call.
            /// </summary>
            NtOpenFile =5
        }

        /// <summary>
        /// Return the type of Alert this exception is raised from.  
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        public static NotificationType GetIoDeviceExceptionType(this DebugEventExceptionInfo that)
        {
            return (NotificationType)that.ExceptionParameter64[ExceptionSubType];
        }

        /// <summary>
        /// Only valid if <see cref="GetIoDeviceExceptionType(DebugEventExceptionInfo)"/> returns an instead of <see cref="NotificationType.CreateFile"/> containing the arguments passed to CreateFileA or W.
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        public static IoDeviceTelemetyCreateFIle GetCreateFileSettings(this DebugEventExceptionInfo that)
        {
            IoDeviceTelemetyCreateFIle ret;
            var Arguments = that.ExceptionParameter32;
            IntPtr Handle = HelperRoutines.OpenProcessForVirtualMemory(that.ProcessID);

            try
            {
                ret = new IoDeviceTelemetyCreateFIle(that.ProcessID, that.ThreadID, (IntPtr)Arguments[CreateFile_OvrridePtr], (IntPtr)Arguments[CreateFile_LastErrorPtr]);
                
                ret.FileName = RemoteStructure.RemoteReadString(Handle, new IntPtr(Arguments[CreateFile_FilenamePtr]), Arguments[CreateFile_FileNameCharLen]);
                ret.DesiredAccess = (AccessMasks)Arguments[CreateFile_DesiredAccess];
                ret.SharedMode = (ShareMasks)Arguments[CreateFile_ShareMode];
                ret.SecurityAttrib = new IntPtr(Arguments[CreateFile_SecurityPtr]);
                ret.CreateDisposition = (CreationDisposition)Arguments[CreateFile_CreationDisposition];
                ret.FlagsAndAttributes = Arguments[CreateFile_FlagsAndAttribs];
                ret.TemplateFile = Arguments[CreateFile_TemplateFile];
                

            }
            finally
            {
                HelperRoutines.CloseHandle(Handle);
            }
            return ret;
        }

    }
}
