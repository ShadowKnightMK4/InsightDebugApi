using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBoxSheath.Wrappers
{

    /// <summary>
    /// encapsulates a native ptr to an unmanged structure that can potentially be freed with with a single call.
    /// </summary>
    public abstract class NativeStaticContainer: IDisposable
    {
        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }
            if (obj is NativeStaticContainer)
            {
                return (((NativeStaticContainer)obj).Native == this.Native);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return Native.GetHashCode();
        }
        public NativeStaticContainer(IntPtr Native)
        {
            this.Native = Native;
        }
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="Native">Native pointer to the structure</param>
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree(). Set TO FALSE if dealing with strucutres declared in C/C++ code vs dynamically allocated</param>
        public NativeStaticContainer(IntPtr Native, bool FreeOnCleanup)
        {
            this.Native = Native;
            this.FreeOnCleanup = FreeOnCleanup;
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
        /// Holds a phyiscal unmanged pointer to a relevant structure. Child classes use this 
        /// </summary>
        protected readonly IntPtr Native;
        /// <summary>
        /// Conditional free on dispose. Set this to true if you're manipulated a block of unmanaged memory dynmaicicly located by the api. Set to false if you're working with a previously allocated structure
        /// </summary>
        protected readonly bool FreeOnCleanup;


        /// <summary>
        /// private value containing if dispose() was callsed
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
                // TODO: set large fields to null
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
