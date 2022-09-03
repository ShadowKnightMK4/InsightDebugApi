using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using static InsightSheath.Detours.ResourceWalker;
using System.IO;
namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Imports the 
    /// </summary>
    internal static class InternalReasourceWalker
    {
        [DllImport("InsightAPI.DLL", CallingConvention= CallingConvention.StdCall, EntryPoint = "ResourceWalker_MakeInstanceTarget", CharSet = CharSet.Unicode, SetLastError =true)]
        public static extern IntPtr ResourceWalker_MakeInstanceTarget(string TargetExe, FileAccess FileAccess, FileShare FileShare);

        [DllImport("InsightAPI.DLL", CallingConvention = CallingConvention.StdCall, EntryPoint = "ResourceWalker_KillInstance", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool ResourceWalker_KillInstance(IntPtr that);

        [DllImport("InsightAPI.DLL", CallingConvention = CallingConvention.StdCall, EntryPoint = "ResourceWalker_DupInstance", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool ResourceWalker_DupInstance(IntPtr that);

        [DllImport("InsightAPI.DLL", CallingConvention = CallingConvention.StdCall, EntryPoint = "ResourceWalker_GetThreadLock", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool ResourceWalker_GetThreadLock(IntPtr that);


        [DllImport("InsightAPI.DLL", CallingConvention = CallingConvention.StdCall, EntryPoint = "ResourceWalker_SetThreadLock", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool ResourceWalker_SetThreadLock(IntPtr that, bool Status);

        // BOOL WINAPI ResourceWalker_EnumResourceNamesW(ResourceWalker* that, LPCWSTR lpType, ENUMRESNAMEPROCW Callback, LONG_PTR CustomArg, DWORD dwFlags, LANGID LangId)

        [DllImport("InsightAPI.DLL", CallingConvention = CallingConvention.StdCall, EntryPoint = "ResourceWalker_EnumResourceNamesW", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool ResourceWalker_EnumResourceNamesW(IntPtr That, IntPtr lpType, EnumResourceNameProcw UserCallback, IntPtr CustomArg, UInt32 dwFlags, int LangId);
    }
}
