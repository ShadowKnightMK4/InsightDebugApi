using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.NativeImports;
using InsightSheath;
using InsightSheath.Remote;
using System.Runtime.InteropServices;

namespace InsightSheath.Wrappers
{


    ///
    /// This file provides a wrapper between c# and the InsightApi.Dll that deals with the debug event structure.
    /// The base class <see cref="DebugEventStaticContainer"/> is the root for the other classes in this file. It
    /// handles holding an <see cref="IntPtr"/> that represents the unmanaged DEBUG_EVENT structure.  This pointer gets
    /// passes to various routines in the InsightApi.Dll DLL which then return data bases on that pointer - usually about 
    /// the DEBUG_EVENT struct.  Each DEBUG_EVENT in the MSDN documentation has its own C# class <see cref="DebugEventType"/>
    /// About Exceptions
    ///     The MSDN documentation has certain exceptions listed. Those have been added to the <see cref="DebugExceptionTypes"/> enum.
    ///     That's not actually exhaustive.  winbase.h lists MANY MANY MANY more.
    
    
    ///<summary>
    /// Type of In Page exception flags.  You can check if the exception is that at <see cref="DebugEventExceptionInfo.ExceptionCode"/>
    /// </summary>
    public enum DebugExceptionInPage_Type
    {
        /// <summary>
        /// Target process thread with the <see cref="DebugExceptionTypes.InPageError"/> tried reading something it's not allowed too
        /// </summary>
        ReadException = 0,
        /// <summary>
        /// Target process thread with the <see cref="DebugExceptionTypes.InPageError"/> tried writing to something it's not allowed too
        /// </summary>
        WriteException = 1,
        /// <summary>
        /// Target process thread had a user mode DEP violation.
        /// </summary>
        DepException = 8,
    }

    ///<summary>
    /// Enum for the exceptions specifies within the MSDN documentation.  NOT EXHAUSTIVE. Sourced from minwinbase.h in Visual studio
    /// </summary>
    public enum DebugExceptionTypes: uint
    {

        /// <summary>
        /// Target tried reading/ writing to virtual memory it is not allowed too.
        /// </summary>
         AccessViolation =  0xC0000005,
         /// <summary>
         /// Target tried accessing an array element that's out of bounds (hardware supported bounds checking? according to MSDN documentation)
         /// </summary>
         ArrayBoundsExceeded = 0xC000008C,
         /// <summary>
         /// Hit a breakpoint in the target
         /// </summary>
         Breakpoint = 0x80000003,
         /// <summary>
         /// tread in the target attempting to read misaligned data 
         /// </summary>
         Datatyp_Misalighment = 0x80000002,
         /// <summary>
         /// MSDN:  float point operation resulted in a value to small to be represented.
         /// </summary>
         FloatPoint_Denormal_Operand = 0xC000008D,
         /// <summary>
         /// Float division by zero
         /// </summary>
         Float_DivideByZero = 0xC000008E,
        /// <summary>
        /// MSDN Can't represent a float as a decimal fraction precisely
        /// </summary>
        Float_InexactResult = 0xC000008F,
         /// <summary>
         /// float's exponent is too big
         /// </summary>
         Float_Overflow = 0xC0000091,
         /// <summary>
         /// Float point value over or underflowed after an operation
         /// </summary>
         Float_StackCheck = 0xC0000092,
         /// <summary>
         /// Float point is less that the magnitude allowed
         /// </summary>
         Float_Underflow = 0xC0000093,
         /// <summary>
         /// Invalid instruction attempted
         /// </summary>
         IllegalInstrution = 0xC000001D,
         /// <summary>
         /// MSDN: Thread tried to access  a page that's not present and system couldn't load page
         /// </summary>
         InPageError= 0xC0000006,
         /// <summary>
         /// Int divide by zero
         /// </summary>
         Int_DivideByZero= 0xC0000094,
         /// <summary>
         /// Inter overflowed the most significant bit
         /// </summary>
         Int_Overflow= 0xC0000095,
         /// <summary>
         /// Exception handled returned invalid disposition
         /// </summary>
         InvalidDisposition= 0xC0000026,
         /// <summary>
         /// Exception triggered when attempting to continue an exception that says that's not possible
         /// </summary>
         NonContinuableException= 0xC0000025,
         /// <summary>
         /// Attempt to execute instructions that aren't allowed in the current machine mode
         /// </summary>
         PrivInstruction= 0xC0000096,
         /// <summary>
         /// target used up its stack
         /// </summary>
        StackOverflow = 0xC00000FD,
        /// <summary>
        /// Console Window/Application: CTRL-C pressed the debugged application
        /// </summary>
         DebugConsoleControlC = 0xC000013A,



    }
    
