using FileSandBoxSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBoxSheath.Remote
{
    /// <summary>
    /// Imports the RemoteRead and RemoteWrite structures for dealing with extracting data from the target process during various events.
    /// </summary>
    public static class RemoteStructure
    {
        public enum PointerSize
        {
            Size4 = 4,
            Size8 = 8
        }
        /// <summary>
        /// Read either a 4 byte or 8 byte pointer from the passed process handle.
        /// </summary>
        /// <param name="ProcessHandle">0= self</param>
        /// <param name="Location">virtual memory location</param>
        /// <param name="Size">either Size4 or Size8 from <seealso cref="PointerSize"/></param>
        /// <returns></returns>
        public static IntPtr RemoteReadPointer(IntPtr ProcessHandle, IntPtr Location, PointerSize Size)
        {
            switch (Size)
            {
                case PointerSize.Size4:
                    return NativeMethods.RemoteReadPointer(ProcessHandle, Location, 4);
                case PointerSize.Size8:
                    return NativeMethods.RemoteReadPointer(ProcessHandle, Location, 8);
                default:
                    return IntPtr.Zero;
            }
        }
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
