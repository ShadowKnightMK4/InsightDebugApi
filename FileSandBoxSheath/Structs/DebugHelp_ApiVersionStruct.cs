using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using FileSandBoxSheath.NativeImports;
using FileSandBoxSheath.Wrappers;
namespace FileSandBoxSheath.Structs
{

    /// <summary>
    /// API_VERSION struct for DebugHelp symbol library.
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct API_VERSION
    {
        /// <summary>
        /// The Major Version of the API
        /// </summary>
        public ushort MajorVersion;
        /// <summary>
        /// Minor Version of the API
        /// </summary>
        public ushort MinorVersion;
        /// <summary>
        /// Revision of the API
        /// </summary>
        public ushort Revision;
        /// <summary>
        /// Reserved  for future use
        /// </summary>
        public ushort Reserved;
    }

    /// <summary>
    /// Wrappers for Api Version struture. Note: Impementation in Insight has the struct as part of it, you DON'T want to free any pointer to it you get from insite.
    /// </summary>
    public class DebugHelp_ApiVersionStruct : NativeStaticContainer
    {
        public DebugHelp_ApiVersionStruct(IntPtr Native) : base(Native)
        {

        }

        public DebugHelp_ApiVersionStruct(IntPtr Native, bool FreeOnClean) : base(Native, false)
        {

        }

        public ushort Revision
        {
            get
            {
                return NativeMethods.DebugHelp_ApiVerion_GetRevision(Native);
            }
        }
        public ushort Major
        {
            get
            {
                return NativeMethods.DebugHelp_ApiVerion_GetMajor(Native);
            }
        }
        public ushort Minor
        {
            get
            {
                return NativeMethods.DebugHelp_ApiVerion_GetMinor(Native);
            }
        }


    }
}
