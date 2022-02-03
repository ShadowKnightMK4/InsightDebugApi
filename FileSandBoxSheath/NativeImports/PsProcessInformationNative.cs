using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using FileSandBoxSheath;

namespace FileSandBoxSheath.NativeImports
{



    internal  static partial class NativeMethods
    {

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "PS_ProcessInformation_GetDetourListSize", ExactSpelling = false)]
        public static extern uint PSProcessInformation_GetDetourListSize(IntPtr that);
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "PS_ProcessInformation_GetDetourListEntryReadOnly", ExactSpelling = false)]
        public static extern IntPtr PSProcessInformation_GetDetourListIndex(IntPtr That,uint index);
        /// <summary>
        /// Imports PS_ProcessInformation_MakeInstance() from FileSandBoxAPi
        /// </summary>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "PS_ProcessInformation_MakeInstance", ExactSpelling = false)]
        public static extern IntPtr CreatePsProcessInformation();
        /// <summary>
        /// Invokes copy constructor via PS_ProcessInformation_DupInstance from FileSandBBoxApi
        /// </summary>
        /// <param name="that"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "PS_ProcessInformation_DupInstance", ExactSpelling = false)]
        public static extern IntPtr CreatePsProcessInformation(IntPtr that);


        /// <summary>
        /// Wrap the PS_ProcessInformation_KillInstance routine
        /// </summary>
        /// <param name="that"></param>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "PS_ProcessInformation_KillInstance", ExactSpelling = false)]
        public static extern void KillPSProcessInformation(IntPtr that);


        /// <summary>
        /// Wrap the PS_ProcessInformation_SetProcessName routine
        /// </summary>
        /// <param name="that"></param>
        /// <param name="NewName"></param>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetProcessName")]
        public static extern void PsProcessInformation_SetProcessName(IntPtr that, string NewName);

        /// <summary>
        /// Wrap the PS_ProcessInformation_GetProcessName 
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_GetProcessName")]
        public static extern IntPtr PsProcessInformation_GetProcessName(IntPtr That);


        /// <summary>
        /// Wraps the PS_ProcessInformation_SpawnProcess method
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SpawnProcess")]
        public static extern IntPtr PsProcessInformation_Spawn(IntPtr That);


        /// <summary>
        /// Wrap the PS_ProcessInformation_SetProcessArgument routine
        /// </summary>
        /// <param name="that"></param>
        /// <param name="NewName"></param>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetProcessArgument")]
        public static extern void PsProcessInformation_SetProcessArgument(IntPtr that, string NewArgument);


        /// <summary>
        /// Wrap the PS_ProcessInformation_GetProcessArgument
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_GetProcessArgument")]
        public static extern IntPtr PsProcessInformation_GetProcessArgument(IntPtr That);

        /// <summary>
        /// Wraps the PS_ProcessInformation_SetCreationFlags command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetCreationFlags")]
        public static extern void PsProcessInformation_SetCreationFlags(IntPtr That, uint NewFlags);

        /// <summary>
        /// Wraps the PS_ProcessInformation_SetCreationFlagDebug command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetCreationFlagDebug")]
        public static extern void PsProcessInformation_SetCreationFlagDebug(IntPtr That, uint NewFlags);

        /// <summary>
        /// Wraps the PS_ProcessInformation_SetCreationFlagDebugOnlyThis command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetCreationFlagDebugOnlyThis")]
        public static extern void PsProcessInformation_SetCreationFlagDebugOnlyThis(IntPtr That, uint NewFlags);


        /// <summary>
        /// Wraps the PS_ProcessInformation_SetCreationFlagDebugSuspended command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetCreationFlagDebugSuspended")]
        public static extern void PsProcessInformation_SetCreationFlagDebugSuspended(IntPtr That, uint NewFlags);

        /// <summary>
        /// Wraps the PS_ProcessInformation_SetCreationFlags command. 
        /// </summary>
        /// <param name="That"></param>
        /// <param name="NewFlags">See CreateProcessW() on MSDN for more info.</param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetCreationFlagDebugOnlyThisSuspended")]
        public static extern void PsProcessInformation_SetCreationFlagDebugOnlyThisSuspended(IntPtr That, uint NewFlags);



        /// <summary>
        /// Wrap PS_ProcessInformation_GetCreationFlags
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_GetCreationFlags")]
        public static extern uint PsProcessInformation_GetCreationFlags(IntPtr That);





        /// <summary>
        /// Wrap the PS_ProcessInformation_SetProcessArgument routine
        /// </summary>
        /// <param name="that"></param>
        /// <param name="NewName"></param>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetWorkingDirectory")]
        public static extern void PsProcessInformation_SetWorkingDirectory(IntPtr that, string NewArgument);


        /// <summary>
        /// Wrap the PS_ProcessInformation_GetProcessArgument
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_GetWorkingDirectory")]
        public static extern IntPtr PsProcessInformation_GetWorkingDirectory(IntPtr That);

        /// <summary>
        /// Specific if processes spawned with the context class want to enheirt the parent's enviroment or not.
        /// </summary>
        /// <param name="That"></param>
        /// <param name="WantDefaultAlso"></param>

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetEnvInherit")]
        public static extern void PsProcessInformation_SetInheritDefaultEnviroment(IntPtr That, bool WantDefaultAlso);


        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_ClearEnvBlock")]
        public static extern void PsProcessInformation_ClearExplicitEnviromentBlock(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_AssignEnvValue")]
        public static extern void PsProcessInformation_SetExplicitEnviromentValue(IntPtr That, string Name, string Value);
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_GetEnvValue")]
        public static extern IntPtr PsProcessInformation_GetExplicitEnviromentValue(IntPtr That, string Name);


        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Ansi, EntryPoint = "PS_ProcessInformation_AddDetoursDllA")]
        public static extern bool PsProcessInformation_AddDetourDllToLoad(IntPtr That, string Name);

        // "PS_ProcessInformation_AddDetoursDllW" is not importated as the implemention just converts to ansi before calling PS_ProcessInformation_AddDetoursDllA
        // the marshaling stuff will take care of that already.
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_ClearDetoursList")]
        public static extern void PsProcessInformation_ClearDetourList(IntPtr That);

 
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetDebugMode")]
        public static extern void PsProcessInformation_SetDebugMode(IntPtr That, [MarshalAs(UnmanagedType.U4)] DebugModeType Ty);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_GetDebugMode")]
        public static extern DebugModeType PsProcessInformation_GetDebugMode(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_GetDebugEventCallback")]
        public static extern Delegate PsProcessInformation_GetDebugCallbackRoutine(IntPtr that);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_SetDebugEventCallback")]
        public static extern IntPtr PsProcessInformation_SetDebugCallbackRoutine(IntPtr that, [MarshalAs(UnmanagedType.FunctionPtr)] Delegate CallBack);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "PS_ProcessInformation_PulseDebugThread")]
        public static extern void PsProcessInformation_PulseDebugEvent(IntPtr that);



        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcessInformation_GetMainThreadHandle")]
        public static extern IntPtr PsProcessInformation_GetMainThreadHandle(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcessInformation_GetMainProcessHandle")]
        public static extern IntPtr PsProcessInformation_GetMainProcessHandle(IntPtr That);

        
    

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcesInformation_GetCommandment")]
        public static extern bool PsProcesInformation_GetCommandment(IntPtr That, uint Cmd);


        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcessInformation_SetCommandment")]
        public static extern bool PsProcessInformation_SetCommandment(IntPtr That, uint Cmd, bool Enabled);

        
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcessInformation_GetMainProcessId")]
        public static extern uint P_ProcessInformation_GetMainProcessId(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcessInformation_GetMainThreadId")]
        public static extern DebugModeType PsProcessInformation_GetMainThreadId(IntPtr That);


        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "Ps_ProcessInformation_SetSymbolHandling", ExactSpelling = false)]
        public static extern bool PsProcessInformation_SetSymbolHandling(IntPtr That, bool NewVal);


        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "Ps_ProcessInformation_GetSymbolHandling", ExactSpelling = false)]
        public static extern bool PsProcessInformation_GetSymbolHandling(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "Ps_ProcessInformation_GetSymbolHandlerClass", ExactSpelling = false)]
        public static extern IntPtr PsProcessInformation_GetSymbolEngineClassPtr(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "Ps_ProcessInformation_AddPriorityDllPathW", ExactSpelling = false, CharSet= CharSet.Unicode)]
        public static extern bool PsProcessInformation_AddPriorityLoadLibraryPath(IntPtr that, string NewLocation);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "Ps_ProcessInformation_GetPriorityDllPath_NumberOf", ExactSpelling = false)]
        public static extern uint PsProcessInformation_GetProrityLoadLibraryPath_NumberOf(IntPtr that);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "Ps_ProcessInformation_IndexPriorityDllPath", ExactSpelling = false)]
        public static extern IntPtr PsProcessInformation_IndexPriorityDllPath(IntPtr that, int index);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, EntryPoint = "Ps_ProcessInformation_ClearPriorityDllPath", ExactSpelling = false)]
        public static extern void PsProcessInformation_ClearPriorityDllPath(IntPtr that);


        
        
    }
}
