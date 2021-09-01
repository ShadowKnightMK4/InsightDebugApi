#include "PS_ProcessInformation.h"
#include "Utility.h"
#include <process.h>
#include "ProcessHandling.h"
#include "DebugEventInternal.h"

/*
* This source file contains the publicly exported rotines to deal with the class
*/

PS_ProcessInformation* WINAPI FS_CreateProcessHandle()
{
	{
		PS_ProcessInformation* ret = new PS_ProcessInformation();
		if (ret != nullptr)
		{
			return ret;
		}

	}
}


LPSTARTUPINFOW WINAPI FS_ProcessHandle_GetStartupInfo(PS_ProcessInformation* that)
{
	if (that != nullptr)
	{
		return that->GetStartupInfoPtr();
	}
	return nullptr;
}



BOOL WINAPI SetCommandment_NoReads(PS_ProcessInformation* that, BOOL NewVal)
{
	
	if (that != nullptr)
	{
		BOOL Old = that->GetCommandmentNoRead();
		that->SetCommentmentNoRead(NewVal);
		return Old;
	}
	return FALSE;
}


/// <returns>returns old state if OK or FALSE on error</returns>
BOOL WINAPI SetCommandment_NoWrites(PS_ProcessInformation* that, BOOL NewVal)
{
	if (that != nullptr)
	{
		BOOL Old = that->GetCommandmentNoWrite();
		that->SetCommandmentNoWrite(NewVal);
		return Old;
	}
	return FALSE;
}


BOOL WINAPI SetCommandment_NoProcessSpawning(PS_ProcessInformation* that, BOOL NewVal)
{
	if (that != nullptr)
	{
		BOOL Old = that->GetCommandmentNoSpawnProcess();
		that->SetCommentmentNoSpawnProcess(NewVal);
		return Old;
	}
	return FALSE;
}


