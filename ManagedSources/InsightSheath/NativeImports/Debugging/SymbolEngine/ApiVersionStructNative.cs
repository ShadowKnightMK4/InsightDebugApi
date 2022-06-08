using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Win32Struct;
using InsightSheath.Debugging.SymbolEngine;
namespace InsightSheath.NativeImports
{

    /// <summary>
    /// Internal Class. This imports the routines used in <see cref="DebugHelp_ApiVersionStruct"/>
    /// </summary>
    internal static class DebugHelpApiVersionStructInternal
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
