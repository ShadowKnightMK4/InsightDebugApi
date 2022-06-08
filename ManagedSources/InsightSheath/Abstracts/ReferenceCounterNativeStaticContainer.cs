using InsightSheath.Misc;
using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Abstract
{
    /// <summary>
    /// This class contains an AddRef()/ Release() compontent for child classes.
    /// Destructors(), Call Release() and if that returns 0, free as appropriate.
    /// Not needed if the class itself is a 1 to 1 ie
    /// 
    /// Sheath class pointer that contains one pointer, this can skip.
    /// Sheath, DebugEvent classes have this in the change, because they
    /// give out references to a native class like candy.  Each multiple class
    /// in the sheath points to a single native class.  when the sheath class
    /// is freed(), 
    /// </summary>
    public abstract class ReferenceCounterNativeStaticContainer : NativeStaticContainer
    {
        public ReferenceCounterNativeStaticContainer(IntPtr Native) : base(Native)
        {
            ReferenceCount.Ref = 1;
        }
        public ReferenceCounterNativeStaticContainer(IntPtr Native, bool FreeOnCleanup, ulong ReferenceCount) : base(Native, FreeOnCleanup)
        {
            this.Native = Native;
            FreeOnCleanupContainer = FreeOnCleanup;
            this.ReferenceCount.Ref = ReferenceCount;
        }
        public ReferenceCounterNativeStaticContainer(IntPtr Native, bool FreeOnCleanup) : base(Native, FreeOnCleanup)
        {
            this.Native = Native;
            FreeOnCleanupContainer = FreeOnCleanup;
            ReferenceCount.Ref = 1;
        }

        /// <summary>
        /// Get reference count without adjusting it. Increase the reference counter when returning a pointer to a class
        /// </summary>
        public ulong ReferenceCounter
        {
            get
            {
                return ReferenceCount.Ref;
            }
        }



        /// <summary>
        /// Increase the Reference count to the native pointer and return the value. Call this when returning a seperate sheath class that may outlive the primary sheath class
        /// </summary>
        /// <returns></returns>
        public ulong AddRef()
        {
            return ReferenceCount.AddRef();
        }

        /// <summary>
        /// Decrease the Reference count to the native pointer and return the value. If zero is returned, your native pointer should be ok to safely cleanup/free
        /// </summary>
        /// <returns></returns>
        public ulong Release()
        {
            return ReferenceCount.DecRef();
        }

        readonly ReferenceCounter ReferenceCount = new();
    }
}
