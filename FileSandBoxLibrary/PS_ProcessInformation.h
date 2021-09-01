#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>

#include "Client.h"
class Client;

/// <summary>
/// Each process spawned or tracked by the library uses this class.
/// </summary>
class PS_ProcessInformation
{
public:



	PS_ProcessInformation()
	{
		ProcessName = L"";
		ProcessArguments = L"";
		ProcessHandle = MainThreadHandle = INVALID_HANDLE_VALUE;
		ProcessID = MainThreadID = 0;
	}

	PS_ProcessInformation(PROCESS_INFORMATION Ps)
	{
		this->MainThreadHandle = Ps.hThread;
		this->MainThreadID = Ps.dwThreadId;
		this->ProcessHandle = Ps.hProcess;
		this->ProcessID = Ps.dwProcessId;
		ProcessName = ProcessArguments = L"";
	}

	/// <summary>
	/// Get name of hte process we will spawn.
	/// </summary>
	/// <returns></returns>
	const wchar_t* GetProcessName();
	/// <summary>
	/// Set the process that we will spawn. 
	/// </summary>
	/// <param name="Name"></param>
	void SetProcessName(std::wstring Name);
	/// <summary>
	/// Set the process we will spawn
	/// </summary>
	/// <param name="Name"></param>
	void SetProcessName(wchar_t* Name);
	/// <summary>
	/// Converts to unicode and sets the result to the process we will spawn
	/// </summary>
	/// <param name="Name"></param>
	void SetProcessName(char* Name);

	const wchar_t* GetProcessArguments();
	void SetProcessArguments(std::wstring Args);
	void SetProcessArguments(wchar_t* Args);
	void SetProcessArguments(char* Args);

	const wchar_t* GetStartingDirectory();
	void SetStartingDirectory(std::wstring StartingDirectory);
	void SetStartingDirectory(wchar_t* StartingDirectory);
	void SetStartingDirectory(char* StartingDirectory);

	const char** GetDetourListContainer();
	const char* GetDetourListIndex(int index);
	void ClearDetourDllList();


	void AddNewDetourDll(char* dll);
	void AddNewDetourDll(wchar_t* Dll);
	void AddNewDetourDll(std::wstring Dll);
	void AddNewDetourDll(std::string Dll);

	size_t GetDetourDllListSize();


	LPSTARTUPINFOW GetStartupInfoPtr();

	void SetCommentmentNoRead(BOOL NewVal);
	BOOL GetCommandmentNoRead();

	void SetCommandmentNoWrite(BOOL NewVal);
	BOOL GetCommandmentNoWrite();

	void SetCommentmentNoSpawnProcess(BOOL NewValue);
	BOOL GetCommandmentNoSpawnProcess();

	DWORD GetCreationFlags();
	void SetCreationFlags(DWORD Flags);

	PROCESS_INFORMATION* GetProcessInformation();



	DWORD SpawnProcess();
private:

	/// <summary>
	/// file that the process spawned from
	/// </summary>
	std::wstring ProcessName;
	/// <summary>
	/// Arguments used to spawn process
	/// </summary>
	std::wstring ProcessArguments;

	/// <summary>
	/// List of Dlls added using Detours
	/// </summary>
	std::vector<std::string> DetourDllList;


	std::wstring StartingDirectory;


	/// <summary>
	/// Startup info for the process
	/// </summary>
	STARTUPINFOW StatupInfo;

	std::map<std::wstring, std::wstring> Enviroment;


	PROCESS_INFORMATION ProcessInformation;
	HANDLE ProcessHandle;
	HANDLE MainThreadHandle;
	DWORD ProcessID;
	DWORD MainThreadID;

	/// <summary>
	/// See the 'commandment' blerb in readme.md for more info.   (Requires debugger+able to see SEH events)   This causes our custom debugger to tell the debugged process ACCESS_DENIED when opening a file to read
	/// </summary>
	BOOL Command_NoReads;
	/// <summary>
	/// See the 'commandment' blerb in readme.md for more info.   (Requires debugger+able to see SEH events)   This causes our custom debugger to tell the debugged process ACCESS_DENIED when opening a file to write
	/// </summary>
	BOOL Command_NoWrites;
	/// <summary>
	/// See the 'commandment' blerb in readme.md for more info.   (Requires debugger+able to see SEH events)   This causes our custom debugger to tell the debugged process ACCESS_DENIED when attempting to spawn a new process
	/// </summary>
	BOOL Command_NoSpawnProcess;

