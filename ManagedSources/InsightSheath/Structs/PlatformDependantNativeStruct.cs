using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using InsightSheath.Wrappers;

namespace InsightSheath.Structs
{
    /// <summary>
    /// This class Encapulates a native pointer to either a x64 bit struct or x86 struct dependant on the target.
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

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
        }
        public StructModeType StructType
        {
            get { return StructTypeContainer; } set { StructTypeContainer = value; }
        }
        protected StructModeType StructTypeContainer;
        /// <summary>
        /// This code should marshal the relevant struct from the native pointer if <see cref="WasBlit"/> is not side and set afterwards. Accessors should call this each time they are called
        /// </summary>
        protected abstract void Blit();
        /// <summary>
        /// <see cref="Blit"/> should set this to true after last reading from the native side.
        /// </summary>
        protected bool WasBlit;

        protected const string MachineTypeUnspecifiedError = "Pick which version of the struct to use. Set StructType to either {0} or {1} then attempt to access. Class name: {2}";
        protected string MachineTypeUnspecifiedErrorMakeMessage(string Type0, string Type1, string classname)
        {
            return string.Format(MachineTypeUnspecifiedError, Type0, Type1, classname);
        }
        /// <summary>
        /// this throws a mesage wither everything except classname prefilled
        /// </summary>
        /// <param name="classname"></param>
        /// <returns></returns>
        protected InvalidOperationException ThrowNewInvalidOpMessage(string classname)
        {
            return new InvalidOperationException(MachineTypeUnspecifiedErrorMakeMessage(Enum.GetName(typeof(StructModeType), StructModeType.Machinex64), Enum.GetName(typeof(StructModeType), StructModeType.Machinex86), classname));
        }
    }
}
