#pragma once


#include "FilesandboxApi_DllStuff.h"
#include "PS_ProcessInformation.h"

#include <Windows.h>
#include "Client.h"






/// <summary>
/// Spawn a process constructed from the FS_ProcessHandle routines.
/// </summary>
/// <param name="that">is a block of memory allocated by FS_CreateProcessHandle</param>
/// <returns>index into the API to reference this process.  NOT A HANDLE Returns (DWORD)-1 on failure to launch</returns>
DWORD WINAPI FS_ProcessHandle_SpawnEx(PS_ProcessInformation* that);



/// <summary>
/// Shorthand for calling ResumeThread() Win32 Api on the MainThread.  The routine will drop calls if that = null or if the MainThreadHandle equals INVALID_HANDLE_VALUE
/// </summary>
/// <param name="that"></param>
VOID WINAPI FS_ProcessHandle_ResumeMainThread(PS_ProcessInformation* that);


/// <summary>
/// Spawn a Process and register it with the passed client ID
/// </summary>
/// <param name="client">ID of the client</param>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="CreationFlags"> CreateProcess Flags</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>
DWORD WINAPI  FS_SpawnProcess(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD CreationFlags, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summary>
/// Spawn a Process and register it with the passed client ID. Tells Windows that you wish to debug only this process and no processes it spawns
/// </summary>
/// <param name="client">ID of the client</param>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>
DWORD WINAPI FS_SpawnProcessDebug(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summary>
/// Spawn a Process and register it with the passed client ID. Tells Windows to wait until you resume the main thread and spawns the process ready to debug
/// </summary>
/// <param name="client">ID of the client</param>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>
DWORD  WINAPI FS_SpawnProcessDebugSuspended(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summary>
/// Spawn a Process and register it with the passed client ID. Tells Windows you wish to debug this spawned process and any child processes it spawns.
/// </summary>
/// <param name="client">ID of the client</param>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>
DWORD WINAPI FS_SpawnProcessDebugChild(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summary>
/// Spawn a Process and register it with the passed client ID. Tells Windows to wait unti until you resume the main thread and also you wish to debug it plus any child processes spawned
/// </summary>
/// <param name="client">ID of the client</param>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>

DWORD WINAPI FS_SpawnProcessDebugChildSuspended(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summery>
/// Spawn a process without the need of resgisteding it with the passed client.
/// </summery>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="CreationFlags"> CreateProcess Flags</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>>
DWORD WINAPI FS_SpawnProcessFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD CreationFlags, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summery>
/// Spawn a process without the need of resgisteding it with the passed client. Tells windows you wish to debug only this process and to let execution begin asap.
/// </summery>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>>
DWORD WINAPI FS_SpawnProcessDebugFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summery>
/// Spawn a process without the need of resgisteding it with the passed client. Tells Windows you want to let the main thread start suspsended until you resume it.
/// </summery>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>>
DWORD  WINAPI FS_SpawnProcessDebugSuspendedFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);

/// <summery>
/// Spawn a process without the need of resgisteding it with the passed client. Tells Windows you want to debug this process and any child processces also
/// </summery>
/// <param name="client">ID of the client</param>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>>
DWORD WINAPI FS_SpawnProcessDebugChildFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);


/// <summery>
/// Spawn a process without the need of resgisteding it with the passed client. Tells Windows you want to debug this process + children as well as have the main thread start suspended.
/// </summery>
/// <param name="client">ID of the client</param>
/// <param name="ProcessLocation">Target Process to launch</param>
/// <param name="ProcessArguments">Process Arguments (can be 0 or NULL)</param>
/// <param name="DetourDllCount">number of Dlls in the DetourDllList Array</param>
/// <param name="Env">Enviroment BLock</param>
/// <param name="StartingDirectory">Starting Directory</param>
/// <param name="DetourDllList">Array of ANSI char* specifying DLLs to force it to load</param>
/// <returns>Process ID of launched process (or 0 on error)</returns>>
DWORD WINAPI FS_SpawnProcessDebugChildSuspendedFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList);