	DWORD CreationFlags;
	/// <summary>
	/// Thread that actually spawned the process if created in debug mode, otherwise INVALID_HANDLE_VALUE
	/// </summary>
	HANDLE DebuggerThreadHandle;
	/// <summary>
	/// When we resume debug events for the ThreadHandle, we pulse this event.
	/// </summary>
	HANDLE EventSync;

	friend unsigned int _stdcall WorkerThread(void* Arg);
};



/*
* The ProcessInformation routine are intended to give C compatabile access and extra control to users.
*/

/// <summary>
/// Allocates and initialized s blclk of memory for use with the other PS_ProcessInformation routines.
/// </summary>
/// <returns></returns>
PS_ProcessInformation* WINAPI FS_CreateProcessHandle();

/// <summary>
/// Allocates and initialized s blclk of memory for use with the other PS_ProcessInformation routines. This adds to to th client to track.
/// </summary>
/// <returns></returns>
PS_ProcessInformation* WINAPI FS_CreateProcessHandleClient(Client* Client);
/// <summary>
/// Return a pointer to the StartupInfo struct containing in a PS_ProcessInformation struct
/// </summary>
/// <param name="that">block of memory allocated by FS_CreateProcessHandle </param>
/// <returns>pointer to a StartupInfoW struct that's within the PS_ProcessInformation </returns>
LPSTARTUPINFOW WINAPI FS_ProcessHandle_GetStartupInfo(PS_ProcessInformation* that);


/// <summary>
/// set the commandnent of NoRead acess to the true false .
/// </summary>
/// <param name="that">>block of memory allocated by FS_CreateProcessHandle</param>
/// <param name="NewVal">New state</param>
/// <returns>returns old state if OK or FALSE on error</returns>
BOOL WINAPI SetCommandment_NoReads(PS_ProcessInformation* that, BOOL NewVal);



/// <summary>
/// set the commandnent of NoWrite acess to the true false .
/// </summary>
/// <param name="that">>block of memory allocated by FS_CreateProcessHandle</param>
/// <param name="NewVal">New state</param>
/// <returns>returns old state if OK or FALSE on error</returns>
BOOL WINAPI SetCommandment_NoWrites(PS_ProcessInformation* that, BOOL NewVal);


/// <summary>
/// set the commandnent of No Process Spawn acess to the true false .
/// </summary>
/// <param name="that">>block of memory allocated by FS_CreateProcessHandle</param>
/// <param name="NewVal">New state</param>
/// <returns>returns old state if OK or FALSE on error</returns>
BOOL WINAPI SetCommandment_NoProcessSpawning(PS_ProcessInformation* that, BOOL NewVal);

/// <summary>
/// Set the target to spawn if the process is not already spawned.  N/A if process is already spawned. 
/// </summary>
/// <param name="that">>block of memory allocated by FS_CreateProcessHandle</param>
/// <param name="NewProcess">unicode string or null - specifies new process</param>
/// <returns>true if value was set ok and faluse if value happened.</returns>
BOOL WINAPI FS_ProcessHandle_SetProcessNameW(PS_ProcessInformation* that, wchar_t* NewProcess);

/// <summary>
/// Set the target to spawn if the process is not already spawned. N/A is process is already spawned
/// </summary>
/// <param name="that">>block of memory allocated by FS_CreateProcessHandle</param>
/// <param name="NewProcess">ANSI string or null of new process to spawn</param>
/// <returns>TRUE on sucess and value on value</returns>
BOOL WINAPI FS_ProcessHandle_SetProcessNameA(PS_ProcessInformation* that, char* NewProcess);


/// <summary>
/// Set the arguments to the process
/// </summary>
/// <param name="that"> is block of memory allocated by FS_CreateProcessHandle</param>
/// <param name="newargs"> is a UNICODE string for the new process to spawn</param>
/// <returns>TRUE on sucess and value on value</returns>
BOOL WINAPI  FS_ProcessHanndle_SetProcessArgumentsW(PS_ProcessInformation* that, wchar_t* newargs);

/// <summary>
/// Set the arguments to the process
/// </summary>
/// <param name="that">is a block of memory allocated by FS_CreateProcessHandle</param>
/// <param name="newargs">is an ANSI string arguments for the new process</param>
/// <returns>TRUE on sucess and value on failure</returns>
BOOL WINAPI FS_ProcessHanndle_SetProcessArgumentsA(PS_ProcessInformation* that, char* newargs);

