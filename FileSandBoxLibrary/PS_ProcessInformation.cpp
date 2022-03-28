#include "PS_ProcessInformation.h"
#include "Utility.h"
#include <process.h>
#include "ProcessHandling.h"
#include "DebugEventInternal.h"
#include <StaticIncludes.h>
#include "InsightHunter.h"
#include "../FilesandboxClientDllProtocol/HelperPayloadGuids.h"
#include <sstream>
/*
* 09726c86073b4beeb1793f0793192459

f716644cad2e4afd9d5525d937df0c00
628dd6dd5aaf45909f849fabbf4d8baa
*/


/// <summary>
/// A default handler that does notheing except sign off on the event and continunes it. Used when the UserCallback is not set (ie null).
/// </summary>
/// <param name="lpCurEvent"></param>
/// <param name="ContinueStatus"></param>
/// <param name="WaitTimer"></param>
/// <param name="CustomArg"></param>
/// <returns></returns>
int WINAPI DefaultHandler(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg)
{
	if (lpCurEvent->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
	{
		*ContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
	}
	else
	{
		*ContinueStatus = DBG_CONTINUE;
	}
	switch (lpCurEvent->dwDebugEventCode)
	{
	case 3:
		if (lpCurEvent->u.CreateProcessInfo.hFile != 0)
		{
			CloseHandle(lpCurEvent->u.CreateProcessInfo.hFile);
		}
		break;
	case 2:
		break;
	case 1:
		break;
	case 5:
		break;
	case 4:
		break;
	case 6:
		break;
	case 8:
		break;
	case 9:
		break;
	case 7:
		break;
	default: 
		break;
	}
	*WaitTimer = INFINITE;
	if (lpCurEvent->dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
	{
		if (lpCurEvent->u.ExitProcess.dwExitCode != STILL_ACTIVE)
		{
			return lpCurEvent->u.ExitProcess.dwExitCode;
		}
		return 1;
	}
	return 0;
	
}
/// <summary>
/// Worker thread when user wants PSINFO_SPAWNMODE_WORKERTHREADED and is debugging a target. Needed for The symbol engine to function
/// </summary>
/// <param name="argument">WorkerThreadData *</param>
/// <returns></returns>
void _cdecl PsPocessInformation_DebugWorkerthread(void* argument)
{
	WorkerThreadData* Args = (WorkerThreadData*)argument;
	DEBUG_EVENT lpEvent;
	int Code = 0;
	// First we spawn the process.

	DWORD dwId = Args->that->SpawnProcessCommon(true);
	SetEvent(Args->EventHandle);
	if (dwId == 0)
	{
		_endthreadex(GetLastError());
	}

	if (Args->that->EnableSymbols == TRUE)
	{
		if (Args->that->Insight == nullptr)
		{
			Args->that->Insight = new InsightHunter(Args->that->GetMainProcessHandle());
		}
		else
		{
			Args->that->Insight->SetMainDebugTarget(Args->that->GetMainProcessHandle());
		}
	}
	else
	{
		
	}
	// Now process the events 
	while (true)
	{
		if (WaitForDebugEventEx(&lpEvent, Args->WaitTImer))
		{
			if (Args->that->EnableSymbols)
			{
				switch (lpEvent.dwDebugEventCode)
				{
				case EXIT_PROCESS_DEBUG_EVENT:
				{
					if (Args->that->Insight != nullptr)
					{
						Args->that->Insight->UnloadExeSymbolInfo(&lpEvent);
					}
				}
				case UNLOAD_DLL_DEBUG_EVENT:
					if (Args->that->Insight != nullptr)
					{
						Args->that->Insight->UnLoadDllSymbolInfo(&lpEvent);
					}
					break;
				case LOAD_DLL_DEBUG_EVENT:
					if (Args->that->Insight != nullptr)
					{
						Args->that->Insight->LoadDllSymbolInfo(&lpEvent);
					}
					break;
				case CREATE_PROCESS_DEBUG_EVENT:
					if (Args->that->Insight != nullptr)
					{
						Args->that->Insight->LoadExeSymbolInfo(&lpEvent);
					}
					break;
				}
			}

			/* Care about updating thread view here*/
			switch (lpEvent.dwDebugEventCode)
			{
				case CREATE_THREAD_DEBUG_EVENT:
				{
					Args->that->ProcessThreads.ProcessCreateThreadDebugEvent(&lpEvent);
					break;
				}
				case EXIT_PROCESS_DEBUG_EVENT:
				{
					Args->that->ProcessThreads.ProcessExitProcessDebugEvent(&lpEvent);
				}
				case EXIT_THREAD_DEBUG_EVENT:
				{
					Args->that->ProcessThreads.ProcessExitThreadDebugEvent(&lpEvent);
					break;
				}
			}
		}
		WaitForSingleObject(Args->EventHandle, INFINITE);
		ResetEvent(Args->EventHandle);
		if (Args->UserCallback == nullptr)
		{
			Code = DefaultHandler(&lpEvent, &Args->ContinueState, &Args->WaitTImer, 0);
		}
		else
		{
			Code = Args->UserCallback(&lpEvent, &Args->ContinueState, &Args->WaitTImer, 0);
		}
		if (Code != 0)
		{
			CloseHandle(Args->EventHandle);
			CloseHandle(Args->ThreadHandle);
			_endthreadex(Code);
		}
		ContinueDebugEvent(lpEvent.dwProcessId, lpEvent.dwThreadId, Args->ContinueState);


	}

	_endthreadex(0);
}



/// <summary>
/// convert the  map to a string suitable for an environment block, and add the inherited block. 
/// If the map defines something the inherited block has already defined, the defined one in map one is kept
/// </summary>
/// <param name="Overwritten">from private PS_ProcessInformation.Environment</param>
/// <param name="IncludeBase">Do you want the default eviroment  block included</param>
/// <param name="Output">Place the results in this location ready to send to CreateProcessW</param>
/// <remarks>I feel this can be glitchy.  I'm unsure of a better solution ATM. We try to read and skip junk data at the start.</remarks>
void BuildEnviromentBlock(std::map<std::wstring, std::wstring>& Overwritten, BOOL IncludeBase, std::wstring& Output)
{
	std::map<std::wstring, std::wstring> InternalMap;
	if (IncludeBase)
	{
		LPWCH Buff = GetEnvironmentStringsW();
		wchar_t Char;
		std::wstring Name;
		std::wstring Value;

		BOOL FillName = TRUE;
		BOOL LastCharWasNull = FALSE;
		for (size_t stepper=0; ; stepper++)
		{
			Char = Buff[stepper];

			if (Char == 0)
			{
				// This is used duing stepping thu viusal studio code to keep at what the string list is.
			//	Buff[stepper] = 'x';
				if (LastCharWasNull == TRUE)
				{
					break;
				}
			}

			if (FillName)
			{
				if (Char == '=')
				{
					FillName = FALSE;
					LastCharWasNull = FALSE;
					continue;
				}
				else
				{
					Name += Char;
					if (Char == 0)
					{
						LastCharWasNull = TRUE;
					}
				}
			}
			else
			{
				if (Char == 0)
				{
					if (Name.size() != 0)
					{
						InternalMap[Name] = Value;
					}
						FillName = TRUE;
						
						Name = L"";
						Value = L"";
						LastCharWasNull = TRUE;
				}
				else
				{
					Value += Char;
				}
			}
		}


		FreeEnvironmentStringsW(Buff);

	}

	for (auto walk = Overwritten.begin(); walk != Overwritten.end(); walk++)
	{
		InternalMap[walk->first] = walk->second;
	}

	// now we convert the map to an enviorment block;
	Output = L"";

	for (auto walk = InternalMap.begin(); walk != InternalMap.end(); walk++)
	{
		Output += walk->first;
		Output += L"=";
		Output += walk->second;
		Output += (wchar_t)0;
	}
	if (Output.length() != 0)
	{
		// add the 2nd null char so the system knows we are done.
		Output += (wchar_t)0;
	}
}

PS_ProcessInformation::~PS_ProcessInformation()
{
	this->ProcessArgumentsContainer = L"";
	this->ProcessArgumentsContainer = L"";
	this->dwCreationFlags = 0;
	this->lpProcessAttributes = this->lpThreadAttributes = nullptr;

	if ((this->PInfo.hProcess != INVALID_HANDLE_VALUE) && (this->PInfo.hProcess != 0))
	{
		CloseHandle(this->PInfo.hProcess);
	}

	if ((this->PInfo.hThread != INVALID_HANDLE_VALUE) && (this->PInfo.hThread != 0))
	{
		CloseHandle(this->PInfo.hThread);
	}
	if (Insight != nullptr)
	{
		delete Insight;
	}
}


PS_ProcessInformation::PS_ProcessInformation(const PS_ProcessInformation& Original)
{
	ProcessNameContainer = Original.ProcessArgumentsContainer;
	ProcessArgumentsContainer = Original.ProcessArgumentsContainer;
	WorkingDirectory = Original.WorkingDirectory;

	StartUpInfo = Original.StartUpInfo;

	if (Original.lpProcessAttributes != NULL)
	{
		lpProcessAttributes = Original.lpProcessAttributes;
	}
	else
	{
		lpProcessAttributes = nullptr;
	}


	if (Original.lpThreadAttributes != NULL)
	{
		lpThreadAttributes = Original.lpThreadAttributes;
	}
	else
	{
		lpThreadAttributes = nullptr;
	}

	dwCreationFlags = Original.dwCreationFlags;

	if (Original.PInfo.dwProcessId != 0)
	{
		PInfo.dwProcessId = Original.PInfo.dwProcessId;
	}
	else
	{
		PInfo.dwProcessId = 0;
	}


	if (Original.PInfo.dwThreadId != 0)
	{
		PInfo.dwThreadId = Original.PInfo.dwThreadId;
	}


	if (Original.PInfo.hProcess != 0)
	{
		DuplicateHandle(GetCurrentProcess(), Original.PInfo.hProcess, GetCurrentProcess(), &PInfo.hProcess, 0, FALSE, DUPLICATE_SAME_ACCESS);
	}
	else
	{
		PInfo.hProcess = 0;
	}

	if (Original.PInfo.hThread != 0)
	{
		DuplicateHandle(GetCurrentProcess(), Original.PInfo.hThread, GetCurrentProcess(), &PInfo.hThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
	}
	else
	{
		PInfo.hProcess = 0;
	}

	bInhertEnviroment = Original.bInhertEnviroment;
	DebugModeHandle = Original.DebugModeHandle;

	for (auto stepper = Original.CommandmentArray.begin(); stepper != Original.CommandmentArray.end(); stepper++)
	{
		CommandmentArray[stepper._Ptr->_Myval.first] = stepper._Ptr->_Myval.second;
	}
	for (auto stepper = Original.Enviroment.begin(); stepper != Original.Enviroment.end(); stepper++)
	{
		Enviroment[stepper._Ptr->_Myval.first] = stepper._Ptr->_Myval.second;
	}


	for (auto stepper = Original.DetoursDll.begin(); stepper != Original.DetoursDll.end(); stepper++)
	{
		DetoursDll.push_back( stepper._Ptr->c_str());
	}



	for (auto stepper = Original.LoadLibraryPriorityFolders.begin(); stepper != Original.LoadLibraryPriorityFolders.end(); stepper++)
	{
		LoadLibraryPriorityFolders.push_back(stepper._Ptr->c_str());
	}



	for (auto stepper = Original.ForcedOverwrites.begin(); stepper != Original.ForcedOverwrites.end(); stepper++)
	{
		ForcedOverwrites[stepper._Ptr->_Myval.first] = stepper._Ptr->_Myval.second;
	}




	EnableSymbols = Original.EnableSymbols;

	ProcessThreads = Original.ProcessThreads;
	Insight = new InsightHunter(Original.Insight);

	RequestDebugPriv = Original.RequestDebugPriv;

}

PS_ProcessInformation::PS_ProcessInformation()
{
	PInfo.hProcess = PInfo.hThread = INVALID_HANDLE_VALUE;
	PInfo.dwProcessId = PInfo.dwThreadId = 0;

	ZeroMemory(&StartUpInfo, sizeof(StartUpInfo));
	ZeroMemory(&this->SyncData, sizeof(SyncData));

	dwCreationFlags = 0;
	lpProcessAttributes = lpThreadAttributes = nullptr;
	this->bInhertEnviroment = false;
	this->DebugModeHandle = PSINFO_DEBUGMODE_NOWORKERTHREAD;
	this->DetoursDll.clear();
	this->Enviroment.clear();
	this->ProcessNameContainer = L"";
	this->WorkingDirectory = L"";
	
	Insight = new InsightHunter();

}

const wchar_t* PS_ProcessInformation::ProcessName()
{
	return this->ProcessNameContainer.c_str();
}

void PS_ProcessInformation::ProcessName(const wchar_t* NewName)
{
	if (NewName == nullptr)
	{
		this->ProcessNameContainer = L"";
		return;
	}
	this->ProcessNameContainer = NewName;

}

const wchar_t* PS_ProcessInformation::ProcessArguments()
{
	return this->ProcessArgumentsContainer.c_str();
}

void PS_ProcessInformation::ProcessArguments(const wchar_t* NewArgs)
{
	if (NewArgs == nullptr)
	{
		this->ProcessArgumentsContainer = L"";
		return;
	}
	this->ProcessArgumentsContainer = NewArgs;
}

DWORD PS_ProcessInformation::CreationFlags()
{
	return this->dwCreationFlags;
}

void PS_ProcessInformation::CreationFlags(DWORD NewFlags)
{
	this->dwCreationFlags = NewFlags;
}

const wchar_t* PS_ProcessInformation::CurrentDirectory()
{
	return WorkingDirectory.c_str();
}

void PS_ProcessInformation::CurrentDirectory(const wchar_t* NewCD)
{
	if (NewCD == nullptr)
	{
		WorkingDirectory = L"";
		return;
	}
	WorkingDirectory = NewCD;
}

void PS_ProcessInformation::ImportSpawnerEnviroment(BOOL Yes)
{
	if (Yes)
	{
		this->bInhertEnviroment = TRUE;
	}
	else
	{
		this->bInhertEnviroment = FALSE;
	}
}

void PS_ProcessInformation::ClearEnviroment()
{
}

const wchar_t* PS_ProcessInformation::Environment(const wchar_t* name)
{
	if (Enviroment.find(name) == Enviroment.end())
	{
		return nullptr;
	}
	else
	{
		return Enviroment[name].c_str();
	}
}

void PS_ProcessInformation::Environment(const wchar_t* name, const wchar_t* value)
{
	this->Enviroment[name] = value;
}

void PS_ProcessInformation::AddDetoursDll(std::wstring Name)
{
	char* ansi = nullptr;
	if (Name.length() != 0)
	{
		ansi = ConvertUnicodeString(Name.c_str());
		if (ansi)
		{
			this->DetoursDll.insert(this->DetoursDll.end(), ansi);
			free(ansi);
		}
	}
}

void PS_ProcessInformation::AddDetoursDll(const char* Name)
{
	if (Name != nullptr)
	{
		if (Name[0] != 0)
		{
			this->DetoursDll.insert(this->DetoursDll.end(), Name);
		}

	}
}

void PS_ProcessInformation::ClearDetoursDll()
{
	this->DetoursDll.clear();
}

const std::vector<std::string> PS_ProcessInformation::GetDetourList()
{
	return DetoursDll;
}

const char* PS_ProcessInformation::IndexDetourList(int index)
{
	if ((index < 0) || (index >= this->DetoursDll.size()))
	{
		return nullptr;
	}
	return this->DetoursDll[index].c_str();
}

DWORD PS_ProcessInformation::GetDetourListSize()
{
	return DetoursDll.size();
}
DWORD PS_ProcessInformation::SpawnProcess()
{
	return SpawnProcessCommon(FALSE);
}


void PS_ProcessInformation::RefreshMemoryStatistics()
{
	ZeroMemory(&this->ProcessMemoryStats, sizeof(PROCESS_MEMORY_COUNTERS_EX));
	this->ProcessMemoryStats.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

	HANDLE ProcessHandle = 0;
	__try
	{
		ProcessHandle = OpenProcesForQueryInformation(this->PInfo.dwProcessId);
		if (ProcessHandle != INVALID_HANDLE_VALUE)
		{
			GetProcessMemoryInfo(ProcessHandle, (PPROCESS_MEMORY_COUNTERS)&this->ProcessMemoryStats, this->ProcessMemoryStats.cb);
		}
	}
	__finally
	{
		if (ProcessHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(ProcessHandle);
		}
	}

}

DWORD PS_ProcessInformation::SpawnProcessCommon(bool NoNotSpawnThread)
{
	bool DebugAskFailure = false;
	wchar_t* Arguments;
	LPCSTR* DetourListPtr;
	const wchar_t* EnvBlocArg;
	std::wstring EnvBlockContainer;
	const wchar_t* CurrDir;
	
	std::vector<const char*> DetourList;

	if (DetoursDll.size() == 0)
	{
		DetourList.clear();
		DetourListPtr = nullptr;
	}
	else
	{
		for (unsigned int step = 0; step < this->DetoursDll.size(); step++)
		{
			DetourList.insert(DetourList.end(), this->DetoursDll[step].c_str());
		}
		DetourListPtr = (LPCSTR*)&DetourList[0];
	}

	if (WorkingDirectory.size() == 0)
	{
		CurrDir = nullptr;
	}
	else
	{
		CurrDir = WorkingDirectory.c_str();
	}


	/// <summary>
	/// This copying and catching NULl for arguments is because MSDN documentation says CreateProcessW may modify the arguments sometimes.
	/// If CreateProcessW does this and source read only memory, an exception may occure.  
	/// </summary>
		if ( (this->ProcessArgumentsContainer.length() == 0) || (this->ProcessArgumentsContainer[0] == 0))
	{
		Arguments = nullptr;
	}
	else
	{
		Arguments = _wcsdup(this->ProcessArgumentsContainer.c_str());
	}


	

	this->StartUpInfo.cb = sizeof(STARTUPINFOW);
	if (this->StartUpInfo.wShowWindow == 0)
	{
		this->StartUpInfo.wShowWindow = SW_SHOWNORMAL;
	}


	BuildEnviromentBlock(this->Enviroment, this->bInhertEnviroment, EnvBlockContainer);


	if (EnvBlockContainer.size() == 0)
	{
		EnvBlocArg = nullptr;
	}
	else
	{
		EnvBlocArg = EnvBlockContainer.c_str();

	}

	
	if (!NoNotSpawnThread)
	{

		if (((dwCreationFlags & DEBUG_PROCESS) == DEBUG_PROCESS) || ((dwCreationFlags & DEBUG_ONLY_THIS_PROCESS) == DEBUG_ONLY_THIS_PROCESS))
		{
			/// <summary>
			/// This code asks if we are to ask for debug priv and sets a flag if the priv was not granted.
			/// </summary>
			if (RequestDebugPriv)
			{
				if (!AskForDebugPriv())
				{
					PInfo.hProcess = PInfo.hThread = INVALID_HANDLE_VALUE;
					PInfo.dwProcessId = PInfo.dwThreadId = 0;
					DebugAskFailure = TRUE;
				}
				else
				{
					DebugAskFailure = FALSE;
				}
			}
			else
			{
				// it didn't fail BUT we didn't ask for it. so we're still OK
				DebugAskFailure = FALSE;
			}

			if ((this->DebugModeHandle == PSINFO_DEBUGMODE_WORKERTHREADED) && (DebugAskFailure == FALSE))
			{
				this->SyncData.ContinueState = this->SyncData.threadID = 0;
				this->SyncData.EventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (this->SyncData.EventHandle != 0)
				{

					this->SyncData.that = this;

					this->SyncData.ThreadHandle = (HANDLE)_beginthread(PsPocessInformation_DebugWorkerthread, 0, &SyncData);
					this->SyncData.threadID = GetThreadId(SyncData.ThreadHandle);


					

					/// <summary>
					/// First wait is always until process creation.
					/// </summary>
					/// <returns></returns>
					WaitForSingleObject(SyncData.EventHandle, INFINITE);
				}
				else
				{

				}
			}
			else
			{
				if (!DetourCreateProcessWithDllsW(ProcessName(),
					Arguments,
					lpProcessAttributes,
					lpThreadAttributes,
					FALSE,
					dwCreationFlags | CREATE_UNICODE_ENVIRONMENT,
					(LPVOID)EnvBlocArg,
					CurrDir,
					& StartUpInfo,
					& PInfo,
					DetoursDll.size(),
					//&DetourList[0],
					DetourListPtr,
					nullptr))
				{
				PInfo.hProcess = PInfo.hThread = INVALID_HANDLE_VALUE;
				PInfo.dwProcessId = PInfo.dwThreadId = 0;
				}
			}
		}
		else
		{
		if (!DetourCreateProcessWithDllsW(ProcessName(),
			Arguments,
			lpProcessAttributes,
			lpThreadAttributes,
			FALSE,
			dwCreationFlags | CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED,
			(LPVOID)EnvBlocArg,
			CurrDir,
			&StartUpInfo,
			&PInfo,
			DetoursDll.size(),
			//&DetourList[0],
			DetourListPtr,
			nullptr))
		{
			PInfo.hProcess = PInfo.hThread = INVALID_HANDLE_VALUE;
			PInfo.dwProcessId = PInfo.dwThreadId = 0;
		}
		else
		{
			//CopyPayloads(PInfo.hProcess);
			ResumeThread(PInfo.hThread);
		}

		}
	}
	else
	{
	if (!DetourCreateProcessWithDllsW(ProcessName(),
		Arguments,
		lpProcessAttributes,
		lpThreadAttributes,
		FALSE,
		dwCreationFlags | CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED,
		(LPVOID)EnvBlocArg,
		CurrDir,
		&StartUpInfo,
		&PInfo,
		DetoursDll.size(),
		//&DetourList[0],
		DetourListPtr,
		nullptr))
	{
		PInfo.hProcess = PInfo.hThread = INVALID_HANDLE_VALUE;
		PInfo.dwProcessId = PInfo.dwThreadId = 0;
	}
	else
	{
		CopyPayloads(PInfo.hProcess);
		ResumeThread(PInfo.hThread);
	}
	}
	if (Arguments != 0)
	{
		free(Arguments);
	}
	return PInfo.dwProcessId;
}

VOID PS_ProcessInformation::CopyPayloads(HANDLE Target)
{
	/*
	* I'm toying with an idea of using DetoursPayloads to copy data to telemetry/helper dlls but it's not ready for use yet.
	*/
	/*
	std::vector<DWORD> ByteHappy;
	std::wstring PriorityLoadLibrary;
	std::wstringstream LoadLibraryForceOverwrides;
	bool HasSpace = false;
	/// <summary>
	/// Assemble of list of DWORD values that are positive in the array and copy to a vector (to ensure we can acctually byte count to target)
	/// </summary>
	for (auto stepper = CommandmentArray.begin(); stepper != CommandmentArray.end(); stepper++)
	{
		if (stepper._Ptr->_Myval.second == TRUE)
		{
			ByteHappy.insert(ByteHappy.end(), stepper._Ptr->_Myval.first);
		}
	}
	if (ByteHappy.size() != 0)
	{
		DetourCopyPayloadToProcessEx(Target, HelperDll_Commandment_Array, &ByteHappy, sizeof(DWORD) * ByteHappy.size());
	}

	/// <summary>
	/// Convert the array of strings intp a  A;B;C style thing before copiyng to target
	/// </summary>
	/// <param name="Target"></param>
	for (auto stepper = this->LoadLibraryPriorityFolders.begin(); stepper != LoadLibraryPriorityFolders.end(); stepper++)
	{
		if (wcsstr(stepper._Ptr->c_str(), L" ") != nullptr)
		{
			LoadLibraryForceOverwrides << L"\"" << stepper._Ptr->c_str() << L"\"";
		}
		else
		{
			LoadLibraryForceOverwrides << stepper._Ptr->c_str();
		}
		LoadLibraryForceOverwrides << L";";

		
	}

	if (LoadLibraryForceOverwrides.str().length() != 0)
	{
		DetourCopyPayloadToProcessEx(Target, HelperDll_PriorityLibraryPaths, LoadLibraryForceOverwrides.str().c_str(), (LoadLibraryForceOverwrides.str().length() + 1) * sizeof(wchar_t));
	}*/
}

VOID  PS_ProcessInformation::SetDebugEventCallback(DebugEventCallBackRoutine Callback)
{
	this->SyncData.UserCallback = Callback;
}

DebugEventCallBackRoutine PS_ProcessInformation::GetDebugEventCallback()
{
	return this->SyncData.UserCallback;
}

void PS_ProcessInformation::PulseDebugEventThread()
{
	SetEvent(this->SyncData.EventHandle);
}

void PS_ProcessInformation::SetDebugMode(DWORD dmMode)
{
	if (dmMode == PSINFO_DEBUGMODE_NOWORKERTHREAD)
	{
		this->DebugModeHandle = dmMode;
	}
	else
	{
		if (dmMode == PSINFO_DEBUGMODE_WORKERTHREADED)
		{
			this->DebugModeHandle = dmMode;
		}
	}
}

DWORD PS_ProcessInformation::GetDebugMode()
{
	return this->DebugModeHandle;
}

HANDLE PS_ProcessInformation::GetMainThreadHandle()
{
	return PInfo.hThread;
}

HANDLE PS_ProcessInformation::GetMainProcessHandle()
{
	return PInfo.hProcess;
}

DWORD PS_ProcessInformation::GetProcessID()
{
	return PInfo.dwProcessId;
}

DWORD PS_ProcessInformation::GetThreadID()
{
	return PInfo.dwThreadId;
}

VOID PS_ProcessInformation::SetDebugPrivState(BOOL WantPriv)
{
	this->RequestDebugPriv = WantPriv;
}

BOOL PS_ProcessInformation::SetCommandment(DWORD CommandMent, BOOL Status)
{
	if ((CommandMent >= 0) && (CommandMent < COMMANDMENT_MAX_VALUE))
	{
		//auto location = CommandmentArray.find(CommandMent);
		
		{
			CommandmentArray[CommandMent] = Status;
		}
	}
	return TRUE;
}

BOOL PS_ProcessInformation::GetCommandment(DWORD Commandment)
{
	if (CommandmentArray.find(Commandment) != CommandmentArray.end())
	{
		return CommandmentArray[Commandment];
	}
	return FALSE;
}

BOOL PS_ProcessInformation::AddPriorityLoadLibraryPathW(LPCWSTR Path)
{
	std::wstring buffer;
	if (Path != 0)
	{
		buffer = Path;
		this->LoadLibraryPriorityFolders.insert(this->LoadLibraryPriorityFolders.end(), buffer);
		return TRUE;
	}
	return FALSE;
}

BOOL PS_ProcessInformation::AddPriorityLoadLibraryPathA(LPCSTR Path)
{
	BOOL result = FALSE;
	if (Path != 0)
	{
		LPCWSTR Tmp = ConvertANSIString(Path);
		if (Tmp != 0)
		{
			result = AddPriorityLoadLibraryPathW(Tmp);
			free((void*)Tmp);
		}
	}
	return result;
}

DWORD PS_ProcessInformation::GetPriorityLoadLibaryPath_NumberOf()
{
	return this->LoadLibraryPriorityFolders.size();
}

LPCWSTR PS_ProcessInformation::GetPriorityLoadLibraryPath_Index(size_t Index)
{
	if (Index < LoadLibraryPriorityFolders.size())
	{
		return LoadLibraryPriorityFolders[Index].c_str();
	}
	return nullptr;
}

VOID PS_ProcessInformation::EmptyPriorityLoadLibaryPath()
{
	LoadLibraryPriorityFolders.clear();
}

DWORD PS_ProcessInformation::GetProcessIDCount()
{
	return this->ProcessThreads.ProcessCount();
}

DWORD PS_ProcessInformation::GetProcessIDs(DWORD* Output, DWORD LargestOutputSize)
{
	return 0;
}

DWORD PS_ProcessInformation::GetThreadListCount(DWORD ProcessID)
{
	return this->ProcessThreads.ThreadCount(ProcessID);
}

DWORD PS_ProcessInformation::GetThreadListCount()
{
	return this->ProcessThreads.ThreadCount(PInfo.dwProcessId);
}




DWORD PS_ProcessInformation::GetThreadIDs(DWORD ProcessID, DWORD* ThreadID, DWORD LargestOutputSize)
{
	if (ThreadID == nullptr)
	{
		return GetThreadListCount(ProcessID) * sizeof(DWORD);
	}
	ZeroMemory(ThreadID, LargestOutputSize);
	if (LargestOutputSize < GetThreadListCount(ProcessID) * sizeof(DWORD))
	{

	}
	return 0;
}

ThreadInsight* PS_ProcessInformation::GetThreadInsightPtr(DWORD ProcessID, DWORD ThreadID)
{
	return this->ProcessThreads.GetThreadInsightPtr(ProcessID, ThreadID);
}

DWORD PS_ProcessInformation::SetSymbolStatus(DWORD NewStatus)
{
	DWORD old = this->EnableSymbols;
	EnableSymbols = NewStatus;
	return old;
}

DWORD PS_ProcessInformation::GetSymbolStatus()
{
	return EnableSymbols;
}

InsightHunter* PS_ProcessInformation::GetSymbolHandlerPtr()
{
	if (Insight != nullptr)
	{
		return Insight;
	}
	else
	{
		Insight = new InsightHunter();
		return Insight;
	}
}

DWORD PS_ProcessInformation::GetPageFaultCount()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PageFaultCount;
}

SIZE_T PS_ProcessInformation::GetPeakWorkingSet()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PeakWorkingSetSize;
}

SIZE_T PS_ProcessInformation::GetWorkingSetSize()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.WorkingSetSize;
}

SIZE_T PS_ProcessInformation::GetQuotaPeakPagePoolUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaPagedPoolUsage;
}

SIZE_T PS_ProcessInformation::GetQuotaPagedPoolUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaPagedPoolUsage;
}

SIZE_T PS_ProcessInformation::GetQuotaPeakNonPageUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaPeakNonPagedPoolUsage;
}

SIZE_T PS_ProcessInformation::GetQuotaNonPageUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaNonPagedPoolUsage;
}

SIZE_T PS_ProcessInformation::GetPageFileUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PagefileUsage;
}

SIZE_T PS_ProcessInformation::GetPeakPageFileUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PeakPagefileUsage;
}

SIZE_T PS_ProcessInformation::GetPrivateUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PrivateUsage;
}

PROCESS_MEMORY_COUNTERS_EX* PS_ProcessInformation::GetMemoryStatsBulkPtr()
{
	RefreshMemoryStatistics();
	return &this->ProcessMemoryStats;
}


