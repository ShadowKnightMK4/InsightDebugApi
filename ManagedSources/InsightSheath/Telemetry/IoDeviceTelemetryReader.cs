using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Wrappers;
using InsightSheath.Remote;
using InsightSheath.Misc;
using System.Runtime.InteropServices;
using System.IO;
namespace InsightSheath.Telemetry
{

    /// <summary>
    /// Large structure for getting LARGE_INTEGER structs into .Net Hands
    /// </summary>
    [StructLayout(LayoutKind.Explicit)]
    public struct LARGE_INTEGER
    {
        
        [FieldOffset(0)]
        public UInt32 LowPart;
        [FieldOffset(4)]
        public Int32 HighPart;
        [FieldOffset(0)]
        public ulong QuadPart;
    }


    public class IoDeviceTelememtryExceptionCommonValues : GeneralTelemtryHelperStruct
    {
        public IoDeviceTelememtryExceptionCommonValues(uint ProcessId, uint ThreadID, IntPtr ForceHandle, IntPtr LastError, MachineType Type) : base(ProcessId, ThreadID, ForceHandle, LastError, Type)
        {

        }
        


        /// <summary>
        /// If you desire to make the call from the x86 process to CreateFileA/W file fail, call <see cref="SetForceHandle"/> with this has a value
        /// </summary>
        public static readonly uint InvalidHandleValue32 = (0xffffffff);
        /// <summary>
        /// If you desire to make the call from the x64 process to CreateFileA/W file fail, call <see cref="SetForceHandle"/> with this has a value
        /// </summary>
        public static readonly ulong InvalidHandleValue64 = (0xffffffffffffffff);

    }


    /// <summary>
    /// From once again winnt.h for values.   <see href="https://docs.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile"/>
    /// </summary>
    public enum NtCreationDisposition : uint 
    {
        /// <summary>
        /// IF File exists already, replace with this new file. If nonexistent, create it.
        /// </summary>
        Supersede = 0x00000000,
        /// <summary>
        /// If file exists, open it. If it does not exist, fail the request.
        /// </summary>
        Open = 0x00000001,
        /// <summary>
        /// If the file exists, fail request. If it does not exist, create the file.
        /// </summary>
        Create = 0x00000002,
        /// <summary>
        /// If the file exists open it. Otherwise create it.
        /// </summary>
        OpenIf = 0x00000003,
        /// <summary>
        /// If the file exists, open and truncate it. Otherwise fail request.
        /// </summary>
        Overwrite = 0x00000004,
        /// <summary>
        /// If the file exists, open and truncate it. If it does not exist, create it.
        /// </summary>
        OverwriteIf = 0x00000005,
        MaxDisposition = 0x00000005
    }


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
    /// from <see cref="https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-samr/262970b7-cd4a-41f4-8c4d-5a27f0092aaa"/>, 
    /// Windows SDK winnt.h and
    /// <see href="https://docs.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile"/>,
    /// these are the accessible flags 
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
        /// NtOpenFile/NtCreateFile DELETE Value from "winnt.h"
        /// </summary>
        Delete = 0x00010000,
        /// <summary>
        /// NtOpenFile/NtCreateFile READ_CONTROL value from "winnt.h"
        /// </summary>
        ReadControl = 0x00020000,
        /// <summary>
        /// Windows NtOpen/NtCreateFile Sourced from winnt.h 
        /// </summary>
        FileReadData = 0x1,
        /// <summary>
        /// Same as <see cref="FileReadData"/> in winnt.h
        /// </summary>
        FileListDirectory = FileReadData,
        /// <summary>
        /// Windows NtOpen/NtCreateFile.  Sourced from winnt.h.  FileAttribute flags, can be set/read.
        /// </summary>
        FileReadAttributes = 0x0080,
        /// <summary>
        /// Windows NtOpen/NtCreateFile Sourced from winnt.h 
        /// </summary>
        FileReadEa = 0x0008,
        /// <summary>
        /// Windows NtOpen/NtCreateFile Sourced from winnt.h 
        /// </summary>
        FileWriteData = 0x0002,
        /// <summary>
        /// Windows NtOpen/NtCreateFile Sourced from winnt.h 
        /// </summary>
        FileWriteAttributes = 0x0100,
        /// <summary>
        /// Windows NtOpen/NtCreateFile Sourced from winnt.h 
        /// </summary>
        FileWriteEa = 0x0010,
        /// <summary>
        /// Windows NtOpen/NtCreate file. From winnt.h. 
        /// </summary>
        FileAppendData = 0x0004,
        /// <summary>
        /// Windows NtOpen/NtCreate file. From winnt.h. 
        /// </summary>
        WriteDac = 0x00040000,
        /// <summary>
        /// Windows NtOpen/NtCreate file. From winnt.h. 
        /// </summary>
        WriteOwner = 0x00080000,
        /// <summary>
        /// Windows NtOpen/NtCreate file. From winnt.h. 
        /// </summary>
        Synchronize = 0x00100000,
        /// <summary>
        /// Windows NtOpen/NtCreate file. From winnt.h. 
        /// </summary>
        FileExecute = 0x0020,
        /// <summary>
        /// Same as FileExceute in winnt.h
        /// </summary>
        FileTraverse = FileExecute,

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

