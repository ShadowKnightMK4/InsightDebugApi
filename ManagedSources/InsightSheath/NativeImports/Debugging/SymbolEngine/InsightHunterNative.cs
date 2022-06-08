using InsightSheath.Debugging.SymbolEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static InsightSheath.Debugging.SymbolEngine.InsightHunter;


namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Internal Class. This imports the routines used in <see cref="InsightHunter"/> class
    /// </summary>
    internal static partial class InsightHunterInternal
    {
        /// <summary>
        /// IntPtr is an API_VERSION struct that's part of the InsightHunter class itself.
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetImageHelpVersionData", ExactSpelling = false)]
      
        public static extern IntPtr Insight_GetImageHelpVersionData(IntPtr That);
        //Insight_GetImageHelpVersionData

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_SetImageHelpCompatability", ExactSpelling = false)]
        public static extern bool Insight_SetImageHelpCompatability(IntPtr That, ushort Major, ushort Minor, ushort Revision);
            //Insight_SetImageHelpCompatability
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_LoadExeSymbolInfo", ExactSpelling = false)]
        public static extern bool Insight_LoadExeSymbolInfo(IntPtr That, IntPtr DebugEventPtr);
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_LoadDllSymbolInfo", ExactSpelling = false)]
        public static extern bool Insight_LoadDllSymbolInfo(IntPtr That, IntPtr DebugEventPtr);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_UnLoadExeSymbolInfo", ExactSpelling = false)]
        public static extern bool Insight_UnLoadExeSymbolInfo(IntPtr That, IntPtr DebugEventPtr);
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_SetParentWindow", ExactSpelling = false)]
        public static extern bool Insight_SetParentWindow(IntPtr that, IntPtr TargetWindow);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetThreadSync", ExactSpelling = false)]
        public static extern bool Insight_GetThreadSync(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_SetThreadSync", ExactSpelling = false)]
        public static extern bool Insight_SetThreadSync(IntPtr That, bool EnableSync);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_RefreshLoadedModules", ExactSpelling = false)]
        public static extern bool Insight_RefreshLoadedModules(IntPtr That);


        // BOOL WINAPI Insight_EnumerateLoadedSymbolsW(InsightHunter* that, wchar_t* Symbol, SymbolSearchCallback* Api)
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_EnumerateLoadedSymbolsW", ExactSpelling = false)]
        public static extern bool Insight_EnumerateLoadedSymbolsW(IntPtr That, string SymbolSeardch,  InsightHunter_SymbolSearchCallBackRoutine DotNotCallback);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetSymbolOptions", ExactSpelling = false)]
        public static extern SymbolOptionsFlags Insight_GetSymbolOptions(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetSymbolOptions", ExactSpelling = false)]
        public static extern SymbolOptionsFlags Insight_SetSymbolOptions(IntPtr That, SymbolOptionsFlags OptionMask);

        
        // BOOL WINAPI Insight_EnumerateLoadedSymbolsW(InsightHunter* that, wchar_t* Symbol, SymbolSearchCallback* Api)
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_EnumerateSourceFiles", ExactSpelling = false)]
        public static extern bool Insight_EnumerateSourceFilesW(IntPtr That, string SearchString, InsightHunter_SymbolSourceCallbackRoutine DotNotCallback);

    }
}
