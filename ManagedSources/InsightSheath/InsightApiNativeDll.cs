using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath
{
    /// <summary>
    /// Set a search location to search for your Dll
    /// </summary>
    public static class InsightApiNativeDllSettings
    {
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        static extern bool SetDllDirectoryW(string Path);

        /// <summary>
        /// Imports https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setdlldirectoryw to let user specify where to search for Dlls
        /// </summary>
        /// <param name="Path"></param>
        /// <returns></returns>
        public static bool SetDllDirectory(string Path)
        {
            return SetDllDirectoryW(Path);
        }


        
    }
}
