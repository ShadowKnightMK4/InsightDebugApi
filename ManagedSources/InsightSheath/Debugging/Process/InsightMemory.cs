using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.NativeImports;
using InsightSheath.Win32Struct;
using InsightSheath.Abstract;

namespace InsightSheath.Debugging.Process
{
    /// <summary>
    /// Get memory data about an arbitrary process of your choice
    /// </summary>
    public class InsightMemory : NativeStaticContainer
    {
        /// <summary>
        /// Create an instance of <see cref="InsightMemory"/> from the unmanaged DLL and return a pointer encapsulated in the .NET class.
        /// </summary>
        /// <returns>Returns an instance of <see cref="InsightMemory"/> already set to point to the unmanaged DLL's code</returns>
        public static InsightMemory CreateInstance()
        {
            return new InsightMemory(InternalInsightMemory.MakeInstance());
        }

        /// <summary>
        /// Duplicate an instance of <see cref="InsightMemory"/> from the passed pointer.  
        /// </summary>
        /// <param name="Handle"></param>
        /// <returns>Returns an instance of <see cref="InsightMemory"/> already set to point to the same process that the passed pointer. Each instance can separately point to other pointers</returns>
        public static InsightMemory CreateInstance(IntPtr Handle)
        {
            var ret = new InsightMemory(InternalInsightMemory.MakeInstance());
            ret.SetTargetProcess(Handle);
            return ret;
        }

        /// <summary>
        /// Create an instance of <see cref="InsightMemory"/> and point it to read about the passed process.
        /// </summary>
        /// <param name="Process">process id to read about. It must be able to be opened for PROCESS_QUERY_INFORMATION and PROECSS_VM_READ </param>
        /// <returns>Returns an instance of <see cref="InsightMemory"/> already set to point to the unmanaged DLL's code. The class is pointed to use the passed process</returns>
        public static InsightMemory CreateInstance(uint Process)
        {
            var ret = new InsightMemory(InternalInsightMemory.MakeInstance());
            ret.SetTargetProcess(Process);
            return ret;
        }
        /// <summary>
        /// Duplicate an instance of <see cref="InsightMemory"/> from an existing copy.
        /// </summary>
        /// <param name="Other">separate instance of <see cref="InsightMemory"/></param>
        /// <returns>Returns a separate  instance of <see cref="InsightMemory"/> already set.</returns>
        public static InsightMemory DuplicateInstance(InsightMemory Other)
        {
            return new InsightMemory(InternalInsightMemory.DupInstance(Other.Native));
        }
        
        /// <summary>
        /// Create an instance of the wrapper class and point to the native pointer for the native side of the class.
        /// </summary>
        /// <param name="that">Non-null pointer</param>
        /// <exception cref="ArgumentNullException">Raises this if argument is null</exception>
        public InsightMemory(IntPtr that): base(that)
        {
            if (that == IntPtr.Zero)
            {
                //throw new ArgumentNullException(nameof(that));
                //throw new ArgumentNullException(WrapperConstructorReceivedNullPointerErrorMsg("Argument", this.GetType().Name + ".CreateInstance", nameof(Native)));
                throw WrapperConstructorReceivedNullPointerErrorException("Argument", GetType().Name + ".CreateInstance", nameof(Native));
            }
        }

        /// <summary>
        /// Create an instance of the wrapper class and point to the native pointer for the native side of the class.
        /// </summary>
        /// <param name="that">Non-null pointer</param>
        /// <param name="FreeOnCleanup">Ignored</param>
        /// <exception cref="ArgumentNullException">Raises this if argument is null</exception>
        public InsightMemory(IntPtr that, bool FreeOnCleanup): base(that, false)
        {
            if (that == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(that));
            }
        }

        /// <summary>
        /// Invoke the proper clean up routine for <see cref="InsightMemory"/> for unmanaged resources and also managed if dispose is true
        /// </summary>
        /// <param name="disposing">set to true if disposing of managed resources also.</param>
        protected override void Dispose(bool disposing)
        {
            if (!IsDisposed)
            {
                if (FreeOnCleanup)
                {
                    if (Native != IntPtr.Zero)
                    {
                        InternalInsightMemory.KillInstance(Native);
                        ClearNative();
                    }
                }
            }
            
            base.Dispose(disposing);
        }

        /// <summary>
        /// Control if each call to one of these values will result in another call to get an updated value. The default is true; however, that can BE A PERFORMANCE PENALTY if you're getting the value individually for each setting frequently in a loop.
        /// Recommend False if you're reading them a lot
        /// </summary>
        public bool AutoUpdateMemory
        {
            get
            {
                return InternalInsightMemory.GetAutoRefreshMemoryStats(Native);
            }
            set
            {
                InternalInsightMemory.SetAutoRefreshMemoryStats(Native, value);
            }
        }


        /// <summary>
        /// Return the x86 bit version of the underlying structure containing the memory statistics.
        /// </summary>
        /// <remarks>Know if you're asking about an x86 or x64 process. While this shouldn't crash, grabbing the wrong one will produce garbage values</remarks>
        public ProcessMemoryCount32? MemoryStatsBulk32
        {
            get
            {
                IntPtr ret =  InternalInsightMemory.InsightMemory_GetMemoryStatsBulk(Native);
                if (ret != IntPtr.Zero)
                {
                    return Marshal.PtrToStructure<ProcessMemoryCount32>(ret);
                }
                return null;
            }
        }

