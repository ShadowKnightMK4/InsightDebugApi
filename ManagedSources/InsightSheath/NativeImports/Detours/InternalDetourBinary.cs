using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Detours;
using static InsightSheath.Detours.DetourBinary;

namespace InsightSheath.NativeImports
{
    /// <summary>
    /// This class imports the various native routines (usualy from InsightAPI.dll) that DetourBinary and StaticBinary require
    /// </summary>
    internal class InternalDetourBinary
    {
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "DetourBinaryResetImports")]
        public static extern bool DetourBinaryResetImports(IntPtr Handle);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "DetourBinaryOpen")]
        public static extern IntPtr DetourBinaryOpen(IntPtr Handle);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "DetourBinaryClose")]
        public static extern bool DetourBinaryClose(IntPtr DetourHandle);
        // public bool EditInputs(IntPtr Context, BinaryByWayscallback pfByway, BinaryFileCallback FpfFile, BinarySymbolCallback pfSymbol, BinaryFinalCallback pfFinal)
        
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "DetourBinaryEditImports")]
        public static extern bool DetourBinaryEditInports(IntPtr DetourHandle,
                                                          IntPtr pContext,
                                                          BinaryByWayscallback pfByWay,
                                                          BinaryFileCallback pfFile,
                                                          BinarySymbolCallback pfSymbol,
                                                          BinaryFinalCallback pfFinal);
        /// <summary>
        /// Write the changes made with this DetourBinary resource to the target. See also <see href="https://github.com/Microsoft/Detours/wiki/DetourBinaryWrite"/>
        /// </summary>
        /// <param name="DetourHandle">From a previously sucessfully call to <see cref="DetourBinaryOpen(IntPtr)"/></param>
        /// <param name="FileHandle">Native file handle to write the changes too.</param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "DetourBinaryWrite")]
        public static extern bool DetourBinaryWrite(IntPtr DetourHandle, IntPtr FileHandle);
    }
}