    /// <summary>
    /// If you've forced the helper DLL to be loaded in the target, you'll get these in addition to the <see cref="DebugExceptionTypes"/>
    /// </summary>
    public enum HelperDllExceptionType: uint
    {
    }

    /// <summary>
    /// Helpful enum for the various debug event types. Also specifies which C# class will assist in reading the event's data.
    /// </summary>
    public enum DebugEventType
    {
        /// <summary>
        /// Event is an exception. <see cref="DebugEventExceptionInfo"/>
        /// </summary>
        ExceptionEvent = 1,
        /// <summary>
        /// Event is a create thread.  <see cref="DebugEventCreateThreadInfo"/>
        /// </summary>
        CreateTheadEvent = 2,
        /// <summary>
        /// Event is a create process event. <see cref="DebugEventCreateProcessInfo"/>
        /// </summary>
        CreateProcessEvent = 3,
        /// <summary>
        /// Event is an exit thread event. <see cref="DebugEventExitThreadInfo"/>
        /// </summary>
        ExitThreadEvent = 4,
        /// <summary>
        /// Event is a exit process event.  <see cref="DebugEventExitProcessInfo"/>
        /// </summary>
        ExitProcessEvent = 5,
        /// <summary>
        /// Event is a load DLL event. <see cref="DebugEventLoadDllInfo"/>
        /// </summary>
        LoadDllEvent = 6,
        /// <summary>
        /// Event is unload DLL event.  <see cref="DebugEventUnloadDllInfo"/>
        /// </summary>
        UnloadDllEvent = 7,
        /// <summary>
        /// Event is output debug string (can use <see cref="RemoteStructure.RemoteReadDebugString(IntPtr, IntPtr)"/> OR <see cref="DebugEventStringInfo"/> which does this for you
        /// </summary>
        OutputDebugString = 8,
        /// <summary>
        /// Event is System RIP Debug Event  <see cref="DebugEventRipInfo"/>
        /// </summary>
        RipEvent = 9

    }

    public enum RipErrorType
    {
        /// <summary>
        /// Only Error value was set
        /// </summary>
        SleNoType = 0,
        /// <summary>
        /// Invalid data passed to function what caused debugged application to fail
        /// </summary>
        SleError = 0x00000001,
        /// <summary>
        /// Invalid data passed to a function maybe won't cause the application to fail
        /// </summary>
        SleMinorError = 0x00000002,
        /// <summary>
        /// Potentially Invalid data passed to a function but function finished any.
        /// </summary>
        SleWarning = 0x00000003
    }





