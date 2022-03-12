using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBoxSheath.NativeImports
{
    internal enum ThreadConext_Arguments: uint
    {
        UseHandle = 0,
        UseThreadId = 1
    }
    internal static partial class NativeMethods
    {
        
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetThreadDescriptionW")]
        public static extern bool ThreadContext_SetTheadDescriptionW(IntPtr That, [MarshalAs(UnmanagedType.LPWStr)] string NewDescrition);
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadDescriptionW")]
        public static extern string ThreadContext_GetThreadDescriptionW(IntPtr that);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_CreateInstance")]
        public static extern IntPtr ThreadContext_CreateInstance(IntPtr HandleOrThreadId, uint Flags);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_KillInstance")]
        public static extern bool ThreadContext_KillInstance(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetTargetThread")]
        public static extern bool ThreadContext_SetTargetThread(IntPtr That, IntPtr HandleOfThread, ThreadConext_Arguments Flags);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetPriority")]
        public static extern int ThreadContext_GetPriority(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_SetPriority")]
        public static extern bool ThreadContext_SetPriority(IntPtr That, int NewPriority);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_DupInstance")]
        public static extern IntPtr ThreadContext_DupInstance(IntPtr That);


        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeCreationTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeCreationTime(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeExitTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeExitTime(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeUserTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeUserTime(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "ThreadContext_GetThreadTimeKernelTime")]
        public static extern IntPtr ThreadContext_GetThreadTimeKernelTime(IntPtr That);

    }
}
