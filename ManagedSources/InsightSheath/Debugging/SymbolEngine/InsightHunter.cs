using InsightSheath.Abstract;
using InsightSheath.Debugging;
using InsightSheath.Win32Struct;
using System;
using System.Runtime.InteropServices;
using InsightSheath.Debugging.Process;
using InsightSheath.NativeImports;

namespace InsightSheath.Debugging.SymbolEngine
{
    /// <summary>
    /// This is the callback for <see cref="InsightHunter.EnumerateSymbols(string, InsightHunter_SymbolSearchCallBackRoutine)"/>. The passed pointer to your routine is a native pointer to a struct <see cref="SymbolInfo"/>'s that exists for while your routine is active. 
    /// </summary>
    /// <param name="SymbolInfo">Your routine will get an <see cref="IntPtr"/> pointing to a an unmanaged memory block containing symbol data that matched. <see cref="SymbolInfo"/>. This unmanaged memory only contains it for the life of your callback. Duplicate if you need to store or use later.</param>
    /// <returns>Your Routine should return TRUE to keep going and FALSE to quit</returns>
    /// <remarks>The exact routine pointer  that this is based off is  typedef BOOL (WINAPI* SymbolSearchCallback)(SYMBOL_INFOW);  MSDC documentation 
    /// for the callback being invoked here: <see href="https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nc-dbghelp-psym_enumsourcefiles_callback"/> and the API being called is <see href="https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-symenumsourcefilesw"/>  </remarks>
    public delegate bool InsightHunter_SymbolSearchCallBackRoutine(IntPtr SymbolInfo);

    /// Callback for Enumerating source files. Return true to continue and false to quit
    //typedef BOOL(WINAPI* SymbolSourceCallBack)(PSOURCEFILEW);
    public delegate bool InsightHunter_SymbolSourceCallbackRoutine(IntPtr SourceInfo);


    /// <summary>
    /// A part of the symbol engine exported in InsightAPI in the Native DLL. 
    /// This class is VERY VERY VERY tightly coupled with the <see cref="InsightProcess"/> class but distinct enough to warrant its own class/wrapper wrappers. 
    /// If you need not have symbol processing, disabling <see cref="InsightProcess.EnableSymbolEngine"/> is set to false if fine. That turns off the code that updates the native <see cref="InsightHunter"/> class when the worker thread receives a debug event
    /// NOTE: You'll need to spawn at least one process with <see cref="InsightProcess"/> to get much use out of this.
    /// </summary>
    public class InsightHunter : NativeStaticContainer
    {
        /// <summary>
        /// Specifies combinations of flags to Set for the symbol engine as seen in MSDN's <see href="https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-symsetoptions"/>
        /// </summary>
        [Flags]
        public enum SymbolOptionsFlags : uint
        {
            /// <summary>
            /// Enable use of symboled stored as absolute instead only of Relative Addressing that get converted to absolute addresses.  Requires DbgHelp version 5.2 or later.
            /// </summary>
            AllowAbsoluteSymbols = 0x00000800,
            /// <summary>
            /// Enable seeing symbols that have no defined address. Default is that the Symbol Engine filters these out.
            /// </summary>
            AllowZeroAddress = 0x01000000,
            /// <summary>
            /// Disables searching publics symbols when searching by address/ enumerating or not found in global and current score.  Ignored if <see cref="PublicsOnly"/> is set also.
            /// </summary>
            AutoPublics = 0x00010000,
            /// <summary>
            /// If set, searches in the symbol engine are case insensitive. 
            /// </summary>
            CaseInsensitive = 0x00000001,
            /// <summary>
            /// Enable additional output to either a callback function (TODO: write that wrapper) or <see href="https://docs.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-outputdebugstringw"/>
            /// </summary>
            DebugMode = 0x80000000,
            /// <summary>
            /// Symbols are not loaded until a reference is encountered.  Better for performance.
            /// </summary>
            DeferredLoad = 0x00000004,
            /// <summary>
            /// Disable the auto detecting of symbol server stores in the server path.  Requires 6.6 or later.
            /// </summary>
            DisableSymServerAutoDetect = 0x02000000,
            /// <summary>
            /// Refuse to load Symbol files that don't have a matching binary. Do not load export symbols.
            /// </summary>
            ExactSymbolRequired = 0x02000000,
            /// <summary>
            /// Should a critical issue such as a resource is not available (CD removed from drive for example), fail the request without informing the user.
            /// </summary>
            FailCriticalErrors = 0x00000200,
            /// <summary>
            /// Tell the Dbghlp routines to favor compressed symbol files over uncompressed should both be accessible.
            /// </summary>
            FavorCompressed = 0x00800000,
            /// <summary>
            /// Symbols are stored in the root directory default downstream store.  Requires Dbghlp 6.2 or greater.
            /// </summary>
            FlatDirectory = 0x00400000,
            /// <summary>
            /// Tell Dbghlp to ignore any CodeView records it finds when loading symbols.
            /// </summary>
            IgnoreCodeViewRecord = 0x00000080,
            /// <summary>
            /// Tell Dbghlp to ignore the image directory.  Requires Dbghlp 6.2 or greater.
            /// </summary>
            IgnoreImageDir = 0x00200000,
            /// <summary>
            /// Do not use the _NT_SYMBOL_PATH if setting an invalid path.  Not supported for Debug Help 5.1.
            /// </summary>
            IgnoreNtSymPath = 0x00001000,
            /// <summary>
            /// x64 Windows.  Tell the symbol engine to load for 32-bit modules also.
            /// </summary>
            Include32BitModules = 0x00002000,
            /// <summary>
            /// Disable checks that ensure loaded files are valid.  Loads the first file found.
            /// </summary>
            LoadAnything = 0x00000040,
            /// <summary>
            /// Load Line information for files.
            /// </summary>
            LoadLines = 0x00000010,
            /// <summary>
            /// This has the symbol engine replace C++ decorated symbols including '::' with '__'.
            /// </summary>
            SYMOPT_NO_CPP = 0x00000008,
            /// <summary>
            /// Disables searching the image for the current symbol path when loading symbols for a module that cannot be read.  Not supported for DbhHlp 5.1
            /// </summary>
            NoImageSearch = 0x00020000,
            /// <summary>
            /// Disabling showing a prompt to validate that the correct symbol server is used.
            /// </summary>
            NoPrompts = 0x00080000,
            /// <summary>
            ///  Disable searching public tables for symbols.  MSDN documentation says global tables likely will have duplicates of public table data. Not supported for DbgHlp 5.1
            /// </summary>
            NoPublics = 0x00008000,
            /// <summary>
            /// Prevent symbols from being loaded when caller examines symbols across multiple module boundaries. 
            /// </summary>
            NoUnqualifiedLoads = 0x00000100,
            /// <summary>
            /// Overwrite download store from the symbol table. Requires Dbghlp 6.2 or greater.
            /// </summary>
            Overwrite = 0x00100000,
            /// <summary>
            /// Disables loading private disable.  Requires DbgHlp 5.2 or greater.  Before 5.2, it only loaded public symbols.
            /// </summary>
            PublicsOnly = 0x00004000,
            /// <summary>
            /// Refuse to load symbols not in the symbol server and _NT_SYMBOL_PATH DbgHelp 6.0-6.1 Says flag can be cleared and 5.1 says not supported.
            /// </summary>
            SecureMode = 0x00040000,
            /// <summary>
            /// Undecorated public symbols when encountered.
            /// </summary>
            UndecorateSymbols = 0x00000002
        }