        /// <summary>
        /// Return the x64 bit version of the underlying structure containing the memory statistics.
        /// </summary>
        /// <remarks>Know if you're asking about an x86 or x64 process. While this shouldn't crash, grabbing the wrong one will produce garbage values</remarks>
        public ProcessMemoryCount64? MemoryStatsBulk64
        {
            get
            {
                IntPtr ret = InternalInsightMemory.InsightMemory_GetMemoryStatsBulk(Native);
                if (ret != IntPtr.Zero)
                {
                    return Marshal.PtrToStructure<ProcessMemoryCount64>(ret);
                }
                return null;
            }
        }
        /// <summary>
        /// Choose which process this instance of the class is going to ask about via a Win32 Handle.
        /// </summary>
        /// <param name="Handle">Win32 Native Handle that should at least have PROCESS_QUERY_INFORMATION and likely PROCESS_VM_READ. Handle is duplicated to a private variable in the native side. You don't need to keep this handle open afterwards</param>
        /// <returns>true if we successfully switched to this handle and false otherwise.</returns>
        /// <remarks>The Native routine duplicates the handle for PROCESS_QUERY_INFORMATION and PROCES_VM_READ. You do not need the keep the handle open afterwards</remarks>
        public bool SetTargetProcess(IntPtr Handle)
        {
            return InternalInsightMemory.SetTargetProcess(Native, (ulong)Handle, true);
        }

        /// <summary>
        /// Choose which process this instance of the class is going to ask about via a process Handle.
        /// </summary>
        /// <param name="ProcessId">The process should be able to be opened for PROCESS_QUERY_INFOMRAITON and likely PROCES_VM_READ.  Handle is kept in a private variable for the life of the class until the Native copy is deleted</param>
        /// <returns>true if we successfully switched to this handle and false otherwise.</returns>
        /// <remarks>The Native routine opens the process for PROCESS_QUERY_INFORMATION and PROCES_VM_READ. You do not need the keep the handle open afterwards</remarks>
        public bool SetTargetProcess(uint ProcessId)
        {
            return InternalInsightMemory.SetTargetProcess(Native, ProcessId, false);
        }

        /// <summary>
        /// Trigger a manual refresh of the Memory information. Note if <see cref="AutoUpdateMemory"/> is true, this routine does NOT need to be called.
        /// </summary>
        /// <returns>returns true if it worked and false if it did not</returns>
        public bool ManualRefreshStats()
        {
            return InternalInsightMemory.UpdateMemoryStats(Native);
        }

        /// <summary>
        /// Get the number of page faults for the targeted process.
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong PageFaultCount
        {
            get
            {
                return InternalInsightMemory.GetPageFaultCount(Native);
            }
        }

        /// <summary>
        /// Get the peak working set size, in bytes for the targeted process. 
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong PeakWorkingSetSize
        {
            get
            {
                return InternalInsightMemory.GetPeakWorkingSetSize(Native);
            }
        }
        /// <summary>
        /// Get the current working set size in bytes for the targeted process
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong WorkingSetSize
        {
            get
            {
                return InternalInsightMemory.GetWorkingSetSize(Native);
            }
        }

        /// <summary>
        /// Get the peak paged pool usage in bytes for the targeted process
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong QuotaPeakPagedPoolUsage
        {
            get
            {
                return InternalInsightMemory.GetQuotaPeakPagedPoolUsage(Native);
            }
        }
        /// <summary>
        /// Get the current paged pool usage in bytes for the targeted process
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong QuotaPagedPoolUsage
        {
            get
            {
                return InternalInsightMemory.GetQuotaPagePoolUsage(Native);
            }
        }
        /// <summary>
        /// Get the peak non paged pool usage in bytes.
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong QuotaPeakNonPagedPoolUsage
        {
            //InsightMemory_GetPeakNonPagePoolUsage
            get
            {
                return InternalInsightMemory.GetPeakNonPagePoolUsage(Native);
            }
        }

        /// <summary>
        /// Get the current non paged pool usage in bytes.
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong QuotaNonPagedPoolUsage
        {
            get
            {
                return InternalInsightMemory.GetQuotaNonPagedPoolUsage(Native);
            }
        }

        /// <summary>
        /// Get the Commit Charge for the targeted process in bytes. This  is the total private amount of memory allocated to the targeted process by Windows.
        /// </summary>
        /// <remarks>In a preWidows 7/ Server 2012 R2 World this is always zero. Please use <see cref="PrivateUsage"/> instead. Don't forget to  <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong PageFileUsage
        {
            get
            {
                return InternalInsightMemory.GetPageFileUsage(Native);
            }
        }
        /// <summary>
        /// Get the peak value in bytes  of the Commit Charge during the targeted process's lifetime.
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong PeakPageFileUsage
        {
            get
            {
                return InternalInsightMemory.GetPeakPageFileUsage(Native);
            }
        }

        /// <summary>
        /// Same as <see cref="PageFileUsage"/> in a post Windows 7+/Windows Server 2012+ world per MSDN documentation
        /// </summary>
        /// <remarks>Call <see cref="ManualRefreshStats"/> if <see cref="AutoUpdateMemory"/> is false get up to date values.</remarks>
        public ulong PrivateUsage
        {
            get
            {
                return InternalInsightMemory.GetPrivateUsage(Native);
            }
        }
    }
}
