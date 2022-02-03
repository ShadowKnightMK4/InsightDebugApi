using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBox_GUI
{
    public static class Utility
    {
        public static string GetProcessNameViaHandle(IntPtr ProcessHandle)
        {
            IntPtr retPtr = NativeMethods.GetModuleNameViaHandleInternal(ProcessHandle);
            if (retPtr == IntPtr.Zero)
            {
                return null;
            }
            else
            {
                string ret = Marshal.PtrToStringUni(retPtr);
                NativeMethods.FreeModuleNameViaHandleInternal(retPtr);
                return ret;
            }
        }

        public static IntPtr OpenProcessForVirtualMemory(uint ProcessId)
        {
            return NativeMethods.OpenProcessForMemoryAccess(ProcessId);
        }

        public static IntPtr OpenPRocessForQueryInformation(uint ProcessId)
        {
            return NativeMethods.OpenProcesForQueryInformation(ProcessId);
        }

        public static bool CloseHandle(IntPtr Handle)
        {
            return NativeMethods.CloseHandle(Handle);
        }
    }
}
