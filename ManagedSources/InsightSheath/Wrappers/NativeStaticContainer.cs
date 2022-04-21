using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Wrappers
{

    /// <summary>
    /// Encapsulates a native pointer to an unmanaged structure that can potentially be freed with a single call to C/C++'s free(). If said Native class needs special free requirements, overwrite <see cref="Dispose(bool)"/> with a a call to a routine to properly free it.
    /// </summary>
    public abstract class NativeStaticContainer : IDisposable
    {
        protected const string ReceivedNullPointerOnConstructor = "{0} Cannot be 0.  Use {1} to make a new instance and use this to point this managed wrapper to a native instance of the class: ({2}}";
        protected string ConstructReceivedNullPointerOnConstructor_message(string ArgumentPrefix, string AlternativeInstance, string ArgumentName)
        {
            return string.Format(ReceivedNullPointerOnConstructor, ArgumentPrefix, AlternativeInstance, ArgumentName);
        }
        /// <summary>
        /// NativeStaticContrain compares both <see cref="Native"/> pointers and returns true if equal
        /// </summary>
        /// <param name="obj">the other one</param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }
            if (obj is NativeStaticContainer container)
            {
                return container.Native == Native;
            }
            return false;
        }

        /// <summary>
        /// Grab a hash of the native pointer
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            return Native.GetHashCode();
        }

        /// <summary>
        /// Create instance of this with the passed Native Pointer. Subclasses of this should include routines to call the native class's functions.
        /// </summary>
        /// <param name="Native"></param>
        public NativeStaticContainer(IntPtr Native)
        {
            this.Native = Native;
        }
        /// <summary>
        /// Create an instance of the class with the object to specify if we free it.
        /// </summary>
        /// <param name="Native">Native pointer to the structure</param>
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree() which itself is a call to C/C++'s free() . Set TO FALSE if dealing with structures declared in C/C++ code vs dynamically allocated</param>
        /// <remarks> FreeOnCleanup is a bit dependent on your knowledge of the native code. If the native pointer points to something dynamically allocated with malloc() / HeapAlloc(),  specify true.  If it's declared in the Native source directly or on the stack as a function variable, use false</remarks>
        public NativeStaticContainer(IntPtr Native, bool FreeOnCleanup)
        {
            this.Native = Native;
            this.FreeOnCleanupContainer = FreeOnCleanup;
        }


        /// <summary>
        /// WARNING! WARNING! WARNING! Return the underlying pointer.  Should this block of memory be freed, the instance of this <see cref="NativeStaticContainer"/> will break.
        /// </summary>
        public IntPtr NativePointer
        {
            get
            {
                return Native;
            }
        }
        /// <summary>
        /// Holds a physical unmanaged pointer to a relevant structure. Child classes use this 
        /// </summary>
        protected readonly IntPtr Native;
        /// <summary>
        /// Conditional free on dispose. Set this to true if you're manipulated a block of unmanaged memory dynamically located by the API. Set to false if you're working with a previously allocated structure
        /// </summary>
        protected bool FreeOnCleanup
        {
            get
            {
                return FreeOnCleanupContainer;
            }
        }

        /// <summary>
        /// Holds the free on cleanup value.  Recommend only disposal() routines modify this.
        /// </summary>
        protected bool FreeOnCleanupContainer;

        /// <summary>
        /// protected value containing if dispose() was called
        /// </summary>
        protected bool disposedValue;
        /// <summary>
        /// Was this class disposed?
        /// </summary>
        public bool IsDisposed { get { return disposedValue; } }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }

                if (FreeOnCleanup)
                {
                    NativeImports.NativeMethods.SimpleFree(Native);
                }
                
                disposedValue = true;
            }
        }

        ~NativeStaticContainer()
        {
            Dispose(disposing: false);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
