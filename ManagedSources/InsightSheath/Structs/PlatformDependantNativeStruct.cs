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
    public abstract class PlatformDependantNativeStruct: NativeStaticContainer
    {
        public PlatformDependantNativeStruct(IntPtr Native): base(Native)
        {

        }

        public PlatformDependantNativeStruct(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }

        public PlatformDependantNativeStruct(IntPtr Native, bool FreeOnCleanup, StructModeType StructType): base(Native, FreeOnCleanup)
        {
            StructTypeContainer = StructType;
        }

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
        /// This code should marshal the relevant struct from the native pointer if <see cref="WasBlit"/> is not side and set afterwards. Accessors should call this each time they are called to retreive values.
        /// </summary>
        protected abstract void Blit();
        /// <summary>
        /// <see cref="Blit"/> should set this to true after last reading successful marshaling was done in <see cref="Blit"/>from the native side.
        /// </summary>
        protected bool WasBlit;

        /// <summary>
        /// canned error message for when user does not specify a machine.
        /// </summary>
        protected const string MachineTypeUnspecifiedError = "Pick which version of the struct to use. Set StructType to either {0} or {1} then attempt to access. Class name: {2}";
        static protected string MachineTypeUnspecifiedErrorMakeMessage(string Type0, string Type1, string classname)
        {
            return string.Format(MachineTypeUnspecifiedError, Type0, Type1, classname);
        }
        /// <summary>
        /// this throws a message wither everything except class name prefilled by calling <see cref="MachineTypeUnspecifiedErrorMakeMessage(string, string, string)"/>
        /// </summary>
        /// <param name="classname"></param>
        /// <returns></returns>
        static protected InvalidOperationException ThrowNewInvalidOpMessage(string classname)
        {
            return new InvalidOperationException(MachineTypeUnspecifiedErrorMakeMessage(Enum.GetName(typeof(StructModeType), StructModeType.Machinex64), Enum.GetName(typeof(StructModeType), StructModeType.Machinex86), classname));
        }
    }
}
