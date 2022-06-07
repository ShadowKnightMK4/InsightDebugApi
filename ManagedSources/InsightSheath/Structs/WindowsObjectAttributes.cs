using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Structs
{


    /// <summary>
    /// This is the Object Attributes struct for a Wow / x86 bit process.  Specs from <see href="https://docs.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_object_attributes"/> 
    /// </summary>
    /// <remarks>For x86 (32-bit) processes, handles, pointers, uints are all 4 bytes long. This is not true for x64 processes and that's why <see cref="ObjectAttributes64"/> exists</remarks>
    [StructLayout(LayoutKind.Sequential)]
    
    public struct ObjectAttributes32
    {
        /// <summary>
        /// Length of the struct. For x86, a proper value is 24.
        /// </summary>
        public uint Length;
        /// <summary>
        /// 4 byte potentially 0 Handle value indicated the root that <see cref="ObjectName"/> specifies.
        /// </summary>
        public uint RootDirectory;
        /// <summary>
        /// a 4 byte pointer to a <see cref="UnicodeString32"/> struct. This is the name of the item this struct points too
        /// </summary>
        public uint ObjectName;
        /// <summary>
        /// 4 byte value indicating object attributes.
        /// </summary>
        public uint Attributes;
        /// <summary>
        /// 4 byte pointer to the defined security descriptor. May be (null/0) if default is fine.
        /// </summary>
        public uint SecurityDescriptor;
        /// <summary>
        /// 4 byte pointer to the defined security quality of service. May be (null/0) if default is fine.
        /// </summary>
        public uint SecurityQoS;

        /// <summary>
        /// Return a 64-bit version with the 32-bit values promoted to 64-bit. Promotion
        /// </summary>
        /// <returns></returns>
        public ObjectAttributes64 Promotion()
        {
            var ret = new ObjectAttributes64
            {
                Length = Length,
                RootDirectory = RootDirectory,
                ObjectName = ObjectName,
                Attributes = Attributes,
                SecurityDescriptor = SecurityDescriptor,
                SecurityQoS = SecurityQoS
            };
            return ret;
        }
    };

    /// <summary>
    /// This is the Object Attributes struct for a 64-bit process  Specs from <see href="https://docs.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_object_attributes"/> 
    /// </summary>
    /// <remarks> <summary>
    /// IMPORTANT! Pointer values and handle are 8 bytes long for x64 bit processes instead of 4 bytes. This is why we need to define two structs. Other struct is <see cref="ObjectAttributes32"/>
    /// </summary></remarks>
    [StructLayout(LayoutKind.Sequential)]
    public struct ObjectAttributes64
    {
        /// <summary>
        /// Length of this struct. For x64 bit code, a proper one is 48.
        /// </summary>
        public uint Length;
        /// <summary>
        /// 8 byte potentially 0 Handle value indicated the root that <see cref="ObjectName"/> specifies.
        /// </summary>
        public ulong RootDirectory;
        /// <summary>
        /// a 8 byte pointer to a <see cref="UnicodeString64"/> struct. This is the name of the item this struct points too
        /// </summary>
        public ulong ObjectName;
        /// <summary>
        /// 4 byte value indicating object attributes.
        /// </summary>
        public uint Attributes;
        /// <summary>
        /// 8 byte pointer to the defined security descriptor. May be (null/0) if default is fine.
        /// </summary>
        public ulong SecurityDescriptor;
        /// <summary>
        /// 8 byte pointer to the defined security quality of service. May be (null/0) if default is fine.
        /// </summary>
        public ulong SecurityQoS;
    };
    /// <summary>
    /// made from specs here <see href="https://docs.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_object_attributes"/>
    /// </summary>
    public class WindowsObjectAttributes: PlatformDependantNativeStruct
    {
        public  WindowsObjectAttributes(IntPtr Native)   : base(Native)
        {

        }

        public WindowsObjectAttributes(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }

        public WindowsObjectAttributes(IntPtr Native, bool FreeOnCleanup, StructModeType StructType) : base(Native, FreeOnCleanup, StructType)
        {

        }
        public WindowsObjectAttributes(IntPtr Native, StructModeType StructType) : base(Native, StructType)
        {

        }

        private bool disposedValue;
        /// <summary>
        /// <see cref="WindowsObjectAttributes"/> cleanup.  We need to call the current native routine to cleanup and clear the cleanup flag so we don't delete mememory already deleted
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                bool bit32Mode;
                if (StructType == StructModeType.Machinex86)
                {
                    bit32Mode = true;
                }
                else
                {
                    if (StructModeType.Machinex64 == StructType)
                    {
                        bit32Mode = false;
                    }
                    else
                    {
                        throw ThrowNewInvalidOpMessage(GetType().Name);
                    }
                }
                if (FreeOnCleanup)
                {
                    NativeImports.NativeMethods.RemoteFreeObjectAttributes(Native, bit32Mode);
                }
                ClearNative();
                disposedValue = true;
            }
            base.Dispose(disposing);
        }
        protected override void Blit()
        {
            if (!WasBlit)
            {
                WasBlit = true;
                switch (StructTypeContainer)
                {
                    case StructModeType.Machinex64:
                        {
                            Ret64 = Marshal.PtrToStructure<ObjectAttributes64>(Native);
                        }
                        break;
                    case StructModeType.Machinex86:
                        {
                            Ret32 = Marshal.PtrToStructure<ObjectAttributes32>(Native);
                        }
                        break;
                    default: throw  ThrowNewInvalidOpMessage(GetType().Name);
                }

            }
        }

        /// <summary>
        /// Not currently implemented
        /// </summary>
        protected override void Apply()
        {
            throw new NotImplementedException();
        }




        /// <summary>
        /// Retrieve the length value of the structure on the native size. <see cref="StructTypeContainer"/> must be set to something other than <see cref="StructModeType.MachineUnknown"/>
        /// </summary>
        public ulong Length
        {
            get
            {
                Blit();
                switch (StructTypeContainer)
                {
                    case StructModeType.Machinex64: return Ret64.Length;
                    case StructModeType.Machinex86: return Ret32.Length;
                }
                throw ThrowNewInvalidOpMessage(GetType().Name);
            }
        }

        /// <summary>
        /// Retrieve the handle value of the structure on the native size. 4 byte values are already promoted to 8 bytes long when returned. 
        /// </summary>
        /// <remarks><see cref="PlatformDependantNativeStruct.StructTypeContainer"/> must be set to something other than <see cref="StructModeType.MachineUnknown"/></remarks>
        public IntPtr RootDirectory
        {
            get
            {
                Blit();
                switch (StructTypeContainer)
                {
                    case StructModeType.Machinex64:
                        {
                            if (Ret64.RootDirectory != 0)
                            {
                                return new IntPtr((uint)Ret64.RootDirectory);
                            }
                            return IntPtr.Zero;
                        }
                    case StructModeType.Machinex86:
                        {
                            if (Ret32.RootDirectory != 0)
                            {
                                return new IntPtr(Ret32.RootDirectory);
                            }
                            return IntPtr.Zero;
                        }
                }
                throw ThrowNewInvalidOpMessage(GetType().Name);
            }
        }

        /// <summary>
        /// Retrieve (and set the current <see cref="StructModeType"/> for the current UnicodeString structure pointed to.  Can return null if that's not set
        /// </summary>
        /// <remarks>The returned object is pointing to the same native object as this class. If you want a unique one, duplicate it. <see cref="PlatformDependantNativeStruct.StructTypeContainer"/> must be set to something other than <see cref="StructModeType.MachineUnknown"/></remarks>
        public WindowsUnicodeString ObjectName
        {
            get
            {
                Blit();
                switch (StructTypeContainer)
                {
                    case StructModeType.Machinex64:
                        {
                            if (Ret64.ObjectName == 0)
                            {
                                return null;
                            }
                            else
                            {
                                WindowsUnicodeString ret = new WindowsUnicodeString(new  IntPtr((long)Ret64.ObjectName), false, StructModeType.Machinex64);
                                
                                return ret;
                            }
                        }
                    case StructModeType.Machinex86:
                        {
                            if (Ret32.ObjectName == 0)
                            {
                                return null;
                            }
                            else
                            {
                                WindowsUnicodeString ret = new WindowsUnicodeString(new IntPtr((int)Ret32.ObjectName), false, StructModeType.Machinex86);
                                
                                return ret;
                            }
                        }
                }
                throw ThrowNewInvalidOpMessage(GetType().Name);
            }
        }

        /// <summary>
        /// Return the object attributes of the native struct.
        /// </summary>
        /// <remarks><see cref="PlatformDependantNativeStruct.StructTypeContainer"/> must be set to something other than <see cref="StructModeType.MachineUnknown"/></remarks>
        public uint Attributes
        {
            get
            {
                switch (StructTypeContainer)
                {
                    case StructModeType.Machinex64:
                        return Ret64.Attributes;
                    case StructModeType.Machinex86:
                        return Ret32.Attributes;
                }
                throw new InvalidOperationException();
            }
        }

        /// <summary>
        /// Return a pointer to the security descriptor (may be not be relative to Your process. <see href="https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory"/> to read the data. 
        /// </summary>
        /// <remarks><see cref="PlatformDependantNativeStruct.StructTypeContainer"/> 
        /// must be set to something other than <see cref="StructModeType.MachineUnknown"/>.
        /// This can be 0 if the default one is OK. 4 byte pointers are promoted to 8 bytes long. 
        /// The <see cref="Remote.RemoteStructure.xxx"/> routine does not read this into your process 
        /// currently.</remarks>
        public IntPtr SecurityDescriptor
        {
            get
            {
                switch (StructTypeContainer)
                {
                    case StructModeType.Machinex64:
                        if (Ret64.SecurityDescriptor == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr((uint) Ret64.SecurityDescriptor);
                    case StructModeType.Machinex86:
                        if (Ret32.SecurityDescriptor == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr(Ret32.SecurityDescriptor);
                }
                throw ThrowNewInvalidOpMessage(GetType().Name);
            }
        }
        /// <summary>
        /// Return a pointer to the security quality of service struct (may be not be relative to Your process. <see href="https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory"/> to read the data. 
        /// </summary>
        /// <remarks><see cref="PlatformDependantNativeStruct.StructTypeContainer"/> 
        /// must be set to something other than <see cref="StructModeType.MachineUnknown"/>.
        /// This can be 0 if the default one is OK. 4 byte pointers are promoted to 8 bytes long. 
        /// The <see cref="Remote.RemoteStructure.xxx"/> routine does not read this into your process 
        /// currently.</remarks>
        public IntPtr SecurityQualityOfService
        {
            get
            {
                switch (StructTypeContainer)
                {
                    case StructModeType.Machinex64:
                        if (Ret64.SecurityQoS == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr((uint)Ret64.SecurityQoS);
                    case StructModeType.Machinex86:
                        if (Ret32.SecurityQoS == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr(Ret32.SecurityQoS);
                }
                throw  ThrowNewInvalidOpMessage(GetType().Name);
            }
        }

        /// <summary>
        /// if <see cref="PlatformDependantNativeStruct.StructTypeContainer"/> is set to <see cref="StructModeType.Machinex86"/>, <see cref="Blit"/> marshals to this structure.
        /// </summary>
        ObjectAttributes32 Ret32;
        /// <summary>
        /// if <see cref="PlatformDependantNativeStruct.StructTypeContainer"/> is set to <see cref="StructModeType.Machinex64"/>, <see cref="Blit"/> marshals to this structure.
        /// </summary>
        ObjectAttributes64 Ret64;
        
    }
}