    public static class IoDeviceTelemetryExceptionExtensionsCheckers
    {
        /// <summary>
        /// Extension to <see cref="DebugEventExceptionInfo"/>, Returns if the contained exception <see cref="DebugEventExceptionInfo"/> is one generated via IoDeviceTelemetry DLL
        /// </summary>
        /// <param name="that">the this parameter or class to test</param>
        /// <returns> Returns if the contained exception <see cref="DebugEventExceptionInfo"/> is one generated via IoDeviceTelemetry DLL</returns>
        public static bool IsIoDeviceTelemetryException(this DebugEventExceptionInfo that)
        {
            if ((uint)that.ExceptionCode == IoDeviceTelemetryReaderExtensions.FixedExceptionCode)
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
                if ((uint)that.GetDebugEventExceptionInfo().ExceptionCode == IoDeviceTelemetryReaderExtensions.FixedExceptionCode)
                {
                    return true;
                }
            }
            return false;
        }

    }

    public class IoDeviceTelemetryNtCreateFile: IoDeviceTelememtryExceptionCommonValues
    {

        public IoDeviceTelemetryNtCreateFile(uint ProcessId, uint ThreadID, IntPtr ForceHandle, IntPtr LastError, MachineType Type): base(ProcessId, ThreadID, ForceHandle, LastError, Type)
        {

        }
        /// <summary>
        /// Pointer to the first argument in NtCreateFile. That will indicate the out handle. For x64 bit processes this will be a 64-bit pointer. For x32 bit processes, this will be a 32-bit pointer.
        /// </summary>
        public readonly IntPtr FileOutHandle;
        /// <summary>
        /// Will contain the passed Access Request flags for NtCreateFile.
        /// </summary>
        public AccessMasks DesiredAccess;
        /// <summary>
        /// May be null. If not null, this will be the Object Attributes read from that file.
        /// </summary>
        public Structs.WindowsObjectAttributes ObjectAttributes;
        /// <summary>
       /// for x64 debugged processes. This is a 64-bit pointer.  For x32-bit processes, this is a 32-bit pointer that may be pretended to be 64-bit depending on .NET
        /// </summary>
        public readonly IntPtr IoStatusBlock;
        /// <summary>
        /// a LARGE_INTEGER struct.
        /// </summary>
        public LARGE_INTEGER AllocationSize;
        /// <summary>
        /// The .NET file attributes for how the IO thing is being opened.
        /// </summary>
        public FileAttributes FileAttributes;
        /// <summary>
        /// The .NET shared mode.
        /// </summary>
        public FileShare ShareAccess;
        /// <summary>
        /// <see cref="https://docs.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile"/> for specifics. TODO: Include enum containing those values.
        /// </summary>
        public uint CreationOptions;
        /// <summary>
        /// NtCreateFile specific flags.
        /// </summary>
        public NtCreationDisposition CreateDisposition;
        /// <summary>
        /// for x64 debugged processes. This is a 64-bit pointer.  For x32-bit processes, this is a 32-bit pointer that may be pretended to be 64-bit depending on .NET
        /// </summary>
        public IntPtr EaBuffer;
        /// <summary>
        /// How many bytes is the EaBufffer.
        /// </summary>
        public ulong EaSize;

        /// <summary>
        /// Pointer to the handle to overwrite if the debugger wants to replace what the call was attempting to open.
        /// </summary>
        //public readonly IntPtr ForceHandle;
        /// <summary>
        /// Occupies the same spot as the last error in reading the EXCEPTION_ARGS struct.  Pointer to the RETURN VALUE to return from the NtCreateFile setting.
        /// </summary>
        //public readonly IntPtr ReturnValue;


        
        /// <summary>
        /// NtCreateFile does not appear to set last error based on documentation. Here Last Error is the return value given back to the debugged process.
        /// </summary>
        /// <param name="ReturnValue"></param>
        public void SetReturnValue(uint ReturnValue)
        {
            SetLastErrorValue(ReturnValue);
        }
    }

    

    /// <summary>
    /// This structure contains data from an exception generated via CreateFileA/W.
    /// </summary>
    public class IoDeviceTelemetyCreateFile: IoDeviceTelememtryExceptionCommonValues
    {
        public IoDeviceTelemetyCreateFile(uint dwProcess, uint dwThread, IntPtr HandlePtr, IntPtr ErrorPtr, MachineType Type): base(dwProcess, dwThread, HandlePtr, ErrorPtr, Type)
        {
            FileName = null;
            DesiredAccess = AccessMasks.NoAccess;
            SharedMode = FileShare.None;
            SecurityAttrib = IntPtr.Zero;
            CreateDisposition = 0;
            FlagsAndAttributes = 0;
            TemplateFile = IntPtr.Zero;
        }


        /// <summary>
        /// Name of the file the debugged process is trying to open
        /// </summary>
        public string FileName;
        /// <summary>
        /// Desired access for the target file
        /// </summary>
        public AccessMasks DesiredAccess;
        /// <summary>
        /// Will the debugged process try to open for sharing?
        /// </summary>
        public FileShare SharedMode;
        /// <summary>
        /// Debugged process 's requested security attributes (IMPORTANT! this points to virtual memory in **THAT** process, not yours.)
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
        /// Template file that's usually (but NOT always) null.
        /// </summary>
        public IntPtr TemplateFile;
        /// <summary>
        /// Pointer to a (x86 4 bytes) (x64 8 byte) sized byte block of memory that can be overwritten with a handle value (it will need to be duplicated into the debugged process) to force use of a different value. Default is 0.
        /// </summary>
        //public readonly IntPtr ForceHandle;

        /// <summary>
        /// contains pointer in the <see cref="dwProcessId"/> context pointing to memory on the stack that contra ins a value to set last error too one execution is resumed.
        /// </summary>
        //public  readonly IntPtr ForceLastError;

        

    }


    /// <summary>
    /// An Exception from the debug event struct that was generated via the IoException Telemetry DLL. This class should contain code to read exceptions generated from telemetry dll IoDeviceTelemetry
    /// </summary>
    public static class IoDeviceTelemetryReaderExtensions
    {
        const uint ExceptionArgType = 0;



        //const uint ExceptionSubType = 0;
        //const uint LastError_Ptr = 1;


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


        const uint NtCreateFile_ReturnHandle = 2;
        const uint NtCreateFile_DesiredAccess = 3;
        const uint NtCreateFile_ObjectAttributes = 4;
        const uint NtCreateFile_IoStatusBlock = 5;
        const uint NtCreateFile_AllocationSize = 6;
        const uint NtCreateFile_FileAttributs = 7;
        const uint NtCreateFile_ShareAccess = 8;
        const uint NtCreateFile_CreateDisposition = 9;
        const uint NtCreateFile_CreateOptions = 10;
        const uint NtCreateFile_EaBuffer = 11;
        const uint NtCreateFile_EaLength = 12;
        const uint NtCreateFile_OverwriteHandle = 13;


        /* if exception is NotificationType.NTCreateFIle*/
        /*
         * ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG)ReturnValue;
	ExceptionArgs[NTCF_AW_OUTPUTHANDLE] = (ULONG)FileHandle;
	ExceptionArgs[NTCT_AW_DESIRED_ACCESS] = (ULONG)DesiredAccess;
	ExceptionArgs[NTCT_AW_OBJECT_ATTRIBUTES] = (ULONG)ObjectAttributes;
	ExceptionArgs[NTCT_AW_IOSTATUSBLOCK] = (ULONG)IoStatusBlock;
	ExceptionArgs[NTCT_AW_ALLOCATION_SIZE] = (ULONG)AllocationSize;
	ExceptionArgs[NTCT_AW_FILEATRIBUTES] = (ULONG)FileAttributes;
	ExceptionArgs[NTCT_AW_SHARE_ACCESS] = (ULONG)ShareAccess;
	ExceptionArgs[NTCT_AW_CREATEOPTION] = (ULONG)CreateDisposition;
	ExceptionArgs[NTCT_AW_EABUFFER] = (ULONG)EaBuffer;
	ExceptionArgs[NTCT_AW_EALENGTH] = (ULONG)EaLength;
	ExceptionArgs[NTCT_AW_OVERRIDE_HANDLE] = (ULONG)OverwriteHandle;
         */



        /// <summary>
        /// This class in question will deal with exceptions of this value from Debugged apps that have had IoDeviceTelemetry loaded into to them.
        /// </summary>
        //public static readonly uint FixedExceptionCode = 0x68ACB7A9;
        // DEBUG ONLY x86 and x64 exception tracking down
        public static readonly uint FixedExceptionCode = 2;
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
            /// NTCreateFile sits below CreateFileA/W.  You'll likely see exceptions generated for both on the same target if debugged target is not calling the routine directly.
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
            return (NotificationType)that.ExceptionParameter64[GeneralTelemetry.ExceptionSubType];
        }

 
        /// <summary>
        /// This retrieves data generated from an exception that matches the IoDevice exception type and was triggered from <see cref="NotificationType.NtCreateFile"/>
        /// </summary>
        /// <param name="that">A <see cref="DebugEvent"/> or <see cref="DebugEventExceptionInfo"/> that contains an exception generated from the telemetry dll for NtCreateFile</param>
        /// <returns></returns>
        public static IoDeviceTelemetryNtCreateFile GetNtCreateFileSettings(this DebugEventExceptionInfo that)
        {
            IoDeviceTelemetryNtCreateFile ret;
            MachineType Type;
            IntPtr Handle = HelperRoutines.OpenProcessForVirtualMemory(that.ProcessID);
            var arguments = that.ExceptionParameter64;
            if (that.IsEventFrom32BitProcess)
            {
                Type = MachineType.MachineI386;
            }
            else
            {
                Type = MachineType.MachineAmd64;
            }
            try
            {
                ret = new IoDeviceTelemetryNtCreateFile(that.ProcessID, that.ThreadID, new IntPtr((long)arguments[NtCreateFile_ReturnHandle]), new IntPtr((long)arguments[GeneralTelemetry.LastError_Ptr]), Type) ;
//                ret.ReturnValue = new IntPtr((long)arguments[LastError_Ptr]);
  //              ret.FileOutHandle = new IntPtr((long)arguments[NtCreateFile_ReturnHandle]);
                ret.DesiredAccess = (AccessMasks) arguments[NtCreateFile_DesiredAccess];
                if (arguments[NtCreateFile_ObjectAttributes] == 0)
                {
                    ret.ObjectAttributes = null;
                }
                else
                {
                    //ret.ObjectAttributes = new Structs.WindowsObjectAttributes(new IntPtr((long)arguments[NtCreateFile_ObjectAttributes]));
                    if (arguments[NtCreateFile_ObjectAttributes] != 0)
                    {
                        ret.ObjectAttributes = RemoteStructure.RemoteReadObjectAttributes(Handle, new IntPtr((long)arguments[NtCreateFile_ObjectAttributes]), that.IsEventFrom32BitProcess, true);
                    }
                    else
                    {
                        ret.ObjectAttributes = null; 
                    }
                    
                }
                if (arguments[NtCreateFile_AllocationSize] != 0)
                {
                    ret.AllocationSize = Marshal.PtrToStructure<LARGE_INTEGER>(new IntPtr((long)arguments[NtCreateFile_AllocationSize]));
                }
                else
                {
                    ret.AllocationSize = new LARGE_INTEGER();
                }
                ret.FileAttributes = (FileAttributes) arguments[NtCreateFile_FileAttributs];
                ret.ShareAccess = (FileShare) arguments[NtCreateFile_ShareAccess];
                
                ret.CreateDisposition = (NtCreationDisposition) arguments[NtCreateFile_CreateDisposition];
                ret.CreationOptions = (uint)arguments[NtCreateFile_CreateOptions];
                ret.EaBuffer = new IntPtr((long)arguments[NtCreateFile_EaBuffer]);
                ret.EaSize = arguments[NtCreateFile_EaLength];
                //ret.ForceHandle = new IntPtr((long)arguments[NtCreateFile_OverwriteHandle]);
                return ret;
            }
            finally
            {
                HelperRoutines.CloseHandle(Handle);
            }
        }

        /// <summary>
        /// Only valid if <see cref="GetIoDeviceExceptionType(DebugEventExceptionInfo)"/> returns an instead of <see cref="NotificationType.CreateFile"/> containing the arguments passed to CreateFileA or W.
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        public static IoDeviceTelemetyCreateFile GetCreateFileSettings(this DebugEventExceptionInfo that)
        {
            MachineType type;
            IoDeviceTelemetyCreateFile ret;
            var Arguments = that.ExceptionParameter64;
            IntPtr Handle = HelperRoutines.OpenProcessForVirtualMemory(that.ProcessID);

            try
            {
                if (that.IsEventFrom32BitProcess)
                {
                    type = MachineType.MachineI386;
                }
                else
                {
                    type = MachineType.MachineAmd64;
                }
                ret = new IoDeviceTelemetyCreateFile(that.ProcessID, that.ThreadID, (IntPtr)Arguments[CreateFile_OvrridePtr], (IntPtr)Arguments[GeneralTelemetry.LastError_Ptr], type)
                {
                    FileName = RemoteStructure.RemoteReadString(Handle, new IntPtr((long)Arguments[CreateFile_FilenamePtr]), Arguments[CreateFile_FileNameCharLen]),
                    DesiredAccess = (AccessMasks)Arguments[CreateFile_DesiredAccess],
                    SharedMode = (FileShare)Arguments[CreateFile_ShareMode],
                    SecurityAttrib = new IntPtr((long)Arguments[CreateFile_SecurityPtr]),
                    CreateDisposition = (CreationDisposition)Arguments[CreateFile_CreationDisposition],
                    FlagsAndAttributes = (uint)Arguments[CreateFile_FlagsAndAttribs],
                    TemplateFile = new IntPtr((long)Arguments[CreateFile_TemplateFile])
                };


            }
            finally
            {
                HelperRoutines.CloseHandle(Handle);
            }
            return ret;
        }

    }
}
