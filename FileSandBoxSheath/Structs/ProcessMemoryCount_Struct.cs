using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Structs
{
    /// <summary>
    /// Process Memory count for 32-bit process
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct ProcessMemoryCount32
    {
        /// <summary>
        /// Size of this structure in unmanaged land.
        /// </summary>
        public uint cb;
        /// <summary>
        /// Number of Page Faults for a given process.
        /// </summary>
        public uint PageFaultCount;
        /// <summary>
        /// Peaked working set size, in bytes
        /// </summary>
        public uint PeakWorkingSetSize;
        /// <summary>
        /// Working set size, in bytes
        /// </summary>
        public uint WorkingSetSize;
        /// <summary>
        /// peaked page pool usage, in bytes
        /// </summary>
        public uint QuotaPeakedPagedPoolUsage;
        /// <summary>
        /// current page pool usage, in bytes
        /// </summary>
        public uint QuotaPagedPoolUsage;
        /// <summary>
        /// peaked non paged pool usage, in bytes
        /// </summary>
        public uint QuotaPeakedNonPagePoolUsage;
        /// <summary>
        /// current non page pool usage, in bytes
        /// </summary>
        public uint QuotaNonPagePoolUsage;
        /// <summary>
        /// Commit change value for the process in bytes.  Note: MSDN Windows 7/ Server 2008 says check <see cref="PrivateUsage"/> instead
        /// </summary>
        public uint PageFileUsage;
        /// <summary>
        /// Peak value of committed change during process lifetime
        /// </summary>
        public uint PeakPageFileUsaged;
        /// <summary>
        /// same as <see cref="PageFileUsage"/>,  Current Commit Charge value in butes for this process.  This is the amount that the memory manage as allocated for this process.
        /// </summary>
        public uint PrivateUsage;
    }

    /// <summary>
    /// Process Memory count for 64-bit process
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct ProcessMemoryCount64
    {
        /// <summary>
        /// Size of this structure in unmanaged land.
        /// </summary>
        public ulong cb;
        /// <summary>
        /// Number of Page Faults for a given process.
        /// </summary>
        public ulong PageFaultCount;
        /// <summary>
        /// Peaked working set size, in bytes
        /// </summary>
        public ulong PeakWorkingSetSize;
        /// <summary>
        /// Working set size, in bytes
        /// </summary>
        public ulong WorkingSetSize;
        /// <summary>
        /// peaked page pool usage, in bytes
        /// </summary>
        public ulong QuotaPeakedPagedPoolUsage;
        /// <summary>
        /// current page pool usage, in bytes
        /// </summary>
        public ulong QuotaPagedPoolUsage;
        /// <summary>
        /// peaked non paged pool usage, in bytes
        /// </summary>
        public ulong QuotaPeakedNonPagePoolUsage;
        /// <summary>
        /// current non page pool usage, in bytes
        /// </summary>
        public ulong QuotaNonPagePoolUsage;
        /// <summary>
        /// Commit change value for the process in bytes.  Note: MSDN Windows 7/ Server 2008 says check <see cref="PrivateUsage"/> instead
        /// </summary>
        public ulong PageFileUsage;
        /// <summary>
        /// Peak value of committed change during process lifetime
        /// </summary>
        public ulong PeakPageFileUsaged;
        /// <summary>
        /// same as <see cref="PageFileUsage"/>,  Current Commit Charge value in butes for this process.  This is the amount that the memory manage as allocated for this process.
        /// </summary>
        public ulong PrivateUsage;
    }
}