    /// <summary>
    /// Implements the basis of disposal the other DebugEvent classes use.  Also exposes routines to read the ProcessID and ThreadID that triggered the event
    /// </summary>
    public abstract class DebugEventStaticContainer : NativeStaticContainer
    {
        public DebugEventStaticContainer(IntPtr Native): base(Native)
        {
            
        }
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="Native">Native pointer to the structure</param>
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree(). Set TO FALSE if dealing with structures declared in C/C++ code vs dynamically allocated</param>
        public DebugEventStaticContainer(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {
            
        }



        protected override void Dispose(bool disposing)
        {
            
            if (!disposedValue)
            {

                if (this.FreeOnCleanup)
                {
                    NativeMethods.SimpleFree(Native);
                    ClearNative();
                }
                // TODO: set large fields to null
                disposedValue = true;
            }
            base.Dispose(disposing);

        }

        ~DebugEventStaticContainer()
         {
                  Dispose(disposing: false);
         }


        private bool disposedValue;

        /// <summary>
        /// Get the process id that this event happed too
        /// </summary>
        public uint ProcessID
        {
            get
            {
                return NativeMethods.DebugEvent_GetProcessID(Native);
            }
        }

        /// <summary>
        /// Get the thread of the code that this event happened too
        /// </summary>
        public uint ThreadID
        {
            get
            {
                return NativeMethods.DebugEvent_GetThreadID(Native);
            }
        }

        /// <summary>
        /// Get which type of structure this debug event contains. <see cref="DebugEventType"/>
        /// </summary>
        public DebugEventType EventType
        {
            get
            {
                return NativeMethods.DebugEvent_GetEventType(Native);
            }
        }
        
        /// <summary>
        /// Makes a couple of calls to GetNativeSystemInfo() and asks IsWow64Process() if the this processID is running under WOW or not.
        /// This is NOT part of DebugEvent structure itself BUT is useful in letter the debugger know is the debugged application is 32-bit vs 64-bit
        /// </summary>
        public bool IsEventFrom32BitProcess
        {
            get
            {
                return NativeMethods.DebugEvent_IsEventFrom32Bit(Native);
            }
        }

    }

    
    public class DebugEventLoadDllInfo : DebugEventStaticContainer
    {
        public DebugEventLoadDllInfo(IntPtr Nat) : base(Nat)
        {

        }

        public DebugEventLoadDllInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup)
        {

        }

        /// <summary>
        /// Get the Raw handle to the file. You should probably close this if the handle is valid once you no longer need it. If it's valid, MSDN  says you'll have read and write access
        /// </summary>
        public IntPtr FileHandle
        {
            get
            {
                return NativeMethods.DebugEvent_LoadDllInfo_GetHFile(Native);
            }
        }

        /// <summary>
        /// Get the base address of where the DLL was loaded in the debugged process.
        /// </summary>
        public IntPtr DllBaseAddress
        {
            get
            {
                return NativeMethods.DebugEvent_LoadDllInfo_GetBaseOfDll(Native);
            }
        }

        /// <summary>
        /// Get the size (in bytes?) of the DLL's debug info
        /// </summary>
        public uint DebugInfoSize
        {
            get
            {
                return NativeMethods.DebugEvent_LoadDllINfo_GetDebugInfoSize(Native);
            }
        }
        /// <summary>
        /// offer to where debug info is located
        /// </summary>
        public uint DebugInfoOffset
        {
            get
            {
                return NativeMethods.DebugEvent_LoadDllINfo_GetDebugInfoOffset(Native);
            }
        }


        /// <summary>
        /// Is the string in the address space specified here valid?
        /// </summary>
        public bool IsImageNameStringUnicode
        {
            get
            {
                if (NativeMethods.DebugEvent_LoadDllInfo_GetUnicodeFlag(Native) != 0) 
                {
                    return true;
                }
                return false;
            }
        }

        /// <summary>
        /// return the debug info specified in the DLL as a series of bytes. MSDN documentation says this should be code view 4.0 format - whatever that is
        /// note: exists in the c# side only
        /// </summary>
        /// <returns></returns>
        public byte[] GetDebugInfo()
        {
            if (this.FileHandle == IntPtr.Zero)
            {
                return null;
            }
            else
            {
                if (DebugInfoSize != 0)
                {
                    var ret = new byte[this.DebugInfoSize];
                    using (var fn = new FileStream(new Microsoft.Win32.SafeHandles.SafeFileHandle(FileHandle,false), FileAccess.ReadWrite))
                    {
                        fn.Seek(DebugInfoOffset, SeekOrigin.Begin);
                        fn.Read(ret, 0, ret.Length);
                        return ret;
                    }
                }
                return null;

            }
        }

