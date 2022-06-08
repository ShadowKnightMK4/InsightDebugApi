using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath;
using InsightSheath.Debugging.Process;
namespace InsightSheath.NativeImports
{


    /// <summary>
    /// Internal Class.  Contains the routines imported from InsightAPI.DLL for use in the <see cref="InsightProcess"/> class
    /// </summary>
    internal static partial class InsightProcessInternal
    {
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetMemoryStatsBulkPtr", ExactSpelling = false)]
        public static extern IntPtr InsightProcess_GetMemoryStatsBulkPtr(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetPrivateUsage", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetPrivateUsage(IntPtr That);




        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetPeakPageFileUsage", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetPeakPageFileUsage(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetPageFileUsage", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetPageFileUsage(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetQuotNonPagePoolUsage", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetQuotNonPagePoolUsage(IntPtr That);




        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetQuotaPeakNonPagePoolUsage", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetQuotaPeakNonPagePoolUsage(IntPtr That);





        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetQuotaPagePoolUsage", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetQuotaPagePoolUsage(IntPtr That);




        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetQuotaPeakPagePoolUsage", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetQuotaPeakPagePoolUsage(IntPtr That);





        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetWorkingSetSize", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetWorkingSetSize(IntPtr That);




        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetPeakWorkingSet", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetPeakWorkingSet(IntPtr That);




        [DllImport("InsightApi.Dll", BestFitMapping =false, CallingConvention = CallingConvention.Winapi,EntryPoint = "InsightProcess_GetPageFaultCount", ExactSpelling =false)]
        public static extern ulong InsightProcess_GetPageFaultCount(IntPtr That);

        /// <summary>
        /// Wrapper for calling InsightAPI's InsightProcess_GetDetourListSize which calls InsightProcess::GetDetourListSize().  
        /// </summary>
        /// <param name="that">Native pointer that points to an instance of the class aka 'this'</param>
        /// <returns>Returns the number of entries in the detour DLL list that the spawned process will be forced to load</returns>
        /// <remarks>The reason for ulong is that's the size of the x64 size_t in C++ land. x86 size is uint sized. The API/sheath use the 64-bit size so the source can match.</remarks>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetDetourListSize", ExactSpelling = false)]
        public static extern ulong InsightProcess_GetDetourListSize(IntPtr that);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetDetourListEntryReadOnly", ExactSpelling = false)]
        public static extern IntPtr PSProcessInformation_GetDetourListIndex(IntPtr That,uint index);
        /// <summary>
        /// Imports InsightProcess_MakeInstance() from InsightApi.Dll
        /// </summary>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_MakeInstance", ExactSpelling = false)]
        public static extern IntPtr CreateInsightProcessNativeClass();
        /// <summary>
        /// Invokes copy constructor via InsightProcess_DupInstance from InsightAPI.DLL
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_DupInstance", ExactSpelling = false)]
        public static extern IntPtr DuplicateInsightProcessNativeClass(IntPtr that);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetStartupInfoClass", ExactSpelling = false)]
        public static extern IntPtr InsightProcess_GetStartupInfoClass(IntPtr That);
        /// <summary>
        /// Wrap the InsightProcess_KillInstance routine
        /// </summary>
        /// <param name="that"></param>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_KillInstance", ExactSpelling = false)]
        public static extern void InsightProcess_KillInstance(IntPtr that);


        /// <summary>
        /// Wrap the InsightProcess_SetProcessName routine
        /// </summary>
        /// <param name="that"></param>
        /// <param name="NewName"></param>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetProcessName")]
        public static extern void InsightProcess_SetProcessName(IntPtr that, string NewName);

        /// <summary>
        /// Wrap the InsightProcess_GetProcessName 
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetProcessName")]
        public static extern IntPtr InsightProcess_GetProcessName(IntPtr That);


        /// <summary>
        /// Wraps the InsightProcess_SpawnProcess method
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SpawnProcess")]
        public static extern IntPtr InsightProcess_Spawn(IntPtr That);


        /// <summary>
        /// Wrap the InsightProcess_SetProcessArgument routine
        /// </summary>
        /// <param name="that"></param>
        /// <param name="NewName"></param>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetProcessArgument")]
        public static extern void InsightProcess_SetProcessArgument(IntPtr that, string NewArgument);


        /// <summary>
        /// Wrap the InsightProcess_GetProcessArgument
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetProcessArgument")]
        public static extern IntPtr InsightProcess_GetProcessArgument(IntPtr That);

        /// <summary>
        /// Wraps the InsightProcess_SetCreationFlags command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetCreationFlags")]
        public static extern void InsightProcess_SetCreationFlags(IntPtr That, uint NewFlags);

        /// <summary>
        /// Wraps the InsightProcess_SetCreationFlagDebug command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetCreationFlagDebug")]
        public static extern void PsProcessInformation_SetCreationFlagDebug(IntPtr That, uint NewFlags);

        /// <summary>
        /// Wraps the InsightProcess_SetCreationFlagDebugOnlyThis command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetCreationFlagDebugOnlyThis")]
        public static extern void InsightProcess_SetCreationFlagDebugOnlyThis(IntPtr That, uint NewFlags);


        /// <summary>
        /// Wraps the InsightProcess_SetCreationFlagDebugSuspended command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetCreationFlagDebugSuspended")]
        public static extern void InsightProcess_SetCreationFlagDebugSuspended(IntPtr That, uint NewFlags);

        /// <summary>
        /// Wraps the InsightProcess_SetCreationFlags command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetCreationFlagDebugOnlyThisSuspended")]
        public static extern void InsightProcess_SetCreationFlagDebugOnlyThisSuspended(IntPtr That, uint NewFlags);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_RequestDebugPriv")]
       
        public static extern bool InsightProcess_RequestDebugPriv(IntPtr That, bool NewMode);

        /// <summary>
        /// Wrap InsightProcess_GetCreationFlags
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetCreationFlags")]
        public static extern uint InsightProcess_GetCreationFlags(IntPtr That);





        /// <summary>
        /// Wrap the InsightProcess_SetProcessArgument routine
        /// </summary>
        /// <param name="that"></param>
        /// <param name="NewName"></param>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetWorkingDirectory")]
        public static extern void InsightProcess_SetWorkingDirectory(IntPtr that, string NewArgument);


        /// <summary>
        /// Wrap the InsightProcess_GetProcessArgument
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetWorkingDirectory")]
        public static extern IntPtr PsProcessInformation_GetWorkingDirectory(IntPtr That);

        /// <summary>
        /// Specific if processes spawned with the context class want to inherit the parent's environment or not.
        /// </summary>
        /// <param name="That"></param>
        /// <param name="WantDefaultAlso"></param>

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetEnvInherit")]
        public static extern void InsightProcess_SetInheritDefaultEnviroment(IntPtr That, bool WantDefaultAlso);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_ClearEnvBlock")]
        public static extern void InsightProcess_ClearExplicitEnviromentBlock(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_AssignEnvValue")]
        public static extern void InsightProcess_SetExplicitEnviromentValue(IntPtr That, string Name, string Value);
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetEnvValue")]
        public static extern IntPtr InsightProcess_GetExplicitEnviromentValue(IntPtr That, string Name);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Ansi, EntryPoint = "InsightProcess_AddDetoursDllA")]
        public static extern bool PsProcessInformation_AddDetourDllToLoad(IntPtr That, string Name);

        // "InsightProcess_AddDetoursDllW" is not imported as the implementation just converts to ANSI before calling InsightProcess_AddDetoursDllA()
        // the marshaling stuff will take care of that already.
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_ClearDetoursList")]
        public static extern void InsightProcess_ClearDetourList(IntPtr That);

 
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetDebugMode")]
        public static extern void InsightProcess_SetDebugMode(IntPtr That, [MarshalAs(UnmanagedType.U4)] DebugModeType Ty);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetDebugMode")]
        public static extern DebugModeType InsightProcess_GetDebugMode(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetDebugEventCallback")]
        public static extern Delegate InsightProcess_GetDebugCallbackRoutine(IntPtr that);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetDebugEventCallback")]
        public static extern IntPtr InsightProcess_SetDebugCallbackRoutine(IntPtr that, [MarshalAs(UnmanagedType.FunctionPtr)] Delegate CallBack);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_PulseDebugThread")]
        public static extern void InsightProcess_PulseDebugEvent(IntPtr that);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetMainThreadHandle")]
        public static extern IntPtr InsightProcess_GetMainThreadHandle(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetMainProcessHandle")]
        public static extern IntPtr InsightProcess_GetMainProcessHandle(IntPtr That);

        
    

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetCommandment")]
        public static extern bool InsightProcess_GetCommandment(IntPtr That, uint Cmd);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_SetCommandment")]
        public static extern bool InsightProcess_SetCommandment(IntPtr That, uint Cmd, bool Enabled);

        
        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetMainProcessId")]
        public static extern uint InsightProcess_GetMainProcessId(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "InsightProcess_GetMainThreadId")]
        public static extern DebugModeType InsightProcess_GetMainThreadId(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_SetSymbolHandling", ExactSpelling = false)]
        public static extern bool InsightProcess_SetSymbolHandling(IntPtr That, bool NewVal);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetSymbolHandling", ExactSpelling = false)]
        public static extern bool InsightProcess_GetSymbolHandling(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetSymbolHandlerClass", ExactSpelling = false)]
        public static extern IntPtr InsightProcess_GetSymbolEngineClassPtr(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_AddPriorityDllPathW", ExactSpelling = false, CharSet= CharSet.Unicode)]
        public static extern bool InsightProcess_AddPriorityLoadLibraryPath(IntPtr that, string NewLocation);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_GetPriorityDllPath_NumberOf", ExactSpelling = false)]
        public static extern uint InsightProcess_GetProrityLoadLibraryPath_NumberOf(IntPtr that);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_IndexPriorityDllPath", ExactSpelling = false)]
        public static extern IntPtr InsightProcess_IndexPriorityDllPath(IntPtr that, int index);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "InsightProcess_ClearPriorityDllPath", ExactSpelling = false)]
        public static extern void InsightProcess_ClearPriorityDllPath(IntPtr that);





    }
}
