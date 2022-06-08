using InsightSheath.Abstract;
using InsightSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Debugging.Thread
{

    
    /// <summary>
    /// ThreadContext is a class indented for reading/writing information about threads receives in the main <see cref="InsightProcess"/> structure
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
            return new ThreadContext(ThreadContextInternal.ThreadContext_CreateInstance(threadId, 1));
        }

        /// <summary>
        /// Create a new instance of <see cref="ThreadContext"/> and point it to the thread specified by this handle. You don't need the handle afterwards.
        /// </summary>
        /// <param name="ThreadHandle">Win32 Handle to thread you want to point this class instance to. You don't need the handle after this.</param>
        public static ThreadContext CreateInstance(IntPtr ThreadHandle)
        {
            return new ThreadContext(ThreadContextInternal.ThreadContext_CreateInstance((ulong)ThreadHandle.ToInt64(), 0));
        }

        /// <summary>
        /// This links an instance of the ThreadContext on the native side with your Managed code.  Use <see cref="CreateInstance()"/> to make a new instance. This constructor links your .Net Wrapper to a Native underlying instance of the C++ class.
        /// </summary>
        /// <param name="Native">A non null pointer to InsightApi's native class (currently housed in ThreadContext.cpp).</param>
        /// <exception cref="ArgumentNullException">If passing null, this is thrown</exception>
        public ThreadContext(IntPtr Native) : base(Native)
        {
            if (NativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(ConstructReceivedNullPointerOnConstructor_message("Argument", "ThreadContext.CreateInstance", nameof(Native)));
            }
            
        }

        /// <summary>
        /// This is required because of how the parent child classes work, Same as <see cref="ThreadContext.ThreadContext(IntPtr)"/> 
        /// </summary>
        /// <param name="Native"></param>
        /// <param name="FreeOnCleanup">Ignored. Always false.  Dispose() has been overwritten to safely kill the instance</param>
#pragma warning disable IDE0060 // Remove unused parameter
        public ThreadContext(IntPtr Native, bool FreeOnCleanup): base(Native, false)
