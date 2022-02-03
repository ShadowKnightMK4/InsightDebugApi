#include "InsightHunter.h"
#include "Utility.h"
#include "ProcessHandling.h"
/// <summary>
/// if we are debugging at least one porocess (syminitializeW) with debug handle
/// </summary>
static volatile bool DebugHelpOnline = false;
/// <summary>
/// 
/// </summary>
static HANDLE HandleToSelf = INVALID_HANDLE_VALUE;

/// <summary>
/// This routien pretty much just passes its thing to the DotNet callback.
/// </summary>
/// <param name="pSynInfo"></param>
/// <param name="SymbolSize"></param>
/// <param name="DotNetCallback"></param>
/// <returns></returns>

BOOL WINAPI  NativeSymbolCallback(PSYMBOL_INFOW pSynInfo,  ULONG SymbolSize, LPVOID DotNetCallback)
{
	DebugBreak();
	return (*(SymbolSearchCallback)DotNetCallback)(pSynInfo);
	
}


InsightHunter::InsightHunter()
{
	if (DebugHelpOnline == false)
	{
		DebugHelpOnline = SymInitializeW(HandleToSelf, nullptr, FALSE);
	}
	InitializeCriticalSection(&SyncVariable);
	MainDebuggedProcess = INVALID_HANDLE_VALUE;
}

InsightHunter::InsightHunter(HANDLE DebuggedProcess)
{
	if (DebugHelpOnline == false)
	{
		DebugHelpOnline = SymInitializeW(DebuggedProcess, nullptr, FALSE);
		if (DebugHelpOnline)
		{
			if (!DuplicateHandle(GetCurrentProcess(), DebuggedProcess, GetCurrentProcess(), &MainDebuggedProcess, PROCESS_ALL_ACCESS, FALSE, DUPLICATE_SAME_ACCESS))
			{
				MainDebuggedProcess = INVALID_HANDLE_VALUE;
			}
		}
	}
	InitializeCriticalSection(&SyncVariable);
}

InsightHunter::~InsightHunter()
{
	if (DebugHelpOnline)
	{
		SymCleanup(HandleToSelf);
	}
	DeleteCriticalSection(&SyncVariable);
	for (auto stepper = AdditionalProcessHandles.begin(); stepper != AdditionalProcessHandles.end(); stepper++)
	{
		if (*stepper._Ptr != INVALID_HANDLE_VALUE)
		{
			CloseHandle(*stepper);
		}
	}

}

bool InsightHunter::LoadExeSymbolInfo(LPDEBUG_EVENT EventData)
{
	DWORD64 vcal;
	bool ret = false;
	wchar_t* ImageName;
	
	if (EventData != nullptr)
	{
		SetLastError(0);
		BeginThreadSynch();
		if (this->MainDebuggedProcess == INVALID_HANDLE_VALUE)
		{
			MainDebuggedProcess = OpenProcessForQueryReadWrite(EventData->dwProcessId);
			if (this->MainDebuggedProcess == INVALID_HANDLE_VALUE)
			{
				ret = false;
				goto endcrit;
			}
		}
		
		ImageName = GetModuleNameViaHandle(EventData->u.CreateProcessInfo.hProcess, 0);
		if (!SymLoadModuleExW(MainDebuggedProcess, EventData->u.CreateProcessInfo.hFile, ImageName, nullptr, 0, 0, nullptr, 0))
		{
			if (GetLastError() != ERROR_SUCCESS)
			{
				ret = false;
				goto endcrit;
			}
		}

		endcrit:
		EndThreadSync();
	}
	return ret;
}

bool InsightHunter::LoadDllSymbolInfo(LPDEBUG_EVENT EventData)
{
	SetLastError(0);
	BeginThreadSynch();
	if (SymLoadModule(HandleToSelf, EventData->u.LoadDll.hFile, nullptr, nullptr, (DWORD)EventData->u.LoadDll.lpBaseOfDll, 0) != 0)
	{
		EndThreadSync();
		return true;
	}
	else
	{
		EndThreadSync();
		if (GetLastError() == 0)
		{
			return true;
		}
	}
	EndThreadSync();
	return false;
}

bool InsightHunter::UnLoadDllSymbolInfo(LPDEBUG_EVENT EventData)
{
	BeginThreadSynch();
	if (SymUnloadModule(HandleToSelf, (DWORD) EventData->u.UnloadDll.lpBaseOfDll) ==true )
	{
		EndThreadSync();
		return true;
	}
	EndThreadSync();
	return false;
}



wchar_t* InsightHunter::WalkSearchPath()
{
	return nullptr;
}

bool InsightHunter::RefreshModuleList()
{
	bool ret = false;
	BeginThreadSynch();
	ret = SymRefreshModuleList(HandleToSelf);
	EndThreadSync();
	return ret;
}

BOOL InsightHunter::EnumerateLoadedSymbols(SymbolSearchCallback* DotNetCallback, wchar_t* SearchString)
{
	BOOL ret = FALSE;
	BeginThreadSynch();
	ret = SymEnumSymbolsExW(HandleToSelf, 0, SearchString, NativeSymbolCallback, DotNetCallback, SYMENUM_OPTIONS_DEFAULT);
	EndThreadSync();
	return ret;
}

bool InsightHunter::SetParentWindow(HWND Window)
{
	bool ret = false;
	BeginThreadSynch();
	ret = SymSetParentWindow(Window);
	EndThreadSync();
	return ret;
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
