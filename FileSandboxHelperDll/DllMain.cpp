#include <StaticIncludes.h>
#include "DetourFunctionStuff.h"

static HMODULE Self = nullptr;
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }
    else
    {
        DetourRestoreAfterWith();
    }
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        if (DetourNtFileRoutines() == false)
        {
            return false;
        }
        // Force the Dll to Stay in memory regardless;
        if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)hinstDLL, &Self))
        {
            return false;
        }
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}