        /// <summary>
        /// This ignores the <see cref="ImageName"/>  underlying native struct member entirely as seen on MSDN LOAD_DLL_DEBUG_EVENT.  We simple fetch name - assuming the native hfile member is valid. Should the system not include the file handle, null is returned instead
        /// </summary>
        public string ImageName
        {
            get
            {
                if (FileHandle != IntPtr.Zero)
                {
                    return HelperRoutines.GetFileNameViaHandle(FileHandle);
                }
                return null; 
            }
        }
    }

    /// <summary>
    /// Class Wrapper dealing with extracting Exception information from a <see cref="DebugEvent"/>.
    /// TODO: Insert code on native side that remaps 32-bit handling to 64-bit and transparently deal with it there. Managed Side need not see the different.
    /// </summary>
    public class DebugEventExceptionInfo : DebugEventStaticContainer
    {
        public DebugEventExceptionInfo(IntPtr Nat) : base(Nat)
        {

        }

        public DebugEventExceptionInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup)
        {

        }

        public uint ExceptionParamterCount
        {
            get
            {
                return NativeMethods.DebugEvent_ExceptionInfo_GetExceptionArgumentCount(Native);
            }
        }

        

        /// <summary>
        /// Retrieve the contents of the exception parameter list for a 32-bit debugged process. If retreving form a 64-bit process, your values are likely truncated and worthless
        /// </summary>
        /// <remarks> The Native implementation promotes the DWORD array into a (C# ulong)/(C++ dword64) array and returns a block of memory with values of 4 bytes long. We free the block returned</remarks>
        public uint[] ExceptionParameter32
        {
            get
            {
                IntPtr ptr = NativeMethods.DebugEvent_ExceptionInfo_GetExceptionInformation(Native);
                if (ptr != IntPtr.Zero)
                {
                    uint[] ret = new uint[ExceptionParamterCount];
                    IntPtr stepper = ptr;
                    for (int step = 0; step < ExceptionParamterCount; step++)
                    {
                        ret[step] = (uint) HelperRoutines.Peek8(stepper);
                        stepper += sizeof(ulong);
                    }
                    NativeMethods.SimpleFree(ptr);
                    return ret;
                }
                return null;
            }
        }

        /// <summary>
        ///  Retrieve the contents of the exception parameter list for a 64-bit debugged process. 
        /// </summary>
        public ulong[] ExceptionParameter64
        {
            get
            {
                IntPtr ptr = NativeMethods.DebugEvent_ExceptionInfo_GetExceptionInformation(Native);
                if (ptr != IntPtr.Zero)
                {
                    ulong[] ret = new ulong[ExceptionParamterCount];
                    IntPtr stepper = ptr;
                    for (int step = 0; step < ExceptionParamterCount; step++)
                    {
                        ret[step] = HelperRoutines.Peek8(stepper);
                        stepper += sizeof(ulong);
                    }
                    NativeMethods.SimpleFree(ptr);
                    return ret;
                }
                return null;
            }
        }

        /// <summary>
        /// return the address where the exception happened in the 64 bit process.
        /// </summary>
        public ulong ExceptionAddress64
        {
            get
            {
                return NativeMethods.DebugEvent_ExceptionInfo_GetExceptionAddress(Native);
            }
        }

        /// <summary>
        /// return address where the exception happens in the 32-bit process. Truncates the address if the process is 64-bit.
        /// </summary>
        public uint ExceptionAddress32
        {
            get
            {
                return (uint)NativeMethods.DebugEvent_ExceptionInfo_GetExceptionAddress(Native);
            }
        }


        public uint ExceptionFlags
        {
            get
            {
                return NativeMethods.DebugEvent_ExceptionInfo_GetExceptionFlags(Native);
            }
        }    


        /// <summary>
        /// Return the Exception for the exception that trigged.
        /// </summary>
        public  uint ExceptionCode
        {
            get
            {
                return NativeMethods.DebugEvent_ExceptionInfo_GetExceptionCode(Native);
            }
        }

