#include "InsightHunter.h"
#include "Utility.h"
#include "ProcessHandling.h"

/*
* InsightHunter.cpp
* 
* header file: insighthunter.h
* source (this): insighthunter.cpp
* C level wrapper: InsightClass_CExports.cpp (Self contained and exported in Export.def)
* C# Sheath class InsightHunter.cs
* C# Sheath C Level Imports: InsightHUnterNative.Cpp (partial NativeMethods class)
* 
* 
* What this is:
* Public facing wrapps and symbol manamger accessable either sperately or directly with processcontext.cpp  
* Providing a way to synch acces to debughelp 
* 
* 
* Dependency class:
* InsightSupport.cpp <-     This deals with the symbol loading and unloading in a class and wraps symobls into a container for a single process
*					 <-		This is not intended to be publicly exported but is usable.  It's subject to change between releases a lot more.
*/

/// <summary>
/// The main process debugging debugged. While this is accessible to each individual InsightHunter instance, its shared between this. 
/// </summary>
HANDLE MainDebuggedProcess = 0;

/// <summary>
/// if we are debugging at least one process (syminitializeW) with debug handle
/// </summary>
static volatile bool DebugHelpOnline = false;

/// <summary>
/// This routine pretty much just passes its thing to the DotNet callback and use used to call the .NET size of the symbol enumeration.
/// </summary>
/// <param name="pSynInfo"></param>
/// <param name="SymbolSize"></param>
/// <param name="DotNetCallback"></param>
/// <returns></returns>

BOOL WINAPI  NativeSymbolCallback(PSYMBOL_INFOW pSynInfo,  ULONG SymbolSize, LPVOID DotNetCallback)
{
	return (*(SymbolSearchCallback)DotNetCallback)(pSynInfo);
}
/// <summary>
/// This routine is used to call the Managed size of Enumerating Source file data.
/// </summary>
/// <param name="pSourceFile"></param>
/// <param name="DotNetCallback"></param>
/// <returns></returns>
BOOL WINAPI NativeSourceFileCallback(PSOURCEFILEW pSourceFile, LPVOID DotNetCallback)
{
	return (*((SymbolSourceCallBack)DotNetCallback))(pSourceFile);
}


InsightHunter::InsightHunter()
{
	InitializeCriticalSection(&this->SyncVariable);
	this->EnableThreadSync = true;
	this->VersionData.MajorVersion = this->VersionData.MinorVersion = this->VersionData.Reserved = this->VersionData.Revision = 0;
}

InsightHunter::InsightHunter(HANDLE DebuggedProcess)
{
	InitializeCriticalSection(&this->SyncVariable);
	this->EnableThreadSync = true;
	this->VersionData.MajorVersion = this->VersionData.MinorVersion = this->VersionData.Reserved = this->VersionData.Revision = 0;
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
				
		}
		EndThreadSync();
	}

	{
		return ret;
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
	
	{
		BeginThreadSynch();
		ret = SymEnumSymbolsExW(MainDebuggedProcess, 0, SearchString, NativeSymbolCallback, DotNetCallback, SYMENUM_OPTIONS_DEFAULT);
		DWORD debug = GetLastError();
		EndThreadSync();
		return ret;
	}
}

BOOL InsightHunter::EnumerateSourceFiles(SymbolSourceCallBack* Callback, ULONG64 Base, wchar_t* SearchString)
{
	BOOL ret = FALSE;
	if (!DebugHelpOnline)
	{
		if (!this->InitializeSymbolEngineIfNot())
		{
			return FALSE;
		}
	}


	{
		BeginThreadSynch();
		ret = SymEnumSourceFilesW(MainDebuggedProcess, Base, SearchString, NativeSourceFileCallback, Callback);
		EndThreadSync();
		return ret;
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
		return TRUE;
	}

	return FALSE;

}

DWORD InsightHunter::SetSymbolOptions(DWORD Options)
{
	DWORD ret = 0;
	BeginThreadSynch();
	ret =  SymSetOptions(Options);
	EndThreadSync();
	return ret;
}

DWORD InsightHunter::GetSymbolOptions()
{
	DWORD ret = 0;
	BeginThreadSynch();
	ret =SymGetOptions();
	EndThreadSync();
	return ret;
}

VOID InsightHunter::SetDebugHelpVersionCompatability(USHORT Major, USHORT Minor, USHORT Revision)
{
	API_VERSION* LastCall = 0;
	API_VERSION Discard;
	Discard.MajorVersion = Major;
	Discard.MinorVersion = Minor;
	Discard.Revision = Revision;
	Discard.Reserved = 0;
	BeginThreadSynch();
	LastCall = ImagehlpApiVersionEx(&Discard);
	if (LastCall != 0)
	{
		this->ReadVersion = true;
		CopyMemory(&this->VersionData, LastCall, sizeof(VersionData));
	}
	EndThreadSync();
}

API_VERSION* InsightHunter::GetDebugHelpVersionCompatability()
{
	if (ReadVersion)
	{
		return &VersionData;
	}
	else
	{
		BeginThreadSynch();
		API_VERSION* tmp = ImagehlpApiVersion();
		if (tmp != nullptr)
		{
			CopyMemory(&VersionData, tmp, sizeof(API_VERSION));
		}
		EndThreadSync();
		return &VersionData;
	}
}

VOID InsightHunter::SetSymbolLoadCallback(SymbolLoadCallbackSignOff* UserCheckAgainst)
{
	return VOID();
}

SymbolLoadCallbackSignOff* InsightHunter::GetSymbolLoadCallBack()
{
	return nullptr;
}

bool InsightHunter::SetParentWindow(HWND Window)
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
		ret = SymSetParentWindow(Window);
		EndThreadSync();
	}
	return TRUE;
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
