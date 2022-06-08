using InsightSheath.Abstract;
using InsightSheath.Debugging;
using InsightSheath.Win32Struct;
using System;
using System.Runtime.InteropServices;
using InsightSheath.Debugging.Process;

namespace InsightSheath.Debugging.SymbolEngine
{
    /// <summary>
    /// This is the callback for <see cref="InsightHunter.EnumerateSymbols(string, InsightHunter_SymbolSearchCallBackRoutine)"/>. The passed pointer to your routine is a native pointer to a struct <see cref="SymbolInfo"/>'s that exists for while your routine is active. 
    /// </summary>
    /// <param name="">Your Routine should return TRUE to keep going and FALSE to quit</param>
    /// <returns></returns>
    /// <remarks>The exact routine pointer  that this is based off is  typedef BOOL (WINAPI* SymbolSearchCallback)(SYMBOL_INFOW);  MSDC documentation 
    /// for the callback being invoked here: <see href="https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nc-dbghelp-psym_enumsourcefiles_callback"/> and the API being called is <see href="https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-symenumsourcefilesw"/>  </remarks>
    public delegate bool InsightHunter_SymbolSearchCallBackRoutine(IntPtr SymbolInfo);

    /// Callback for Enumerating source files. Return true to continue and false to quit
    //typedef BOOL(WINAPI* SymbolSourceCallBack)(PSOURCEFILEW);
    public delegate bool InsightHunter_SymbolSourceCallbackRoutine(IntPtr SourceInfo);


    /// <summary>
    /// A part of the symbol engine exported in InsightAPI in the Native DLL. 
    /// This class is VERY VERY VERY tightly coupled with the <see cref="InsightProcess"/> class but distinct enough to warrent its own class/wrapper wrappers. 
    /// If you need not have symbol processing, disabling <see cref="InsightProcess.EnableSymbolEngine"/> is set to false if fine. That turns off the code that updates the native <see cref="InsightHunter"/> class when the worker thread receives a debug event
    /// NOTE: You'll need to spawn at least one process with <see cref="InsightProcess"/> to get much use out of this.
    /// </summary>
    public class InsightHunter : NativeStaticContainer
    {
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
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="InsightProcess"/>. This tells the symbol engine to load an exe's debug data in reponse to a CREATE_PROCESS_DEBUG_EVENT
        /// </summary>
        /// <param name="debugEvent"><see cref="DebugEvent"/> class instance containing a <see cref="DebugEventType.CreateProcessEvent"/> event</param>
        /// <returns>true if ok, false on failuree</returns>
        public bool LoadExeSymbolInfo(DebugEvent debugEvent)
        {
            return NativeImports.InsightHunterInternal.Insight_LoadExeSymbolInfo(Native, debugEvent.NativePointer);
        }

        /// <summary>
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="InsightProcess"/>. This tells the symbol engine to load an exe's debug data in reponse to a CREATE_PROCESS_DEBUG_EVENT
        /// </summary>
        /// <param name="debugEvent"><see cref="DebugEvent"/> class instance containing a <see cref="DebugEventType.CreateProcessEvent"/> event</param>
        /// <returns>true if ok, false on failuree</returns>
        public bool LoadExeSymbolInfo(IntPtr DebugEvent)
        {

            return NativeImports.InsightHunterInternal.Insight_LoadExeSymbolInfo(Native, DebugEvent);
        }


        /// <summary>
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="InsightProcess"/>. This tells the symbol engine to load an exe's debug data in resposne to a LOAD_DLL_DEBUG_EVENT
        /// </summary>
        /// <param name="DebugEvent">class instance containing a <see cref="DebugEventType.LoadDllEvent"/> event</param>
        /// <returns>true if ok, false on failuree</returns>
        public bool LoadDllSymbolInfo(DebugEvent DebugEvent)
        {
            return NativeImports.InsightHunterInternal.Insight_LoadDllSymbolInfo(Native, DebugEvent.NativePointer);
        }



