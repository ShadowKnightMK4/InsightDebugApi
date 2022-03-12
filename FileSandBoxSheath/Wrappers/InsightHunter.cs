using FileSandBoxSheath.Structs;
using System;
using System.Runtime.InteropServices;

namespace FileSandBoxSheath.Wrappers
{
    /// <summary>
    /// A part of the symbol engine is known as InsightHunter in the FileSanboxNative api.  It is VERY VERY VERY tightly coupled with the PsProcessInformation class but distict enough to warenete wrappers. If you need not have symbol processing, disablying is fine.
    /// NOTE: You'll need to spawn at least one pocess with <see cref="PsProcessInformation"/> to get much use out of this.
    /// </summary>
    public class InsightHunter : NativeStaticContainer
    {
        // typedef BOOL (WINAPI* SymbolSearchCallback)(SYMBOL_INFOW);
        /// <summary>
        /// This is the callback for <see cref="InsightHunter.EnumerateSymbols(string, SymbolSearchCallBackRoutine)"/>. Pass passed pointer to your routine to <see cref="SymbolInfo"/>'s contructor to use
        /// </summary>
        /// <param name="">Your Routine should return TRUE to keep going and FALSE to quit</param>
        /// <returns></returns>
        public delegate bool SymbolSearchCallBackRoutine(IntPtr SymbolInfo);
        /// <summary>
        /// Specifies combinations of flags to Set for the symbol engine as seein in MSDN's SymSetOptions
        /// </summary>
        [Flags]
        public enum SymbolOptionsFlags : uint
        {
            AllowAbsoluteSymbols = 0x00000800,
            AllowZeroAddress = 0x01000000,
            AutoPublics = 0x00010000,
            CaseInsentive = 0x00000001,
            DebugMode = 0x80000000,
            DeferredLoad = 0x00000004,
            DisableSymServerAutoDetect = 0x02000000,
            ExactSymbolRequired = 0x02000000,
            FailCriticalErrors = 0x00000200,
            FavorCompressed = 0x00800000,
            FlatDirectory = 0x00400000,
            IgnoreCodeViewRecord = 0x00000080,
            IgnoreImageDir = 0x00200000,
            IgnoreNtSymPath = 0x00001000,
            Include32BitModules = 0x00002000,
            LoadAnything = 0x00000040,
            LoadLines = 0x00000010,
            SYMOPT_NO_CPP = 0x00000008,
            NoImageSearch = 0x00020000,
            NoPrompts = 0x00080000,
            NoPublics = 0x00008000,
            NoUnqualifiedLoads = 0x00000100,
            Overwrite = 0x00100000,
            PublicsOnly = 0x00004000,
            /// <summary>
            /// WARNING YOU CANNOT CLEAR this flag once set.
            /// </summary>
            SecureMode = 0x00040000,
            UndecorateSymbols = 0x00000002
        }

        public InsightHunter(IntPtr That) : base(That)
        {
            SyncAccess = true;
        }

        public InsightHunter(IntPtr That, bool FreeOnDispose) : base(That, FreeOnDispose)
        {
            SyncAccess = true;
        }

        #region public exported api
        /// <summary>
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="PsProcessInformation"/>. This tells the symbol engine to load an exe's debug data in reponse to a CREATE_PROCESS_DEBUG_EVENT
        /// </summary>
        /// <param name="debugEvent"><see cref="DebugEvent"/> class instance containing a <see cref="DebugEventType.CreateProcessEvent"/> event</param>
        /// <returns>true if ok, false on failuree</returns>
        public bool LoadExeSymbolInfo(DebugEvent debugEvent)
        {
            return NativeImports.NativeMethods.Insight_LoadExeSymbolInfo(Native, debugEvent.NativePointer);
        }

        /// <summary>
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="PsProcessInformation"/>. This tells the symbol engine to load an exe's debug data in reponse to a CREATE_PROCESS_DEBUG_EVENT
        /// </summary>
        /// <param name="debugEvent"><see cref="DebugEvent"/> class instance containing a <see cref="DebugEventType.CreateProcessEvent"/> event</param>
        /// <returns>true if ok, false on failuree</returns>
        public bool LoadExeSymbolInfo(IntPtr DebugEvent)
        {

            return NativeImports.NativeMethods.Insight_LoadExeSymbolInfo(Native, DebugEvent);
        }


