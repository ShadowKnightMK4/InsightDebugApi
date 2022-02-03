using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FileSandBoxSheath.NativeImports;
using FileSandBoxSheath;
using FileSandBoxSheath.Remote;
using System.Runtime.InteropServices;

namespace FileSandBoxSheath.Wrappers
{


    ///
    /// This file provides a wrapper between c# and the filesandboxapi.dll that deals with the debug event structure.
    /// The base class <see cref="DebugEventStaticContainer"/> is the root for the other classes in this file. It
    /// handles holding an <see cref="IntPtr"/> that represents the unmanged DEBUG_EVENT structue.  This pointer gets
    /// passes to various routines in the FileSandBoxApi dll which then return data bases on that pointer - usually about 
    /// the DEBUG_EVENT struct.  Each DEBUG_EVENT in the MSDN documentation has its own C# class <see cref="DebugEventType"/>
    /// About Exceptions
    ///     The MSDN docmentation has certain exceptions listed. Those have been added to the <see cref="DebugExceptionTypes"/> enum.
    ///     That's not actually exhaustive.  winbase.h lists MANY MANY MANY more.



    ///<summary>
    /// Type of In Page exception flags.  You can check if the excepture is that at <see cref="DebugEventExceptionInfo.ExceptionCode"/>
    /// </summary>
    public enum DebugExceptionInPage_Type
    {
        /// <summary>
        /// Target process thread with the <see cref="DebugExceptionTypes.InPageError"/> tried reading something it's not allowed too
        /// </summary>
        ReadException = 0,
        /// <summary>
        /// Target process thead with the <see cref="DebugExceptionTypes.InPageError"/> tried writing to something it's not allowed too
        /// </summary>
        WriteException = 1,
        /// <summary>
        /// Target process thread had a user mode DEP voilation.
        /// </summary>
        DepException = 8,
    }

    ///<summary>
    /// Enum for the exceptions specifies within the msdn documentation.  NOT EXHAUSTIVE. Sourced from minwinbase.h in Visual studio
    /// </summary>
    public enum DebugExceptionTypes: uint
    {

        /// <summary>
        /// Target tried reading/ writing to virtual memory it is not allowed too.
        /// </summary>
         AccessViolation =  0xC0000005,
         /// <summary>
         /// Target tried accessing an array elemeny that's out of bounds (hardware supported bounds checking? according to MSDN documentation)
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
         /// MSDN:  float point opereatation resulted in avalue to small to be respented.
         /// </summary>
         FloatPoint_Denormal_Operand = 0xC000008D,
         /// <summary>
         /// Float divisision by zero
         /// </summary>
         Float_DivideByZero = 0xC000008E,
         /// <summary>
         /// MSDN Can't respsent a float as a decimal fraction precisilay
         /// </summary>
         Float_InexactResult = 0xC000008F,
         /// <summary>
         /// float's exponant is too big
         /// </summary>
         Float_Overflow = 0xC0000091,
         /// <summary>
         /// Float point value over or underflowed after an aoperation
         /// </summary>
         Float_StackCheck = 0xC0000092,
         /// <summary>
         /// Float point is less that the magnitiue allowed
         /// </summary>
         Float_Underflow = 0xC0000093,
         /// <summary>
         /// Invalid instruction attempted
         /// </summary>
         IllegalInstrution = 0xC000001D,
         /// <summary>
         /// MSDN: Thread tried to access  a page that's not present and systen couldn't load page
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
         /// Exeption handled returned anvalid disposition
         /// </summary>
         InvalidDisposition= 0xC0000026,
         /// <summary>
         /// Exception triggeed when attempting to continue an exception that says that's not possible
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
        /// Conosole app: CTRL-C pressing the the target app
        /// </summary>
         DebugConsoleControlC = 0xC000013A,



    }
    
