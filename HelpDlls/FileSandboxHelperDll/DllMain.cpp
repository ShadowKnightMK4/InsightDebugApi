#include <Windows.h>
#include <detours.h>
#include <winternl.h>

#include "DetourFunctionStuff.h"

static HMODULE Self = nullptr;
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    
// A DLL loaded by a Helper process does NOT need to do anything other than return true.
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }
    else
    {
	// this tells Detours to Fix PE table it messed with when forcing
	// your target program to load this Dll.
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
        // Force the Dll to Stay in memory regardless
	// Not strictly required but you'll get some exceptions if 
	// your Dll is unloaded before undoing your Detoured Routiens.


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