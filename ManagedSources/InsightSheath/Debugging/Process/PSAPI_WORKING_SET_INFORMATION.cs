using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Composition;

namespace InsightSheath.Debugging.Process
{
    // https://docs.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-psapi_working_set_ex_information
    public struct PSAPI_WORKING_SET_EX_INFORMATION
    {
        public nint Flags;
    }

    /// <summary>
    /// https://docs.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-psapi_working_set_ex_block
    /// </summary>
    struct PSAPI_WORKING_SET_EX_BLOCK
    {
    }

}