        /// <summary>
        /// Initialize an instance of the wrapper for the unmanaged <see cref="InsightHunter"/> class to use the copy pointed to by that. 
        /// </summary>
        /// <param name="That">non null pointer to an instance of the unmanaged class.</param>
        public InsightHunter(IntPtr That) : base(That)
        {
            if (That == IntPtr.Zero)
            {
                throw WrapperConstructorReceivedNullPointerErrorException("ERROR:", "InsightProcess.GetSymbolHandler()", nameof(That));
            }
            SyncAccess = true;
        }
        /// <summary>
        /// Initialize an instance of the wrapper for the unmanaged <see cref="InsightHunter"/> class to use the copy pointed to by that. 
        /// </summary>
        /// <param name="That">non null pointer to an instance of the unmanaged class.</param>
        /// <param name="FreeOnDispose">Indicate if the object is subject to being freed() when GC cleans up. Recommend FALSE if needing a duplication wrapper to the unmanaged class</param>
        public InsightHunter(IntPtr That, bool FreeOnDispose) : base(That, FreeOnDispose)
        {
            if (That == IntPtr.Zero)
            {
                throw WrapperConstructorReceivedNullPointerErrorException("ERROR:", "InsightProcess.GetSymbolHandler()", nameof(That));
            }
            SyncAccess = true;
        }

