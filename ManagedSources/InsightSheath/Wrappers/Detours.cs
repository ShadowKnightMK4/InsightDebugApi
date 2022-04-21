using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.NativeImports;
namespace InsightSheath.Wrappers
{
 
    /// <summary>
    /// Intended for wrapps
    /// </summary>
    public static class Detours
    {
        /// <summary>
        /// Like DetourEnumerateModules() but one can specific a target process.
        /// TODO: NOT FUNCTIONAL
        /// </summary>
        /// <param name="hProcess"></param>
        /// <param name="hModule"></param>
        /// <returns></returns>
        public static IntPtr DetourEnumerateModulesEx(IntPtr hProcess, IntPtr hModule)
        {
            return NativeMethods.DetourEnumerateModulesEx(hProcess, hModule);
        }
    }
}
