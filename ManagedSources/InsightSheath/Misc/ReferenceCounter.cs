using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Misc
{
    /// <summary>
    /// USed by the <see cref="ReferenceCounterNativeStaticContainer"/> to track and change a reference.
    /// This is primary used by the <see cref="DebugEvent"/> class collection as we're handing out
    /// Multiple REferences to the same Native Pointer like candy and need to take steps to prevent users
    /// of the library from prematuring freeing the same sheath.
    /// 
    /// Why?
    ///     GC knows DebugEvent, DebugEventCreateProcess both end up using NativecStaticContainer
    ///     GC does NOT know of the Native Side being a block of memory for a DEBUG_EVENT struct.
    ///     GC does not know that DebugEvent hands out DebugEventCreateProcesses that point to the same NativePointer
    ///     
    ///     Got the above? Consider this example
    ///           GC frees DebugEventCreateProcess, memory is invalided on the nativeside
    ///           GC has not freed DebugEvent but memory thsi class points too is know invalid
    ///             DebugEvent breaks.
    ///     
    /// </summary>
    public class ReferenceCounter
    {
        public ulong AddRef()
        {
            if (RefContainer != ulong.MaxValue)
                RefContainer++;
            return RefContainer;
        }

        public ulong DecRef()
        {
            if (RefContainer != ulong.MinValue)
                RefContainer--;
            return RefContainer;
        }

        public ulong Ref
        {
            get
            {
                return RefContainer;
            }
            set
            {
                RefContainer = value;
            }
        }

        private ulong RefContainer;

        
    }
}
