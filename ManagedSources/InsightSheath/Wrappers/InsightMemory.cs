using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.NativeImports;
using InsightSheath.Structs;

namespace InsightSheath.Wrappers
{
    /// <summary>
    /// Get memory data about an arbitrary process of your choice
    /// </summary>
    public class InsightMemory : NativeStaticContainer
    {
        public static InsightMemory CreateInstance()
        {
            return new InsightMemory(NativeMethods.InsightMemory_MakeInstance());
        }

        public static InsightMemory CreateInstance(IntPtr Handle)
        {
            var ret = new InsightMemory(NativeMethods.InsightMemory_MakeInstance());
            ret.SetTargetProcess(Handle);
            return ret;
        }

        public static InsightMemory CreateInstance(uint Process)
        {
            var ret = new InsightMemory(NativeMethods.InsightMemory_MakeInstance());
            ret.SetTargetProcess(Process);
            return ret;
        }
        public static InsightMemory DuplicateInstance(InsightMemory Other)
        {
            return new InsightMemory(NativeImports.NativeMethods.InsightMemory_DupInstance(Other.Native));
        }

        public InsightMemory(IntPtr that): base(that)
        {
            if (that == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(that));
            }
        }

        public InsightMemory(IntPtr that, bool FreeOnCleanup): base(that, false)
        {
            if (that == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(that));
            }
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            NativeImports.NativeMethods.InsightMemory_KillInstance(Native);
            GC.SuppressFinalize(this);
        }

        public bool AutoUpdateMemory
        {
            get
            {
                return NativeMethods.InsightMemory_GetAutoRefreshMemoryStats(Native);
            }
            set
            {
                NativeMethods.InsightMemory_SetAutoRefreshMemoryStats(Native, value);
            }
        }

        public bool SetTargetProcess(IntPtr Handle)
        {
            return NativeImports.NativeMethods.InsightMemory_SetTargetProcess(Native, (ulong)Handle, true);
        }
        public bool SetTargetProcess(uint ProcessId)
        {
            return NativeImports.NativeMethods.InsightMemory_SetTargetProcess(Native, (ulong)ProcessId, false);
        }

        public bool ManualRefreshStats()
        {
            return NativeMethods.InsightMemory_UpdateMemoryStats(Native);
        }

        public ulong PageFaultCount
        {
            get
            {
                return NativeMethods.InsightMemory_GetPageFault(Native);
            }
        }

        public ulong PeakWorkingSet
        {
            get
            {
                return NativeMethods.InsightMemory_GetPeakWorkingSetSize(Native);
            }
        }

        public ulong WorkingSet
        {
            get
            {
                return NativeMethods.InsightMemory_GetWorkingSetSize(Native);
            }
        }

        public ulong QuotaPeakPagedPool
        {
            get
            {
                return NativeMethods.InsightMemory_GetQuotaPeakPagedPoolUsage(Native);
            }
        }


        public ulong PeakPagedPool
        {
            get
            {
                return NativeMethods.InsightMemory_GetPeakPagePoolUsage(Native);
            }
        }


        public ulong PageFileUsage
        {
            get
            {
                return NativeMethods.InsightMemory_GetPageFileUsage(Native);
            }
        }

        public ulong PeakPageFileUsage
        {
            get
            {
                return NativeMethods.InsightMemory_GetPeakPagePoolUsage(Native);
            }
        }



        public ulong PrivateDataUsage
        {
            get
            {
                return NativeMethods.InsightMemory_GetPrivateUsage(Native);
            }
        }

        public ProcessMemoryCount32? MemoryStatsBulk
        {
            get
            {
                IntPtr ret = NativeMethods.InsightMemory_GetMemoryStatsBulk(Native);
                if (ret != IntPtr.Zero)
                {
                    return Marshal.PtrToStructure<ProcessMemoryCount32>(ret);
                }
                return null;
            }
        }


    }
}
