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
    /// This class contains an AddRef()/ Release() component for child classes.
    /// Destructor(), Call Release() and if that returns 0, free as appropriate.
    /// Not needed if the class itself is a i.e. 1 to 1 
    /// Sheath class pointer that contains one pointer and do not give it out, this can skip.
    /// Sheath DebugEvent classes have this in the class hierarchy because they
    /// give out multiple references to a native class like candy.  Each multiple class
    /// in the sheath points to a single native class.  when the sheath class
    /// is freed() we need to ensure we don't prematurely free the unmanaged component of the class, 
    /// </summary>
    public abstract class ReferenceCounterNativeStaticContainer : NativeStaticContainer
    {
        /// <summary>
        /// Create instance of this with the passed Native Pointer. Subclasses of this should include routines to call the native class's functions. Reference counter is set to 1.
        /// </summary>
        /// <param name="Native">The Native pointer to keep track of.</param>
        public ReferenceCounterNativeStaticContainer(IntPtr Native) : base(Native)
        {
            ReferenceCount.Ref = 1;
        }

        /// <summary>
        /// Create instance of this with the passed Native Pointer. Subclasses of this should include routines to call the native class's functions. Reference counter is set to RefereneCount.
        /// </summary>
        /// <param name="Native">The Native pointer to keep track of.</param>
        /// <param name="FreeOnCleanup">indicate  if we are to invoke the unmanaged cleanup routine for free()</param>
        /// <param name="ReferenceCount">Starting reference count number. The unmanaged  resource should not be freed during disposal if this is more than 0 after decreasing by 1</param>
        public ReferenceCounterNativeStaticContainer(IntPtr Native, bool FreeOnCleanup, ulong ReferenceCount) : base(Native, FreeOnCleanup)
        {
            this.Native = Native;
            FreeOnCleanupContainer = FreeOnCleanup;
            this.ReferenceCount.Ref = ReferenceCount;
        }
        /// <summary>
        /// Create instance of this with the passed Native Pointer. Subclasses of this should include routines to call the native class's functions. Reference counter is set to 11.
        /// </summary>
        /// <param name="Native">The Native pointer to keep track of.</param>
        /// <param name="FreeOnCleanup">indicate  if we are to invoke the unmanaged cleanup routine for free()</param>
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
        /// Increase the Reference count to the native pointer and return the value. Call this when returning a separate sheath class that may outlive the primary sheath class
        /// </summary>
        /// <returns></returns>
        public ulong AddRef()
        {
            return ReferenceCount.AddRef();
        }

        /// <summary>
        /// Decrease the Reference count to the native pointer and return the value. If zero is returned, your unmanaged native pointer should be OK to safely cleanup/free
        /// </summary>
        /// <returns></returns>
        public ulong Release()
        {
            return ReferenceCount.DecRef();
        }

        readonly ReferenceCounter ReferenceCount = new();
    }
}