        #region public exported API
        /// <summary>
        ///        
        /// </summary>
        /// <param name="debugEvent"><see cref="DebugEvent"/> class instance containing a <see cref="DebugEventType.CreateProcessEvent"/> event</param>
        /// <returns>true if OK, false on failure</returns>
        public bool LoadExeSymbolInfo(DebugEvent debugEvent)
        {
            return NativeImports.InternalInsightHunter.Insight_LoadExeSymbolInfo(Native, debugEvent.NativePointer);
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="DebugEvent"><see cref="DebugEvent"/> class instance containing a <see cref="DebugEventType.CreateProcessEvent"/> event</param>
        /// <returns>true if OK, false on failure</returns>
        public bool LoadExeSymbolInfo(IntPtr DebugEvent)
        {

            return InternalInsightHunter.Insight_LoadExeSymbolInfo(Native, DebugEvent);
        }


        /// <summary>
        ///
        /// </summary>
        /// <param name="DebugEvent">class instance containing a <see cref="DebugEventType.LoadDllEvent"/> event</param>
        /// <returns>true if OK, false on failure</returns>
        public bool LoadDllSymbolInfo(DebugEvent DebugEvent)
        {
            return InternalInsightHunter.Insight_LoadDllSymbolInfo(Native, DebugEvent.NativePointer);
        }



        /// <summary>
        ///        
        /// </summary>
        /// <param name="DebugEvent">class instance containing a <see cref="DebugEventType.UnloadDllEvent"/> event</param>
        /// <returns>true if it worked, false otherwise</returns>
        public bool UnloadDllSymbolInfo(DebugEvent DebugEvent)
        {
            return NativeImports.InternalInsightHunter.Insight_UnLoadExeSymbolInfo(Native, DebugEvent.NativePointer);
        }
        /// <summary>
        ///        
        /// </summary>
        /// <param name="DebugEvent">class instance containing a <see cref="DebugEventType.UnloadDllEvent"/> event</param>
        /// <returns>true if it worked, false otherwise</returns>
        public bool UnloadDllSymbolInfo(IntPtr DebugEvent)
        {
            return InternalInsightHunter.Insight_UnLoadExeSymbolInfo(Native, DebugEvent);
        }



        /// <summary>
        /// Set the parent window for the symbol engine/ debug help API
        /// </summary>
        /// <param name="HWND">Handle to Win32 Window that the debug help API  will assume you're outputting info too</param>
        /// <returns>True if it worked and false if it did not.</returns>
        public bool SetParentWindow(IntPtr HWND)
        {
            return InternalInsightHunter.Insight_SetParentWindow(Native, HWND);
        }


        /// <summary>
        /// Enumerate loaded symbols for the debugged process
        /// </summary>
        /// <param name="SearchString"></param>
        /// <param name="DotNetCallback"></param>
        /// <returns></returns>
        public bool EnumerateSymbols(string SearchString, InsightHunter_SymbolSearchCallBackRoutine DotNetCallback)
        {
            return InternalInsightHunter.Insight_EnumerateLoadedSymbolsW(Native, SearchString, DotNetCallback);
        }

        /// <summary>
        /// Enumerate loaded source files for symbol 
        /// </summary>
        /// <param name="SearchString"></param>
        /// <param name="DotNetCallBack"></param>
        /// <returns></returns>
        public bool EnumerateSourceFiles(string SearchString, InsightHunter_SymbolSourceCallbackRoutine DotNetCallBack)
        {
            return NativeImports.InternalInsightHunter.Insight_EnumerateSourceFilesW(Native, SearchString, DotNetCallBack);
        }
        /// <summary>
        /// reload loaded modules.  
        /// </summary>
        /// <returns>true if it worked and false otherwise</returns>
        public bool RefreshModules()
        {
            return InternalInsightHunter.Insight_RefreshLoadedModules(Native);
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
                return InternalInsightHunter.Insight_GetThreadSync(Native);
            }
            set
            {
                InternalInsightHunter.Insight_SetThreadSync(Native, value);
            }
        }

        /// <summary>
        /// Specify what you want to the symbol handler to too.
        /// </summary>
        public SymbolOptionsFlags SymbolOptions
        {
            get
            {
                return InternalInsightHunter.Insight_GetSymbolOptions(Native);
            }
            set
            {
                InternalInsightHunter.Insight_SetSymbolOptions(Native, value);
            }
        }

        /// <summary>
        /// Get or set Version Data on the Debug Help API the symbol engine actually uses.
        /// </summary>
        public DebugHelp_ApiVersionStruct DebugHelp_Version
        {
            get
            {
                return  new DebugHelp_ApiVersionStruct(InternalInsightHunter.Insight_GetImageHelpVersionData(Native));
            }
            set
            {
                InternalInsightHunter.Insight_SetImageHelpCompatability(value.NativePointer, value.Major, value.Minor, value.Revision);
            }
        }

        /// <summary>
        /// Get or a copy of the Debug Version or set version compatibility. 
        /// </summary>
        public API_VERSION DebugHelp_Version2
        {
            get
            {
                return Marshal.PtrToStructure<API_VERSION>( NativeImports.InternalInsightHunter.Insight_GetImageHelpVersionData(Native));
            }
            set
            {
                NativeImports.InternalInsightHunter.Insight_SetImageHelpCompatability(this.Native, value.MajorVersion, value.MinorVersion, value.Revision);
            }
        }
        #endregion
    }
}