        /// <summary>
        /// return if the debugger (that's you) has seen this first chance exception before.
        /// </summary>
        public bool IsFirstChanceException
        {
            get
            {
                if ( NativeMethods.DebugEvent_ExceptionInfo_GetFirstChance(Native) != 0)
                {
                    return true;
                }
                return false;
            }
        }
    }


    public class DebugEventCreateThreadInfo : DebugEventStaticContainer
    {
        public DebugEventCreateThreadInfo(IntPtr Nat) : base(Nat)
        {

        }

        public DebugEventCreateThreadInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup)
        {

        }

        /// <summary>
        /// if NOT Null (IntPtr.zero),  handle should have THREAD_GET_CONTEXT/ THREAD_SET_CONTEXT and THREAD_SUSPEND_RESUME access to the thread per MSDN documentation.
        /// </summary>
        public IntPtr ThreadHandle
        {
            get
            {
                return NativeMethods.DebugEvent_CreateThreadInfo_GetThreadHandle(Native);
            }
        }

        /// <summary>
        /// Get the approx start address
        /// </summary>
        public IntPtr ThreadStartAddress
        {
            get
            {
                return NativeMethods.DebugEvent_CreateThreadInfo_GetThreadStartAddress(Native);
            }
        }

        /// <summary>
        /// get the thread location storage pointer here.   MSDN says offset 0x2C is the ThreadLocationStorage pointer which presumably would let you track what per thread stuff your target is doing.
        /// </summary>
        public IntPtr ThreadLocalBase
        {
            get
            {
                return NativeMethods.DebugEvent_CreateThreadInfo_GetThreadLocalBase(Native);
            }
        }

        /// <summary>
        /// ThreadLocalBase + 0x2C
        /// </summary>
        public IntPtr ThreadLocalStoragePointer
        {
            get
            {
                return new IntPtr(ThreadLocalBase.ToInt32() + 0x2C);
            }
        }
    }




    public class DebugEventExitThreadInfo : DebugEventStaticContainer
    {
        public DebugEventExitThreadInfo(IntPtr Nat) : base(Nat)
        {

        }

        public DebugEventExitThreadInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup)
        {

        }


        /// <summary>
        /// Get the code that the thread returned when finishing
        /// </summary>
        public uint ExitCode
        {
            get
            {
                return NativeMethods.DebugEvent_ExitThreadInfo_GetExitCode(Native);
            }
        }
    }


    public class DebugEventExitProcessInfo : DebugEventStaticContainer
    {
        public DebugEventExitProcessInfo(IntPtr Nat) : base(Nat)
        {

        }

        public DebugEventExitProcessInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup)
        {

        }


        /// <summary>
        /// Get the code that the Process returned when finishing
        /// </summary>
        public uint ExitCode
        {
            get
            {
                return NativeMethods.DebugEvent_ExitProcessInfo_GetExitCode(Native);
            }
        }
    }


    public class DebugEventCreateProcessInfo :DebugEventStaticContainer
    {
        public DebugEventCreateProcessInfo(IntPtr Nat): base(Nat)
        {
            
        }

        public DebugEventCreateProcessInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup)
        {

        }

        /// <summary>
        /// Get the HANDLE out of the structure. Warning handle may be null
        /// </summary>
        public IntPtr NativeFileHandle
        {
            get
            {
                return NativeMethods.DebugEvent_CreateProcessGetFileHandle(Native);
            }
        }


        /// <summary>
        /// Get a string for the image name of the create process event this contains. Depends on the NativeFileHandle being valid and will return null otherwise
        /// </summary>
        public string ImageName
        {
            get
            {
                return HelperRoutines.GetFileNameViaHandle(NativeFileHandle); 
            }
        }

        /// <summary>
        /// Returns the contents of the exefile that the debug create process game from. Will Be null if the CreateProcess Event did not include a valid hFile handle. 
        /// </summary>
        /// <exception cref="IOException"> IO Related exceptions can be thrown if a problem occurs in reading the main module's file contents. <see cref="File.ReadAllBytes(string)"/> to see the list</exception>
        public byte[] GetFileContents()
        {
            
            {
                if (NativeFileHandle != IntPtr.Zero)
                {
                   
                    if (ImageName != null)
                    {
                        return File.ReadAllBytes(ImageName);
                    }
                }
                return null;
            }
        }

        


 
        
    }
    /// <summary>
    /// Providers a wrapper to process specific <see cref="DebugEventType.OutputDebugString"/> components.
    /// </summary>
    public class DebugEventStringInfo: DebugEventStaticContainer
    {
        public DebugEventStringInfo(IntPtr Native) :base(Native)
        {
            
        }

        public DebugEventStringInfo(IntPtr Native, bool FreeOnCleanup) : base(Native, FreeOnCleanup)
        { }

        /// <summary>
        /// Return the string messaged contained within the debug event. ANSI output strings are converted to Unicode on the Native side before being sent to the Managed side.
        /// </summary>
        public string OutputString
        {
            get
            {
                
                if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.OutputDebugString)
                {
                    return null;
                }
                else
                {
                    IntPtr NativeHandle = NativeMethods.OpenProcessForMemoryAccess(NativeMethods.DebugEvent_GetProcessID(Native));
                    if (NativeHandle == IntPtr.Zero)
                    {
                        return null;
                    }
                    ///
                    /// <see cref="RemoteStructure.RemoteReadDebugString(IntPtr, IntPtr)"/> is assumed to free the unmanaged memory after reading it
                    ///
                    string Managed = RemoteStructure.RemoteReadDebugString(NativeHandle, Native);
                    NativeMethods.CloseHandle(NativeHandle);
                    return Managed;
                }
            }
        }
        
    }

    /// <summary>
    /// Providers a wrapper to access the underlying structure. Assumes memory management is done elsewhere.
    /// </summary>
    public class DebugEventRipInfo: DebugEventStaticContainer
    {
        public DebugEventRipInfo(IntPtr NativePtr) : base(NativePtr)
        {
            
        }
        public DebugEventRipInfo(IntPtr NativePtr, bool FreeOnCleanup) : base(NativePtr, FreeOnCleanup)
        {

        }



        /// <summary>
        /// Get the Error number that caused the debugging RIP event.
        /// </summary>
        public uint Error
            {
                get
                {
                    return NativeMethods.DebugEvent_RipGetError(Native);
                }
            }


        /// <summary>
        /// Should be one of the enum values for <see cref="RipErrorType"/>. If It's zero, MSDN documentation says only sdwError is set.
        /// </summary>
            public RipErrorType ErrorType
            {
                get
                {
                return NativeMethods.DebugEvent_RipGetErrorType(Native);
                }
            }

        }



    public class DebugEventUnloadDllInfo: DebugEventStaticContainer
    {
        public DebugEventUnloadDllInfo(IntPtr NativePtr, bool FreeOnDispose) : base(NativePtr, FreeOnDispose)
        {

        }

        /// <summary>
        /// Creation.  Does NOT free the underling pointer 
        /// </summary>
        /// <param name="NativePtr"></param>
        public DebugEventUnloadDllInfo(IntPtr NativePtr) : base(NativePtr)
        {

        }

        /// <summary>
        /// The base address of the DLL that was previously loaded.
        /// </summary>
        public IntPtr BaseOfDll
        {
            get
            {
                return NativeMethods.DebugEvent_UnloadDllInfo_GetBaseAddress(Native);
            }
        }

    }

    /// <summary>
    /// This encapsulates the Native DebugEvent structure via calling exported C routines in InsightAPI.dll.  IMPORTANT!!!! The LoadDLlDebugEvent does not close the passed handle if valid.
    /// </summary>
    public class DebugEvent : DebugEventStaticContainer
    {
        private static readonly string error_msg_bad_event_fetch = "Attempt to fetch {0} from an event that does not contain the event {1}";
        /// <summary>
        /// 
        /// </summary>
        /// <param name="NativePtr">pass a native pointing contain a debug event structure.</param>
        /// <param name="FreeOnCleanup">Set this to true for chunks explicit allocated to the structure by C/C++'s malloc() ext... </param>
        public DebugEvent(IntPtr NativePtr, bool FreeOnCleanup) : base(NativePtr, FreeOnCleanup)
        {

        }

        /// <summary>
        /// You don't need to use this if the let the debugger code use the worker thread. You'll need to allocate a native block to hold the debug struct should you decide to roll your own
        /// </summary>
        /// <returns>a new <see cref="DebugEvent"/> class instance with its own private native structure</returns>
        public static DebugEvent CreatePrivateStruct()
        {
            return new DebugEvent(NativeMethods.DebugEvent_AllocateStructure(), true);
        }
        /// <summary>
        /// Creation.  Does NOT free the underling pointer 
        /// </summary>
        /// <param name="NativePtr"></param>
        public DebugEvent(IntPtr NativePtr) : base(NativePtr)
        {
            FreeOnCleanupContainer = false;
        }


        bool disposedValue;
        protected override void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                // we need to close the handle 
                if (NativeMethods.DebugEvent_GetEventType(Native) == DebugEventType.LoadDllEvent)
                {

                }
                disposedValue = true;
            }
            base.Dispose(disposing);
        }



        #region Wrapper Creation
        /// <summary>
        /// Return an instance of the wrapper to deal with <see cref="DebugEventType.CreateProcessEvent"/> events. The class still points to the native struct and does not need to be freed().
        /// </summary>
        /// <returns></returns>
        public DebugEventCreateProcessInfo GetDebugEventCreateProcessInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.CreateProcessEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Create Process Information", " Create Process Event" }));
            }
            return new DebugEventCreateProcessInfo(Native, false);
        }

        public DebugEventCreateThreadInfo GetDebugEventCreateThreadInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.CreateTheadEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Create Thread Information", " Create Thread Event" }));
            }
            return new DebugEventCreateThreadInfo(Native, false);
        }


        public DebugEventExceptionInfo GetDebugEventExceptionInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.ExceptionEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Exception Information", " Exception Event" }));
            }
            return new DebugEventExceptionInfo(Native, false);
        }

        public DebugEventExitProcessInfo GetEventExitProcessInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.ExitProcessEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Process Exit Information", " Process Exit Event" }));
            }
            return new DebugEventExitProcessInfo(Native, false);
        }

        public DebugEventExitThreadInfo GetEventExitThreadInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.ExitThreadEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Thread Exit Information", " Thread Exit Event" }));
            }
            return new DebugEventExitThreadInfo(Native, false);
        }


        public DebugEventLoadDllInfo GetDebugEventLoadDll()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.LoadDllEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "DLL Load Information", " DLL Load  Event" }));
            }
            return new DebugEventLoadDllInfo(Native, false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public DebugEventStringInfo GetDebugEventStringInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.OutputDebugString)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Debug String Information", " Debug String Struct" }));
            }

            return new DebugEventStringInfo(Native, false);
        }


        /// <summary>
        /// Get class suitable to reading DEBUG_EVENT_RIP_INFO from this class.  Will throw <see cref="InvalidOperationException"/> if not the correct event
        /// </summary>
        /// <exception cref="InvalidOperationException"> Is thrown when the contained even is not a <see cref=DebugEventType.RipEvent"/></exception>
        /// <returns>If the contained event is <see cref="DebugEventType.RipEvent"/> returns a <see cref="DebugEventRipInfo"/> pointing to this event. </returns>
        public DebugEventRipInfo GetDebugEventRipInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.RipEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Rip Information", "Rip Struct" }));
            }
            return new DebugEventRipInfo(Native, false);
        }

        public DebugEventUnloadDllInfo GetDebugEventUnloadDllInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.UnloadDllEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Dll free/unload Information", " Unload Dll Event" }));
            }
            return new DebugEventUnloadDllInfo(Native, false);
        }

        #endregion
        #region C# boiler plate dispose and the underlying struct




        ~DebugEvent()
         {
             // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
             Dispose(disposing: false);
         }


        #endregion
 
    }
}
