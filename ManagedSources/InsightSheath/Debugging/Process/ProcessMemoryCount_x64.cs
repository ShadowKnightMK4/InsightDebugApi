using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Debugging.Process
{
    /// <summary>
    /// COntains the expected cb sizes for x86 <see cref="ProcessMemoryCount32"/> and <see cref="ProcessMemoryCount64"/>
    /// </summary>
    public static class ProcessMemoryCountConstants
    {
        /// <summary>
        /// The expected unmanaged size returned for x64 bit. Not a part of the structure in unamanged land.
        /// </summary>
        public const uint Expected64BitSize = 80;
        /// <summary>
        /// The expected unmanaged size returned for x86 bit. Not a part of the structure in unamanged land.
        /// </summary>
        public const uint Expected86BitSize = 44;

    }

    /// <summary>
    /// Process Memory count for 64-bit process
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct ProcessMemoryCount64
    {
        /// <summary>
        /// Size of this structure in unmanaged land. Should equal <see cref="ProcessMemoryCountConstants.Expected64BitSize"/>
        /// </summary>
        public uint cb;
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
        /// same as <see cref="PageFileUsage"/>,  Current Commit Charge value in bytes for this process.  This is the amount that the memory manage as allocated for this process.
        /// </summary>
        public ulong PrivateUsage;
    }
}
