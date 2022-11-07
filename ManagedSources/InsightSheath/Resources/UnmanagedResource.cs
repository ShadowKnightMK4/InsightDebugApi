using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Resource
{
    /// <summary>
    /// Primarily intended to be what <see cref="ResourceWalker"/> gives to let the user load a specific resource into NativeLand.
    /// </summary>
    internal class ResourceWalker
    {

        [DllImport("kernel32.dll", SetLastError = true, EntryPoint = "FindResourceExW", CharSet = CharSet.Unicode)]
        internal static extern IntPtr FindResourceExW(IntPtr hModule, IntPtr lpType, IntPtr lpName, int wLangauge);
        [DllImport("kernel32.dll", SetLastError = true, EntryPoint = "LoadResource", CharSet = CharSet.Unicode)]
        internal static extern IntPtr LoadResource(IntPtr hModule, IntPtr hResInfo);

        [DllImport("kernel32.dll", SetLastError = true, EntryPoint = "SizeofResource", CharSet = CharSet.Unicode)]
        internal static extern uint SizeofResource(IntPtr hModule, IntPtr hResInfo);

    }
}
