#include <windows.h>

#include "ProcessHandling.h"
#include <vector>
#include <string>
/// <summary>
/// Describes a process we've spawned
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
	std::vector<std::wstring> DetourDllList;


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


};

/// <summary>
/// Internally, we track processes we spawn with a class of this type.
/// </summary>
class ProcessCollection
{
public:
	std::vector<PS_ProcessInformation> Collection;
private:
	CONDITION_VARIABLE SyncAccess;
};

ProcessCollection TrackedProcesses;


DWORD FS_SpawnProcess(wchar_t* Process, wchar_t* Name, DWORD CreateAttributes, DWORD DetourDllArrayCount, char** DetourDllArray)
{
	PS_ProcessInformation* Info = new PS_ProcessInformation();
	Info.
	SetLastError(0);
	// FIRST idio check the args
	if (Process == nullptr)
	{
		SetLastError(E_INVALIDARG);
		return 0;
	}
	if (Name == 0)
	{
		SetLastError(E_INVALIDARG);
		return 0;
	}

	if ((DetourDllArrayCount == 0) && (DetourDllArray != nullptr))
	{
		SetLastError(E_INVALIDARG);
	}
	else
	{
		for (DWORD step = 0; step < DetourDllArrayCount; step++)
		{

		}
	}
}


