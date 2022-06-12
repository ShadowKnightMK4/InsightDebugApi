using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Abstract;
using InsightSheath.NativeImports;
using InsightSheath.Wrappers;
namespace InsightSheath.Debugging.SymbolEngine
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
        /// <summary>
        /// Initialize this wrapper class to point to an unmanaged memory structure containing one API_VERISON structy
        /// </summary>
        /// <param name="Native">non null native pointer</param>
        public DebugHelp_ApiVersionStruct(IntPtr Native) : base(Native)
        {

        }

        /// <summary>
        /// Initialize this wrapper class to point to an unmanaged memory structure containing one API_VERISON structy.
        /// </summary>
        /// <param name="Native">non null native pointer</param>
        /// <param name="FreeOnClean">Ignored. Always false when calling the base <see cref="NativeStaticContainer"/> contructor</param>
        public DebugHelp_ApiVersionStruct(IntPtr Native, bool FreeOnClean) : base(Native, false)
        {

        }

        /// <summary>
        /// Get the Revison member of the API_VERSION data struct the Native pointer points too
        /// </summary>
        public ushort Revision
        {
            get
            {
                return DebugHelpApiVersionStructInternal.DebugHelp_ApiVerion_GetRevision(Native);
            }
        }
        /// <summary>
        /// Get the Major member of the API_VERSION data struct the Native pointer points too
        /// </summary>
        public ushort Major
        {
            get
            {
                return DebugHelpApiVersionStructInternal.DebugHelp_ApiVerion_GetMajor(Native);
            }
        }

        /// <summary>
        /// Get the Minor member of the API_VERSION data struct the Native pointer points too
        /// </summary>
        public ushort Minor
        {
            get
            {
                return DebugHelpApiVersionStructInternal.DebugHelp_ApiVerion_GetMinor(Native);
            }
        }


    }
}
