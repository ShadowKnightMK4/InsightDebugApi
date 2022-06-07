using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace InsightSheath.NativeImports
{
    internal static partial class NativeMethods
    {
        [DllImport("InsightAPI.dll",CallingConvention= CallingConvention.Winapi, EntryPoint = "ImageHlp_GetBaseOfImage")]
        public static extern ulong ImageHlp64_GetBaseOfImage(IntPtr ImageHlpModule64);
    }
}
