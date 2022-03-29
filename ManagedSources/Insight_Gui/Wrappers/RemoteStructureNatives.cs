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
        /// <summary>
        /// WWraps RemoteReadDebugString.  Return value is a string that needs to be freed afterwares.
        /// </summary>
        /// <param name="ProcessHandle"></param>
        /// <param name="DebugEventPtr"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteReadDebugString")]
        public static extern IntPtr RemoteReadDebugStringInternal(IntPtr ProcessHandle, IntPtr DebugEventPtr);

        /// <summary>
        /// Imports RemoteFreeDEbugString() from the dll. Currently that forwards to "RemoteRead_SimpleFree" in the code. 
        /// If the implmentation of RemoteReadDebugString changes, RemoteRead_SimpleFree may no longer free properly. USE RemoteFreeDebugString()
        /// <param name="Memory">Pointer to a IntPtr returned by<see cref=">RemoteReadDebugStringInternl"/></param>
        [DllImport("FileSandBoxApi", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "RemoteReadDebugString")]
        public static extern void RemoteFreeDebugStringInternal(IntPtr Memory);
    }
}
