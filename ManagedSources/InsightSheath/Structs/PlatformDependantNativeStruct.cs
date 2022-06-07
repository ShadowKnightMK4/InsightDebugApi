using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using InsightSheath.Wrappers;

namespace InsightSheath.Structs
{
    /// <summary>
    /// This class serves as a foundation to classes that encapsulates a native pointer to either a x64 bit struct or x86 struct dependent on the target.
    /// </summary>
    public abstract class PlatformDependantNativeStruct: OnDemandMarshalNativeStruct
    {
        public PlatformDependantNativeStruct(IntPtr Native): base(Native)
        {

        }

        /// <summary>
        ///  Create an instance of the class with the object to specify if we free it.
        /// </summary>
        /// <param name="Native">Native pointer to the structure this class will reference</param>
        /// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree() which itself is a call to C/C++'s free() . Set TO FALSE if dealing with structures declared in C/C++ code vs dynamically allocated</param>
        /// <remarks> FreeOnCleanup is a bit dependent on your knowledge of the native code. If the native pointer points to something dynamically allocated with malloc() / HeapAlloc(),  specify true.  If it's declared in the Native source directly or on the stack as a function variable, use false</remarks>
        public PlatformDependantNativeStruct(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }

        public PlatformDependantNativeStruct(IntPtr Native, bool FreeOnCleanup, StructModeType StructType): base(Native, FreeOnCleanup)
        {
            StructTypeContainer = StructType;
        }

        /// <summary>
        /// Make an instance of this class with the <see cref="StructType"/> specified.
        /// </summary>
        /// <param name="Native">Pointer to an unmanaged memory block this class is dealing with.</param>
        /// <param name="StructType">Structures with points are different sizes depending on if they're lifting from x86 or x64 code. Indicate if this is a <see cref="StructModeType.Machinex86"/> (for 4 byte pointers) or a <see cref="StructModeType.Machinex64"/> for 8 byte pointers</param>
        public PlatformDependantNativeStruct(IntPtr Native, StructModeType StructType): base(Native)
        {
            StructTypeContainer = StructType;
        }

        /// <summary>
        /// Platform Dependant native structure does not allocate anything and does nothing other than hold the specifics for the child class. Specifically struct
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
        }

        /// <summary>
        /// Get or set the type of structure that's correct for this class to use. Also resets a flag to trigger Marshalling from that native side when set.
        /// </summary>
        public StructModeType StructType
        {
            get { return StructTypeContainer; } 
            set {
                if (StructTypeContainer != value)
                {
                    StructTypeContainer = value;
                    WasBlit = false;
                }
            }
        }
        /// <summary>
        /// protected container for <see cref="StructType"/>
        /// </summary>
        protected StructModeType StructTypeContainer;
        
     
        /// <summary>
        /// canned error message for when user does not specify a machine.
        /// </summary>
        protected const string MachineTypeUnspecifiedError = "Pick which version of the struct to use. Set StructType to either {0} or {1} then attempt to access. Class name: {2}";
        static protected string MachineTypeUnspecifiedErrorMakeMessage(string Type0, string Type1, string classname)
        {
            return string.Format(MachineTypeUnspecifiedError, Type0, Type1, classname);
        }
        /// <summary>
        /// This throws a message wither everything except class name prefilled by calling <see cref="MachineTypeUnspecifiedErrorMakeMessage(string, string, string)"/>
        /// </summary>
        /// <param name="classname"></param>
        /// <returns></returns>
        static protected InvalidOperationException ThrowNewInvalidOpMessage(string classname)
        {
            return new InvalidOperationException(MachineTypeUnspecifiedErrorMakeMessage(Enum.GetName(typeof(StructModeType), StructModeType.Machinex64), Enum.GetName(typeof(StructModeType), StructModeType.Machinex86), classname));
        }
    }
}
