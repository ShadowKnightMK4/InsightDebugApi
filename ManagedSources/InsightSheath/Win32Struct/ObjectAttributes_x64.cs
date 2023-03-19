using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Win32Struct
{

    /// <summary>
    /// This is the Object Attributes struct for a 64-bit process  Specs from <see href="https://docs.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_object_attributes"/> 
    /// </summary>
    /// <remarks> <summary>
    /// IMPORTANT! Pointer values and handle are 8 bytes long for x64 bit processes instead of 4 bytes. This is why we need to define two structures. Other struct is <see cref="ObjectAttributes32"/>
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
}