#pragma warning restore IDE0060 // Remove unused parameter
        {
            if (NativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(ConstructReceivedNullPointerOnConstructor_message("Argument", "ThreadContext.CreateInstance", nameof(Native)));
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
            if (FreeOnCleanup)
            {
                ThreadContextInternal.ThreadContext_KillInstance(Native);
            }
            ClearNative();
            base.Dispose(disposing);
        }



        /// <summary>
        /// Suspend either a wow64 thread or x64 bit thread. Will trigger LoadLibrary calls the first time used on wow64
        /// </summary>
        /// <param name="ThreadHandleNative"></param>
        /// <returns>current suspend value. Values > 0 mean thread suspended.</returns>
        public uint SuspendThread()
        {
            return ThreadContextInternal.ThreadContext_SuspendThread(Native);
        }


        /// <summary>
        /// Resume either a wow64 thread or x64 bit thread. Will trigger LoadLibrary calls the first time used on wow64
        /// </summary>
        /// <param name="ThreadHandleNative"></param>
        /// <returns>current suspend value. Values > 0 mean thread suspended.</returns>
        public uint ResumeThread()
        {
            return ThreadContextInternal.ThreadContext_ResumeThread(Native);
        }

        /// <summary>
        /// Set the thread this class is handling based on thread parameter. Once set, you may do what you will with  the original handle
        /// </summary>
        /// <param name="ThreadHandleNative"></param>
        /// <returns>returns true if the call worked and false if it did not</returns>
        public bool SetTargetThread(IntPtr ThreadHandleNative)
        {
            return ThreadContextInternal.ThreadContext_SetTargetThread(Native, ThreadHandleNative, ThreadConext_Arguments.UseHandle);
        }

        /// <summary>
        /// Set the thread this class is handling based on thread id. Discards existing info that this class knowns about for the original thread (if any)
        /// </summary>
        /// <param name="ThreadId"></param>
        /// <returns></returns>
        public bool SetTargetThread(uint ThreadId)
        {
            return ThreadContextInternal.ThreadContext_SetTargetThread(Native,  (IntPtr) ThreadId, ThreadConext_Arguments.UseThreadId);
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
                return ThreadContextInternal.ThreadContext_GetThreadDescriptionW(Native);
            }
            set
            {
                ThreadContextInternal.ThreadContext_SetTheadDescriptionW(Native, value);
            }
        }

        
        /// <summary>
        /// Get the ID of the thread this class is paired with.
        /// </summary>
        public uint ThreadId
        {
            get
            {
                return ThreadContextInternal.ThreadContext_GetTargetThreadId(Native);
            }
        }

        /// <summary>
        /// Get or set the Ideal / preferred processor for this thread.
        /// </summary>
        public int IdealProcessor
        {
            get
            {
                return (int)ThreadContextInternal.ThreadContext_GetIdealProcessor(Native);
            }
            set
            {
                _ = ThreadContextInternal.ThreadContext_SetIdealProcessor(Native, (uint)value);
            }
        }
    
        public ThreadPriorityLevel ThreadPriority
        {
            get
            {
                return (ThreadPriorityLevel)ThreadContextInternal.ThreadContext_GetPriority(Native);
            }
            set
            {
                ThreadContextInternal.ThreadContext_SetPriority(Native, (int)value);
            }
        }

        /// <summary>
        /// Get or set the value indicating if Windows should temporarily boost the thread's priority if a window that the thread handles receives focus
        /// </summary>
        public bool PriorityBoost
        {
            get
            {
                return ThreadContextInternal.ThreadContext_GetThreadPriorityBoostSetting(Native);
            }
            set
            {
                ThreadContextInternal.ThreadContext_SetThreadPriorityBoostSetting(Native, value);
            }
        
        }


        /// <summary>
        /// Get or set processor affinity / desired process this thread runs on. Warning. Does not work support more than 64-bit logical processors
        /// </summary>

        public UInt32 ProcessorAffinity
        {
            get
            {
                return ThreadContextInternal.ThreadContext_GetThreadProcessAffinityMask(Native);
            }
            set
            {
                ThreadContextInternal.ThreadContext_SetThreadProcessAffinityMask(Native, value);
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
                return  new TimeSpan((long)LocalUnmanagedMemory.Peek8(ThreadContextInternal.ThreadContext_GetThreadTimeCreationTime(Native)));
            }
        }

        /// <summary>
        /// If thread active,  (defined as MSDN's getThreadExitCode gets 259 for a exit code), returns 0 instead of ExitTime,  Note this Refershs ALL <see cref="ThreadCreationTime"/>, <see cref=ThreadExitTime"/>,  <see cref="UserProcessorTime"/>, <see cref=KernelProcessorTime"/> values and returns the one you want
        /// </summary>
        
        public TimeSpan ThreadExitTime
        {
            get
            {
                return new TimeSpan((long)LocalUnmanagedMemory.Peek8(ThreadContextInternal.ThreadContext_GetThreadTimeExitTime(Native)));
            }
        }

        /// <summary>
        /// Amount of Time that the thread runs user level code,  Note this Refreshes ALL <see cref="ThreadCreationTime"/>, <see cref=ThreadExitTime"/>,  <see cref="UserProcessorTime"/>, <see cref=KernelProcessorTime"/> values and returns the one you want
        /// </summary>
        
        public TimeSpan UserProcessorTime
        {
            get
            {
                return new TimeSpan((long)LocalUnmanagedMemory.Peek8(ThreadContextInternal.ThreadContext_GetThreadTimeUserTime(Native)));
            }
        }

        /// <summary>
        /// Amount of Time  that the thread runs kernel level code,  Note this Refershs ALL <see cref="ThreadCreationTime"/>, <see cref=ThreadExitTime"/>,  <see cref="UserProcessorTime"/>, <see cref=KernelProcessorTime"/> values and returns the one you want
        /// </summary>
        
        public TimeSpan KernelProcessorTime
        {
            get
            {
                return new TimeSpan((long)LocalUnmanagedMemory.Peek8(ThreadContextInternal.ThreadContext_GetThreadTimeKernelTime(Native)));
            }
        }

        public TimeSpan TotalProcessorTime
        {
            get
            {
                return KernelProcessorTime + UserProcessorTime;
            }
        }

        /// <summary>
        /// Retreieve a pointer to either a wow64_context or a Context struct depending on the thread's type. 
        /// </summary>
        public IntPtr Context
        {
            get
            {
                return ThreadContextInternal.ThreadContext_GetContext(Native);
            }
            set
            {
                ThreadContextInternal.ThreadContext_SetContext(Native, value);
            }
        }

        public IntPtr Wow64Context
        {
            get
            {
                return ThreadContextInternal.ThreadContext_GetWow64Context(Native);
            }
            set
            {
                ThreadContextInternal.ThreadContext_SetWow64Context(Native, value);
            }
        }
    }
}
