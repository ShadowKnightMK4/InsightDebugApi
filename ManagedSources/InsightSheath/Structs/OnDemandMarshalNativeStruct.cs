using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Structs
{
    /// <summary>
    /// Class that is the base for classes that retrieve there data from a native struct and marshal to a private struct + expose properties to access this struct.
    /// </summary>
    public abstract class OnDemandMarshalNativeStruct: NativeStaticContainer
    {
        public OnDemandMarshalNativeStruct(IntPtr Native): base(Native)
        {

        }

        public OnDemandMarshalNativeStruct(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }
        /// <summary>
        /// This code should marshal the relevant struct from the native pointer if <see cref="WasBlit"/> is not set and set afterwards. Accessors should call this each time they are called to retrieve values.
        /// Additionally, if the .NET struct will need to make changes to the land, you're going to need to call <see cref="Apply"/> BEFORE reading the managed struct. Not doing this will revert your changes on the managed side
        /// to the equivalent native values.
        /// </summary>
        protected abstract void Blit();

        /// <summary>
        /// This code should convert the relevant struct in .NET land and place it in the <see cref="Native"/> if the protected value <see cref="StaleManagedStruct"/> was set to true Unmanaged land. It should throw 
        /// a <see cref="InvalidOperationException"/> if it's not a valid thing (native pointer is known to be read only memory/ exist for a callback function for example) 
        /// <see cref="NotImplementedException"/> exception if not done yet ecct...
        /// </summary>
        /// <exception cref="InvalidOperationException">Throw this when writing back to the unmanaged struct is not valid (for example read only memory or a local variable in a callback routine allocated on Unamanged heap)</exception>
        /// <exception cref="NotSupportedException">Throw this when writing back to the unmanaged struct is not valid (for example read only memory or a local variable in a callback routine allocated on Unamanged heap)</exception>
        /// <exception cref="NotImplementedException">The routine was added but not implemented yet.</exception>
        protected abstract void Apply();


        /// <summary>
        /// public access to protects routine to send changes to the managed struct copy back to native land. If the class does not implement or that's not a valid option for the class, an exception may occure
        /// </summary>
        /// <exception cref="InvalidOperationException"></exception>
        /// <exception cref="NotSupportedException"></exception>
        /// <exception cref="NotImplementedException"></exception>
        public void MarshalToNative()
        {
            Apply();
        }
        /// <summary>
        /// <see cref="Blit"/> should set this to true after last reading successful marshaling was done in <see cref="Blit"/>from the native side.
        /// </summary>
        protected bool WasBlit;

        /// <summary>
        /// When an accessory sets property in the managed struct that needs to be updated in native land, set this.
        /// </summary>
        protected bool StaleManagedStruct;

    }
}
