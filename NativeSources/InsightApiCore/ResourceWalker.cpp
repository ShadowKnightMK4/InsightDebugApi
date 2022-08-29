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


	helpme->Results.insert( helpme->Results.end(),  )
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
	this->TargetExe = Target;
	DllList = 0;
	Exclusive = false;
	ModuleCount = 0;

	
}

ResourceWalker::ResourceWalker(HANDLE Target, BOOL ExclusiveControl)
{
	this->TargetExe = Target;
	DllList = 0;
	Exclusive = ExclusiveControl;
	ModuleCount = 0;
}



ResourceWalker::ResourceWalker(LPCWSTR Target, DWORD Access, DWORD SharePermssions)
{
	this->TargetExe = CreateFile(Target, Access, SharePermssions, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (TargetExe != 0)
	{
		this->MainExe = LoadLibraryExW(Target, 0, LOAD_LIBRARY_AS_DATAFILE);
		LoadModules();
	}
}

ResourceWalker::~ResourceWalker()
{
	if (this->MainExe != 0)
	{
		FreeLibrary(MainExe);
	}


	if (DllList != 0) 
	{
		free(DllList);
	}

	if (Exclusive)
	{
		CloseHandle(TargetExe);
	}
	
}

BOOL ResourceWalker::EnumerateResourceNamesEx(LPCWSTR lpType, ENUMRESNAMEPROCW Callback, LONG_PTR CustomArg, DWORD dwFlags, LANGID LangId)
{
	LoadModules();
	BOOL AtLeaseOneFail = FALSE;
	for (DWORD step = 0; step < this->ModuleCount; step++)
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
	LoadModules();
	BOOL AtLeaseOneFail = FALSE;
	for (DWORD step = 0; step < this->ModuleCount; step++)
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
	for (DWORD step = 0; step < this->ModuleCount; step++)
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

/// @brief lead 
void ResourceWalker::LoadModules()
{
	if (LockStatus)
	{
		EnterCriticalSection(&Crit);
	}
	HMODULE* Group = DllList;
	DWORD Needed = 0;


	while (1)
	{
		if (EnumProcessModulesEx(this->MainExe, Group, sizeof(HMODULE) * this->ModuleCount, &Needed, LIST_MODULES_ALL) == FALSE)
		{
			if (Needed > sizeof(HMODULE))
			{
				free(Group);
				Group = (HMODULE*)malloc(Needed);
				ModuleCount = Needed / sizeof(HMODULE);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	this->DllList = Group;
	if (LockStatus)
	{
		LeaveCriticalSection(&Crit);
	}
}
