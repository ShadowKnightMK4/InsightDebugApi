using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace FileSandBoxSheath.NativeImports
{
    internal static partial class NativeMethods
    {
        [DllImport("FileSandBoxApi.dll",CallingConvention= CallingConvention.Winapi, SetLastError =true,EntryPoint ="DetourEnumerateModulesEx")]
        public static extern IntPtr DetourEnumerateModulesEx(IntPtr hProcess, IntPtr hModule);
    }
}
