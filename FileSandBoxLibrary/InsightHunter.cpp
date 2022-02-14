#include "InsightHunter.h"
#include "Utility.h"
#include "ProcessHandling.h"


/// <summary>
/// The main process debuging debugged
/// </summary>
HANDLE MainDebuggedProcess = 0;

/// <summary>
/// if we are debugging at least one porocess (syminitializeW) with debug handle
/// </summary>
static volatile bool DebugHelpOnline = false;

/// <summary>
/// This routien pretty much just passes its thing to the DotNet callback.
/// </summary>
/// <param name="pSynInfo"></param>
/// <param name="SymbolSize"></param>
/// <param name="DotNetCallback"></param>
/// <returns></returns>

BOOL WINAPI  NativeSymbolCallback(PSYMBOL_INFOW pSynInfo,  ULONG SymbolSize, LPVOID DotNetCallback)
{
	return (*(SymbolSearchCallback)DotNetCallback)(pSynInfo);
	
}


InsightHunter::InsightHunter()
{
	InitializeCriticalSection(&this->SyncVariable);
}

InsightHunter::InsightHunter(HANDLE DebuggedProcess)
{
	InitializeCriticalSection(&this->SyncVariable);
	SetMainDebugTarget(DebuggedProcess);
}

InsightHunter::~InsightHunter()
{	
}



bool InsightHunter::LoadExeSymbolInfo(LPDEBUG_EVENT EventData)
{
	BOOL ret = false;
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return ret;
		}
	}
	if (EventData == nullptr)
	{
		return ret;
	}
	if (EventData->dwDebugEventCode != CREATE_PROCESS_DEBUG_EVENT)
	{
		return ret;
	}

	if (DebugHelpOnline)
	{
		BeginThreadSynch();
		auto LocateMe = this->HandleContainer.find(EventData->dwProcessId);
		if (LocateMe != this->HandleContainer.end())
		{
			ret = LocateMe._Ptr->_Myval.second->LoadExe(MainDebuggedProcess, EventData);
		}
		else
		{
			
			this->HandleContainer.insert({ EventData->dwProcessId , new InsightSupport_SymbolHandle(MainDebuggedProcess, EventData) });
				//std::pair<DWORD, InsightSupport_SymbolHandle>(EventData->dwProcessId,  EventData->dwDebugEventCode, new InsightSupport_SymbolHandle(MainDebuggedProcess, EventData )));
		}
		EndThreadSync();
	}
}

bool InsightHunter::LoadDllSymbolInfo(LPDEBUG_EVENT EventData)
{
	BOOL ret = FALSE;
	if (EventData == nullptr)
	{
		return ret;
	}
	if (EventData->dwDebugEventCode != LOAD_DLL_DEBUG_EVENT)
	{
		return ret;
	}
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return ret;
		}
	}

	if (DebugHelpOnline)
	{
		BeginThreadSynch();
		auto LocateMe = this->HandleContainer.find(EventData->dwProcessId);
		if (LocateMe != this->HandleContainer.end())
		{
			ret = LocateMe._Ptr->_Myval.second->LoadDll(MainDebuggedProcess, EventData);
		}
		else
		{
			// ret = FALSE;
		}
		EndThreadSync();
	}
	return ret;
}

bool InsightHunter::UnLoadDllSymbolInfo(LPDEBUG_EVENT EventData)
{
	BOOL ret = false;
	if (EventData == nullptr)
	{
		return ret;
	}
	if (EventData->dwDebugEventCode != UNLOAD_DLL_DEBUG_EVENT)
	{
		return ret;
	}
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return ret;
		}
	}
	
	if (DebugHelpOnline)
	{
		BeginThreadSynch();
		auto LocateMe = this->HandleContainer.find(EventData->dwDebugEventCode);
		if (LocateMe != this->HandleContainer.end())
		{
			ret = LocateMe._Ptr->_Myval.second->UnloadDll(MainDebuggedProcess, EventData);
		}
		else
		{
			// ret = false;
		}
		EndThreadSync();
	}
	return ret;
}

