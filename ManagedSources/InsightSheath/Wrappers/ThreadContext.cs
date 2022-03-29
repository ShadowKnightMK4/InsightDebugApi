using InsightSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Wrappers
{
    /// <summary>
    /// Possible Thread Priorities.  <see href="https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadpriority"/> 
    /// </summary>
    public enum ThreadPriority:int
    {
        /// <summary>
        /// Current thread only:  Set background status
        /// </summary>
        BackgroundBegin = 0x0010000,
        /// <summary>
        /// Current thread only: end background status
        /// </summary>
        BackgroundEnd =   0x0020000,
        /// <summary>
        /// About Normal Priority
        /// </summary>
        AboveNormal = 1,
        /// <summary>
        /// Below Normal Priority
        /// </summary>
        BelowNormal = -1,
        /// <summary>
        /// Highest Priority 
        /// </summary>
        Highest = 2,
        /// <summary>
        /// Idle Priority
        /// </summary>
        Idle = -15,
        /// <summary>
        /// Lowest Priority
        /// </summary>
        Lowest = -2,
        /// <summary>
        /// Normal
        /// </summary>
        Normal = 0,
        /// <summary>
        /// Time Critical Priority
        /// </summary>
        Critical = 15

    }

    
    /// <summary>
    /// ThreadContext is a class indented for reading/writing information about threads receives in the main PsContext structure
    /// We duplicate some of the functionality of the <see cref="ProcessThread"/> class for C# user familiarity. 
    /// </summary>
    public class ThreadContext : NativeStaticContainer
    {


        /// <summary>
        /// Create a new instance of <see cref="ThreadContext"/> and point it to the passed thread whose id is this.
        /// </summary>
        /// <param name="threadId">id of thread to point this class to</param>
        public static ThreadContext CreateInstance(uint threadId)
        {
            
            return new ThreadContext(NativeMethods.ThreadContext_CreateInstance(new IntPtr(threadId), 1));
        }

        /// <summary>
        /// Create a new instance of <see cref="ThreadContext"/> and point it to the thread specified by this handle. You don't need the handle afterwards.
        /// </summary>
        /// <param name="ThreadHandle">Win32 Handle to thread you want to point this class instance to. You don't need the handle after this.</param>
        public static ThreadContext CreateInstance(IntPtr ThreadHandle)
        {
            return new ThreadContext(NativeMethods.ThreadContext_CreateInstance(ThreadHandle, 0));
        }

        /// <summary>
        /// This links an instance of the ThreadContext on the native side with your Managed code.  Do NOT USE for instancing a new context
        /// </summary>
        /// <param name="Native"></param>
        /// <exception cref="ArgumentNullException">If passing null, this is thrown</exception>
        public ThreadContext(IntPtr Native) : base(Native)
        {
            if (NativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(Native), new Exception(nameof(Native) + "Cannot be 0.  Use ThreadContext.CreateInstance() to make a new instance and use this to point this managed wrapper to a native instance of the class."));
            }
            
        }

        /// <summary>
        /// This is required because of how the parent child classes work, Same as <see cref="ThreadContext.ThreadContext(IntPtr)"/> 
        /// </summary>
        /// <param name="Native"></param>
        /// <param name="FreeOnCleanup">Ignored. Always false.  Dispose() has been overwritten to safely kill the instance</param>
        public ThreadContext(IntPtr Native, bool FreeOnCleanup): base(Native, false)
        {
            if (NativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(Native) + "Cannot be 0.  Use ThreadContext.CreateInstance() to make a new instance and use this to point this managed wrapper to a native instance of the class.");
            }
        }
        /// <summary>
        /// call the proper disposal routine for ThreadContext
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {


            }
            NativeMethods.ThreadContext_KillInstance(Native);
            base.Dispose(disposing);
        }


        /// <summary>
        /// Set the thread this class is handling based on thread parameter. Once set, you may do what you will with  the original handle
        /// </summary>
        /// <param name="ThreadHandleNative"></param>
        /// <returns>returns true if the call worked and false if it did not</returns>
        public bool SetTargetThread(IntPtr ThreadHandleNative)
        {
            return NativeMethods.ThreadContext_SetTargetThread(Native, ThreadHandleNative, ThreadConext_Arguments.UseHandle);
        }

        /// <summary>
        /// Set the thread this class is handling based on thread id. Discards existing info that this class knowns about for the original thread (if any)
        /// </summary>
        /// <param name="ThreadId"></param>
        /// <returns></returns>
        public bool SetTargetThread(uint ThreadId)
        {
            return NativeMethods.ThreadContext_SetTargetThread(Native,  (IntPtr) ThreadId, ThreadConext_Arguments.UseThreadId);
        }
        /// <summary>
        /// retrieve or set the thread's description. WARNING May Heap Corruption on Getting value. 
        /// Current fix is in Native Debug Build and it just does not call local free per the MSDN documentation BUT does not cause appear memory leak after looping thru code to get/set 1000000 times so either so ¯\_(ツ)_/¯
        /// Want to ensure the fix follows the documentation on MSDN unless MSDN is wrong.
        /// </summary>
        public string ThreadDescription
        {
            get
            {
                return NativeMethods.ThreadContext_GetThreadDescriptionW(Native);
            }
            set
            {
                NativeMethods.ThreadContext_SetTheadDescriptionW(Native, value);
            }
        }

        
        /// <summary>
        /// Get the ID of the thread this class is paired with.
        /// </summary>
        public uint ThreadId
        {
            get
            {
                return NativeMethods.ThreadContext_GetTargetThreadId(Native);
            }
        }

        /// <summary>
        /// Get or set the Ideal / preferred processor for this thread.
        /// </summary>
        public int IdealProcessor
        {
            get
            {
                return (int)NativeMethods.ThreadContext_GetIdealProcessor(Native);
            }
            set
            {
                _ = NativeMethods.ThreadContext_SetIdealProcessor(Native, (uint)value);
            }
        }
    
        public ThreadPriorityLevel ThreadPriority
        {
            get
            {
                return (ThreadPriorityLevel)NativeMethods.ThreadContext_GetPriority(Native);
            }
            set
            {
                NativeMethods.ThreadContext_SetPriority(Native, (int)value);
            }
        }

        /// <summary>
        /// Get or set the value indicating if Windows should temporarily boost the thread's priority if a window that the thread handles receives focus
        /// </summary>
        public bool PriorityBoost
        {
            get
            {
                return NativeMethods.ThreadContext_GetThreadPriorityBoostSetting(Native);
            }
            set
            {
                NativeMethods.ThreadContext_SetThreadPriorityBoostSetting(Native, value);
            }
        
        }


        /// <summary>
        /// Get or set processor affinity / desired process this thread runs on. Warning. Does not work support more than 64-bit logical processors
        /// </summary>

        public UInt32 ProcessorAffinity
        {
            get
            {
                return NativeMethods.ThreadContext_GetThreadProcessAffinityMask(Native);
            }
            set
            {
                NativeMethods.ThreadContext_SetThreadProcessAffinityMask(Native, value);
            }
        }

        /// <summary>
        /// NOT IMPLEMENTED
        /// </summary>
        public IntPtr ThreadStartAddress
        {
            get
            {
                throw new NotImplementedException(nameof(ThreadStartAddress));
            }
        }


        /// <summary>
        /// Here for future.  NOT IMPLEMENTED for this class. TODO: Add this feature with support of the native DLL
        /// </summary>
        public ThreadState ThreadState
        {
            get 
            {
                throw new NotImplementedException(nameof(ThreadState));
            }
            set
            {
               throw new NotImplementedException(nameof(ThreadState));
            }
        }

        /// <summary>
        /// Here for future.  NOT IMPLEMENTED for this class. TODO: Add this feature with support of the native DLL
        /// </summary>
        public ThreadWaitReason ThreadWaitReason
        {
            get
            {
                throw new NotImplementedException(nameof(ThreadWaitReason));
            }
            set
            {
                throw new NotImplementedException(nameof(ThreadWaitReason));
            }
        }
        /// <summary>
        /// When the Thread was created. Note this Refresh ALL items <see cref="ThreadCreationTime"/>, <see cref=ThreadExitTime"/>,  <see cref="UserProcessorTime"/>, <see cref=KernelProcessorTime"/> values and returns the one you want
        /// </summary>
        /// <remarks>Because of performance reasons in watching in debugger browsing/viewing - this is not browsable. Use <see cref="Th"/></remarks>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public TimeSpan ThreadCreationTime
        {
            get
            {
                return  new TimeSpan((long)NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeCreationTime(Native)));
            }
        }

        /// <summary>
        /// If thread active,  (defined as MSDN's getThreadExitCode gets 259 for a exit code), returns 0 instead of ExitTime,  Note this Refershs ALL <see cref="ThreadCreationTime"/>, <see cref=ThreadExitTime"/>,  <see cref="UserProcessorTime"/>, <see cref=KernelProcessorTime"/> values and returns the one you want
        /// </summary>
        
        public TimeSpan ThreadExitTime
        {
            get
            {
                return new TimeSpan((long)NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeExitTime(Native)));
            }
        }

        /// <summary>
        /// Amount of Time that the thread runs user level code,  Note this Refreshes ALL <see cref="ThreadCreationTime"/>, <see cref=ThreadExitTime"/>,  <see cref="UserProcessorTime"/>, <see cref=KernelProcessorTime"/> values and returns the one you want
        /// </summary>
        
        public TimeSpan UserProcessorTime
        {
            get
            {
                return new TimeSpan((long)NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeUserTime(Native)));
            }
        }

        /// <summary>
        /// Amount of Time  that the thread runs kernel level code,  Note this Refershs ALL <see cref="ThreadCreationTime"/>, <see cref=ThreadExitTime"/>,  <see cref="UserProcessorTime"/>, <see cref=KernelProcessorTime"/> values and returns the one you want
        /// </summary>
        
        public TimeSpan KernelProcessorTime
        {
            get
            {
                return new TimeSpan((long)NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeKernelTime(Native)));
            }
        }

        public TimeSpan TotalProcessorTime
        {
            get
            {
                return KernelProcessorTime + UserProcessorTime;
            }
        }
    }
}
