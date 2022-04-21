using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Structs
{
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct SourceFile
    {
        /// <summary>
        /// Base address of the module this source file is to be used for
        /// </summary>
        public ulong ModBase;
        /// <summary>
        /// location to read the source file from.
        /// </summary>
        [MarshalAs(UnmanagedType.LPWStr)]
        public string FileName;
    }
}
