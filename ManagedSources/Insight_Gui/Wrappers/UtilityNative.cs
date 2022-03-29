using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBox_GUI
{
    internal static partial class NativeMethods
    {
        [DllImport("FileSandBoxApi.DLL", CallingConvention=CallingConvention.Winapi, EntryPoint ="UtilGetModuleNameViaHandle")]
        public static extern IntPtr GetModuleNameViaHandleInternal(IntPtr ProcessHandle);
        [DllImport("FileSandBoxApi.DLL", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilFreeModuleNameViaHandle")]
        public static extern IntPtr FreeModuleNameViaHandleInternal(IntPtr NativeStringPtr);


        [DllImport("FileSandBoxApi.dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilOpenProcessForMemory")]
        public static extern IntPtr OpenProcessForMemoryAccess(uint ProcessID);

        [DllImport("FileSandBoxApi.dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilOpenProcesForQueryInformation")]
        public static extern IntPtr OpenProcesForQueryInformation(uint ProcessID);

        [DllImport("kernel32.dll", CallingConvention=CallingConvention.Winapi)]
        public static extern bool CloseHandle(IntPtr Handle);
    }
}
