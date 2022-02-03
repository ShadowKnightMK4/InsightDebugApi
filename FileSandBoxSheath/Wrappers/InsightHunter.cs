using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FileSandBoxSheath.Wrappers;
using FileSandBoxSheath;


namespace FileSandBoxSheath.Wrappers
{
    /// <summary>
    /// A part of the symbol engine is known as InsightHunter in the FileSanboxNative api.  It is VERY VERY VERY tightly coupled with the PsProcessInformation class but distict enough to warenete wrappers. If you need not have symbol processing, disabling that is fine
    /// </summary>
    public class InsightHunter: NativeStaticContainer
    {
        // typedef BOOL (WINAPI* SymbolSearchCallback)(SYMBOL_INFOW);
        /// <summary>
        /// This is the callback for <see cref="InsightHunter.EnumerateSymbols(string, SymbolSearchCallBackRoutine)"/>. Pass passed pointer to your routine to <see cref="SymbolInfo"/>'s contructor to use
        /// </summary>
        /// <param name="">Your Routine should return TRUE to keep going and FALSE to quit</param>
        /// <returns></returns>
        public delegate bool SymbolSearchCallBackRoutine(IntPtr SymbolInfo);
        public InsightHunter(IntPtr That): base(That)
        {
            SyncAccess = true;
        }

        public InsightHunter(IntPtr That, bool FreeOnDispose): base(That, FreeOnDispose)
        {
            SyncAccess = true;
        }

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
        /// <summary>
        /// Default (On DotNetSize) is true.  This being true means the NativeClass InsightHunter synchronizes calls to the Debug Help Symbol Engine with a CRITICAL_SECTION
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
    }
}
