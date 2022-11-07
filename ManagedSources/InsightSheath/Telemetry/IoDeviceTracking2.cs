using InsightSheath.Debugging;
using InsightSheath.Win32Struct;
using InsightSheath.Win32Struct.Remote;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Telemetry
{
    public class IoDeviceTelemetryNtCreateFile2 : IoDeviceTelememtryExceptionCommonValues
    {

        public IoDeviceTelemetryNtCreateFile2(uint ProcessId, uint ThreadID, IntPtr ForceHandle, IntPtr LastError, MachineType Type) : base(ProcessId, ThreadID, ForceHandle, LastError, Type)
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
        public WindowsObjectAttributes ObjectAttributes;
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
        /// <see href="https://docs.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile"/> for specifics. TODO: Include enum containing those values.
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
    public class IoDeviceTelemetyCreateFile2 : IoDeviceTelememtryExceptionCommonValues
    {
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


        public IoDeviceTelemetyCreateFile2(uint dwProcess, uint dwThread, IntPtr HandlePtr, IntPtr ErrorPtr, MachineType Type) : base(dwProcess, dwThread, HandlePtr, ErrorPtr, Type)
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


    public static class TelemetryIoDeviceTracking
    {
        /// <summary>
        /// This static class registeres with <see cref="Telemetry"/> to handle exceptions of this value.
        /// </summary>
        public const uint ExceptionCode = 0x68ACB7A9;

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


        public enum SupportedNotificationType
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
            /// Exception from a CloseHandle call. (Other than the one generated by the routine itself when debugging and closing invalid handle)
            /// </summary>
            CloseHandle = 3,
            /// <summary>
            /// NTCreateFile sits below CreateFileA/W.  You'll likely see exceptions generated for both on the same target if debugged target is not calling the routine directly.
            /// </summary>
            NtCreateFile = 4,
            /// <summary>
            /// TODO:: A Lower Level NtOpen file call.
            /// </summary>
            NtOpenFile = 5
        }
        static TelemetryIoDeviceTracking()
        {
            Telemetry.AddHandler(ExceptionCode, Builder);
        }

        /// <summary>
        /// Trigger the static constructor
        /// </summary>
        public static void Touch()
        {

        }
        /// <summary>
        /// examine the exception code, its arguments and return an instance of an approproate handler we support it. 
        /// </summary>
        /// <param name="Info"></param>
        /// <returns></returns>
        private static object Builder(DebugEventExceptionInfo Info)
        {
            IntPtr ProcessHandle = IntPtr.Zero;
            MachineType type;
            
            object ret = null;
            if ( (Info != null)    && (Info.EventType == DebugEventType.ExceptionEvent) && (Info.ExceptionCode_as_int == ExceptionCode))
            {
                if (Info.IsEventFrom32BitProcess)
                {
                    type = MachineType.MachineI386;
                }
                else
                {
                    type = MachineType.MachineAmd64;
                }
                var Args = Info.ExceptionParameter64;
                if (Args.Length == 15) // the IoDeviceTelemetry will always return 15 args.
                {
                    try
                    {
                        switch ((SupportedNotificationType)Args[GeneralTelemetry.ExceptionSubType])
                        {
                            case SupportedNotificationType.CreateFile:
                                var createfileret = new IoDeviceTelemetyCreateFile2(Info.ProcessID, Info.ThreadID, (IntPtr)Args[CreateFile_OvrridePtr], (IntPtr)Args[GeneralTelemetry.LastError_Ptr], type);
                                createfileret.FileName = RemoteStructure.RemoteReadString(ProcessHandle, new IntPtr((long)Args[CreateFile_FilenamePtr]), Args[CreateFile_FileNameCharLen]);
                                createfileret.DesiredAccess = (AccessMasks)Args[CreateFile_DesiredAccess];
                                createfileret.SharedMode = (FileShare)Args[CreateFile_ShareMode];
                                createfileret.SecurityAttrib = new IntPtr((long)Args[CreateFile_CreationDisposition]);
                                createfileret.CreateDisposition = (CreationDisposition)Args[CreateFile_CreationDisposition];
                                createfileret.FlagsAndAttributes = (uint)Args[CreateFile_FlagsAndAttribs];
                                createfileret.TemplateFile = new IntPtr((long)Args[CreateFile_TemplateFile]);
                                ret = createfileret;
                                break;
                            case SupportedNotificationType.NtCreateFile:
                                var ntcreatefileret = new IoDeviceTelemetryNtCreateFile2(Info.ProcessID, Info.ThreadID, new IntPtr((long)Args[NtCreateFile_ReturnHandle]), new IntPtr((long)Args[GeneralTelemetry.LastError_Ptr]), type);
                                ntcreatefileret.DesiredAccess = (AccessMasks)Args[NtCreateFile_DesiredAccess];
                                if (Args[NtCreateFile_ObjectAttributes] == 0)
                                {
                                    ntcreatefileret.ObjectAttributes = null;
                                }
                                else
                                {
                                    if (Args[NtCreateFile_ObjectAttributes] != 0)
                                    {
                                        ntcreatefileret.ObjectAttributes = RemoteStructure.RemoteReadObjectAttributes(ProcessHandle, new IntPtr((long)Args[NtCreateFile_ObjectAttributes]), Info.IsEventFrom32BitProcess, true);
                                    }
                                    else
                                    {
                                        ntcreatefileret.ObjectAttributes = null;
                                    }
                                }

                                if (Args[NtCreateFile_AllocationSize] != 0)
                                {
                                    ntcreatefileret.AllocationSize = Marshal.PtrToStructure<LARGE_INTEGER>(new IntPtr((long)Args[NtCreateFile_AllocationSize]));
                                }
                                else
                                {
                                    ntcreatefileret.AllocationSize = new LARGE_INTEGER();
                                }

                                ntcreatefileret.FileAttributes = (FileAttributes)Args[NtCreateFile_FileAttributs];
                                ntcreatefileret.ShareAccess = (FileShare)Args[NtCreateFile_ShareAccess];
                                ntcreatefileret.CreateDisposition = (NtCreationDisposition)Args[NtCreateFile_CreateDisposition];
                                ntcreatefileret.CreationOptions = (uint)Args[NtCreateFile_CreateOptions];
                                ntcreatefileret.EaBuffer = new IntPtr((long)Args[NtCreateFile_EaBuffer]);
                                ntcreatefileret.EaSize = Args[NtCreateFile_EaLength];
                                ret = ntcreatefileret;
                                break;
                            case SupportedNotificationType.NtOpenFile:
                                break;
                            case SupportedNotificationType.CloseHandle:
                                break;
                            case SupportedNotificationType.CreateFileTransacted:
                                break;
                        }
                    }
                    finally
                    {
                        if ( (ProcessHandle != IntPtr.Zero) )
                        {
                            HelperRoutines.CloseHandle(ProcessHandle);
                        }
                    }
                }
            }
            return ret;
        }
    }
}