        /// <summary>
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="PsProcessInformation"/>. This tells the symbol engine to load an exe's debug data in resposne to a LOAD_DLL_DEBUG_EVENT
        /// </summary>
        /// <param name="DebugEvent">class instance containing a <see cref="DebugEventType.LoadDllEvent"/> event</param>
        /// <returns>true if ok, false on failuree</returns>
        public bool LoadDllSymbolInfo(DebugEvent DebugEvent)
        {
            return NativeImports.NativeMethods.Insight_LoadDllSymbolInfo(Native, DebugEvent.NativePointer);
        }



        /// <summary>
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="PsProcessInformation"/>. This tells the symbol engine to load an exe's debug data in resposne to a UNLOAD_DLL_DEBUG_EVENT
        /// </summary>
        /// <param name="DebugEvent">class instance containing a <see cref="DebugEventType.UnloadDllEvent"/> event</param>
        /// <returns>true if it worked, false otherwise</returns>
        public bool UnloadDllSymbolInfo(DebugEvent DebugEvent)
        {
            return NativeImports.NativeMethods.Insight_UnLoadExeSymbolInfo(Native, DebugEvent.NativePointer);
        }

        public bool UnloadDllSymbolInfo(IntPtr DebugEvent)
        {
            return NativeImports.NativeMethods.Insight_UnLoadExeSymbolInfo(Native, DebugEvent);
        }



        /// <summary>
        /// Set the parent window for the symbol engine/ debug help api
        /// </summary>
        /// <param name="HWND">Handle to Win32 Window that the debug help api will assume you're outputting info too</param>
        /// <returns></returns>
        public bool SetParentWindow(IntPtr HWND)
        {
            return NativeImports.NativeMethods.Insight_SetParentWindow(Native, HWND);
        }


        public bool EnumerateSymbols(string SearchString, SymbolSearchCallBackRoutine DotNetCallback)
        {
            return NativeImports.NativeMethods.Insight_EnumerateLoadedSymbolsW(Native, SearchString, DotNetCallback);
        }
        /// <summary>
        /// reload loaded modules.  
        /// </summary>
        /// <returns>true if it worked and false otherwise</returns>
        public bool RefreshModules()
        {
            return NativeImports.NativeMethods.Insight_RefreshLoadedModules(Native);
        }
        #endregion

        #region Properties
        /// <summary>
        /// Default is true.  This being true means the NativeClass InsightHunter synchronizes calls to the Debug Help Symbol Engine with a CRITICAL_SECTION Win32 Object
        /// </summary>
        public bool SyncAccess
        {
            get
            {
                return NativeImports.NativeMethods.Insight_GetThreadSync(Native);
            }
            set
            {
                NativeImports.NativeMethods.Insight_SetThreadSync(Native, value);
            }
        }

        /// <summary>
        /// Specify what you want to the symbol handler to too.
        /// </summary>
        public SymbolOptionsFlags SymbolOptions
        {
            get
            {
                return NativeImports.NativeMethods.Insight_GetSymbolOptions(Native);
            }
            set
            {
                NativeImports.NativeMethods.Insight_SetSymbolOptions(Native, value);
            }
        }

        /// <summary>
        /// Get Version Data on the Debug Help API the symbol engine actuallly uses.
        /// </summary>
        public DebugHelp_ApiVersionStruct DebugHelp_Version
        {
            get
            {
                return  new DebugHelp_ApiVersionStruct(NativeImports.NativeMethods.Insight_GetImageHelpVersionData(Native));
            }
            set
            {
                NativeImports.NativeMethods.Insight_SetImageHelpCompatability(value.NativePointer, value.Major, value.Minor, value.Revision);
            }
        }

        public API_VERSION DebugHelp_Version2
        {
            get
            {
                return Marshal.PtrToStructure<API_VERSION>( NativeImports.NativeMethods.Insight_GetImageHelpVersionData(Native));
            }
            set
            {
                NativeImports.NativeMethods.Insight_SetImageHelpCompatability(this.Native, value.MajorVersion, value.MinorVersion, value.Revision);
            }
        }
        #endregion
    }
}