bool InsightHunter::UnloadExeSymbolInfo(LPDEBUG_EVENT EventData)
{	
	bool ret = false;
	if (EventData == nullptr)
	{
		return ret;
	}
	if (EventData->dwDebugEventCode != EXIT_PROCESS_DEBUG_EVENT)
	{
		return ret;
	}

	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return ret;
		}
	}
	
	if (DebugHelpOnline)
	{
		BeginThreadSynch();
		auto LocateMe = this->HandleContainer.find(EventData->dwDebugEventCode);
		if (LocateMe != this->HandleContainer.end())
		{
			ret = LocateMe._Ptr->_Myval.second->UnloadProcess(MainDebuggedProcess, EventData);
		}
		else
		{
			// ret = false;
		}
		EndThreadSync();
	}
	return ret;
}



wchar_t* InsightHunter::WalkSearchPath()
{
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return FALSE;
		}
	}
	else
	{
		BeginThreadSynch();
		EndThreadSync();
	}
	return nullptr;
}

bool InsightHunter::RefreshModuleList()
{
	bool ret = false;
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return ret;
		}
	}
	else
	{
		BeginThreadSynch();
		ret = SymRefreshModuleList(MainDebuggedProcess);
		EndThreadSync();
	}
	return ret;
}

BOOL InsightHunter::EnumerateLoadedSymbols(SymbolSearchCallback* DotNetCallback, wchar_t* SearchString)
{
	BOOL ret = FALSE;
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return FALSE;
		}
	}
	else
	{
		BeginThreadSynch();
		ret = SymEnumSymbolsExW(MainDebuggedProcess, 0, SearchString, NativeSymbolCallback, DotNetCallback, SYMENUM_OPTIONS_DEFAULT);
		EndThreadSync();
	}
}

BOOL InsightHunter::SetMainDebugTarget(DWORD TargetID)
{
	BOOL ret = FALSE;
	HANDLE TmpHandler = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, FALSE, TargetID);
	if (TmpHandler != 0)
	{
		ret = SetMainDebugTarget(TmpHandler);
		CloseHandle(TmpHandler);
	}
	return ret;
}

BOOL InsightHunter::SetMainDebugTarget(HANDLE MainDebug)
{
	HANDLE tmp = 0;
	if (DebugHelpOnline)
	{
		ClearSymbolContainer();
	}
	


	if (!DebugHelpOnline)
	{
		BeginThreadSynch();
		if (DuplicateHandle(GetCurrentProcess(), MainDebug, GetCurrentProcess(), &tmp, PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, FALSE, 0) == TRUE)
		{
			if (MainDebuggedProcess != 0)
			{
				CloseHandle(MainDebuggedProcess);
			}
			MainDebuggedProcess = tmp;
			DebugHelpOnline = SymInitializeW(MainDebuggedProcess, FALSE, FALSE);
		}
		
		EndThreadSync();
	}

	return FALSE;

}

bool InsightHunter::SetParentWindow(HWND Window)
{
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return FALSE;
		}
	}
	else
	{
		BeginThreadSynch();
		EndThreadSync();
	}
}

bool InsightHunter::EnforceThreadSync(BOOL NewVal)
{
	bool OldVal = this->EnableThreadSync;
	EnableThreadSync = NewVal;
	return OldVal;

}

bool InsightHunter::GetThreadSyncState()
{
	return EnableThreadSync;
}

void InsightHunter::ClearSymbolContainer()
{
	for (auto stepper = this->HandleContainer.begin(); stepper != this->HandleContainer.end(); stepper++)
	{
		delete stepper._Ptr->_Myval.second;
	}
	this->HandleContainer.clear();

	if (DebugHelpOnline)
	{
		SymCleanup(MainDebuggedProcess);
	}
	if (MainDebuggedProcess!= INVALID_HANDLE_VALUE)
	{
		CloseHandle(MainDebuggedProcess);
	}
	DebugHelpOnline = false;
}

BOOL InsightHunter::InitializeSymbolEngineIfNot()
{
	if (!DebugHelpOnline)
	{
		DebugHelpOnline = SymInitializeW(MainDebuggedProcess, 0, FALSE);
		return DebugHelpOnline;
	}
	return FALSE;
}


BOOL InsightHunter::LoadModuleInfo(LPDEBUG_EVENT EventData)
{
	return FALSE;
}

void InsightHunter::BeginThreadSynch()
{
	if (EnableThreadSync)
	{
		EnterCriticalSection(&SyncVariable);
	}
}

void InsightHunter::EndThreadSync()
{
	if (EnableThreadSync)
	{
		LeaveCriticalSection(&SyncVariable);
	}
}
