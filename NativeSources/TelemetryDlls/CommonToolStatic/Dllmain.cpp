
#include <windows.h>
#include "DetouringRoutine.h"
#include "detours.h"
#include "TelemetrySupport.h"

/*
* You have these options in the build config settings to define to modify this template
*
* PIN_TELEMETRY_DLL     ->      DllMain() will now pin itself in the memory of the module it's loaded in to prevent premature loading.
*                       ->      Default is defined
*                       ->      Clearing this and not providing  a way to undetour any detoured routines will case exceptions when code attempts
*                               to call your detoured routine that has been unloaded prematurely.
*/



/* if you find you're getting garbage from the exception reading in the sheath, uncomment this and put a call info the DllMain() here.
    Run the Visual studio debugger and inspect the exception arguments .
*/
void test_exception()
{

    ULONG_PTR Argsp[EXCEPTION_MAXIMUM_PARAMETERS];
    OutputDebugString(L"Preparing Test Exception of Value 1.  ULONG_PTR size = (");
    //OutputDebugString(sizeof(ULONG_PTR));
    OutputDebugString(L") Value of array { ");
    


    for (int step = 0; step < EXCEPTION_MAXIMUM_PARAMETERS; step++)
    {
        Argsp[step] = step;
        OutputDebugString( (LPCWSTR) Argsp[step]);
        OutputDebugString(L", ");

    }
    OutputDebugString(L"} \r\n");
    




    __try
    {
        RaiseException(1, 0, 15, Argsp);
    }
    __except (GetExceptionCode() == 1)
    {
        ;
    }
}
#ifdef PIN_TELEMETRY_DLL
static HMODULE Self = 0;
#endif
int WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID Reserved)
{

    if (DetourIsHelperProcess())
    {
        OutputDebugStringW(L"Loaded as Helper Process\r\n");
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
#ifdef PIN_TELEMETRY_DLL
            if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCWSTR)hInstDll, &Self))
            {
                return FALSE;
            }
#endif
#ifdef _DEBUG
            //test_exception();
#endif // _DEBUG

            return DetourTargetRoutines();
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

