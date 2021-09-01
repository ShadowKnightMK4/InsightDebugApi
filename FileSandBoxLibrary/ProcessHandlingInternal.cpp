#include <windows.h>

#include "ProcessHandling.h"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <detours.h>

// don't forget the header that contains the class def for each entry we will be spawning/tracking.
#include "PS_ProcessInformation.h"
#include "Utility.h"
#include "ClientHandling.h"


/*
*	Contains the routines that deal with process creation and handling
*/





/// <summary>
/// Internally, we track processes we spawn with a class of this type.
/// </summary>
class ProcessCollection
{
public:
	std::map<DWORD, PS_ProcessInformation> Collection;
	ProcessCollection()
	{
		InitializeConditionVariable(&SyncAccess);
	}
	~ProcessCollection()
	{

	}
private:
	CONDITION_VARIABLE SyncAccess;
};

ProcessCollection TrackedProcesses;




PS_ProcessInformation* WINAPI FS_CreateProcessHandleClient(Client* Client)
{
	if (Client != nullptr)
	{
		PS_ProcessInformation* ret = new PS_ProcessInformation();
		if (ret != nullptr)
		{
			Client->RegisterNewTrackedProcess(ret);
			return ret;
		}

	}
	return nullptr;
}


/// <summary>
/// Common internal routien for the spawn process routine collection
/// </summary>
/// <param name="client">return value returned via FSStartup().  Use 0 to not do any client handling after spawning the process
/// <param name="ProcessLocation">target to spawn.</param>
/// <param name="ProcessArguments">arguments. May be null</param>
/// <param name="CreationFlags">CreateProcess CreationFlags</param>
/// <param name="DetourDllCount">number of entries in the list</param>
/// <param name="Env">Custom Enviroment Block</param>
/// <param name="StartingDirectory">the process begins with this directory.</param>
/// <param name="DetourDllList"></param>
/// <returns>HANDLE for use in the DLL.   This is not a true handle to the process itself.  Use FS_GetProcessHandle() to do that</returns>

DWORD WINAPI FS_SpawnProcessExInternal(Client* client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD CreationFlags, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	PS_ProcessInformation* that;
	// first very args arn't 0.
	if (ProcessLocation == nullptr)
	{
		return (DWORD)-1;
	}

	if ((DetourDllCount == 0) && (DetourDllList != nullptr))
	{
		return (DWORD)-1;
	}
	if ((DetourDllCount != 0) && (DetourDllList == nullptr))
	{
		return (DWORD)-1;
	}

	// both Env and StartinngDirectory may be null.


	that = new PS_ProcessInformation();
	that->SetProcessName(ProcessLocation);
	that->SetProcessArguments(ProcessArguments);


	if (Env != nullptr)
	{
		// TODO: Enumerate the envirment block and add to the list.
		OutputDebugString(L"FS_SpawnProcessEX does not currently implment the Env argument. It's ignored.");
	}
	else
	{
		//	that->Enviroment.clear();
	}
	that->SetCreationFlags(CreationFlags);
	that->GetStartupInfoPtr()->cb = sizeof(STARTUPINFOW);


	that->SetStartingDirectory(StartingDirectory);


	for (DWORD step = 0; step < DetourDllCount; step++)
	{
		that->AddNewDetourDll(DetourDllList[step]);
	}

	if (client == 0)
	{
		DWORD result = FS_ProcessHandle_SpawnEx(that);
		if (result != (DWORD)-1)
		{
			return TrackedProcesses.Collection[result].GetProcessInformation()->dwProcessId;
		}
		else
		{
			return (DWORD)-1;
		}
	}
	else
	{
		DWORD result = FS_ProcessHandle_SpawnEx(that);
		if (result != (DWORD)-1)
		{
			client->RegisterNewTrackedProcess(that);
			return result;
		}
		return 0;
	}
}



