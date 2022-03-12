using FileSandBoxSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBoxSheath.Wrappers
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
    /// </summary>
    public class ThreadContext : NativeStaticContainer
    {

        public static ThreadContext CreateInstance(uint threadId)
        {
            return new ThreadContext(NativeMethods.ThreadContext_CreateInstance(new IntPtr(threadId), 1));
        }

        public static ThreadContext CreateInstance(IntPtr ThreadHandle)
        {
            return new ThreadContext(NativeMethods.ThreadContext_CreateInstance(ThreadHandle, 0));
        }

        /// <summary>
        /// This links an instance of the ThreadContext on the native side with your Managed code
        /// </summary>
        /// <param name="Native"></param>
        public ThreadContext(IntPtr Native) : base(Native)
        {
           
        }

        /// <summary>
        /// This 
        /// </summary>
        /// <param name="Native"></param>
        /// <param name="FreeOnCleanup">Ignored. Always false.  Dispose() has been overrwritten to safely kill the instance</param>
        public ThreadContext(IntPtr Native, bool FreeOnCleanup): base(Native, false)
        {

        }
        /// <summary>
        /// call the proper dispoal routine for ThreadContext
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
        /// Set the thread this class is handling based on thread paramemter. Once set, you may do what you will with  the original handle
        /// </summary>
        /// <param name="ThreadHandleNative"></param>
        /// <returns>returns true if the call worked and false if it did not</returns>
        public bool SetTargetThread(IntPtr ThreadHandleNative)
        {
            return NativeMethods.ThreadContext_SetTargetThread(Native, ThreadHandleNative, ThreadConext_Arguments.UseHandle);
        }

        /// <summary>
        /// Set the thread this class is handling based on thread id.
        /// </summary>
        /// <param name="ThreadId"></param>
        /// <returns></returns>
        public bool SetTargetThread(uint ThreadId)
        {
            return NativeMethods.ThreadContext_SetTargetThread(Native,  (IntPtr) ThreadId, ThreadConext_Arguments.UseThreadId);
        }
        /// <summary>
        /// retrieve or set the thread's description. WARNING May Heap Corruption on Getting value. 
        /// Current fix is in debug and it just does not call local free per the MSDN documentation BUT does not cause appear memory leak after looping thru code to get/set 1000000 times so either so ¯\_(ツ)_/¯
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
                NativeImports.NativeMethods.ThreadContext_SetTheadDescriptionW(Native, value);
            }
        }
        
        /// <summary>
        /// Retrieve or set the thread's priority 
        /// </summary>
        public ThreadPriority ThreadPriority
        {
            get
            {
                return (ThreadPriority)NativeMethods.ThreadContext_GetPriority(Native);
            }
            set
            {
                NativeMethods.ThreadContext_SetPriority(Native, (int)value);
            }
        }

        /// <summary>
        /// When the Thread was created. Note this Refershs ALL <see cref="ThreadTimeCreationTime"/>, <see cref=ThreadTimeExitTime"/>,  <see cref="ThreadTimeUserTime"/>, <see cref=ThreadTimeKernelTime"/> values and returns the one you want
        /// </summary>
        public long ThreadTimeCreationTime
        {
            get
            {
                return  (long)NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeCreationTime(Native));
            }
        }

        /// <summary>
        /// If thread active,  (defined as msdn's getThreadExitCode gets 259 for a exit code), returns 0 instead of ExitTime,  Note this Refershs ALL <see cref="ThreadTimeCreationTime"/>, <see cref=ThreadTimeExitTime"/>,  <see cref="ThreadTimeUserTime"/>, <see cref=ThreadTimeKernelTime"/> values and returns the one you want
        /// </summary>
        public long ThreadTimeExitTime
        {
            get
            {
                return (long) NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeExitTime(Native));
            }
        }

        /// <summary>
        /// Amount of Time that the thread runs user level code,  Note this Refershs ALL <see cref="ThreadTimeCreationTime"/>, <see cref=ThreadTimeExitTime"/>,  <see cref="ThreadTimeUserTime"/>, <see cref=ThreadTimeKernelTime"/> values and returns the one you want
        /// </summary>
        public long ThreadTimeUserTime
        {
            get
            {
                return (long)NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeUserTime(Native));
            }
        }

        /// <summary>
        /// Amount of Time  that the thread runs kernel level code,  Note this Refershs ALL <see cref="ThreadTimeCreationTime"/>, <see cref=ThreadTimeExitTime"/>,  <see cref="ThreadTimeUserTime"/>, <see cref=ThreadTimeKernelTime"/> values and returns the one you want
        /// </summary>
        public ulong ThreadTimeKernelTime
        {
            get
            {
                return NativeMethods.Peek8(NativeMethods.ThreadContext_GetThreadTimeKernelTime(Native));
            }
        }

    }
}
