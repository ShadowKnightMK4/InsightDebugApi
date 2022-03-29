using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBox_GUI
{

    [Flags]
    public enum DebugModeType
    {
        /// <summary>
        /// When Spawning a debugged process,  WE DO NOT SPAWN a seperate thread to handle events/ debug stuff. Thats on the caller
        /// </summary>
        NoWorkerThread = 0,
        /// <summary>
        /// When Spawnign a debugged process, we spawn a worker thread.  Put a call to <see cref="PsProcessInformation.PulseDebugEventThead"/> to continue after handling your debug event.
        /// </summary>
        WorkerThread = 1

    }

    internal  static partial class NativeMethods
    {
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

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcessINformation_GetMainProcessId")]
        public static extern uint P_ProcessINformation_GetMainProcessId(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, EntryPoint = "Ps_ProcessINformation_GetMainThreadId")]
        public static extern DebugModeType PsProcessINformation_GetMainThreadId(IntPtr That);



    }
}
