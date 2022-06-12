using InsightSheath.Debugging.Process;
using InsightSheath.Win32Struct;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Abstract
{

    /// <summary>
    /// Encapsulates a native pointer to an unmanaged structure that can potentially be freed with a single call to C/C++'s free(). If said Native class needs special free requirements, overwrite <see cref="Dispose(bool)"/> with a a call to a routine to properly free it. If your wrapper is handing out multiple natiove pointers to the same object, you'll need something like <see cref="ReferenceCounterNativeStaticContainer"/>
    /// </summary>
    public abstract class NativeStaticContainer : IDisposable
    {
        /// <summary>
        /// This is using in <see cref="NativeStaticContainer"/> based classes when their static builder (For example <see cref="InsightProcess.CreateInstance"/>) receive null from the unmanaged constructor routine
        /// </summary>
        protected const string UnmanagedConstructorReturnedNull = "The unmanaged routine {0} to build the unmanaged part of {1} returned null.";
        /// <summary>
        /// This is used in <see cref="NativeStaticContainer"/> based classes a template to an exception message to indicate it received null when being built
        /// </summary>
        protected const string ReceivedNullPointerOnConstructor = "{0} Cannot be 0.  Use {1} to make a new instance and use this to point this managed wrapper to a native instance of the class: ({2}}";

   

        /// <summary>
        /// Used to make the <see cref="ArgumentNullException"/> message using <see cref="ReceivedNullPointerOnConstructor"/> template
        /// </summary>
        /// <param name="ArgumentPrefix"></param>
        /// <param name="AlternativeInstance"></param>
        /// <param name="ArgumentName"></param>
        /// <returns></returns>
        protected static ArgumentNullException WrapperConstructorReceivedNullPointerErrorException(string ArgumentPrefix, string AlternativeInstance, string ArgumentName)
        {
            return new ArgumentNullException(string.Format(ReceivedNullPointerOnConstructor, ArgumentPrefix, AlternativeInstance, ArgumentName));
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
        /// Grab a hash of the native pointer. Logic being if two hashes are the same, we're holding the same native struct/class
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            return Native.GetHashCode();
        }

        /// <summary>
        /// Create instance of this with the passed Native Pointer. Subclasses of this should include routines to call the native class's functions.
        /// </summary>
        /// <param name="Native">The Native pointer to keep track of.</param>
        public NativeStaticContainer(IntPtr Native)
        {
            this.Native = Native;
        }
        /// <summary>
        /// Create an instance of the class with the object to specify if we free it.
        /// </summary>
        /// <param name="Native">Native pointer to the structure this class will reference</param>
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree() which itself is a call to C/C++'s free() . Set TO FALSE if dealing with structures declared in C/C++ code vs dynamically allocated</param>
        /// <remarks> FreeOnCleanup is a bit dependent on your knowledge of the native code. If the native pointer points to something dynamically allocated with malloc() / HeapAlloc(),  specify true.  If it's declared in the Native source directly or on the stack as a function variable, use false.</remarks>
        ///
        public NativeStaticContainer(IntPtr Native, bool FreeOnCleanup)
        {
            this.Native = Native;
            FreeOnCleanupContainer = FreeOnCleanup;
        }


        /// <summary>
        /// WARNING! WARNING! WARNING! Return the underlying pointer.  Should this block of memory be freed (releases/ deleted), the instance of this <see cref="NativeStaticContainer"/> will break.
        /// </summary>
        public IntPtr NativePointer
        {
            get
            {
                return Native;
            }
        }

        /// <summary>
        /// place in disposable and call it after calling the current unmanaged cleanup routine.  
        /// </summary>
        protected void ClearNative()
        {
            Native = IntPtr.Zero;
        }
        /// <summary>
        /// Holds a physical unmanaged pointer to a relevant structure. Child classes use this 
        /// </summary>
        protected IntPtr Native;
        /// <summary>
        /// Conditional free on dispose. Set this to true if you're manipulated a block of unmanaged memory dynamically located by the API. Set to false if you're working with a previously allocated structure or the struct was given as part of a callback to you or part of a larger structure
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
        /// value containing if dispose() was called
        /// </summary>
        private bool disposedValue;
        /// <summary>
        /// Was this class disposed?
        /// </summary>
        public bool IsDisposed { get { return disposedValue; } }

        /// <summary>
        /// Basic <see cref="NativeStaticContainer"/> disposal.  Checks if not disposed, if FreeOnCleanup is set and Native != 0 null before calling a routine exported in InsightAPI to free the memory. 
        /// </summary>
        /// <param name="disposing">set to true if disposing of managed resources in addition to unmanaged.</param>
        /// <remarks>IMPORTANT! If your child class needs special needs beyond C/C++'s free(),  override this and take care of them before calling base(). 
        /// Should your custom class allocate multiple things for example <see cref="WindowsUnicodeString"/>'s implementation for reading from remote processes <see cref="Win32Struct.Remote.RemoteStructure.RemoteReadUnicodeString(IntPtr, IntPtr, bool, bool)"/>, you will need to make a routine to properly free.  That routine allocates 2 buffers instead of 1. Calling C/C++'s free() alone is leaking memory.
        /// Design checks:
        ///<list type="number">
        ///<listheader>
        ///<description > First, override disposal() </description >
        ///</listheader >
        ///<item>
        /// <description> Nest, add a private variable in your class called isDisposed. Set this to true  when your class's native pointer is cleaned up completely </description >
        ///</item >
        ///<item>
        ///<description>if <see cref="ReferenceCounterNativeStaticContainer"/> is NOT in the hierarchy then your routine should dispose of unmanaged resourced when isDisposed is true, FreeOnCleanup is true and the Native pointer is not null. It should set the native pointer to null when FULLY done disposing it.</description>
        ///</item>
        ///<item>
        ///<description>if <see cref="ReferenceCounterNativeStaticContainer"/> IS in the hierarchy then your routine should dispose of unmanaged resourced when isDisposed is true, FreeOnCleanup is AND THE REFERENCE COUNTER being decreased is 0, and the Native pointer is not null.   If the  reference counter is more than 0 when decreased, set native to null instead of disposing.</description>
        ///</item>
        /// </list>
        /// </remarks>
        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                /*
                if (disposing)
                {
                    // nothing managed to dispose
                }*/

                if ( (Native != IntPtr.Zero) && (FreeOnCleanup))
                {
                    // if we made it this far, we should free() the native pointer if non zero and where made with that flag on.
                    {
                        NativeImports.NativeMethods.SimpleFree(Native);
                    }
                    // clear native pointer.
                    ClearNative();
                }
                disposedValue = true;
            }
        }

        /// <summary>
        /// clean up only the unmanaged parts of <see cref="NativeStaticContainer"/>
        /// </summary>
        ~NativeStaticContainer()
        {
            Dispose(disposing: false);
        }

        /// <summary>
        /// Trigger cleanup of this <see cref="NativeStaticContainer"/> based class
        /// </summary>
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
