using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileSandBoxSheath.HelperDll
{
    /// <summary>
    /// when the HelperDll DllMain() reasies its first exception, it passes a pointer to a astructure of this kind. Debugger fills it out and writes it back to the target app. 
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    class HelperDll_FirstExecutionStruct
    {
        /// <summary>
        /// passed the mailslot to the helperdll. The "\\.\mailsot\" is automaticcaled added
        /// The 
        /// </summary>
        char[] MailSlotName= new char[2048];
    };

    /// <summary>
    /// The helper Dll 
    /// </summary>
    class HelperDll_Communication
    {
        [DllImport("kernel32.dll",EntryPoint ="CreateMailSlotW")]
        private static extern IntPtr CreateMailSlotW(string lpName, uint MaxMessageSize, uint ReadTimout, IntPtr SecurityAttributes);
    }
}
