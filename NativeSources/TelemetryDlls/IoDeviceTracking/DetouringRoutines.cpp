#include <Windows.h>
#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include "TelemetrySupport.h"

#include <detours.h>
#include <string>
#include <sstream>
HMODULE Kernel32;
HMODULE Ntdll;
const char* ntdll_string = "ntdll.dll";
const char* kernel32_string = "kernel32.dll";

const char* NtCreateFile_string = "NtCreateFile";
const char* NtOpenFile_string = "NtOpenFile";
const char* NtCloseHandle_string = "NtCloseHandle";
const char* CloseHandle_string = "CloseHandle";
const char* CreateFileA_string = "CreateFileA";
const char* CreateFileW_string = "CreateFileW";
const char* CreateFile2_string = "CreateFile2";
const char* CreateFileTransactedA_string = "CreateFileTransactedA";
const char* CreateFileTransactedW_string = "CreateFileTransactedW";


bool DetourTargetRoutines()
{
	LONG detour = 0;

	Kernel32 = LoadLibraryA(kernel32_string);
	Ntdll = LoadLibraryA(ntdll_string);

	if (Ntdll)
	{
		OriginalNtCreateFile = (NtCreateFilePtr)GetProcAddress(Ntdll, NtCreateFile_string);
		if (OriginalNtCreateFile == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(FALSE, L"NtCreateFile", L"ntdll.dll");
#endif
		}
		OriginalNtOpenFile = (NtOpenFilePtr)GetProcAddress(Ntdll, NtOpenFile_string);
		if (OriginalNtOpenFile == 0) 
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(FALSE, L"NtOpenFile", L"ntdll.dll");
#endif
		}
	}
	if (Kernel32 != nullptr)
	{
		OriginalCloseHandle =  (CloseHandlePtr) GetProcAddress(Kernel32, CloseHandle_string);
		if (OriginalCloseHandle == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, L"CloseHandle", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}


		OriginalCreateFileW = (CreateFileWPtr)GetProcAddress(Kernel32, CreateFileW_string);
		if (OriginalCreateFileW == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, L"CreateFileW", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}


		OriginalCreateFileA = (CreateFileAPtr)GetProcAddress(Kernel32, CreateFileA_string);
		if (OriginalCreateFileA == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, L"CreateFileA", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}

		OriginalCreateFileTransactedA = (CreateFileTransactedAPtr)GetProcAddress(Kernel32, CreateFileTransactedA_string);

		if (OriginalCreateFileTransactedA == 0)

		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, L"CreateFileTransactedW", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}

		OriginalCreateFileTransactedW = (CreateFileTransactedWPtr)GetProcAddress(Kernel32, CreateFileTransactedW_string);

		if (OriginalCreateFileTransactedW == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, L"CreateFileTransactedW", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}

		OriginalCreateFile2 = (CreateFile2Ptr)GetProcAddress(Kernel32, CreateFile2_string);

		if (OriginalCreateFile2 == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, L"CreateFile2", L"kernel32.dll");
#endif
			return false;
		}

		detour = DetourTransactionBegin();
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_DetourTransBeginFail(detour);
#endif
			return false;
		}
		detour = DetourUpdateThread(GetCurrentThread());
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_DetourTargetThreadFail(detour);
#endif
			return false;
		}

		if (OriginalNtCreateFile != 0)
		{
			detour = DetourAttach((PVOID*)&OriginalNtCreateFile, DetouredNtCreateFile);
#ifdef _DEBUG
			if (detour != 0)
			{
				OutputDebugString_ErrorFailedToAttach(detour, L"NtCreateFile", OriginalCreateFileA, DetouredCreateFileA);
			}
			else
			{
				OutputDebugString(L"NtCreateFile Detoured!");
			}
#endif
		}
		else
		{
#ifdef _DEBUG
			OutputDebugString(L"Failed to grab pointer to NtCreateFile. Can't Detour");
#endif
		}

		if (OriginalNtOpenFile != 0)
		{
			detour = DetourAttach((PVOID*)&OriginalNtOpenFile, DetouredNtOpenFile);
#ifdef _DEBUG
			if (detour != 0)
			{
				OutputDebugString_ErrorFailedToAttach(detour, L"NtOpenFile", OriginalNtOpenFile, DetouredCreateFileA);
			}
			else
			{
				OutputDebugString(L"NtOpenFile Detoured!");
			}
#endif
		}
		else
		{
#ifdef _DEBUG
			OutputDebugString(L"Failed to grab pointer to NtOpen. Can't Detour");
#endif
		}

		detour = DetourAttach((PVOID*)&OriginalCreateFileA, DetouredCreateFileA);
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_ErrorFailedToAttach(detour, L"CreateFileA", OriginalCreateFileA, DetouredCreateFileA);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFileW, DetouredCreateFileW);
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_ErrorFailedToAttach(detour, L"CreateFileW", OriginalCreateFileW, DetouredCreateFileW);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFileTransactedA, DetouredCreateFileTransactedA);
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_ErrorFailedToAttach(detour, L"CreateFileTransactedA", CreateFileTransactedA, DetouredCreateFileTransactedA);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFileTransactedW, DetouredCreateFileTransactedW);
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_ErrorFailedToAttach(detour, L"CreateFileTransactedW", CreateFileTransactedW, DetouredCreateFileTransactedW);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFile2, DetouredCreateFile2);
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_ErrorFailedToAttach(detour, L"CreateFile2", CreateFile2, DetouredCreateFile2);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCloseHandle, DetouredCloseHandle);
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_ErrorFailedToAttach(detour, L"CloseHandle", OriginalCloseHandle, DetouredCloseHandle);
#endif
			return false;
		}
		detour = DetourTransactionCommit();
		if (detour != 0)
		{
			OutputDebugString_DetourCantCommit(detour, L"IoDeviceTrackingTelemetry");
		}
		return (detour == 0);
	}
	return false;
}