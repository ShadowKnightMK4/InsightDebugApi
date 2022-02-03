using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBox_GUI
{
    /// <summary>
    /// Imports the RemoteRead and RemoteWrite structures for dealing with extracting data from the target process during various events.
    /// </summary>
    public static class RemoteStructure
    {

        public static string RemoteReadDebugString(IntPtr ProcessHandle, IntPtr DebugEventStruct)
        {
            IntPtr RetPtr = NativeMethods.RemoteReadDebugStringInternal(ProcessHandle, DebugEventStruct);
            if (RetPtr == IntPtr.Zero)
            {
                return null;
            }
            else
            {
                string ret = Marshal.PtrToStringUni(RetPtr);
                NativeMethods.RemoteFreeDebugStringInternal(RetPtr);
                return ret;
            }
        }
    }
}