BOOL WINAPI FS_ProcessHandle_SetProcessNameW(PS_ProcessInformation* that, wchar_t* NewProcess)
{
	if (that != nullptr)
	{
		if (NewProcess != nullptr)
		{
			that->SetProcessName(NewProcess);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WINAPI FS_ProcessHandle_SetProcessNameA(PS_ProcessInformation* that, char* NewProcess)
{
	if (that != nullptr)
	{
		if (NewProcess != nullptr)
		{
			that->SetProcessName(NewProcess);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WINAPI FS_ProcessHanndle_SetProcessArgumentsW(PS_ProcessInformation* that, wchar_t* newargs)
{
	if (that != nullptr)
	{
		if (newargs != nullptr)
		{
			that->SetProcessArguments(newargs);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WINAPI FS_ProcessHanndle_SetProcessArgumentsA(PS_ProcessInformation* that, char* newargs)
{
	if (that != nullptr)
	{
		if (newargs != nullptr)
		{
			that->SetProcessArguments(newargs);
			return TRUE;
		}
	}
	return FALSE;
}



const wchar_t* PS_ProcessInformation::GetProcessName()
{
	return ProcessName.c_str();
}

void PS_ProcessInformation::SetProcessName(std::wstring Name)
{
	ProcessName = Name;
}

void PS_ProcessInformation::SetProcessName(wchar_t* Name)
{
	if (Name == nullptr)
	{
		ProcessName.clear();
	}
	else
	{
		ProcessName = Name;
	}
}

void PS_ProcessInformation::SetProcessName(char* Name)
{
	ProcessName.clear();
	if (Name != nullptr)
	{
		wchar_t* Dup = ConvertANSIString(Name);
		if (Dup != nullptr)
		{
			ProcessName.assign(Dup);
			free(Dup);
		}
	}
}

const wchar_t* PS_ProcessInformation::GetProcessArguments()
{
	return ProcessArguments.c_str();
}

void PS_ProcessInformation::SetProcessArguments(std::wstring Args)
{
	this->ProcessArguments = Args;
}

void PS_ProcessInformation::SetProcessArguments(wchar_t* Args)
{
	if (Args != nullptr)
	{
		this->ProcessArguments.assign(Args);
		return;
	}
	this->ProcessArguments.erase();

}

void PS_ProcessInformation::SetProcessArguments(char* Args)
{
	if (Args != nullptr)
	{
		wchar_t* dup = ConvertANSIString(Args);
		if (dup != nullptr)
		{
			this->ProcessArguments.assign(dup);
			free(dup);
		}
	}
}

const wchar_t* PS_ProcessInformation::GetStartingDirectory()
{
	return StartingDirectory.c_str();
}

void PS_ProcessInformation::SetStartingDirectory(std::wstring StartingDirectory)
{
	this->StartingDirectory = StartingDirectory;
}

void PS_ProcessInformation::SetStartingDirectory(wchar_t* StartingDirectory)
{
	if (StartingDirectory == nullptr)
	{
		this->StartingDirectory.clear();
	}
	else
	{
		this->StartingDirectory = StartingDirectory;
	}
}

void PS_ProcessInformation::SetStartingDirectory(char* StartingDirectory)
{
	
	if (StartingDirectory != nullptr)
	{
		wchar_t* dup = ConvertANSIString(StartingDirectory);
		if (dup != nullptr)
		{
			this->StartingDirectory = dup;
			free(dup);
		}
	}
	else
	{
		this->StartingDirectory.clear();
	}
}

const char** PS_ProcessInformation::GetDetourListContainer()
{
	if (this->DetourDllList.size() != 0)
	{
		return (const char**)&this->DetourDllList[0];
	}
	return nullptr;
	
}

const char* PS_ProcessInformation::GetDetourListIndex(int index)
{
	if ((index < 0) || (index >= this->DetourDllList.size()))
	{
		return nullptr;
	}
	else
	{
		return this->DetourDllList[index].c_str();
	}
}



void PS_ProcessInformation::ClearDetourDllList()
{
	this->DetourDllList.clear();
}

void PS_ProcessInformation::AddNewDetourDll(char* dll)
{
	if (dll != nullptr)
	{
		DetourDllList.push_back(dll);
	}
}

void PS_ProcessInformation::AddNewDetourDll(wchar_t* Dll)
{
	if (Dll != nullptr)
	{
		char* Dup = ConvertUnicodeString(const_cast<wchar_t*>(Dll));
		if (Dup != nullptr)
		{
			DetourDllList.push_back(Dup);
			free(Dup);
		}
	}
}

void PS_ProcessInformation::AddNewDetourDll(std::wstring Dll)
{
	char* Dup = ConvertUnicodeString(const_cast<wchar_t*>(Dll.c_str()));
	if (Dup != nullptr)
	{
		DetourDllList.push_back(Dup);
		free(Dup);
	}
}

void PS_ProcessInformation::AddNewDetourDll(std::string Dll)
{
	DetourDllList.push_back(Dll);
}

size_t PS_ProcessInformation::GetDetourDllListSize()
{
	return this->DetourDllList.size();
}

LPSTARTUPINFOW PS_ProcessInformation::GetStartupInfoPtr()
{
	return &this->StatupInfo;
}

void PS_ProcessInformation::SetCommentmentNoRead(BOOL NewVal)
{
	this->Command_NoReads = NewVal;
}

BOOL PS_ProcessInformation::GetCommandmentNoRead()
{
	return this->Command_NoReads;
}

void PS_ProcessInformation::SetCommandmentNoWrite(BOOL NewVal)
{
	this->Command_NoWrites = NewVal;
}

BOOL PS_ProcessInformation::GetCommandmentNoWrite()
{
	return this->Command_NoWrites;
}

void PS_ProcessInformation::SetCommentmentNoSpawnProcess(BOOL NewValue)
{
	this->Command_NoSpawnProcess = NewValue;
}

BOOL PS_ProcessInformation::GetCommandmentNoSpawnProcess()
{
	return this->Command_NoSpawnProcess;
}

DWORD PS_ProcessInformation::GetCreationFlags()
{
	return this->CreationFlags;
}

void PS_ProcessInformation::SetCreationFlags(DWORD Flags)
{
	this->CreationFlags = Flags;
}

PROCESS_INFORMATION* PS_ProcessInformation::GetProcessInformation()
{
	return &this->ProcessInformation;
}



/// <summary>
/// Worker thread code used by SpawnProcess().  SpawnProcess() creates this susspended, creates the event and then Resumes the thread.
/// </summary>
/// <param name="Arg"></param>
/// <returns></returns>
unsigned int _stdcall WorkerThread(void* Arg)
{
	PS_ProcessInformation* that = reinterpret_cast<PS_ProcessInformation*>(Arg);
	if (Arg == nullptr)
	{
		return -1;
	}
	else
	{
		DWORD dwID = FS_ProcessHandle_SpawnEx(that);
		if (dwID == 0)
		{
			// FS_ProcessHandle_SpawnEx() returns 0 if it could not spawn the process.
			CloseHandle(that->EventSync);
			return GetLastError();
		}
		else
		{
			// If we're debugging, we wait 
			if (((that->CreationFlags && DEBUG_PROCESS) == (DEBUG_PROCESS)) || (that->CreationFlags && DEBUG_ONLY_THIS_PROCESS) == DEBUG_ONLY_THIS_PROCESS)
			{
				DEBUG_EVENT CurEvent;
				while (true)
				{
					if (WaitForDebugEventEx(&CurEvent, INFINITE) == TRUE)
					{
						
						WaitForSingleObject(that->EventSync, INFINITE);
						ResetEvent(that->EventSync);
					}
				}
			}
			else
			{
				WaitForSingleObject(that->ProcessInformation.hProcess, INFINITE);
				DWORD dwErr = 0;
				if (GetExitCodeProcess(that->ProcessInformation.hProcess, &dwErr))
				{
					CloseHandle(that->ProcessInformation.hProcess);
					CloseHandle(that->ProcessInformation.hThread);
					return dwErr;
				}
				else
				{
					return -1;
				}
			}
		}
	}
}

DWORD PS_ProcessInformation::SpawnProcess()
{
	HANDLE Han = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, WorkerThread, this, CREATE_SUSPENDED, nullptr));

	if (Han != INVALID_HANDLE_VALUE)
	{
		this->EventSync = CreateEvent(nullptr, TRUE, FALSE, NULL);
		ResumeThread(Han);
		return 1;
	}
	return 0;



}

