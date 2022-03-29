using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// NativeImports contains  one class - <see cref="InsightSheath.NativeImports.NativeMethods"/>.  This class is the import data between the C# sheath and the native side C level exports
/// </summary>
namespace InsightSheath.NativeImports
{

    /// <summary>
    /// Internal Class that imports filesandbox stuff
    /// </summary>
    internal static partial class NativeMethods
    {
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugHelp_ApiVerionStruct_GetRevision", ExactSpelling = false)]
        public static extern ushort DebugHelp_ApiVerion_GetRevision(IntPtr that);
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugHelp_ApiVerionStruct_GetMajor", ExactSpelling = false)]
        public static extern ushort DebugHelp_ApiVerion_GetMajor(IntPtr that);
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "DebugHelp_ApiVerionStruct_GetMinor", ExactSpelling = false)]
        public static extern ushort DebugHelp_ApiVerion_GetMinor(IntPtr that);
        /*
        DebugHelp_ApiVerionStruct_GetMajor
        DebugHelp_ApiVerionStruct_GetMinor

        DebugHelp_ApiVerionStruct_GetRevision
        */
    }
}