        /// <summary>
        /// Not normally need to  be called (if using the working thread<see cref=DebugModeType.WorkerThread"/> in <see cref="InsightProcess"/>. This tells the symbol engine to load an exe's debug data in resposne to a UNLOAD_DLL_DEBUG_EVENT
        /// </summary>
        /// <param name="DebugEvent">class instance containing a <see cref="DebugEventType.UnloadDllEvent"/> event</param>
        /// <returns>true if it worked, false otherwise</returns>
        public bool UnloadDllSymbolInfo(DebugEvent DebugEvent)
        {
            return NativeImports.InsightHunterInternal.Insight_UnLoadExeSymbolInfo(Native, DebugEvent.NativePointer);
        }

        public bool UnloadDllSymbolInfo(IntPtr DebugEvent)
        {
            return NativeImports.InsightHunterInternal.Insight_UnLoadExeSymbolInfo(Native, DebugEvent);
        }



        /// <summary>
        /// Set the parent window for the symbol engine/ debug help api
        /// </summary>
        /// <param name="HWND">Handle to Win32 Window that the debug help api will assume you're outputting info too</param>
        /// <returns></returns>
        public bool SetParentWindow(IntPtr HWND)
        {
            return NativeImports.InsightHunterInternal.Insight_SetParentWindow(Native, HWND);
        }


        /// <summary>
        /// Enumerate loaded symbols for the debugged exe
        /// </summary>
        /// <param name="SearchString"></param>
        /// <param name="DotNetCallback"></param>
        /// <returns></returns>
        public bool EnumerateSymbols(string SearchString, InsightHunter_SymbolSearchCallBackRoutine DotNetCallback)
        {
            return NativeImports.InsightHunterInternal.Insight_EnumerateLoadedSymbolsW(Native, SearchString, DotNetCallback);
        }

        /// <summary>
        /// Enumerate loaded source files for symbol 
        /// </summary>
        /// <param name="SearchString"></param>
        /// <param name="DotNetCallBack"></param>
        /// <returns></returns>
        public bool EnumerateSourceFiles(string SearchString, InsightHunter_SymbolSourceCallbackRoutine DotNetCallBack)
        {
            return NativeImports.InsightHunterInternal.Insight_EnumerateSourceFilesW(Native, SearchString, DotNetCallBack);
        }
        /// <summary>
        /// reload loaded modules.  
        /// </summary>
        /// <returns>true if it worked and false otherwise</returns>
        public bool RefreshModules()
        {
            return NativeImports.InsightHunterInternal.Insight_RefreshLoadedModules(Native);
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
                return NativeImports.InsightHunterInternal.Insight_GetThreadSync(Native);
            }
            set
            {
                NativeImports.InsightHunterInternal.Insight_SetThreadSync(Native, value);
            }
        }

        /// <summary>
        /// Specify what you want to the symbol handler to too.
        /// </summary>
        public SymbolOptionsFlags SymbolOptions
        {
            get
            {
                return NativeImports.InsightHunterInternal.Insight_GetSymbolOptions(Native);
            }
            set
            {
                NativeImports.InsightHunterInternal.Insight_SetSymbolOptions(Native, value);
            }
        }

        /// <summary>
        /// Get Version Data on the Debug Help API the symbol engine actuallly uses.
        /// </summary>
        public DebugHelp_ApiVersionStruct DebugHelp_Version
        {
            get
            {
                return  new DebugHelp_ApiVersionStruct(NativeImports.InsightHunterInternal.Insight_GetImageHelpVersionData(Native));
            }
            set
            {
                NativeImports.InsightHunterInternal.Insight_SetImageHelpCompatability(value.NativePointer, value.Major, value.Minor, value.Revision);
            }
        }

        public API_VERSION DebugHelp_Version2
        {
            get
            {
                return Marshal.PtrToStructure<API_VERSION>( NativeImports.InsightHunterInternal.Insight_GetImageHelpVersionData(Native));
            }
            set
            {
                NativeImports.InsightHunterInternal.Insight_SetImageHelpCompatability(this.Native, value.MajorVersion, value.MinorVersion, value.Revision);
            }
        }
        #endregion
    }
}
