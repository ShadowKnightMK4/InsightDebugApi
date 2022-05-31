
#include <windows.h>

#include "detours.h"

/*
* You have these options in the build config settings to define to modify this template
*
* PIN_TELEMETRY_DLL     ->      DllMain() will now pin itself in the memory of the module it's loaded in to prevent premature loading.
*                       ->      Default is defined
*                       ->      Clearing this and not providing  a way to undetour any detoured routines will case exceptions when code attempts
*                               to call your detoured routine that has been unloaded prematurely.
*/


typedef int (WINAPI* MessageBoxWPtr)(
    HWND    hWnd,
    LPCTSTR lpText,
    PCTSTR lpCaption,
    UINT    uType
    );


/// <summary>
/// Will hold user32's MessageBoxW
/// </summary>
MessageBoxWPtr Original = 0;

/// <summary>
/// The capture is replaced with a message. That's all.
/// </summary>
/// <param name="hWnd"></param>
/// <param name="lpText"></param>
/// <param name="lpCaption"></param>
/// <param name="uType"></param>
/// <returns></returns>
int Replacement(
                   HWND    hWnd,
                   LPCWSTR lpText,
                   LPCWSTR lpCaption,
                   UINT    uType
)
{
    return Original(hWnd, lpText, L"DETOUR MESSAGE REPLACEMENT TEXT", uType);
}

void DetourTarget()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((PVOID*)&Original, Replacement);
    DetourTransactionCommit();

}


#ifdef PIN_TELEMETRY_DLL
static HMODULE Self = 0;
#endif
int WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID Reserved)
{
    Original = MessageBoxW;
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }
    else
    {
        DetourRestoreAfterWith();
    }

    {
        // Perform actions based on the reason for calling.
        switch (fdwReason)
        {
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.

             DetourTarget();
             return TRUE;
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
}

