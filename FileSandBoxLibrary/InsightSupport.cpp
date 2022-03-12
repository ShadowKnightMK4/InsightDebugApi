#include "InsightSupport.h"
#include "Utility.h"
#include "ProcessHandling.h"
/*
* InsightSuport.cpp
* 
* Classes in here support InsightHunter's class. This one is subject to change from release to release and may not be compataible with 
* previous versions.  
* 
* The current implementation groups the main exe and loaded dlls in one area.  Additional Exes + load dlls in seperate processes will
* get your own process.
* 
* destructure~ frees any allocated IMAGEHLP_MODULE64 loeaded for Dlls and unleads loaded Symbols.
* WORD of Advice:
* This class is highly likely to subject to change bvetween releases.  There's a reason why routines aren't exported to instance and create this class.
* This class depends on InsightHunter to cordinate calls to the symbol engine. It assumes that's already done.
* 
* MainDebuggedProcess is the handle passed to SymInitilize() 
*/

InsightSupport_SymbolHandle::InsightSupport_SymbolHandle()
{
	this->ProcessHandle = INVALID_HANDLE_VALUE;
	this->ProcessID = 0;
	memset(&this->SymbolData, 0, sizeof(IMAGEHLP_MODULE64));
}

InsightSupport_SymbolHandle::InsightSupport_SymbolHandle(HANDLE MainDebuggedProcess, LPDEBUG_EVENT EventData)
{
	DWORD64 Est;
	this->ProcessHandle = MainDebuggedProcess;
	this->FailBit = false;


	if (EventData != nullptr)
	{
		if (EventData->dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT)
		{
			Est = SymLoadModuleExW(MainDebuggedProcess, EventData->u.CreateProcessInfo.hFile, nullptr, nullptr, 0, 0, nullptr, 0);
			if (Est != 0)
			{
				memset(&SymbolData, 0, sizeof(SymbolData));
				SymbolData.SizeOfStruct = sizeof(IMAGEHLP_MODULEW64);

				SymGetModuleInfo64(MainDebuggedProcess, Est, &this->SymbolData);
			}
		}
	}
}

/// <summary>
/// Descructor for the class. KEEP THESE THINGS IN MIND.
/// Handle is the same one the parent class "insighthunter" handles. we don't; touch it.
/// We unloead the core module symbol info loaded at SymbolData.baseofimage if not zero.
/// We also loop thru the loaded dlls and do the same thing.  We also delete data if ptr != 0
/// </summary>
InsightSupport_SymbolHandle::~InsightSupport_SymbolHandle()
{
	for (auto stepper = this->LoadedDlls.begin(); stepper != this->LoadedDlls.end(); stepper++)
	{
		SymUnloadModule64(ProcessHandle, stepper._Ptr->_Myval.first);
		if (stepper._Ptr->_Myval.second != 0)
		{
			free(stepper._Ptr->_Myval.second);
		}
	}
	if (SymbolData.BaseOfImage != 0)
	{
		SymUnloadModule64(ProcessHandle, SymbolData.BaseOfImage);
	}
	
}



bool InsightSupport_SymbolHandle::LoadDll(HANDLE MainDebuggedProcess, LPDEBUG_EVENT EventData)
{
	IMAGEHLP_MODULEW64* tmp=0;
	DWORD64 Est = 0;
	tmp = (IMAGEHLP_MODULEW64*)malloc(sizeof(IMAGEHLP_MODULEW64));
	if (tmp != nullptr)
	{
		memset(tmp, 0, sizeof(IMAGEHLP_MODULEW64));
		tmp->SizeOfStruct = sizeof(IMAGEHLP_MODULEW64);

		if ((EventData != 0) && (EventData->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT))
		{
			Est = SymLoadModuleExW(MainDebuggedProcess, EventData->u.LoadDll.hFile, nullptr, nullptr, (DWORD64)EventData->u.LoadDll.lpBaseOfDll, 0, nullptr, 0);
			if (Est != 0)
			{
				
				if (SymGetModuleInfo64(MainDebuggedProcess, Est, tmp)) 
				{
					LoadedDlls.insert({ EventData->dwProcessId, tmp });
					return true;
				}
				else
				{
					free(tmp);
				}
			}

		}
	}
	return false;
}

bool InsightSupport_SymbolHandle::LoadExe(HANDLE MainDebuggedProcess, LPDEBUG_EVENT EventData)
{
	return false;
}

bool InsightSupport_SymbolHandle::UnloadDll(HANDLE MainDebuggedProcess, LPDEBUG_EVENT EventData)
{
	if ((EventData != 0) && (EventData->dwDebugEventCode == UNLOAD_DLL_DEBUG_EVENT))
	{
		auto LocateMe = LoadedDlls.find((DWORD64)EventData->u.UnloadDll.lpBaseOfDll);
		if (LocateMe != LoadedDlls.end())
		{
			if (LocateMe._Ptr->_Right->_Myval.second != 0)
			{
				SymUnloadModule64(MainDebuggedProcess, (DWORD64)LocateMe->second->BaseOfImage);
				free(LocateMe._Ptr->_Right->_Myval.second);
			}
		}
		LoadedDlls.erase(EventData->dwProcessId);
	}
	return false;
}

bool InsightSupport_SymbolHandle::GetFailBit()
{
	return FailBit;
}

bool InsightSupport_SymbolHandle::UnloadProcess(HANDLE MainDebuggedProcess, LPDEBUG_EVENT EventData)
{
	bool ret = false;
	if (EventData == nullptr)
	{
		return ret;
	}
	if ((EventData->dwDebugEventCode != EXIT_PROCESS_DEBUG_EVENT) )
	{
		return ret;
	}
	else
	{
		if (EventData->dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
		{
			// loop thru and cleanip each dll
			for (auto stepper = this->LoadedDlls.begin(); stepper != this->LoadedDlls.end(); stepper++)
			{
				if (stepper != LoadedDlls.end())
				{
					if (stepper->second != nullptr)
					{
						SymUnloadModule64(MainDebuggedProcess, (DWORD64)stepper->second->BaseOfImage);
						free(stepper->second);
					}
				}
				this->LoadedDlls.erase(EventData->dwProcessId);
			}
			/// <summary>
			/// dont forget the exe
			/// </summary>
			if (this->SymbolData.BaseOfImage != 0)
			{
				SymUnloadModule64(MainDebuggedProcess, (DWORD64)this->SymbolData.BaseOfImage);
			}
		}
	}
	return ret;
}
