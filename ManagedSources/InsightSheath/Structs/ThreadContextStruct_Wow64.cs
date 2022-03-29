using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
namespace InsightSheath.Structs
{
// WOW64_MAXIMUM_SUPPORTED_EXTENSION is defined as 512 when vewing in C/C++
    [StructLayout(LayoutKind.Sequential,Pack =4)]
    /// <summary>
    /// Structure for a Wow64 32 bit running app on a 64-bit windows.
    /// </summary>
    public struct ThreadContextStruct_Wow64
    {
        uint ContextFLags;
        uint Dr0;
        uint Dr1;
        uint Dr2;
        uint Dr3;
        uint Dr6;
        uint Dr7;
        //WOW64_FLOATING_SAVE_AREA FloatSave;

        uint SegGs;
        uint SegFs;
        uint SegEs;
        uint SegDs;
        uint Edi;
        uint Esi;
        uint Ebx;
        uint Edx;
        uint Ecx;
        uint Eax;
        uint Ebp;
        uint Eip;
        uint SegCs;
        uint EFlags;
        uint Esp;
        uint SegSs;
        [MarshalAs(UnmanagedType.LPArray, SizeConst = 512)]
        byte[] ExtendedRegisters;
    }
}
