#include "ResourceWalker.h"
#include <Psapi.h>
typedef BOOL (WINAPI* DotNetNameResourceEnumCallback)(HMODULE hMod, BOOL IsMakeIntResourceType, LPCWSTR lpType, BOOL IsMakeIntResourceName, LPWSTR Name, LONG_PTR CystomArg );


class CallbackHelper
{
public:
	ENUMRESNAMEPROCW Callback;
	LONG_PTR CustomArg;
	ArrayHelper* Ret;
	VOID* DotNotCallbackPtr;
	std::vector<LPCWSTR> Results;
};


BOOL Enumresnameprocw_helper(HMODULE  hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR Args)
{
	CallbackHelper* helpme = (CallbackHelper*)Args;
	DotNetNameResourceEnumCallback* Trigger = (DotNetNameResourceEnumCallback*) helpme->DotNotCallbackPtr;

	
	BOOL Result = (*Trigger)(hModule,IS_INTRESOURCE(lpType), lpType, IS_INTRESOURCE(lpName), lpName, helpme->CustomArg);


	//helpme->Results.insert( helpme->Results.end(),  )
	return Result;
}



/*
* Resource Walker is intended for unmanaged resource browsing, editing, enumation and extraction wrapped in a single
* class.  The Sheath c# dll uses this for the StaticBinary class for its funcationality.
*/

/// <summary>
/// This class is the Unmanaged Resource Browser for apps we're loading and examing.
/// </summary>


ResourceWalker::ResourceWalker(HANDLE Target)
{
	this->TargetExeFileHandle = Target;
	DllList = 0;
	Exclusive = false;
	DllListSize = 0;

	
}

ResourceWalker::ResourceWalker(HANDLE Target, BOOL ExclusiveControl)
{
	this->TargetExeFileHandle = Target;
	DllList = 0;
	Exclusive = ExclusiveControl;
	DllListSize = 0;
}