    /// <summary>
    /// If you've forced the helper dll to be loaded in the target, you'll get these in addition to the <see cref="DebugExceptionTypes"/>
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
        /// Event is a load dll event. <see cref="DebugEventLoadDllInfo"/>
        /// </summary>
        LoadDllEvent = 6,
        /// <summary>
        /// Event is unload dll event.  <see cref="DebugEventUnloadDllInfo"/>
        /// </summary>
        UnloadDllEvent = 7,
        /// <summary>
        /// Event is output debug string (can use <see cref="RemoteStructure.RemoteReadDebugString(IntPtr, IntPtr)"/> OR <see cref="DebugEventStringInfo"/> which does it for you
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
        /// Invalid data passsed to function what caused app to fail
        /// </summary>
        SleError = 0x00000001,
        /// <summary>
        /// Invalid data passed to a function maybe won't cause the app to fail
        /// </summary>
        SleMinorError = 0x00000002,
        /// <summary>
        /// Potentially Invalid data passed to a function but function finished any.
        /// </summary>
        SleWarning = 0x00000003
    }





    /// <summary>
    /// Implements the basis of idiposal the other DebugEvent classes use.  Also exposes routines to read the ProcessID and ThreadID that triggered the event
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
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree(). Set TO FALSE if dealing with strucutres declared in C/C++ code vs dynamically allocated</param>
        public DebugEventStaticContainer(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {
            
        }



        /// <summary>
        /// private value containing if dispose() was callsed
        /// </summary>
        private bool disposedValue;
        /// <summary>
        /// Was this class disposed?
        /// </summary>
        public bool IsDisposed { get { return disposedValue; } }

        protected virtual void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }

                if (this.FreeOnCleanup)
                {
                    NativeImports.NativeMethods.SimpleFree(Native);
                }
                // TODO: set large fields to null
                disposedValue = true;
            }
            
        }

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
        /// Makes a couple of calls to GetNativeSystemInfo() and asks IsWow64Process() if the this processID is running under it
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
        /// Get the Raw handle to the file. You should probably close this if the handle is valid once you no longer need it. If it's valid, msdn says you'll have read and write access
        /// </summary>
        public IntPtr FileHandle
        {
            get
            {
                return NativeMethods.DebugEvent_LoadDllInfo_GetHFile(Native);
            }
        }

        /// <summary>
        /// Get the base address of where the dll was loaded in the debugged process.
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
        /// Is the string in the address space specfied here valid?
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
        /// return the debug info specified in the dll as a series of bytes. MSDN documentio nsays this should be codeview 4.0 format - whatever that is
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
                if (this.DebugInfoSize != 0)
                {
                    var ret = new byte[this.DebugInfoSize];
                    using (var fn = new FileStream(new Microsoft.Win32.SafeHandles.SafeFileHandle(FileHandle,false), FileAccess.ReadWrite))
                    {
                        fn.Seek(this.DebugInfoOffset, SeekOrigin.Begin);
                        fn.Read(ret, 0, ret.Length);
                        return ret;
                    }
                }
                return null;

            }
        }

        /// <summary>
        /// This ignores the imagename struct entiraly as seen on MSDN LOAD_DLL_DEBUG_EVENT.  We jutch fetch name asssuming the Hfile is valid. Should the system not include the file handole, null is returned instead
        /// </summary>
        public string ImageName
        {
            get
            {
                if (FileHandle != IntPtr.Zero)
                {
                    return HelperRoutines.GetFileNameViaHandle(FileHandle);
                }
                return null; ;
            }
        }
    }

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

        

        public uint[] ExceptionParamter32
        {
            get
            {
                List<uint> ret = new List<uint>();
                IntPtr ptr = (IntPtr)NativeMethods.DebugEvent_ExceptionInfo_GetExceptionInformation(Native);

                for (int step = 0; step < ExceptionParamterCount;step++)
                {
                    ret.Add((uint)Marshal.ReadInt32(ptr));
                    ptr = (IntPtr)(ptr.ToInt64() + 4);
                }
                return ret.ToArray();
            }
        }

        /// <summary>
        /// return the address where the excception happened in the 64 bit process.
        /// </summary>
        public ulong ExceptionAddress64
        {
            get
            {
                return NativeMethods.DebugEvent_ExceptionInfo_GetExceptionAddress(Native);
            }
        }

        /// <summary>
        /// return address where the esxception happend in the 32-bit process. Truncates the address if the process is 64-bit.
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
        /// Return the Exception for the the exception that trigged.
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
        /// if NOT Null (IntPtr.zero),  handle should have THREAD_GET_CONTEXT/ THREAD_SET_CONTEXT and THREAD_SUSPEND_RESUME access to the thread per msdn documnetion.
        /// </summary>
        public IntPtr ThreadHandle
        {
            get
            {
                return NativeMethods.DebugEvent_CreateThreadInfo_GetThreadHandle(Native);
            }
        }

        /// <summary>
        /// Get the aprox start address
        /// </summary>
        public IntPtr ThreadStartAddress
        {
            get
            {
                return NativeMethods.DebugEvent_CreateThreadInfo_GetThreadStartAddress(Native);
            }
        }

        /// <summary>
        /// get the thread locator base.   MSDN says offset 0x2C is the ThreadLocationStorage pointer which resumably would let you track what per thread stuff your target is doing.
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
        /// <exception cref="IOException"> IO Related excewptions can be thrown if a problem occure. <see cref="File.ReadAllBytes(string)"/> to see the list</exception>
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
        /// Return the string messaged contained within the debug event. It's converted to unicode if ANSI on the NativeSide
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
                    /// <see cref="RemoteStructure.RemoteReadDebugString(IntPtr, IntPtr)"/> is assumed to free the unamanged memory after reading it
                    ///
                    string Managed = RemoteStructure.RemoteReadDebugString(NativeHandle, Native);
                    NativeMethods.CloseHandle(NativeHandle);
                    return Managed;
                }
            }
        }
        
    }

    /// <summary>
    /// Providers a wrapper to excess the underlying structure. Assumes memory manamgement is done elsewhere.
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
        /// Get the Error number tha caused the debugging RIP event.
        /// </summary>
        public uint Error
            {
                get
                {
                    return NativeMethods.DebugEvent_RipGetError(Native);
                }
            }


        /// <summary>
        /// Should be one of the enums for <see cref="RipErrorType"/>. If It's zero, msdn documentation says only sdwError is set.
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
        /// Creation.  Does NOT free the undyling pointer 
        /// </summary>
        /// <param name="NativePtr"></param>
        public DebugEventUnloadDllInfo(IntPtr NativePtr) : base(NativePtr)
        {

        }

        /// <summary>
        /// The base address of the Dll that was previously loaded.
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
    /// This excapsolates the Native DebugEvent structure via calling exported C routines in filesandbox.dll.  IMPORTANT!!!! The LoadDLlDebugEvent does not close the passed handle if valid.
    /// </summary>
    public class DebugEvent : DebugEventStaticContainer
    {
        private static string error_msg_bad_event_fetch = "Attempt to fetch {0} from an event that does not contain the event {1}";
        /// <summary>
        /// 
        /// </summary>
        /// <param name="NativePtr">pass a native pointing containg a debug event structure.</param>
        /// <param name="FreeOnDispose">Set this to true for chunks explicity allocated to the structure by malloc() ext... </param>
        public DebugEvent(IntPtr NativePtr, bool FreeOnDispose) : base(NativePtr, FreeOnDispose)
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
        /// Creation.  Does NOT free the undyling pointer 
        /// </summary>
        /// <param name="NativePtr"></param>
        public DebugEvent(IntPtr NativePtr) : base(NativePtr)
        {

        }


        protected override void Dispose(bool disposing)
        {
            // we need t oclose the handle 
            if (NativeMethods.DebugEvent_GetEventType(Native) == DebugEventType.LoadDllEvent)
            {

            }
            base.Dispose(disposing);
        }



        #region Wrapper Creation


        /// <summary>
        /// Get the Debug Rip Info.  Will thru InvalidOperation if not the correct event
        /// </summary>
        /// <returns></returns>
        public DebugEventRipInfo GetDebugEventRipInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.RipEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] {"Rip Information", "Rip Struct" }));
            }
            return new DebugEventRipInfo(Native);
        }

        public DebugEventStringInfo GetDebugEventStringInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.OutputDebugString)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Debug String Information", " Debug String Struct" }));
            }
            
            return new DebugEventStringInfo(Native);
        }

        public DebugEventCreateProcessInfo GetDebugEventCreateProcessInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.CreateProcessEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Create Process Information", " Create Process Event" }));
            }
            return new DebugEventCreateProcessInfo(Native);
        }

        public DebugEventExceptionInfo GetDebugEventExceptionInfo()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.ExceptionEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Exception Information", " Exception Event" }));
            }
            return new DebugEventExceptionInfo(Native);
        }

        public DebugEventLoadDllInfo GetDebugEventLoadDll()
        {
            if (NativeMethods.DebugEvent_GetEventType(Native) != DebugEventType.LoadDllEvent)
            {
                throw new InvalidOperationException(string.Format(error_msg_bad_event_fetch, new object[] { "Dll Load Information", " Dll Load  Event" }));
            }
            return new DebugEventLoadDllInfo(Native);
        }
        #endregion
        #region C# boiler plate dispse and the underlying struct




        ~DebugEvent()
         {
             // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
             Dispose(disposing: false);
         }


        #endregion
 
    }
}
