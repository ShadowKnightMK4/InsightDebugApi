using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static InsightSheath.Wrappers.InsightHunter;

namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Native methods is the catch all class for Imports from Native DLLs
    /// </summary>
    internal static partial class NativeMethods
    {
        /// <summary>
        /// IntPtr is an API_VERSION struct that's part of the Insight Private memebers 
        /// </summary>
        /// <param name="That"></param>
        /// <returns></returns>
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetImageHelpVersionData", ExactSpelling = false)]
      
        public static extern IntPtr Insight_GetImageHelpVersionData(IntPtr That);
        //Insight_GetImageHelpVersionData

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_SetImageHelpCompatability", ExactSpelling = false)]
        public static extern bool Insight_SetImageHelpCompatability(IntPtr That, ushort Major, ushort Minor, ushort Revision);
            //Insight_SetImageHelpCompatability
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_LoadExeSymbolInfo", ExactSpelling = false)]
        public static extern bool Insight_LoadExeSymbolInfo(IntPtr That, IntPtr DebugEventPtr);
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_LoadDllSymbolInfo", ExactSpelling = false)]
        public static extern bool Insight_LoadDllSymbolInfo(IntPtr That, IntPtr DebugEventPtr);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_UnLoadExeSymbolInfo", ExactSpelling = false)]
        public static extern bool Insight_UnLoadExeSymbolInfo(IntPtr That, IntPtr DebugEventPtr);
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_SetParentWindow", ExactSpelling = false)]
        public static extern bool Insight_SetParentWindow(IntPtr that, IntPtr TargetWindow);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetThreadSync", ExactSpelling = false)]
        public static extern bool Insight_GetThreadSync(IntPtr That);


        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_SetThreadSync", ExactSpelling = false)]
        public static extern bool Insight_SetThreadSync(IntPtr That, bool EnableSync);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_RefreshLoadedModules", ExactSpelling = false)]
        public static extern bool Insight_RefreshLoadedModules(IntPtr That);


        // BOOL WINAPI Insight_EnumerateLoadedSymbolsW(InsightHunter* that, wchar_t* Symbol, SymbolSearchCallback* Api)
        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_EnumerateLoadedSymbolsW", ExactSpelling = false)]
        public static extern bool Insight_EnumerateLoadedSymbolsW(IntPtr That, string SymbolSeardch,  SymbolSearchCallBackRoutine DotNotCallback);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetSymbolOptions", ExactSpelling = false)]
        public static extern SymbolOptionsFlags Insight_GetSymbolOptions(IntPtr That);

        [DllImport("FileSandBoxApi.dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "Insight_GetSymbolOptions", ExactSpelling = false)]
        public static extern SymbolOptionsFlags Insight_SetSymbolOptions(IntPtr That, SymbolOptionsFlags OptionMask);

    }
}
