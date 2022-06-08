using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using InsightSheath.Wrappers;
using InsightSheath.Debugging.SymbolEngine;

namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Internal Class. This imports the routines used in <see cref="SymbolInfo"/> class
    /// </summary>
    internal static class SymbolInfoInternal
    {
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetStructSize", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetStructSize(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetStructSize", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetTypeIndex(IntPtr That);

        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetStructSize", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetSymbolSize(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetFlags", ExactSpelling = false)]
        public static extern SymbolFlags SymbolInfo_GetFlags(IntPtr That);


        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetVal", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetVal(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetReg", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetReg(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetValOrReg", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetValOrReg(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetScope", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetScope(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetName", ExactSpelling = false)]
        public static extern IntPtr SymbolInfo_GetName(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetNameCharCount", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetNameLemngth(IntPtr That);



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetIndex", ExactSpelling = false)]
        public static extern uint SymbolInfo_GetIndex(IntPtr That);


        
        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfoW_GetModBase", ExactSpelling = false)]
        public static extern ulong SymbolInfo_GetModuleBase(IntPtr That);

        



        [DllImport("InsightApi.Dll", BestFitMapping = false, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, EntryPoint = "SymbolInfo_GetTagType", ExactSpelling = false)]
        public static extern SymbolTagType SymbolInfo_GetTagType(IntPtr That);


    }
}
