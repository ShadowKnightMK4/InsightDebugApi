#include "InsightProcess.h"
#include "Utility.h"
#include <process.h>
#include "ProcessHandling.h"
#include "DebugEventInternal.h"
#include "detours.h"
#include "InsightHunter.h"

#include <sstream>
/*
* 09726c86073b4beeb1793f0793192459

f716644cad2e4afd9d5525d937df0c00
628dd6dd5aaf45909f849fabbf4d8baa
*/

#include <gsl>


/// <summary>
/// A default handler that does nothing except sign off on the event and continues it. Used when the UserCallback is not set (ie null).
/// </summary>
/// <param name="lpCurEvent"></param>
/// <param name="ContinueStatus"></param>
/// <param name="WaitTimer"></param>
/// <param name="CustomArg"></param>
/// <returns></returns>
int WINAPI DefaultHandler(LPDEBUG_EVENT lpCurEvent, gsl::not_null<DWORD*> ContinueStatus, gsl::not_null<DWORD*> WaitTimer, DWORD CustomArg) noexcept
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
	WorkerThreadData* Args = static_cast<WorkerThreadData*>(argument);
	DEBUG_EVENT lpEvent{};
	int Code = 0;
	// First we spawn the process.

	const DWORD dwId = Args->that->SpawnProcessCommon(true);
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
		BOOL EventOK = FALSE;
		ZeroMemory(&lpEvent, sizeof(DEBUG_EVENT));
		SetLastError(0);
		EventOK = WaitForDebugEventEx(&lpEvent, Args->WaitTimer);
		if ( (EventOK) && (GetLastError() == 0))
		{
			switch (lpEvent.dwDebugEventCode)
			{
			case EXIT_PROCESS_DEBUG_EVENT:
			{
				if ((Args->that->Insight != nullptr) && (Args->that->EnableSymbols))
				{
					Args->that->Insight->UnloadExeSymbolInfo(&lpEvent);
				}
				Args->that->ProcessThreads->ProcessExitProcessDebugEvent(&lpEvent);
				break;
			}
			case UNLOAD_DLL_DEBUG_EVENT:
			{
				if ((Args->that->Insight != nullptr) && (Args->that->EnableSymbols))
				{
					Args->that->Insight->UnLoadDllSymbolInfo(&lpEvent);
				}
				break;
			}
			case LOAD_DLL_DEBUG_EVENT:
			{
				if ((Args->that->Insight != nullptr) && (Args->that->EnableSymbols))
				{
					Args->that->Insight->LoadDllSymbolInfo(&lpEvent);
				}
				break;
			}
			case CREATE_PROCESS_DEBUG_EVENT:
			{
				if ((Args->that->Insight != nullptr) && (Args->that->EnableSymbols))
				{
					Args->that->Insight->LoadExeSymbolInfo(&lpEvent);
				}
				break;
			}
			case CREATE_THREAD_DEBUG_EVENT:
			{
				Args->that->ProcessThreads->ProcessCreateThreadDebugEvent(&lpEvent);
				break;
			}
			case EXIT_THREAD_DEBUG_EVENT:
			{
				Args->that->ProcessThreads->ProcessExitThreadDebugEvent(&lpEvent);
				break;
			}

			/* AT this level in the worker thread we do not need to handle the rest of the things*/
				case EXCEPTION_DEBUG_EVENT:
				case OUTPUT_DEBUG_STRING_EVENT:
				case RIP_EVENT:
				default:
				{
				}
			}



		}

		WaitForSingleObject(Args->EventHandle, INFINITE);
		ResetEvent(Args->EventHandle);

		if ((Args->that->DebugModeHandle && PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_DROPEVENT_MASK) == PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_DROPEVENT_MASK)
		{
			if ((EventOK) && (GetLastError() == 0))
			{
				if (lpEvent.dwDebugEventCode != 0)
				{
					if (Args->UserCallback == nullptr)
					{
						Code = DefaultHandler(&lpEvent, &Args->ContinueState, &Args->WaitTimer, 0);
					}
					else
					{
						Code = Args->UserCallback(&lpEvent, &Args->ContinueState, &Args->WaitTimer, 0);
					}
				}
			}
		}
		else
		{
			if ((EventOK) && (GetLastError() == 0))
			{
				if (Args->UserCallback == nullptr)
				{
					Code = DefaultHandler(&lpEvent, &Args->ContinueState, &Args->WaitTimer, 0);
				}
				else
				{
					Code = Args->UserCallback(&lpEvent, &Args->ContinueState, &Args->WaitTimer, 0);
				}
			}
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
/// <param name="IncludeBase">Do you want the default environment  block included</param>
/// <param name="Output">Place the results in this location ready to send to CreateProcessW</param>
/// <remarks>I feel this can be glitchy.  I'm unsure of a better solution ATM. We try to read and skip junk data at the start.</remarks>
void BuildEnviromentBlock(std::map<std::wstring, std::wstring>& Overwritten, BOOL IncludeBase, std::wstring& Output)
{
	std::map<std::wstring, std::wstring> InternalMap;
	if (IncludeBase)
	{
		LPWCH Buff = GetEnvironmentStringsW();
		wchar_t Char = 0;
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

InsightProcess::~InsightProcess()
{
	zeroout(this, true, true);

}


InsightProcess::InsightProcess(const InsightProcess& Original)
{
	dupto(&Original, this, true, false);

}


InsightProcess::InsightProcess(InsightProcess&& Original)
{
	if (this != &Original)
	{
		
		dupto(&Original, this, false, false);
		zeroout(&Original, false, false);
	}
}

InsightProcess& InsightProcess::operator=(const InsightProcess& other)
{
	if (this != &other)
	{
		dupto(&other, this, true, false);
	}
	
	return *this;
}

InsightProcess& InsightProcess::operator=(InsightProcess&& other)
{
	if (this != &other)
	{
		dupto(&other, this, false, false);
		zeroout(&other, false, false);
	}
	return *this;
}

InsightProcess::InsightProcess()
{
	init(this);
}

const wchar_t* InsightProcess::ProcessName()
{
	return this->ProcessNameContainer.c_str();
}

void InsightProcess::ProcessName(const wchar_t* NewName)
{
	if (NewName == nullptr)
	{
		this->ProcessNameContainer = L"";
		return;
	}
	this->ProcessNameContainer = NewName;

}

const wchar_t* InsightProcess::ProcessArguments()
{
	return this->ProcessArgumentsContainer.c_str();
}

void InsightProcess::ProcessArguments(const wchar_t* NewArgs)
{
	if (NewArgs == nullptr)
	{
		this->ProcessArgumentsContainer = L"";
		return;
	}
	this->ProcessArgumentsContainer = NewArgs;
}

DWORD InsightProcess::CreationFlags() 
{
	return this->dwCreationFlags;
}

void InsightProcess::CreationFlags(DWORD NewFlags)
{
	this->dwCreationFlags = NewFlags;
}

const wchar_t* InsightProcess::CurrentDirectory()
{
	return WorkingDirectory.c_str();
}

void InsightProcess::CurrentDirectory(const wchar_t* NewCD)
{
	if (NewCD == nullptr)
	{
		WorkingDirectory = L"";
		return;
	}
	WorkingDirectory = NewCD;
}

void InsightProcess::InheritSpawnEnvironment(BOOL Yes)
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

BOOL InsightProcess::InheritSpawnEnvironment()
{
	return this->bInhertEnviroment;
}

void InsightProcess::ClearEnviroment()
{
	Enviroment->clear();
}

const wchar_t* InsightProcess::Environment(const wchar_t* name)
{
	if (Enviroment->find(name) == Enviroment->end())
	{
		return nullptr;
	}
	else
	{
		return (*Enviroment)[name].c_str();
	}
}

void InsightProcess::Environment(const wchar_t* name, const wchar_t* value)
{
	if (value != nullptr)
	{
		(*Enviroment)[name] = value;
	}
	else
	{
		(*Enviroment).erase(name);
	}
}

void InsightProcess::AddDetoursDll(std::wstring Name)
{
	char* ansi = nullptr;
	if (Name.length() != 0)
	{
		ansi = ConvertUnicodeString(Name.c_str());
		if (ansi)
		{
			DetoursDll->insert(DetoursDll->end(), ansi);
		}
	}
}

void InsightProcess::AddDetoursDll(const char* Name)
{
	if ( (Name != nullptr) && (Name[0] != 0))
	{
		DetoursDll->emplace_back(Name);
	}
}

void InsightProcess::ClearDetoursDll()
{
	DetoursDll->clear();
	DetoursDll->shrink_to_fit();
	/*for (auto step = DetoursDll->begin(); step != DetoursDll->end(); step++)
	{
		
	}*/
}

//const std::vector<LPCSTR> InsightProcess::GetDetourList()
const std::vector<std::string> InsightProcess::GetDetourList()
{
	return *DetoursDll;
}

const char* InsightProcess::IndexDetourList(int index)
{
	if ((index < 0) || (index >= DetoursDll->size()))
	{
		return nullptr;
	}
	//return (DetoursDll[index])[0];
	throw;
	return DetoursDll[index][0].c_str();
}

unsigned long long InsightProcess::GetDetourListSize() noexcept
{
	return DetoursDll->size();
}
DWORD InsightProcess::SpawnProcess()
{
	return SpawnProcessCommon(FALSE);
}



void InsightProcess::init(InsightProcess* target)
{
	if (target != nullptr)
	{
		target->PInfo.hProcess = target->PInfo.hThread = INVALID_HANDLE_VALUE;
		target->PInfo.dwProcessId = target->PInfo.dwThreadId = 0;
		ZeroMemory(&target->SyncData, sizeof(SyncData));

		target->dwCreationFlags = 0;
		target->lpProcessAttributes = lpThreadAttributes = nullptr;
		target->bInhertEnviroment = false;
		target->DebugModeHandle = PSINFO_DEBUGMODE_NOWORKERTHREAD;
		
		target->ProcessNameContainer = L"";
		target->WorkingDirectory = L"";

		target->Insight = new InsightHunter();
		target->StartUpInfo = new StartupInfoWrapper();
		target->CommandmentArray = new std::map<DWORD, BOOL>();
		target->Enviroment = new std::map<std::wstring, std::wstring>();
		//target->DetoursDll = new std::vector<LPCSTR>();
		target->DetoursDll = new std::vector<std::string>();
		target->ProcessThreads = new ThreadContainer();

	}
}

void InsightProcess::zeroout(InsightProcess* target, bool closehandles, bool killpointers)
{
	if (target != nullptr)
	{
		if (closehandles)
		{
			if (target->PInfo.hProcess != INVALID_HANDLE_VALUE)
			{
				CloseHandle(target->PInfo.hProcess);
			}
			if (target->PInfo.hThread != INVALID_HANDLE_VALUE)
			{
				CloseHandle(target->PInfo.hThread);
			}
		}
		target->PInfo.hProcess = target->PInfo.hThread = INVALID_HANDLE_VALUE;
		target->PInfo.dwProcessId = target->PInfo.dwThreadId = 0;
		ZeroMemory(&target->SyncData, sizeof(SyncData));

		target->dwCreationFlags = 0;
		target->lpProcessAttributes = lpThreadAttributes = nullptr;
		target->bInhertEnviroment = false;
		target->DebugModeHandle = PSINFO_DEBUGMODE_NOWORKERTHREAD;
		target->DetoursDll->clear();
		target->Enviroment->clear();
		target->ProcessNameContainer = L"";
		target->WorkingDirectory = L"";

		if (killpointers)
		{
			if (target->Insight != nullptr)
			{
				delete target->Insight;
			}
			if (target->StartUpInfo != nullptr)
			{
				delete target->StartUpInfo;
			}

			if (target->CommandmentArray != nullptr)
			{
				delete target->CommandmentArray;
			}

			if (target->DetoursDll != nullptr)
			{
				delete target->DetoursDll;
			}

			if (target->Enviroment != nullptr)
			{
				delete target->Enviroment;
			}
			if (target->ProcessThreads != nullptr)
			{
				delete target->ProcessThreads;
			}
		}
		target->StartUpInfo = nullptr;
		target->Insight = nullptr;
		target->CommandmentArray = nullptr;
		target->DetoursDll = nullptr;
		target->Enviroment = nullptr;
		target->ProcessThreads = nullptr;
	}
}

void InsightProcess::dupto(const InsightProcess* source, InsightProcess* target, bool DeepCopy, bool initialized)
{
	if (source == target)
		return;

	if ((source == nullptr) || (target == nullptr))
	{
		return;
	}

#pragma region RequestDebugPriv
	// copy the non pointer value
	target->RequestDebugPriv = source->RequestDebugPriv;
#pragma endregion
#pragma region Insight
	// free the target's pointer value if valid for InsightHunter
	if (initialized)
	{
		if (target->Insight != nullptr)
		{
			delete target->Insight;
			target->Insight = nullptr;
		}
	}

	if ( (DeepCopy) && (source->Insight != nullptr))
	{
		target->Insight = new InsightHunter(source->Insight);
	}
	else
	{
		target->Insight = source->Insight;
	}
#pragma endregion
#pragma region ProcessThreads
	// free the target's pointer value if valid for ThreadContainer
	if (initialized)
	{
		if (target->ProcessThreads != nullptr)
		{
			delete target->ProcessThreads;
			target->ProcessThreads = nullptr;
		}
	}



	if ((DeepCopy) && (source->ProcessThreads != nullptr))
	{
		target->ProcessThreads = new ThreadContainer(*source->ProcessThreads);
	}
	else
	{
		target->ProcessThreads =source->ProcessThreads;
	}
#pragma endregion
#pragma region EnableSymbols
	// non pointer value,  assigment is enough
	target->EnableSymbols = source->EnableSymbols;
#pragma endregion

#pragma region ProcessMemoryStats
	// struct backed into the class.  assigment is ok as long as they don't add pointers
	target->ProcessMemoryStats = source->ProcessMemoryStats;
#pragma endregion

#pragma region CommandmentArrray
	if (initialized)
	{
		if (target->CommandmentArray != nullptr)
		{
			delete target->CommandmentArray;
			target->CommandmentArray = nullptr;
		}
	}

	if ((DeepCopy) && (source->CommandmentArray != nullptr))
	{
		target->CommandmentArray = new std::map<DWORD, BOOL>(*source->CommandmentArray);
	}
	else
	{
		target->CommandmentArray = source->CommandmentArray;
	}
#pragma endregion

#pragma region SyncData
	// TODO: figure a way to copy SyncData
#pragma endregion

#pragma region DebugModeHandle
	// non pointer value assigment is ok
	target->DebugModeHandle = source->DebugModeHandle;
#pragma endregion

#pragma region bInhertEnviroment

	// non pointer value assigment is ok
	target->bInhertEnviroment = source->bInhertEnviroment;
#pragma endregion


#pragma region PInfo

	// take care of any PInfo process / thread handles before clobbering the targets value
	if (initialized)
	{
		if (target->PInfo.hProcess != INVALID_HANDLE_VALUE)
		{
			CloseHandle(target->PInfo.hProcess);
		}
		if (target->PInfo.hThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(target->PInfo.hThread);
		}
	}

	if (DeepCopy)
	{
		if ((source->PInfo.hProcess != 0) && (source->PInfo.hProcess != INVALID_HANDLE_VALUE))
		{
			target->PInfo.hProcess = LocalHandleDup(source->PInfo.hProcess, 0, TRUE);
		}
		else
		{
			target->PInfo.hProcess = source->PInfo.hProcess;
		}
	}
	else
	{
		target->PInfo.hProcess = source->PInfo.hProcess;
	}

	if (DeepCopy)
	{
		if ((source->PInfo.hThread != 0) && (source->PInfo.hThread != INVALID_HANDLE_VALUE))
		{
			target->PInfo.hThread = LocalHandleDup(source->PInfo.hThread, 0, TRUE);
		}
		else
		{
			target->PInfo.hThread = source->PInfo.hThread;
		}
	}
	else
	{
		target->PInfo.hThread = source->PInfo.hThread;
	}


	target->PInfo.dwProcessId = source->PInfo.dwThreadId;
	target->PInfo.dwProcessId = source->PInfo.dwThreadId;
#pragma endregion


#pragma region StartupInfo
	// take care of the target's startupinfo before overriding it.
	if (initialized)
	{
		if (target->StartUpInfo != 0)
		{
			delete target->StartUpInfo;
			target->StartUpInfo = 0;
		}
	}

	if ((DeepCopy) && (source->StartUpInfo != nullptr))
	{
		target->StartUpInfo = new StartupInfoWrapper(*source->StartUpInfo);
	}
	else
	{
		target->StartUpInfo = 0;
	}
#pragma endregion

#pragma region dwCreationFlags

	// non pointer value assigment is ok
	target->dwCreationFlags = source->dwCreationFlags;
#pragma endregion

#pragma region ForcedOverrites

	// take care of the target's pointer 
	if (initialized)
	{
		if (target->ForcedOverwrites != nullptr)
		{
			delete target->ForcedOverwrites;
			target->ForcedOverwrites = 0;
		}
	}

	if ((DeepCopy) && (source->ForcedOverwrites != nullptr))
	{
		target->ForcedOverwrites = new std::map < std::wstring, std::wstring>(*source->ForcedOverwrites);
	}
	else
	{
		target->ForcedOverwrites = 0;
	}
#pragma endregion

#pragma region LoadLibraryPriorityFolders
	// take care of the target's pointer 
	if (initialized)
	{
		if (target->LoadLibraryPriorityFolders != nullptr)
		{
			delete target->LoadLibraryPriorityFolders;
			target->LoadLibraryPriorityFolders = 0;
		}
	}

	if ((DeepCopy) && (source->LoadLibraryPriorityFolders != nullptr))
	{
		target->LoadLibraryPriorityFolders = new std::vector < std::wstring>(*source->LoadLibraryPriorityFolders);
	}
	else
	{
		target->LoadLibraryPriorityFolders = 0;
	}
#pragma endregion

#pragma region DetoursDll
	// take care of the target's pointer 
	if (initialized)
	{
		if (target->DetoursDll != nullptr)
		{
			delete target->DetoursDll;
			target->DetoursDll = 0;
		}
	}

	if ((DeepCopy) && (source->DetoursDll != nullptr))
	{
		//target->DetoursDll = new std::vector < LPCSTR>(*source->DetoursDll);
		target->DetoursDll = new std::vector < std::string>(*source->DetoursDll);
	}
	else
	{
		target->DetoursDll = 0;
	}
#pragma endregion


#pragma region Enviroment
	// take care of the target's pointer 
	if (initialized)
	{
		if (target->Enviroment != nullptr)
		{
			delete target->Enviroment;
			target->Enviroment = 0;
		}
	}

	if ((DeepCopy) && (source->Enviroment != nullptr))
	{
		target->Enviroment = new std::map < std::wstring, std::wstring>(*source->Enviroment);
	}
	else
	{
		target->Enviroment = 0;
	}
#pragma endregion


#pragma region Process and thread Attribute pointer stubs
	// TODO: when we actually add support for copying security attributes. UPDATE this.
#pragma message("Notice: The routine dupto() will need to be updated when Process and Thread Security Attributes support is added otherwise it won't be copied currently.")
	target->lpProcessAttributes = source->lpProcessAttributes;
	target->lpThreadAttributes = source->lpThreadAttributes;
#pragma endregion



#pragma region WorkingDirectory ProcessArguments and ProcessName
	// STL takes care of this value

	target->WorkingDirectory = source->WorkingDirectory;

	// STL takes care of this value
	target->ProcessArgumentsContainer = source->ProcessArgumentsContainer;


	// STL takes care of this value
	target->ProcessNameContainer = source->ProcessNameContainer;
#pragma endregion



}

void InsightProcess::PrivateDefault(InsightProcess* target)
{
}


void InsightProcess::RefreshMemoryStatistics()
{
	ZeroMemory(&this->ProcessMemoryStats, sizeof(PROCESS_MEMORY_COUNTERS_EX));
	this->ProcessMemoryStats.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

	HANDLE ProcessHandle = 0;
	__try
	{
		ProcessHandle = OpenProcesForQueryInformation(this->PInfo.dwProcessId);
		if (ProcessHandle != INVALID_HANDLE_VALUE)
		{
			GetProcessMemoryInfo(ProcessHandle, reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&this->ProcessMemoryStats), this->ProcessMemoryStats.cb);
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





DWORD InsightProcess::SpawnProcessCommon(bool NoNotSpawnThread)
{
	bool DebugAskFailure = false;
	wchar_t* Arguments = 0;
	LPCSTR* DetourListPtr = 0;
	const wchar_t* EnvBlocArg = nullptr;
	std::wstring EnvBlockContainer;
	const wchar_t* CurrDir;	
	

		if (DetoursDll->size() == 0)
		{
			DetourListPtr = nullptr;
		}
		else
		{
			DetourListPtr = (LPCSTR*)&this->DetoursDll->at(0);
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
		/// If CreateProcessW does this and source read only memory, an exception may occur.  
		/// </summary>
		if ((this->ProcessArgumentsContainer.length() == 0) || (this->ProcessArgumentsContainer[0] == 0))
		{
			Arguments = nullptr;
		}
		else
		{
			Arguments = _wcsdup(this->ProcessArgumentsContainer.c_str());
		}




		/*this->StartUpInfo.cb = sizeof(STARTUPINFOW);
		if (this->StartUpInfo.wShowWindow == 0)
		{
			this->StartUpInfo.wShowWindow = SW_SHOWNORMAL;
		}*/
		if (StartUpInfo->wShowWindow() == 0)
		{
			StartUpInfo->wShowWindow(SW_NORMAL);
		}



		BuildEnviromentBlock(*this->Enviroment, this->bInhertEnviroment, EnvBlockContainer);


		/* CreateProcessW says if enviroment block is null, child process inheirts ours.  reason for this is to use null instead of pointer to emptry string*/
		if (EnvBlockContainer.size() != 0)
		{
			EnvBlocArg = EnvBlockContainer.c_str();
		}
		else
		{
			
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

				if ( ((this->DebugModeHandle && PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_MASK) == PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_MASK) && (DebugAskFailure == FALSE))
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
					//if (!DetourCreateProcessWithDllsW(ProcessName(),
					//if (!DetourCreateProcessWithDllExW(ProcessName(),
					if (!DetourCreateProcessWithDllsW(ProcessName(),
						Arguments,
						lpProcessAttributes,
						lpThreadAttributes,
						FALSE,
						dwCreationFlags | CREATE_UNICODE_ENVIRONMENT,
						(LPVOID)EnvBlocArg,
						CurrDir,
						(LPSTARTUPINFOW) & ((StartUpInfo->GetPtr())->StartupInfo),
						&PInfo,
						DetoursDll->size(),
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
					(LPSTARTUPINFOW) & ((StartUpInfo->GetPtr())->StartupInfo),
					&PInfo,
					DetoursDll->size(),
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
					if (!(dwCreationFlags & CREATE_SUSPENDED) == (CREATE_SUSPENDED) )
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
				(LPSTARTUPINFOW) & (this->StartUpInfo->GetPtr()->StartupInfo),
				&PInfo,
				DetoursDll->size(),
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


	{
		if (Arguments != nullptr)
		{
free(Arguments);
		}
	}
	return PInfo.dwProcessId;
}

VOID InsightProcess::CopyPayloads(HANDLE Target)
{
	/*
	* I'm toying with an idea of using DetoursPayloads to copy data to telemetry/helper dlls but it's not ready for public use yet
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

/// <summary>
/// Wait for either the entire known process chain to quit or only the main one. 
/// </summery> 
BOOL InsightProcess::WaitUntilExit(DWORD TimeWait, BOOL OnlyMain)
{
	if (OnlyMain)
	{
		return MsgWaitForMultipleObjects(1, &this->PInfo.hProcess, TRUE, TimeWait, QS_ALLEVENTS);
	}
	else
	{

	}
}

StartupInfoWrapper* InsightProcess::GetStartupInfoHandler()
{
	return this->StartUpInfo;
}

VOID  InsightProcess::SetDebugEventCallback(DebugEventCallBackRoutine Callback)
{
	this->SyncData.UserCallback = Callback;
}

DebugEventCallBackRoutine InsightProcess::GetDebugEventCallback()
{
	return this->SyncData.UserCallback;
}

void InsightProcess::PulseDebugEventThread()
{
	SetEvent(this->SyncData.EventHandle);
}

void InsightProcess::SetDebugMode(DWORD dmMode)
{

	if ((dmMode >= PSINFO_DEBUGMODE_LOWER) && (dmMode <= PSINFO_DEBUGMODE_UPPER))
	{
		/*
		* we check for campativility.
		* 
		* Is the worker thread wanted?
		*		is the drop no event flag set?
		*/
		if ((dmMode && PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_MASK) == PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_MASK)
		{
			this->DebugModeHandle = dmMode;
		}
		else
		{
			if ((dmMode && PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_DROPEVENT_MASK) != (PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_DROPEVENT_MASK))
			{
				this->DebugModeHandle = dmMode;
			}
		}
	}
	/*if (dmMode == PSINFO_DEBUGMODE_NOWORKERTHREAD)
	{
		this->DebugModeHandle = dmMode;
	}
	else
	{
		if (dmMode == PSINFO_DEBUGMODE_WORKERTHREADED)
		{
			this->DebugModeHandle = dmMode;
		}
	}*/
}

DWORD InsightProcess::GetDebugMode()
{
	return this->DebugModeHandle;
}

HANDLE InsightProcess::GetMainThreadHandle()
{
	return PInfo.hThread;
}

HANDLE InsightProcess::GetMainProcessHandle()
{
	return PInfo.hProcess;
}

DWORD InsightProcess::GetProcessID()
{
	return PInfo.dwProcessId;
}

DWORD InsightProcess::GetThreadID()
{
	return PInfo.dwThreadId;
}

VOID InsightProcess::SetDebugPrivState(BOOL WantPriv)
{
	this->RequestDebugPriv = WantPriv;
}

BOOL InsightProcess::SetCommandment(DWORD CommandMent, BOOL Status)
{
	if ((CommandMent >= 0) && (CommandMent < COMMANDMENT_MAX_VALUE))
	{
		{
			CommandmentArray->insert_or_assign(CommandMent, Status);
		}
	}
	return TRUE;
}

BOOL InsightProcess::GetCommandment(DWORD Commandment)
{
	auto locate = CommandmentArray->find(Commandment);

	if (locate != CommandmentArray->end())
	{
		return locate->second;
	}
	return FALSE;
}

BOOL InsightProcess::AddPriorityLoadLibraryPathW(LPCWSTR Path)
{
	std::wstring buffer;
	if (Path != 0)
	{
		buffer = Path;
		LoadLibraryPriorityFolders->insert(LoadLibraryPriorityFolders->end(), buffer);
		return TRUE;
	}
	return FALSE;
}

BOOL InsightProcess::AddPriorityLoadLibraryPathA(LPCSTR Path)
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

DWORD InsightProcess::GetPriorityLoadLibaryPath_NumberOf()
{
	return LoadLibraryPriorityFolders->size();
}

LPCWSTR InsightProcess::GetPriorityLoadLibraryPath_Index(size_t Index)
{
	if (Index < LoadLibraryPriorityFolders->size())
	{
		return (*(LoadLibraryPriorityFolders))[Index].c_str();
	}
	return nullptr;
}

VOID InsightProcess::EmptyPriorityLoadLibaryPath()
{
	LoadLibraryPriorityFolders->clear();
}

DWORD InsightProcess::GetProcessIDCount()
{
	return ProcessThreads->ProcessCount();
}

DWORD InsightProcess::GetProcessIDs(DWORD* Output, DWORD LargestOutputSize)
{
	return 0;
}

DWORD InsightProcess::GetThreadListCount(DWORD ProcessID)
{
	return ProcessThreads->ThreadCount(ProcessID);
}

DWORD InsightProcess::GetThreadListCount()
{
	return ProcessThreads->ThreadCount(PInfo.dwProcessId);
}




DWORD InsightProcess::GetThreadIDs(DWORD ProcessID, DWORD* ThreadID, DWORD LargestOutputSize)
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

ThreadInsight* InsightProcess::GetThreadInsightPtr(DWORD ProcessID, DWORD ThreadID)
{
	return ProcessThreads->GetThreadInsightPtr(ProcessID, ThreadID);
}

DWORD InsightProcess::SetSymbolStatus(DWORD NewStatus) noexcept
{
	const DWORD old = this->EnableSymbols;
	EnableSymbols = NewStatus;
	return old;
}

DWORD InsightProcess::GetSymbolStatus() noexcept
{
	return EnableSymbols;
}

InsightHunter* InsightProcess::GetSymbolHandlerPtr()
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

DWORD InsightProcess::GetPageFaultCount()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PageFaultCount;
}

SIZE_T InsightProcess::GetPeakWorkingSet()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PeakWorkingSetSize;
}

SIZE_T InsightProcess::GetWorkingSetSize()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.WorkingSetSize;
}

SIZE_T InsightProcess::GetQuotaPeakPagePoolUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaPagedPoolUsage;
}

SIZE_T InsightProcess::GetQuotaPagedPoolUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaPagedPoolUsage;
}

SIZE_T InsightProcess::GetQuotaPeakNonPageUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaPeakNonPagedPoolUsage;
}

SIZE_T InsightProcess::GetQuotaNonPageUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.QuotaNonPagedPoolUsage;
}

SIZE_T InsightProcess::GetPageFileUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PagefileUsage;
}

SIZE_T InsightProcess::GetPeakPageFileUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PeakPagefileUsage;
}

SIZE_T InsightProcess::GetPrivateUsage()
{
	RefreshMemoryStatistics();
	return this->ProcessMemoryStats.PrivateUsage;
}

PROCESS_MEMORY_COUNTERS_EX* InsightProcess::GetMemoryStatsBulkPtr()
{
	RefreshMemoryStatistics();
	return &this->ProcessMemoryStats;
}


