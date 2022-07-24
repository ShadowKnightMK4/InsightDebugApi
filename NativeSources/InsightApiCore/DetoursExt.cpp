
#include <windows.h>
#include "detours.h"
#include <Psapi.h>

#include <map>
#include <string>
#include <vector>
extern "C" {

    class DetourBinary_ExtractInputs_handler
    {
    public:
        std::map<std::string, std::vector<std::string>>  ImportList;
        bool Recursion;
    };


    ///
    ///  Make a list of imports
    ///  
    LPCWSTR WINAPI DetourBinary_ExtractInputs(LPVOID pContext,
        PF_DETOUR_BINARY_BYWAY_CALLBACK pfByWay,
        PF_DETOUR_BINARY_FILE_CALLBACK pfFile,
        PF_DETOUR_BINARY_SYMBOL_CALLBACK pfSymbol,
        PF_DETOUR_BINARY_COMMIT_CALLBACK pfFinal)
    {

    }


    BOOL DetourEnumerateImportsExRemote(
        _In_opt_ HMODULE hModule,
        _In_opt_ PVOID pContext,
        _In_opt_ PF_DETOUR_IMPORT_FILE_CALLBACK pfImportFile,
        _In_opt_ PF_DETOUR_IMPORT_FUNC_CALLBACK_EX pfImportFunc,
        _In_ DWORD dwProcessID)
    {
        return FALSE;
    }
}