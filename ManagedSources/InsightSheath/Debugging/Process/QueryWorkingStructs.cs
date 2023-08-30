using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Debugging.Process
{
    [StructLayout(LayoutKind.Explicit,Pack =4)]
    public struct PSAPI_WORKING_SET_BLOCK
    {
        
        [FieldOffset(0)]
        UInt32 Flags;
        [FieldOffset(4)]
        UInt32 No;
    }

    public struct PSAPI_WORKING_SET_INFORMATION
    { 

    }
}
