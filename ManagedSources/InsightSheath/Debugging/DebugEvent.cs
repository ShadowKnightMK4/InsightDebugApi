using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.NativeImports;
using InsightSheath;
using System.Runtime.InteropServices;
using InsightSheath.Abstract;
using InsightSheath.Win32Struct.Remote;
using System.Diagnostics;

namespace InsightSheath.Debugging
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
         /// Hit a breakpoint in the target. Important! Windows will generate a breakpoint once its finished loading the process before sending your first debugger message to you.
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
         /// <summary>
         /// Is generated by Kernel32's CloseHandle() when an invalid handle is attempting to be closed in the process being debugged.  
         /// IMPORTANT.  How you handle this depends if you're debugging / learning about a target. If it's debug resistant (for example malware), your debugger is NOT going  to want to mark that exception as being handled as then it likely will have just revealed itself.
         /// </summary>
         InvalidHandleCloseException = 0xC0000008,


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

    /// <summary>
    /// Type of error that is contained <see cref="DebugEventRipInfo.ErrorType"/>
    /// </summary>
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
    public abstract class DebugEventStaticContainer : ReferenceCounterNativeStaticContainer
    {
        /// <summary>
        /// Constructor for the base <see cref="DebugEvent"/> abstract class
        /// </summary>
        /// <param name="Native">Native pointer to the structure</param>
        public DebugEventStaticContainer(IntPtr Native): base(Native)
        {
            
        }
        /// <summary>
        /// Constructor for the base <see cref="DebugEvent"/> abstract class
        /// </summary>
        /// <param name="Native">Native pointer to the structure</param>
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree(). Set TO FALSE if dealing with structures declared in C/C++ code vs dynamically allocated</param>
        public DebugEventStaticContainer(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {
            
        }

        /// <summary>
        /// Constructor for the base <see cref="DebugEvent"/> class
        /// </summary>
        /// <param name="Native">Native pointer to the structure</param>
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree(). Set TO FALSE if dealing with structures declared in C/C++ code vs dynamically allocated</param>
        /// <param name="ReferenceCounter">Specify the starting reference count.</param>
        public DebugEventStaticContainer(IntPtr Native, bool FreeOnCleanup, ulong ReferenceCounter): base(Native, FreeOnCleanup, ReferenceCounter)
        {

        }

        /// <summary>
        ///  my personal disposed value
        /// </summary>
        private bool disposedValue;

        /// <summary>
        /// Generic <see cref="DebugEventStaticContainer"/> dispose action. 
        /// </summary>
        /// <param name="disposing">Dispose of managed also?</param>
        protected override void Dispose(bool disposing)
        {

            if (!IsDisposed)
            {
                if (Release() == 0)
                {
                    if (FreeOnCleanup)
                    {
                        NativeMethods.SimpleFree(Native);
                    }
                    ClearNative();
                }
                else
                {
                    ClearNative();
                }
                disposedValue = true;
            }
            base.Dispose(disposing);

        }

  
        ///<summary>
        /// Finalize for the base <see cref="DebugEvent"/> class
        ///</summary>
        ~DebugEventStaticContainer()
         {
                  Dispose(disposing: false);
         }



        /// <summary>
        /// Get the process id that this event happed too
        /// </summary>
        public uint ProcessID
        {
            get
            {
                return DebugEventNative.DebugEvent_GetProcessID(Native);
            }
        }

        /// <summary>
        /// Get the thread of the code that this event happened too
        /// </summary>
        public uint ThreadID
        {
            get
            {
                return DebugEventNative.DebugEvent_GetThreadID(Native);
            }
        }

        /// <summary>
        /// Get which type of structure this debug event contains. <see cref="DebugEventType"/>
        /// </summary>
        public DebugEventType EventType
        {
            get
            {
                return DebugEventNative.DebugEvent_GetEventType(Native);
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
                return DebugEventNative.DebugEvent_IsEventFrom32Bit(Native);
            }
        }

    }

    
    /// <summary>
    /// Provides access to a <see cref="DebugEventType.LoadDllEvent"/> in a <see cref="DebugEvent"/> struct
    /// </summary>
    public class DebugEventLoadDllInfo : DebugEventStaticContainer
    {
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.LoadDllEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventLoadDll"/></param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventLoadDllInfo(IntPtr Nat) : base(Nat)
        {

        }
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.LoadDllEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventLoadDll"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventLoadDllInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }

        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.LoadDllEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to an unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventLoadDll"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventLoadDllInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }

        /// <summary>
        /// Get the Raw handle to the file. You should probably close this if the handle is valid once you no longer need it. If it's valid, MSDN  says you'll have read and write access
        /// </summary>
        public IntPtr FileHandle
        {
            get
            {
                return DebugEventNative.DebugEvent_LoadDllInfo_GetHFile(Native);
            }
        }

        /// <summary>
        /// Get the base address of where the DLL was loaded in the debugged process.
        /// </summary>
        public IntPtr DllBaseAddress
        {
            get
            {
                return DebugEventNative.DebugEvent_LoadDllInfo_GetBaseOfDll(Native);
            }
        }

        /// <summary>
        /// Get the size (in bytes?) of the DLL's debug info
        /// </summary>
        public uint DebugInfoSize
        {
            get
            {
                return DebugEventNative.DebugEvent_LoadDllINfo_GetDebugInfoSize(Native);
            }
        }
        /// <summary>
        /// offer to where debug info is located
        /// </summary>
        public uint DebugInfoOffset
        {
            get
            {
                return DebugEventNative.DebugEvent_LoadDllINfo_GetDebugInfoOffset(Native);
            }
        }


        /// <summary>
        /// Is the string in the address space specified here valid?
        /// </summary>
        public bool IsImageNameStringUnicode
        {
            get
            {
                if (DebugEventNative.DebugEvent_LoadDllInfo_GetUnicodeFlag(Native) != 0) 
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
    /// </summary>
    public class DebugEventExceptionInfo : DebugEventStaticContainer
    {
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExceptionEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventExceptionInfo"/></param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventExceptionInfo(IntPtr Nat) : base(Nat)
        {

        }


        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExceptionEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventExceptionInfo"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventExceptionInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }

        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExceptionEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventExceptionInfo"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventExceptionInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }

        /// <summary>
        /// Return the length of the parameter array for the exception. Range is 0 to 15 or (in C/C++ land EXCEPTION_MAXIMUM_PARAMETERS #define)
        /// </summary>
        public uint ExceptionParameterCount
        {
            get
            {
                return DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionArgumentCount(Native);
            }
        }

        

        // temp fix
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        /// <summary>
        /// Retrieve the contents of the exception parameter list for a 32-bit debugged process. If retrieving form a 64-bit process, your values are likely truncated and worthless
        /// </summary>
        /// <remarks> The Native implementation promotes the DWORD array into a (C# ulong)/(C++ dword64) array and returns a block of memory with values of 8 bytes long. We free the block returned</remarks>
        public uint[] ExceptionParameter32
        {
            get
            {
                IntPtr ptr = DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionInformation(Native);
                if (ptr != IntPtr.Zero)
                {
                    uint[] ret = new uint[ExceptionParameterCount];
                    IntPtr stepper = ptr;
                    for (int step = 0; step < ExceptionParameterCount; step++)
                    {
                        ret[step] = (uint) MemoryNative.Peek8(stepper);
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
                IntPtr ptr = DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionInformation(Native);
                if (ptr != IntPtr.Zero)
                {
                    ulong[] ret = new ulong[ExceptionParameterCount];
                    IntPtr stepper = ptr;
                    for (int step = 0; step < ExceptionParameterCount; step++)
                    {
                        ret[step] = MemoryNative.Peek8(stepper);
                        stepper += sizeof(ulong);
                    }
                    NativeMethods.SimpleFree(ptr);
                    return ret;
                }
                return null;
            }
        }

        /// <summary>
        /// return the address where the exception happened in the 64 bit process.  Reading from the upper part for an x86 process should be all 0s.
        /// </summary>
        public ulong ExceptionAddress64
        {
            get
            {
                return DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionAddress(Native);
            }
        }

        /// <summary>
        /// return address where the exception happens in the 32-bit process. Truncates the address if the process is 64-bit.
        /// </summary>
        public uint ExceptionAddress32
        {
            get
            {
                return (uint)DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionAddress(Native);
            }
        }


        /// <summary>
        /// Return the flags specific for this exception
        /// </summary>
        public uint ExceptionFlags
        {
            get
            {
                return DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionFlags(Native);
            }
        }    


        /// <summary>
        /// Return the Exception value for the exception that trigged as an uint.  Sourced from the same part of the unmanaged struct as <see cref="ExceptionCode"/>
        /// </summary>
        public uint ExceptionCode_as_int
        {
            get
            {
                return DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionCode(Native);
            }
        }

        /// <summary>
        /// Return the exception value for the exception that this contains as a convenient enumeration. Sourced from the same part of the unmanaged struct as <see cref="ExceptionCode_as_int"/>
        /// </summary>
        public DebugExceptionTypes ExceptionCode
        {
            get
            {
                return (DebugExceptionTypes)DebugEventNative.DebugEvent_ExceptionInfo_GetExceptionCode(Native);
            }
        }

        /// <summary>
        /// return if the debugger (that's you) has seen this first chance exception before.
        /// </summary>
        public bool IsFirstChanceException
        {
            get
            {
                if (DebugEventNative.DebugEvent_ExceptionInfo_GetFirstChance(Native) != 0)
                {
                    return true;
                }
                return false;
            }
        }
    }


    /// <summary>
    /// Holds a reference to a <see cref="DebugEvent"/> containing an event of the type of <see cref="DebugEventType.CreateTheadEvent"/>. You'll typically get this when calling <see cref="DebugEvent.GetDebugEventCreateThreadInfo"/>
    /// </summary>
    public class DebugEventCreateThreadInfo : DebugEventStaticContainer
    {
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.CreateTheadEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventCreateThreadInfo(IntPtr Nat) : base(Nat)
        {

        }

        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.CreateTheadEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to an unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventCreateThreadInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }

        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.CreateTheadEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to an unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventCreateThreadInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }

        /// <summary>
        /// if NOT Null (IntPtr.zero),  handle should have THREAD_GET_CONTEXT/ THREAD_SET_CONTEXT and THREAD_SUSPEND_RESUME access to the thread per MSDN documentation.
        /// </summary>
        public IntPtr ThreadHandle
        {
            get
            {
                return DebugEventNative.DebugEvent_CreateThreadInfo_GetThreadHandle(Native);
            }
        }

        /// <summary>
        /// Get the approx start address
        /// </summary>
        public IntPtr ThreadStartAddress
        {
            get
            {
                return DebugEventNative.DebugEvent_CreateThreadInfo_GetThreadStartAddress(Native);
            }
        }

        /// <summary>
        /// get the thread location storage pointer here.   MSDN says offset 0x2C is the ThreadLocationStorage pointer which presumably would let you track what per thread stuff your target is doing.
        /// </summary>
        public IntPtr ThreadLocalBase
        {
            get
            {
                return DebugEventNative.DebugEvent_CreateThreadInfo_GetThreadLocalBase(Native);
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




    /// <summary>
    /// Holds a reference to a <see cref="DebugEvent"/> containing an event of the type of <see cref="DebugEventType.ExitThreadEvent"/>. You'll typically get this when calling <see cref="DebugEvent.GetEventExitThreadInfo"/>
    /// </summary>
    public class DebugEventExitThreadInfo : DebugEventStaticContainer
    {
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExitThreadEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventExitThreadInfo(IntPtr Nat) : base(Nat)
        {

        }

        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExitThreadEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventExitThreadInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExitThreadEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventExitThreadInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }


        /// <summary>
        /// Get the code that the thread returned when finishing
        /// </summary>
        public uint ExitCode
        {
            get
            {
                return DebugEventNative.DebugEvent_ExitThreadInfo_GetExitCode(Native);
            }
        }
    }

    /// <summary>
    /// Holds a reference to a <see cref="DebugEvent"/> containing an event of the type of <see cref="DebugEventType.ExitProcessEvent"/>. You'll typically get this when calling <see cref="DebugEvent.GetEventExitProcessInfo"/>
    /// </summary>
    public class DebugEventExitProcessInfo : DebugEventStaticContainer
    {
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExitProcessEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventExitProcessInfo(IntPtr Nat) : base(Nat)
        {

        }
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExitProcessEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventExitProcessInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.ExitProcessEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent"/></param>
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventExitProcessInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }

        /// <summary>
        /// Get the code that the Process returned when finishing
        /// </summary>
        public uint ExitCode
        {
            get
            {
                return DebugEventNative.DebugEvent_ExitProcessInfo_GetExitCode(Native);
            }
        }
    }


    /// <summary>
    /// Holds a reference to a <see cref="DebugEvent"/> containing an event of the type of <see cref="DebugEventType.CreateProcessEvent"/>. You'll typically get this when calling <see cref="DebugEvent.GetDebugEventCreateProcessInfo"/>
    /// </summary>
    public class DebugEventCreateProcessInfo :DebugEventStaticContainer
    {
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.CreateProcessEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventCreateProcessInfo"/></param> 
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventCreateProcessInfo(IntPtr Nat): base(Nat)
        {
            
        }
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.CreateProcessEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventCreateProcessInfo"/></param> 
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>      
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventCreateProcessInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }

        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.CreateProcessEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventCreateProcessInfo"/></param> 
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventCreateProcessInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }

        /// <summary>
        /// Get the HANDLE out of the structure. Warning handle may be null
        /// </summary>
        public IntPtr NativeFileHandle
        {
            get
            {
                return DebugEventNative.DebugEvent_CreateProcessGetFileHandle(Native);
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
        /// Returns the contents of the EXE for the process that the debug create process event came from. Will Be null if the CreateProcess Event did not include a valid hFile handle. 
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
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.OutputDebugString"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventStringInfo"/></param> 
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventStringInfo(IntPtr Nat) :base(Nat)
        {
            
        }
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.OutputDebugString"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventStringInfo"/></param> 
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventStringInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }


        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.OutputDebugString"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventStringInfo"/></param> 
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventStringInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }

        /// <summary>
        /// For the user's continence, returns <see cref="OutputString"/>
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return OutputString;
        }
        /// <summary>
        /// Return the string messaged contained within the debug event. ANSI output strings are converted to Unicode on the Native side before being sent to the Managed side. Native side opts into wanting Unicode Strings too.
        /// </summary>
        public string OutputString
        {
            get
            {
                
                if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.OutputDebugString)
                {
                    return null;
                }
                else
                {
                    IntPtr NativeHandle = NativeMethods.OpenProcessForMemoryAccess(DebugEventNative.DebugEvent_GetProcessID(Native));
                    if (NativeHandle == IntPtr.Zero)
                    {
                        return null;
                    }
                    
                    /* Note:   <see cref="RemoteStructure.RemoteReadDebugString(IntPtr, IntPtr)"/> is assumed to free the unmanaged memory after reading it */
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
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.RipEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventRipInfo"/></param> 
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventRipInfo(IntPtr Nat) : base(Nat)
        {

        }
        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.RipEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventRipInfo"/></param> 
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <remarks>The reference counter for this class is also set to 1</remarks>
        public DebugEventRipInfo(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }

        /// <summary>
        /// Construct a wrapper class for a <see cref="DebugEvent"/> containing a <see cref="DebugEventType"/> of <see cref="DebugEventType.RipEvent"/>
        /// </summary>
        /// <param name="Nat">pointer to a unmanaged DEBUG_EVENT struct one a valid pointer from <see cref="DebugEvent.GetDebugEventRipInfo"/></param> 
        /// <param name="FreeOnCleanup">Set if you want the unmanaged pointer released. </param>
        /// <param name="RefCount">Specific the current reference counter. When it reaches 0 during cleanup, the unmanaged pointer is freed. </param>
        public DebugEventRipInfo(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }



        /// <summary>
        /// Get the Error number that caused the debugging RIP event.
        /// </summary>
        public uint Error
            {
                get
                {
                    return DebugEventNative.DebugEvent_RipGetError(Native);
                }
            }


        /// <summary>
        /// Should be one of the enum values for <see cref="RipErrorType"/>. If It's zero, MSDN documentation says only sdwError is set.
        /// </summary>
            public RipErrorType ErrorType
            {
                get
                {
                    return DebugEventNative.DebugEvent_RipGetErrorType(Native);
                }
            }

        }



    /// <summary>
    /// Wrapper for a Debug Event containing an UNLOAD_DLL_DEBUG_EVENT.
    /// </summary>
    public class DebugEventUnloadDllInfo : DebugEventStaticContainer
    {

        /// <summary>
        /// Creation.  Does NOT free the underling pointer. Reference counter set to 1.
        /// </summary>
        /// <param name="NativePtr">native pointer to a DEUBG_EVENT struct containing one UNLOAD_DLL_DEBUG_EVENT. </param>
        public DebugEventUnloadDllInfo(IntPtr NativePtr) : base(NativePtr)
        {

        }

        /// <summary>
        /// Creation.  Reference counter set to one.
        /// </summary>
        /// <param name="NativePtr">native pointer to a DEUBG_EVENT struct containing one UNLOAD_DLL_DEBUG_EVENT. </param>
        /// <param name="FreeOnCleanup">Indicate if we're calling free() when GC clean this instance up.</param>
        public DebugEventUnloadDllInfo(IntPtr NativePtr, bool FreeOnCleanup) : base(NativePtr, FreeOnCleanup, 1)
        {

        }


        /// <summary>
        /// Creation.  Reference counter set to one.
        /// </summary>
        /// <param name="NativePtr">native pointer to a DEUBG_EVENT struct containing one UNLOAD_DLL_DEBUG_EVENT. </param>
        /// <param name="FreeOnCleanup">Indicate if we're clinging free() when GC clean this instance up.</param>
        /// <param name="RefCount">Indicate the current reference counter for the unmanaged memory block.  Block is freed only if reference counter decreases to zero during GC collection.</param>
        public DebugEventUnloadDllInfo(IntPtr NativePtr, bool FreeOnCleanup, ulong RefCount) : base(NativePtr, FreeOnCleanup, RefCount)
        {

        }


        /// <summary>
        /// The base address of the DLL that was previously loaded. 
        /// </summary>
        public IntPtr BaseOfDll
        {
            get
            {
                return new IntPtr((long)DebugEventNative.DebugEvent_UnloadDllInfo_GetBaseAddress(Native));
            }
        }

    }

    /// <summary>
    /// This encapsulates the Native DebugEvent structure via calling exported C routines in InsightAPI.dll.
    /// </summary>
    public class DebugEvent : DebugEventStaticContainer
    {
       
        private static readonly string error_msg_bad_event_fetch = "Attempt to fetch {0} from an event that does not contain the event {1}";

        /// <summary>
        /// Creation.  Does NOT free the underling pointer.
        /// </summary>
        /// <param name="NativePtr">Native pointer to one DEBUG</param>
        public DebugEvent(IntPtr NativePtr) : base(NativePtr)
        {
            FreeOnCleanupContainer = false;
        }


        /// <summary>
        /// Create the wrapper for a DEBUG_EVENT native struct. 
        /// </summary>
        /// <param name="Nat">block of unmanaged memory that points to a buffer the sizeof of a DEBUG_EVENT</param>
        /// <param name="FreeOnCleanup">true to call free() when done and false if not</param>
        /// <remarks>Reference count is set to 1</remarks>
        public DebugEvent(IntPtr Nat, bool FreeOnCleanup) : base(Nat, FreeOnCleanup, 1)
        {

        }
        /// <summary>
        /// Create the wrapper for a DEBUG_EVENT native struct. 
        /// </summary>
        /// <param name="Nat">block of unmanaged memory that points to a buffer the sizeof of a DEBUG_EVENT</param>
        /// <param name="FreeOnCleanup">true to call free() when done and false if not</param>
        /// <param name="RefCount">Specifies the reference count</param>
        /// <remarks>Reference count is set to 1</remarks>

        public DebugEvent(IntPtr Nat, bool FreeOnCleanup, ulong RefCount) : base(Nat, FreeOnCleanup, RefCount)
        {

        }


        /// <summary>
        /// You don't need to use this if the let the debugger code use the worker thread. You'll need to allocate a native block to hold the debug struct should you decide to roll your own
        /// </summary>
        /// <returns>a new <see cref="DebugEvent"/> class instance with its own private native structure</returns>
        public static DebugEvent CreatePrivateStruct()
        {
            return new DebugEvent(DebugEventNative.DebugEvent_AllocateStructure(), true);
        }
  


        bool disposedValue;
        /// <summary>
        /// Nothing special needed to dispose off. Defaults OK.
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
        }



        #region Wrapper Creation
        /// <summary>
        /// Return an instance of the wrapper to deal with <see cref="DebugEventType.CreateProcessEvent"/> events. The class still points to the native struct and does not need to be freed().
        /// </summary>
        /// <returns></returns>
        public DebugEventCreateProcessInfo GetDebugEventCreateProcessInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.CreateProcessEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Create Process Information", " Create Process Event" }));
            }
            AddRef();
            var ret = new DebugEventCreateProcessInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;
        }

        /// <summary>
        /// Retrieve an instance of <see cref="DebugEventCreateThreadInfo"/> pointing to the same unmanaged memory as this class.
        /// </summary>
        /// <returns>Returns a new instance of <see cref="DebugEventCreateThreadInfo"/> if the contained event is <see cref="DebugEventType.CreateTheadEvent"/>. If it does not contain the specific event, it throws <see cref="InvalidOperationException"/> and potentially returns null if something else happens</returns>
        public DebugEventCreateThreadInfo GetDebugEventCreateThreadInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.CreateTheadEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Create Thread Information", " Create Thread Event" }));
            }
            AddRef();
            var ret = new DebugEventCreateThreadInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;
        }

        /// <summary>
        /// If this is an event of <see cref="DebugEventType.ExceptionEvent"/>, return an instance of <see cref="DebugEventExceptionInfo"/>
        /// </summary>
        /// <returns>returns instance of <see cref="DebugEventExceptionInfo"/> if the contained event matches. Throws <see cref="InvalidOperationException"/> if not correct type</returns>
        /// <exception cref="InvalidOperationException">This is thrown if the event is not the correct type</exception>
        public DebugEventExceptionInfo GetDebugEventExceptionInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.ExceptionEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Exception Information", " Exception Event" }));
            }

            AddRef();
            var ret = new DebugEventExceptionInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;

            
        }
        /// <summary>
        /// If this is an event of <see cref="DebugEventType.ExitProcessEvent"/>, return an instance of <see cref="DebugEventExitProcessInfo"/>
        /// </summary>
        /// <returns>returns instance of <see cref="DebugEventExitProcessInfo"/> if the contained event matches. Throws <see cref="InvalidOperationException"/> if not correct type</returns>
        /// <exception cref="InvalidOperationException">This is thrown if the event is not the correct type</exception>
        public DebugEventExitProcessInfo GetEventExitProcessInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.ExitProcessEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Process Exit Information", " Process Exit Event" }));
            }

            AddRef();
            var ret = new DebugEventExitProcessInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;

        }
        /// <summary>
        /// If this is an event of <see cref="DebugEventType.ExitThreadEvent"/>, return an instance of <see cref="DebugEventExitThreadInfo"/>
        /// </summary>
        /// <returns>returns instance of <see cref="DebugEventExitThreadInfo"/> if the contained event matches. Throws <see cref="InvalidOperationException"/> if not correct type</returns>
        /// <exception cref="InvalidOperationException">This is thrown if the event is not the correct type</exception>
        public DebugEventExitThreadInfo GetEventExitThreadInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.ExitThreadEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Thread Exit Information", " Thread Exit Event" }));
            }

            AddRef();
            var ret = new DebugEventExitThreadInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;
        }

        /// <summary>
        /// If this is an event of <see cref="DebugEventType.LoadDllEvent"/>, return an instance of <see cref="DebugEventLoadDllInfo"/>
        /// </summary>
        /// <returns>returns instance of <see cref="DebugEventLoadDllInfo"/> if the contained event matches. Throws <see cref="InvalidOperationException"/> if not correct type</returns>
        /// <exception cref="InvalidOperationException">This is thrown if the event is not the correct type</exception>
        public DebugEventLoadDllInfo GetDebugEventLoadDll()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.LoadDllEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "DLL Load Information", " DLL Load  Event" }));
            }

            AddRef();
            var ret = new DebugEventLoadDllInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;
        }

        /// <summary>
        /// If this is an event of <see cref="DebugEventType.OutputDebugString"/>, return an instance of <see cref="DebugEventStringInfo"/>
        /// </summary>
        /// <returns>returns instance of <see cref="DebugEventStringInfo"/> if the contained event matches. Throws <see cref="InvalidOperationException"/> if not correct type</returns>
        /// <exception cref="InvalidOperationException">This is thrown if the event is not the correct type</exception>
        public DebugEventStringInfo GetDebugEventStringInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.OutputDebugString)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Debug String Information", " Debug String Struct" }));
            }

            AddRef();
            var ret = new DebugEventStringInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;
        }


        /// <summary>
        /// Get class suitable to reading DEBUG_EVENT_RIP_INFO from this class.  Will throw <see cref="InvalidOperationException"/> if not the correct event
        /// </summary>
        /// <exception cref="InvalidOperationException"> Is thrown when the contained even is not a <see cref="DebugEventType.RipEvent"/></exception>
        /// <returns>If the contained event is <see cref="DebugEventType.RipEvent"/> returns a <see cref="DebugEventRipInfo"/> pointing to this event. </returns>
        public DebugEventRipInfo GetDebugEventRipInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.RipEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Rip Information", "Rip Struct" }));
            }
            AddRef();
            var ret = new DebugEventRipInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;
        }

        /// <summary>
        /// If this is an event of <see cref="DebugEventType.UnloadDllEvent"/>, return an instance of <see cref="DebugEventUnloadDllInfo"/>
        /// </summary>
        /// <returns>returns instance of <see cref="DebugEventUnloadDllInfo"/> if the contained event matches. Throws <see cref="InvalidOperationException"/> if not correct type</returns>
        /// <exception cref="InvalidOperationException">This is thrown if the event is not the correct type</exception>
        public DebugEventUnloadDllInfo GetDebugEventUnloadDllInfo()
        {
            if (DebugEventNative.DebugEvent_GetEventType(Native) != DebugEventType.UnloadDllEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "DLL free/unload Information", " Unload DLL Event" }));
            }
            AddRef();
            var ret = new DebugEventUnloadDllInfo(Native, FreeOnCleanup, ReferenceCounter);
            if (ret == null)
            {
                Release();
            }
            return ret;
        }

        #endregion
        #region C# boiler plate dispose and the underlying struct




        /// <summary>
        /// The destructor that calls the dispose routine
        /// </summary>
        ~DebugEvent()
         {
             // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
             Dispose(disposing: false);
         }


        #endregion
 
    }
}
