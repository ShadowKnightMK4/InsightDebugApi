using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Debugging.Process;
namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Internal Class. Contains the routines imported from InsightAPI.DLL for use in the <see cref="InsightMemory"/> class. Not really planned to be accessible outside of this project.
    /// </summary>
    internal class InternalInsightMemory
    {
        /// <summary>
        /// Imports "InsightMemory_MakeInstance" from InsightAPI.DLL which creates the native class that <see cref="InsightMemory"/> wraps.
        /// </summary>
        /// <returns>Returns a pointer to the unmanaged class that <see cref="InsightMemory"/>  wraps null</returns>
        /// <remarks>You're likely going to get an out of memory C/C++ exception if this actually does not have enough to allocate. C++ users, this is a raw pointed to a C++ class made via the new operator.</remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_MakeInstance", ExactSpelling = false)]
        public static extern IntPtr MakeInstance();

        /// <summary>
        /// Imports "InsightMemory_DupInstance" from InsightAPI.DLL which creates the a copy of the <see cref="InsightMemory"/> class you specified via pointer.
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>Returns a pointer to the new instance of the class or null</returns>    
        /// <remarks>You're likely going to get an out of memory C/C++ exception if this actually does not have enough to allocate. C++ users, this is a raw pointed to a C++ class made via the new operator.</remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_DupInstance", ExactSpelling = false)]
        public static extern IntPtr DupInstance(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_KillInstance" from InsightAPI.dll which cleans up the native class that <see cref="InsightMemory"/> wraps.
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>true if the call worked and false if it did not</returns>
        /// <remarks>C++ usages, this is calling delete against the pointer you indicate.  Ideally it should a pointer returned via <see cref="MakeInstance"/> or <see cref="DupInstance(IntPtr)"/> </remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_KillInstance", ExactSpelling = false)]
        public static extern bool KillInstance(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_SetTargetProcess", ExactSpelling = false)]
        public static extern bool SetTargetProcess(IntPtr That, ulong ProcessHandle, bool HandleInstance);

        /// <summary>
        /// Get the ID of the process that was targetted via a call to <see cref="SetTargetProcess(nint, ulong, bool)"/>
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.DLL", BestFitMapping =false, CharSet = CharSet.Unicode, CallingConvention=CallingConvention.Winapi, EntryPoint = "InsightMemory_GetTargetProcessID")]
        public static extern uint GetTargetProcessID(IntPtr That);

        /// <summary>
        /// Imports "InsightMemory_UpdateMemoryStats" from InsightAPI.dll which manually refreshes the native side of this class's info
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>TRUE if it did work and FALSE if it did not</returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_UpdateMemoryStats", ExactSpelling = false)]
        public static extern bool UpdateMemoryStats(IntPtr That);

        /// <summary>
        /// Imports "InsightMemory_GetPageFaultCount" which may trigger a refresh of the native class data and then returns the relevant memory requested (PageFaultCount)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the number of page faults memory stored within the native class (and zeros 0 on error) </returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPageFaultCount", ExactSpelling = false)]
        public static extern ulong GetPageFaultCount(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_GetPeakWorkingSetSize" which may trigger a refresh of the native class data and then returns the relevant memory requested (PeakWorkingSetSize)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the peaked working set size member stored within the native class (and zeros 0 on error) </returns>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPeakWorkingSetSize", ExactSpelling = false)]
        public static extern ulong GetPeakWorkingSetSize(IntPtr That);

        /// <summary>
        /// Imports "InsightMemory_GetWorkingSetSize" which may trigger a refresh of the native class data and then returns the relevant memory requested (WorkingSetSize)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the current working set size member stored within the native class (and zeros 0 on error) </returns>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetWorkingSetSize", ExactSpelling = false)]
        public static extern ulong GetWorkingSetSize(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_GetQuotaPeakPagedPoolUsage" which may trigger a refresh of the native class data and then returns the relevant memory requested (QuotaPeakPagedPoolUsage)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the peaked page pool usage member stored within the native class (and zeros 0 on error) </returns
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetQuotaPeakPagedPoolUsage", ExactSpelling = false)]
        public static extern ulong GetQuotaPeakPagedPoolUsage(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_GetQuotaPagePoolUsage" which may trigger a refresh of the native class data and then returns the relevant memory requested (QuotaPagedPoolUsage)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the current page usage member stored within the native class (and zeros 0 on error) </returns>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetQuotaPagePoolUsage", ExactSpelling = false)]
        public static extern ulong GetQuotaPagePoolUsage(IntPtr That);


        /// <summary>
        /// Imports "GetPeakNonPagePoolUsage" which may trigger a refresh of the native class data and then returns the relevant memory requested (PeakNonPagedPoolUsage)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the peak nonpaged pool usage member stored within the native class (and zeros 0 on error) </returns>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPeakNonPagePoolUsage", ExactSpelling = false)]
        public static extern ulong GetPeakNonPagePoolUsage(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_GetQuotaNonPagedPoolUsage" which may trigger a refresh of the native class data and then returns the relevant memory requested (QuotaNonPagedPoolUsage)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the current nonpaged pool usage member stored within the native class (and zeros 0 on error) </returns>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetQuotaNonPagedPoolUsage", ExactSpelling = false)]
        public static extern ulong GetQuotaNonPagedPoolUsage(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_GetQuotaNonPagedPoolUsage" which may trigger a refresh of the native class data and then returns the relevant memory requested (PageFileUsage)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the current "Page File"/Commit Charge member stored within the native class (and zeros 0 on error).  </returns>
        /// <remarks>Note that In Windows versions before Windows 7 and Windows Server 2012 R2+, MSDN documentation says to use PrivateUsage member instead aka <see cref="GetPrivateUsage(IntPtr)"/></remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPageFileUsage", ExactSpelling = false)]
        public static extern ulong GetPageFileUsage(IntPtr That);

        /// <summary>
        /// Imports "InsightMemory_GetPeakPageFileUsage" which may trigger a refresh of the native class data and then returns the relevant memory requested (PeakPageFileUsage)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the peak paged file usage member stored within the native class (and zeros 0 on error) </returns>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPeakPageFileUsage", ExactSpelling = false)]
        public static extern ulong GetPeakPageFileUsage(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_GetPrivateUsage" which may trigger a refresh of the native class data and then returns the relevant memory requested (PrivateUsage)
        /// </summary>
        /// <param name="That">Unmanaged pointer to an instance of an <see cref="InsightMemory"/> wrapped class</param>
        /// <returns>This returns the Private Memory member (Commit Charge) stored within the native class (and zeros 0 on error) </returns>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetPrivateUsage", ExactSpelling = false)]
        public static extern ulong GetPrivateUsage(IntPtr That);


        /// <summary>
        /// Imports "InsightMemory_GetMemoryStatsBulk" which may rigger a refresh of the native class data and then returns a pointer to a "PROCESS_MEMORY_COUNTERS_EX" structure
        /// </summary>
        /// <param name="That"></param>
        /// <returns>This returns a pointer specifically a <see href="https://docs.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-process_memory_counters_ex"/> PROCESS_MEMORY_COUNTERS_EX. It does not and should not need to be freed until you're done with the class as is part of the class <see cref="InsightMemory"/> wraps directly.</returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetMemoryStatsBulk", ExactSpelling = false)]
        public static extern IntPtr InsightMemory_GetMemoryStatsBulk(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_SetAutoRefreshMemoryStats", ExactSpelling = false)]
        public static extern bool SetAutoRefreshMemoryStats(IntPtr That, bool Enable);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetAutoRefreshMemoryStats", ExactSpelling = false)]
        public static extern bool GetAutoRefreshMemoryStats(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_GetWorkingSetContainerInfo", ExactSpelling = false)]
        public static extern IntPtr GetWorkingSet(IntPtr that);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightMemory_FreeWorkingSetContainerInfo", ExactSpelling = false)]
        public static extern IntPtr FreeWorkingSetPtr(IntPtr that);





    }
}
