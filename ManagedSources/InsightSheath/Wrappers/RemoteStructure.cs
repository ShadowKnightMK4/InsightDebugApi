using InsightSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Structs;
namespace InsightSheath.Remote
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

        public static WindowsObjectAttributes RemoteReadObjectAttributes(IntPtr ProcessHandle, IntPtr Location, bool IsTarget32Bit,  bool FreeOnCleanup)
        {
            IntPtr target;
            target = NativeMethods.RemoteReadObjectAttributes(ProcessHandle, Location, IsTarget32Bit);
            if (target != IntPtr.Zero)
            {
                var ret = new WindowsObjectAttributes(target, FreeOnCleanup);
                if (IsTarget32Bit)
                {
                    ret.StructType = StructModeType.Machinex86;
                }
                else
                {
                    ret.StructType = StructModeType.Machinex64;
                }
                return ret;
            }
            return null;
        }

        public static WindowsUnicodeString RemoteReadUnicodeString(IntPtr ProcessHandle, IntPtr Location, bool IsTarget32Bit, bool FreeOnCleanup)
        {
            IntPtr retptr = IntPtr.Zero;
            WindowsUnicodeString ret;
            retptr = NativeImports.NativeMethods.RemoteReadUnicodeString(ProcessHandle, Location, IsTarget32Bit);
            if (retptr != IntPtr.Zero)
            {
                if (IsTarget32Bit)
                {
                    ret = new WindowsUnicodeString(retptr, true, StructModeType.Machinex86);
                }
                else
                {
                    ret = new WindowsUnicodeString(retptr, true, StructModeType.Machinex64);
                }
                return ret;
            }
            return null;
        }

        /// <summary>
        /// Primary for writing HANDLEs to debugged process. This writes 4 byte value to the remote process in question at the location at the virtual memory location specified in the target
        /// </summary>
        /// <param name="ProcessHandle">at minimum should have PROCESS_VM_WRITE and/or PROCESS_VM_OPERATION access</param>
        /// <param name="value">value to write</param>
        /// <param name="RemoteLocation">virtual memory in the TARGET process location to write too.</param>
        /// <returns></returns>
        public static bool RemotePoke4NoExceptions(IntPtr ProcessHandle, uint value, IntPtr RemoteLocation)
        {
            return NativeMethods.RemotePoke4(ProcessHandle, value, (ulong)RemoteLocation);
        }

        /// <summary>
        /// Primary for writing HANDLEs to debugged process. This writes 4 byte value to the remote process in question at the location specified in the virtual memory of the target
        /// </summary>
        /// <param name="ProcessHandle">at minimum should have PROCESS_VM_WRITE and/or PROCESS_VM_OPERATION access</param>
        /// <param name="value">value to write</param>
        /// <param name="RemoteLocation"></param>
        /// <exception cref="Win32Exception"> Win32Exceptions are thrown if the write failed (for security, bad memory location, in correct process and so on)</exception>
        public static void RemotePoke4(IntPtr ProcessHandle, uint value, IntPtr RemoteLocation)
        {
            bool ret= NativeMethods.RemotePoke4(ProcessHandle, value, (ulong)RemoteLocation);
            if (!ret)
            {
                throw new Win32Exception(Marshal.GetLastWin32Error());
            }
        }


        /// <summary>
        /// Extract an Unicode string from the remote process. You will need to know how long the string is.
        /// </summary>
        /// <param name="ProcessHandle">Process should be open for VM_READ rights at least</param>
        /// <param name="StringLocation">virtual memory location in the remote process</param>
        /// <param name="CharCount">how many chars in the string (Unicode)</param>
        /// <returns></returns>
        public static string RemoteReadString(IntPtr ProcessHandle, IntPtr StringLocation, uint CharCount)
        {
            string str = null;
            IntPtr ret = NativeMethods.RemoteReadStringInternal(ProcessHandle, StringLocation, CharCount);
            if (ret != IntPtr.Zero)
            {
                str = Marshal.PtrToStringUni(ret);
                NativeMethods.SimpleFree(ret);
            }
            return str;
        }

        /// <summary>
        /// Extract a DebugString from an OUTPUT_DEBUG_EVENT
        /// </summary>
        /// <param name="ProcessHandle">Handle to the process</param>
        /// <param name="DebugEventStruct">Pointer to a DebugEvent native structure</param>
        /// <returns></returns>
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