DWORD WINAPI FS_ProcessHandle_SpawnEx(PS_ProcessInformation* that)
{
	if (that != nullptr)
	{
		PROCESS_INFORMATION* Info;
		const char** DetourList;;
		wchar_t* Arguments;           //   <-------  MSDN says an exception may occure using CreateProcessW on const memory.  We copy the memory
		const wchar_t* StartingDirectory; //  <----- So CreateProcessW() bonks if StartingDirectory is an empty string.  We check if the class's var's length is 0 or nullptr and set this to null on that case
		DWORD DetourListSize = 0;


		Info = that->GetProcessInformation();


		Arguments = const_cast<wchar_t*>(that->GetProcessArguments());
		if ((Arguments != nullptr) && (Arguments[0] == 0))
		{
			Arguments = nullptr;
		}
		else
		{
			if (Arguments != 0)
			{
				Arguments = _wcsdup(Arguments);
			}
			// else ARgument=0
		}


		StartingDirectory = that->GetStartingDirectory();
		if ((StartingDirectory != nullptr) && (StartingDirectory[0] == 0))
		{
			StartingDirectory = nullptr;
		}
		//if (!that->Enviroment.empty())
		{
			OutputDebugString(L"Enviroment block not supported yet with FS_ProcessHandle_SpawnEx()");
		}

		DetourListSize = that->GetDetourDllListSize();
		if (DetourListSize != 0)
		{
			DetourList = that->GetDetourListContainer();
		}
		else
		{
			DetourList = nullptr;
		}

		LPSTARTUPINFOW StartInfo = that->GetStartupInfoPtr();
		if (StartInfo->wShowWindow == 0)
		{
			StartInfo->wShowWindow = SW_NORMAL;
		}
		BOOL Result = DetourCreateProcessWithDllsW( that->GetProcessName(),
												    Arguments, 
												    nullptr,
												    nullptr,
												    FALSE,
												    that->GetCreationFlags(),
													nullptr,
													StartingDirectory,
													that->GetStartupInfoPtr(), 
													Info,
													DetourListSize,
													DetourList,
													nullptr);
		
		if (Arguments != nullptr)
		{
			free(Arguments);
		}
		if (Result == TRUE)
		{
		//	TrackedProcesses.Collection[Info->dwProcessId] = *that;
			return Info->dwProcessId;
		}
		else
		{
			DWORD LastError = GetLastError();
			Info->hProcess = Info->hThread = INVALID_HANDLE_VALUE;
			Info->dwProcessId = Info->dwThreadId = 0;
			return 0;
		}
	}
	return 0;
}


VOID WINAPI FS_ProcessHandle_ResumeMainThread(PS_ProcessInformation* that)
{
	if (that != nullptr)
	{

		if (that->GetProcessInformation()->hThread != INVALID_HANDLE_VALUE)
		{
			ResumeThread(that->GetProcessInformation()->hThread);
		}

	}
}

DWORD __stdcall FS_SpawnProcess(DWORD ClientID, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD CreationFlags, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	Client* handle;
	if (ClientID == 0)
	{
		return 0;
	}
	else
	{
		handle = FSClientIdToPoint(ClientID);
		if (handle == 0)
		{
			return 0;
		}
	}
	return FS_SpawnProcessExInternal(handle, ProcessLocation, ProcessArguments, CreationFlags, DetourDllCount, Env, StartingDirectory, DetourDllList);
}




DWORD WINAPI FS_SpawnProcessDebug(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcess(client, ProcessLocation, ProcessArguments, DEBUG_ONLY_THIS_PROCESS, DetourDllCount, Env, StartingDirectory, DetourDllList);
}
DWORD  WINAPI FS_SpawnProcessDebugSuspended(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcess(client, ProcessLocation, ProcessArguments, DEBUG_ONLY_THIS_PROCESS | CREATE_SUSPENDED, DetourDllCount, Env, StartingDirectory, DetourDllList);
}
DWORD WINAPI FS_SpawnProcessDebugChild(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcess(client, ProcessLocation, ProcessArguments, DEBUG_PROCESS, DetourDllCount, Env, StartingDirectory, DetourDllList);
}
DWORD WINAPI FS_SpawnProcessDebugChildSuspended(DWORD client, wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcess(client, ProcessLocation, ProcessArguments, DEBUG_PROCESS | CREATE_SUSPENDED, DetourDllCount, Env, StartingDirectory, DetourDllList);
}




DWORD WINAPI FS_SpawnProcessFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD CreationFlags, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcessExInternal(NULL, ProcessLocation, ProcessArguments, CreationFlags, DetourDllCount, Env, StartingDirectory, DetourDllList);
}

DWORD WINAPI FS_SpawnProcessDebugFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcessExInternal(NULL, ProcessLocation, ProcessArguments, DEBUG_ONLY_THIS_PROCESS, DetourDllCount, Env, StartingDirectory, DetourDllList);
}
DWORD  WINAPI FS_SpawnProcessDebugSuspendedFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcessExInternal(NULL, ProcessLocation, ProcessArguments, DEBUG_ONLY_THIS_PROCESS | CREATE_SUSPENDED, DetourDllCount, Env, StartingDirectory, DetourDllList);
}
DWORD WINAPI FS_SpawnProcessDebugChildFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcessExInternal(NULL, ProcessLocation, ProcessArguments, DEBUG_PROCESS , DetourDllCount, Env, StartingDirectory, DetourDllList);
}
DWORD WINAPI FS_SpawnProcessDebugChildSuspendedFreelance(wchar_t* ProcessLocation, wchar_t* ProcessArguments, DWORD DetourDllCount, LPVOID Env, wchar_t* StartingDirectory, char** DetourDllList)
{
	return FS_SpawnProcessExInternal(NULL, ProcessLocation, ProcessArguments, DEBUG_PROCESS | CREATE_SUSPENDED, DetourDllCount, Env, StartingDirectory, DetourDllList);
}
