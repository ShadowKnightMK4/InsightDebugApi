
#include "FileSandBoxHelperDll.h"
#include "DetourFunctionStuff.h"
#include "ReplacementFileFileFunctions.h"
HMODULE NTDLL = 0;


bool DetourNtFileRoutines()
{
	LONG fallback;
	if (NTDLL == 0)
	{
		NTDLL = LoadLibraryW(L"NTDLL.DLL");
	}
	if (NTDLL == 0)
	{
		
		return FALSE;
	}
	else
	{
		NtCreateFileOriginal = (NtCreateFilePtr)GetProcAddress(NTDLL, "NtCreateFile");
		NtCreateUserProcessOriginal = (NtCreateUserProcessPtr)GetProcAddress(NTDLL, "NtCreateUserProcess");
		
		fallback = DetourTransactionBegin();
		if (fallback != 0)
		{
			OutputDebugString(L"Failed At Detours Begin()");
			return FALSE;
		}
		if (NtCreateFileOriginal == 0)
		{
			OutputDebugString(L"Unable to locate ntdll's NtCreateFile. File logging ability reduced");
		}
		else
		{
			fallback = DetourAttach((PVOID*)&NtCreateFileOriginal, NtCreateFileDetoured);
			if (fallback != 0)
			{
				OutputDebugString(L"Unable to detour NtCreateFile()");
				DetourTransactionAbort();
				return FALSE;
			}
		}

		if (NtCreateUserProcessOriginal != 0)
		{
			fallback = DetourAttach((PVOID*)&NtCreateUserProcessOriginal, NtCreateUserProcesDetoured);
			if (fallback != 0)
			{
				OutputDebugString(L"Unable to detour NtCreateCreateUserProcess()");
				DetourTransactionAbort();
				return FALSE;
			}
		}
		//else
		{
			//OutputDebugString(L"Unable to locate ntdll's NtCreateUserProcess. Ability to propergate Filesandboxhelper.dll to spawned processes is not usable.");
		}
		if (DetourTransactionCommit() != 0)
		{
			DetourTransactionAbort();
			return FALSE;
		}

		return true;
	}

}
