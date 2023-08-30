using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Debugging.Process
{
    /// <summary>
    /// Process Memory count for 32-bit process
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct ProcessMemoryCount32
    {
        /// <summary>
        /// For when you want an equivelent version of this as <see cref="ProcessMemoryCount64"/>. Note cb is set to the siz eof the 64-bit verison
        /// </summary>
        /// <returns></returns>
        public ProcessMemoryCount64 Promotion()
        {
            ProcessMemoryCount64 ret = new ProcessMemoryCount64();
            ret.cb = ProcessMemoryCountConstants.Expected64BitSize;
            ret.PageFaultCount = this.PageFaultCount;
            ret.PeakWorkingSetSize = this.PeakWorkingSetSize;
            ret.WorkingSetSize = this.WorkingSetSize;
            ret.QuotaPeakedPagedPoolUsage = this.QuotaPeakedPagedPoolUsage;
            ret.QuotaPagedPoolUsage = this.QuotaPagedPoolUsage;
            ret.QuotaPeakedNonPagePoolUsage = this.QuotaPeakedNonPagePoolUsage;
            ret.QuotaNonPagePoolUsage = this.QuotaNonPagePoolUsage;
            ret.PageFileUsage = this.PageFileUsage;
            ret.PeakPageFileUsaged= this.PeakPageFileUsaged;
            ret.PrivateUsage = this.PrivateUsage;
            return ret;
        }
        /// <summary>
        /// Size of this structure in unmanaged land. Should Should equal <see cref="ProcessMemoryCountConstants.Expected86BitSize"/>
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
        /// same as <see cref="PageFileUsage"/>,  Current Commit Charge value in bytes for this process.  This is the amount that the memory manage as allocated for this process.
        /// </summary>
        public uint PrivateUsage;
    }

}
