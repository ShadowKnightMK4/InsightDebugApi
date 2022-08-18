using InsightSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Win32Struct;
using InsightSheath.Debugging;
namespace InsightSheath.Win32Struct.Remote
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
                    return RemoteStructureInternal.RemoteReadPointer(ProcessHandle, Location, 4);
                case PointerSize.Size8:
                    return RemoteStructureInternal.RemoteReadPointer(ProcessHandle, Location, 8);
                default:
                    return IntPtr.Zero;
            }
        }

        public static WindowsObjectAttributes RemoteReadObjectAttributes(IntPtr ProcessHandle, IntPtr Location, bool IsTarget32Bit,  bool FreeOnCleanup)
        {
            IntPtr target;
            target = RemoteStructureInternal.RemoteReadObjectAttributes(ProcessHandle, Location, IsTarget32Bit);
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


        
        /// <summary>
        /// Read either a <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/> from the location. Note: This does assume the NativeExport from InsightAPI.DLL will always return a 64-bit version of the struct and quietly promotes the 32-bit version when extracting from the target to an x64 bit version
        /// </summary>
        /// <param name="ProcessHandle">Handle to Read From</param>
        /// <param name="Location">location to read from</param>
        /// <param name="IsTarget32Bit">determines which type of struct we're reading from</param>
        /// <param name="FreeOnCleanup">argument terminates if we trigger GC </param>
        /// <returns>Returns a unique instance of a <see cref="WindowsUnicodeString"/> ready for use.</returns>
        public static WindowsUnicodeString RemoteReadUnicodeString(IntPtr ProcessHandle, IntPtr Location, bool IsTarget32Bit, bool FreeOnCleanup)
        {
            IntPtr retptr = IntPtr.Zero;
            WindowsUnicodeString ret;
            retptr = RemoteStructureInternal.RemoteReadUnicodeString(ProcessHandle, Location, IsTarget32Bit);
            if (retptr != IntPtr.Zero)
            {
                ret = new WindowsUnicodeString(retptr, FreeOnCleanup, StructModeType.Machinex64);
                return ret;
            }
            return null;
        }


        /// <summary>
        /// Extract an Unicode string from the remote process. You will need to know how long the string is.
        /// </summary>
        /// <param name="ProcessHandle">Process should be open for VM_READ rights at least</param>
        /// <param name="StringLocation">virtual memory location in the remote process</param>
        /// <param name="CharCount">how many chars in the string (Unicode)</param>
        /// <returns>Returns a .NET managed string on OK and null on a problem.</returns>
        public static string RemoteReadString(IntPtr ProcessHandle, IntPtr StringLocation, ulong CharCount)
        {
            string str = null;
            IntPtr ret = RemoteStructureInternal.RemoteReadStringInternal(ProcessHandle, StringLocation, CharCount);
            if (ret != IntPtr.Zero)
            {
                str = Marshal.PtrToStringUni(ret);
                NativeMethods.SimpleFree(ret);
            }
            return str;

        }

        /// <summary>
        /// Extract a DebugString from an OUTPUT_DEBUG_EVENT <see cref="DebugEvent"/> and <see cref="DebugEventStringInfo"/> for other ways.   
        /// </summary>
        /// <param name="ProcessHandle">Handle to the process</param>
        /// <param name="DebugEventStruct">Pointer to a DebugEvent native structure</param>
        /// <returns></returns>
        public static string RemoteReadDebugString(IntPtr ProcessHandle, IntPtr DebugEventStruct)
        {
            IntPtr RetPtr = RemoteStructureInternal.RemoteReadDebugStringInternal(ProcessHandle, DebugEventStruct);
            if (RetPtr == IntPtr.Zero)
            {
                return null;
            }
            else
            {
                string ret = Marshal.PtrToStringUni(RetPtr);
                RemoteStructureInternal.RemoteFreeDebugStringInternal(RetPtr);
                return ret;
            }
        }
    }
}
