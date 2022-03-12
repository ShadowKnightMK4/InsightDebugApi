﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FileSandBoxSheath.NativeImports;
namespace FileSandBoxSheath.Wrappers
{
 
    /// <summary>
    /// Indented to Expose Detours stuff that the software can use.
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
