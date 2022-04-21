using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.NativeImports
{
    internal partial class NativeMethods
    {
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_MakeInstance", ExactSpelling = false)]
        public static extern IntPtr InsightMemory_MakeInstance();

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_DupInstance", ExactSpelling = false)]
        public static extern IntPtr InsightMemory_DupInstance(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_KillInstance", ExactSpelling = false)]
        public static extern bool InsightMemory_KillInstance(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_SetTargetProcess", ExactSpelling = false)]
        public static extern bool InsightMemory_SetTargetProcess(IntPtr That, ulong ProcessHandle, bool HandleInstance);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_UpdateMemoryStats", ExactSpelling = false)]
        public static extern bool InsightMemory_UpdateMemoryStats(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPageFault", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetPageFault(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPeakWorkingSetSize", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetPeakWorkingSetSize(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetWorkingSetSize", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetWorkingSetSize(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetQuotaPeakPagedPoolUsage", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetQuotaPeakPagedPoolUsage(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetQuotaPagePoolUsage", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetQuotaPagePoolUsage(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPeakPagePoolUsage", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetPeakPagePoolUsage(IntPtr That);




        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPageFileUsage", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetPageFileUsage(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPeakPageFileUsage", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetPeakPageFileUsage(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPrivateUsage", ExactSpelling = false)]
        public static extern ulong InsightMemory_GetPrivateUsage(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetMemoryStatsBulk", ExactSpelling = false)]
        public static extern IntPtr InsightMemory_GetMemoryStatsBulk(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_SetAutoRefreshMemoryStats", ExactSpelling = false)]
        public static extern bool InsightMemory_SetAutoRefreshMemoryStats(IntPtr That, bool Enable);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetAutoRefreshMemoryStats", ExactSpelling = false)]
        public static extern bool InsightMemory_GetAutoRefreshMemoryStats(IntPtr That);









    }
}
