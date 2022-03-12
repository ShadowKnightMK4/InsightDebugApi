#include <Windows.h>
#include "..\..\Detours-master\src\detours.h"

/*
* MemmoryTackingTelemetryDll aka
* 
* Telemem32.dll and Telemem64.dll  build from this project.
* 
* 
* 
* They detour The Heap and Global Alloc Memory Routines and forward them a debugger for information purposes
* 
* They work by thowing an Exception with RaiseExcpetion()
* This also is an exception of taking advandtange of DEbugging pluse injecting a dll into a newply spawned process
* VALUE = TELEMETRY_CONST
* Args[0] = Notificaiton Type
*		File
*		Heap
*			
*		Virtual
*		Process
*		Registry
* 
*/


#include <windows.h>

/*
* You have these options in the build config settings to define to modify this template
*
* PIN_TELEMETRY_DLL     ->      DllMain() will now pin itself in the memory of the module it's loaded in to prevent premature loading.
*                       ->      Default is defined
*                       ->      Clearing this and not providing  a way to undetour any detoured routines will cause exceptions when the target process tries calling one of the routines you've detoured that no longer exist
*/

#ifdef PIN_TELEMETRY_DLL
// Holds a handle to self after pinning the module to prevent premature loading.
static HMODULE Self = 0;
#endif


/// <summary>
/// EntryPoint for the DLL.   Also exported because a Detour Dll must export at least 1 think and this is as good as any.
/// </summary>
/// <param name="hInstDll"></param>
/// <param name="fdwReason"></param>
/// <param name="Reserved"></param>
/// <returns></returns>
int __declspec(dllexport) WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID Reserved)
{
    /// <summary>
    /// Helper process documentation is says to just return TRUE. 
    /// </summary>
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }

    {
        // Perform actions based on the reason for calling.
        switch (fdwReason)
        {
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
#ifdef PIN_TELEMETRY_DLL
            // PINNING the DLL will prevent premature unloaded so that your detoured routines are restored to normal
            if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCWSTR)hInstDll, &Self))
            {
                return FALSE;
            }
#endif
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

