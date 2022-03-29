using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBox_GUI
{
    /// <summary>
    /// choose an enviroment and spawn a process.
    /// This class is a wrapper for the C++ Class named "PS_ProcessInformation" implemented as a native dll in the source PS_ProcessInformation.cpp
    /// </summary>
    public class PsProcessInformation : IDisposable
    {
        /// <summary>
        /// Used to make 'typeical' flags like ResumeTHread, DebugProcessa nd DebugOnlyThisPRocess without needing to look values up
        /// </summary>
        [Flags]
        public enum SpaceCaseCreation
        {
            None = 0,
            DebugOnlyThis = 1,
            DebugChild = 2,
            CreateSuspended = 3,
        }

        public PsProcessInformation()
        {
            That = NativeMethods.CreatePsProcessInformation();
            if (That == IntPtr.Zero)
            {
                throw new InvalidOperationException("Native DLL FileSandbox Failed to instance a copy of its PS_ProcessCreation via the exported routine");
            }
        }


        #region Exported Routines
        /// <summary>
        /// Spawn the process contained within the udneryling class.  Return Value is the spawned process's ID on ok and 0 on falure.
        /// </summary>
        /// <returns></returns>
        public Int32 SpawnProcess()
        {
            return NativeMethods.PsProcessInformation_Spawn(That).ToInt32();

        }

        #endregion

        #region Member Access 



        public DebugModeType DebugMode
        {
            get
            {
                return NativeMethods.PsProcessInformation_GetDebugMode(That);
            }
            set
            {
                NativeMethods.PsProcessInformation_SetDebugMode(That, value);
            }
        }


        /// <summary>
        /// Get (or set) the routine that the debug worker thread will be calling.
        /// null (or unasssinged) means use the default one which does nother with events, does not handle exceptions, and continues until it gets a single exit process debug event
        /// </summary>
        public Delegate UserDebugCallRoutine
        {
            get
            {
                return NativeMethods.PsProcessInformation_GetDebugCallbackRoutine(That);
            }
            set
            {
                NativeMethods.PsProcessInformation_SetDebugCallbackRoutine(That, value);
            }
        }


       /// <summary>
       /// When  Spawning a process with a DEBUG option and nd <see cref="DebugMode"/> is set to <see cref="DebugModeType.WorkerThread"/>, the native dll spawns a thread to handle consuming events.
       /// The thread also using an native Event object to sync giving the events back to the caller so that the caller's gui is not interppted.
         /// </summary>
        public void PulseDebugEventThead()
        {
            NativeMethods.PsProcessInformation_PulseDebugEvent(That);
        }


        /// <summary>
        /// Contains the process that was / will be launched.
        /// </summary>
        public string ProcessName
        {
            get
            {
                return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetProcessName(That));
            }
            set
            {
                NativeMethods.PsProcessInformation_SetProcessName(That, value);
            }
        }

        /// <summary>
        /// Contains the process argumentst hat was / will be launched
        /// </summary>
        public string ProcessArguments
        {
            get
            {
                return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetProcessArgument(That));
            }
            set
            {
                NativeMethods.PsProcessInformation_SetProcessArgument(That, value);
            }
        }

        /// <summary>
        ///  Set the starting directory
        /// </summary>
        public string WorkingDirectory
        {
            get
            {
                return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetWorkingDirectory(That));
            }
            set
            {
                NativeMethods.PsProcessInformation_SetWorkingDirectory(That, value);
            }
        }


        /// <summary>
        /// Default False.  Specifiy if you want the calling process (that's yours) to provide that starting envirment. Any Explictly defined values will however overwrite the envir.
        /// </summary>
        public bool InheritDefaultEnviroment
        {
            set
            {
                NativeMethods.PsProcessInformation_SetInheritDefaultEnviroment(That, value);
            }
        }

        /// <summary>
        /// Specify process creation flags (or set then) 
        /// </summary>
        public uint CreationFlags
        {
            get
            {
                uint Val =  NativeMethods.PsProcessInformation_GetCreationFlags(That);
                if (ExtraFlags != SpaceCaseCreation.None)
                {
                    if (ExtraFlags.HasFlag(SpaceCaseCreation.DebugOnlyThis))
                    {
                        // DEBUG_PROCESS ONLY
                        Val |= 1;
                    }
                    else
                    {
                        if (ExtraFlags.HasFlag(SpaceCaseCreation.DebugChild))
                        {
                            //  DEBUG THIS PROCESS and kids.
                            Val |= 2;
                        }
                    }

                    if (ExtraFlags.HasFlag(SpaceCaseCreation.CreateSuspended))
                    {
                        /// CREATE_SUSPSENDED
                        Val |= 4;
                    }
                }
                return Val;
            }
            set
            {
                uint Val = value;
                if (ExtraFlags != SpaceCaseCreation.None)
                {
                    if (ExtraFlags.HasFlag(SpaceCaseCreation.DebugOnlyThis))
                    {
                        // DEBUG_PROCESS ONLY
                        Val |= 1;
                    }
                    else
                    {
                        if (ExtraFlags.HasFlag(SpaceCaseCreation.DebugChild))
                        {
                            //  DEBUG THIS PROCESS and kids.
                            Val |= 2;
                        }
                    }

                    if (ExtraFlags.HasFlag(SpaceCaseCreation.CreateSuspended))
                    {
                        /// CREATE_SUSPSENDED
                        Val |= 4;
                    }
                }
                NativeMethods.PsProcessInformation_SetCreationFlags(That, Val);
            }
        }

        /// <summary>
        /// Specify some specicase case flags. Or you can just directly Set CreationFlags itself.
        /// Implemented only in the C# wrapper. NOT IMPLMENTING in the C++ code.
        /// </summary>
        public SpaceCaseCreation ExtraFlags;


        /// <summary>
        /// Specify an explicit environmental value.  values that match the default envornment will overwrite it for the process
        /// </summary>
        /// <param name="Name"></param>
        /// <param name="Value"></param>
        public void SetExplicitEnviromentValue(string Name, string Value)
        {
            NativeMethods.PsProcessInformation_SetExplicitEnviromentValue(That, Name, Value);
        }

        /// <summary>
        /// Get an explicit environmental value from a previous call to SetExplicitEnviromentValue()
        /// </summary>
        /// <param name="Name"></param>
        /// <returns>returns a string if the value existed or null if it does NOT</returns>
        public string GetExplicitEnviromentValue(string Name)
        {
            return Marshal.PtrToStringUni(NativeMethods.PsProcessInformation_GetExplicitEnviromentValue(That, Name));
        }
    

        /// <summary>
        /// Add a dll to list of Detours Dlls to force the target process to load on spawn.
        /// DOES NOT WORK if process is already running.
        /// </summary>
        /// <param name="NewDllToForceLoad"></param>
        public void AddDetoursDll(string NewDllToForceLoad)
        {
            NativeMethods.PsProcessInformation_AddDetourDllToLoad(That, NewDllToForceLoad);
        }

        /// <summary>
        /// Clear the detours list
        /// </summary>
        public void ResetDetoursDllList()
        {
            NativeMethods.PsProcessInformation_ClearDetourList(That);
        }

        #endregion

        #region Descontructors

        ~PsProcessInformation()
        {
            Dispose(false);
        }

        public virtual void Dispose()
        {
            Dispose(true);
        }

        protected virtual void Dispose(bool Managed)
        {
            if (IsCleanned)
            {
                throw new ObjectDisposedException(nameof(PsProcessInformation));
            }
            if (Managed)
            {

            }
            if (That != IntPtr.Zero)
            {
                NativeMethods.KillPSProcessInformation(That);
            }
            IsCleanned = true;
        }
        IntPtr That = IntPtr.Zero;
        /// <summary>
        /// Once the dipose()/ finalize() is ran, this is set to true
        /// </summary>
        bool IsCleanned = false;
        #endregion
    }
}
