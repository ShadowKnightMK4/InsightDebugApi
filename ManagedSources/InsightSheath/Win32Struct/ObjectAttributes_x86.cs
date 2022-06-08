using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Win32Struct
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
        /// Return a 64-bit version with the 32-bit values promoted to 64-bit.
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
}
