using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// HelperDll namespace contains classes to deal with telling the HelperDll how to work in the target debugged process
/// </summary>
namespace InsightSheath.HelperDll
{
    /// <summary>
    /// this class controls how to modify LoadLibraryXXX() folders and files. Requires Helper Dll to be loaded
    /// </summary>
    public static class LoadLibraryRedirects
    {
        struct HardwareEntry
        {
            string MatchThis;
            string ReplaceWithThis;
        }
    }
}