ResourceWalker::ResourceWalker(LPCWSTR Target, DWORD Access, DWORD SharePermssions)
{

	this->TargetExeFileHandle = CreateFile(Target, Access, SharePermssions, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (TargetExeFileHandle != 0)
	{
		this->TargetExeMainHmodule = LoadLibraryExW(Target, 0, 0);
		this->ProcessName = Target;
		if (MakeSuspendedInstance())
		{
			PopulateModuleList(SuspendMe.hProcess);
		}
	}
}

ResourceWalker::~ResourceWalker()
{
	if (this->TargetExeMainHmodule != 0)
	{
		FreeLibrary(TargetExeMainHmodule);
	}


	if (DllList != 0) 
	{
		free(DllList);
	}

	if (Exclusive)
	{
		CloseHandle(TargetExeFileHandle);
	}
	
}

BOOL ResourceWalker::EnumerateResourceNamesEx(LPCWSTR lpType, ENUMRESNAMEPROCW Callback, LONG_PTR CustomArg, DWORD dwFlags, LANGID LangId)
{
	//LoadModules();
	BOOL AtLeaseOneFail = FALSE;
	return EnumResourceNamesExW(this->TargetExeMainHmodule, lpType, Callback, CustomArg, dwFlags, LangId);
	for (DWORD step = 0; step < this->DllListSize; step++)
	{
		if (!EnumResourceNamesExW(this->DllList[step], lpType, Callback, CustomArg, dwFlags, LangId))
		{
			AtLeaseOneFail = TRUE;
		}
	}
	return AtLeaseOneFail;
}

BOOL ResourceWalker::EnumerateResourceTypesExW(ENUMRESTYPEPROCW Callback, LONG_PTR lParam, DWORD dwFlags, LANGID LangId)
{
	//LoadModules();
	BOOL AtLeaseOneFail = FALSE;
	return EnumResourceTypesExW(this->TargetExeMainHmodule, Callback, lParam, dwFlags, LangId);

	for (DWORD step = 0; step < this->DllListSize; step++)
	{
		if (!EnumResourceTypesExW(this->DllList[step], Callback, lParam, dwFlags, LangId))
		{
			AtLeaseOneFail = TRUE;
		}
	}
	return AtLeaseOneFail;
}

BOOL ResourceWalker::EnumerateResourceLanguagesExW(LPCWSTR lpType, LPCWSTR lpName, ENUMRESLANGPROCW lpEnumFunc, LONG_PTR lParam, DWORD dwFlags, LANGID LangId)
{
	LoadModules();
	BOOL AtLeaseOneFail = FALSE;
	for (DWORD step = 0; step < this->DllListSize; step++)
	{
		if (!EnumResourceLanguagesExW(this->DllList[step], lpType, lpName, lpEnumFunc, lParam, dwFlags, LangId))
		{
			AtLeaseOneFail = TRUE;
		}
	}
	return AtLeaseOneFail;
}

BOOL ResourceWalker::GetThreadLock()
{
	return this->LockStatus;
}

BOOL ResourceWalker::SetThreadLock(BOOL Status)
{
	BOOL ret = this->LockStatus;
	this->LockStatus = Status;
	return ret;
}

BOOL ResourceWalker::MakeSuspendedInstance()
{
	bool result;
	STARTUPINFOW Nothing = { 0 };
	Nothing.cb = sizeof(STARTUPINFOW);
	if (SuspendMe.dwProcessId == 0)
	{
		result = CreateProcessW(ProcessName.c_str(), 0, 0, 0, FALSE, CREATE_SUSPENDED, 0, 0, &Nothing, &this->SuspendMe);
		if (result)
		{
			PopulateModuleList(SuspendMe.hProcess);
		}
		else
		{
			PopulateModuleList(NULL);
		}
	}
	else
	{
		TerminateProcess(SuspendMe.hProcess, 0);
		CloseHandle(SuspendMe.hProcess);
		CloseHandle(SuspendMe.hThread);
		ZeroMemory(&SuspendMe, sizeof(SuspendMe));
		return MakeSuspendedInstance();
	}

	return result;
	

}

bool ResourceWalker::PopulateModuleList(HANDLE ProcessHandle)
{
	DWORD err = 0;
	BOOL result = false;
	HMODULE* List = this->DllList;
	DWORD needed = 0;
	if (List == nullptr)
	{
		List = (HMODULE*)malloc(sizeof(HMODULE) * 10);
		if (List)
		{
			ZeroMemory(List, sizeof(HMODULE) * 10);
		}
		DllListSize = 10;
	}
	else
	{
		List = DllList;
	}
	while (1)
	{
		result = EnumProcessModulesEx(ProcessHandle, List, sizeof(HMODULE) * DllListSize, &needed, 3);
		if (result) 
		{
			if (needed < DllListSize)
			{
				free(List);
				DllListSize *= 2;
				List = (HMODULE*)malloc(sizeof(HMODULE) * DllListSize);
				if (List)
				{
					ZeroMemory(List, sizeof(HMODULE) * DllListSize);
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			err = GetLastError();
			break;
		}
	}
	return result;
}

/// @brief lead 
void ResourceWalker::LoadModules()
{
	return;
	if (LockStatus)
	{
		EnterCriticalSection(&Crit);
	}
	HMODULE* Group = DllList;
	DWORD Needed = 0;

	
	while (1)
	{
		if (Group == 0)
		{
			Group = (HMODULE*)malloc(sizeof(HMODULE) * 10);
			DllListSize = 10;
		}
		if (EnumProcessModulesEx(this->TargetExeMainHmodule,
			Group, 
			sizeof(HMODULE) * this->DllListSize,
			&Needed,
			LIST_MODULES_ALL))
		{
			if (Needed > (sizeof(HMODULE) * DllListSize))
			{
				free(Group);
				Group = (HMODULE*)malloc(Needed);
				DllListSize = Needed / sizeof(HMODULE);
			}
			else
			{
				break;
			}
		}
		else
		{
			
		}
		DWORD err = GetLastError();
	}

	this->DllList = Group;
	if (LockStatus)
	{
		LeaveCriticalSection(&Crit);
	}
}
