#include <Windows.h>
#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include "TelemetrySupport.h"

#include <detours.h>
#include <string>
#include <sstream>

#include "Headers/StringConstants.h"
HMODULE Kernel32;
HMODULE Ntdll;


#define _DEBUG 1
bool DetourTargetRoutines()
{
	LONG detour = 0;
	Kernel32 = LoadLibraryExW(kernel32_StringW, 0, 0);
	Ntdll = LoadLibraryExW(ntdll_StringW, 0, 0);

	if (Ntdll)
	{
		OriginalNtCreateFile = (NtCreateFilePtr)GetProcAddress(Ntdll, NtCreateFile_StringA);
		if (OriginalNtCreateFile == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(FALSE, NtCreateFile_StringW, ntdll_StringW);
#endif
		}
		OriginalNtOpenFile = (NtOpenFilePtr)GetProcAddress(Ntdll, NtOpenFile_StringA);
		if (OriginalNtOpenFile == 0) 
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(FALSE, NtOpenFile_StringW, ntdll_StringW);
#endif
		}
	}
	if (Kernel32 != nullptr)
	{
		OriginalCloseHandle =  (CloseHandlePtr) GetProcAddress(Kernel32, CloseHandle_StringA);
		if (OriginalCloseHandle == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, CloseHandle_StringW, kernel32_StringW);
#endif // _DEBUG
			return false;
		}


		OriginalCreateFileW = (CreateFileWPtr)GetProcAddress(Kernel32, CreateFileW_StringA);
		if (OriginalCreateFileW == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, CreateFileW_StringW, kernel32_StringW);
#endif // _DEBUG
			return false;
		}


		OriginalCreateFileA = (CreateFileAPtr)GetProcAddress(Kernel32, CreateFileA_StringA);
		if (OriginalCreateFileA == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, CreateFileA_StringW, kernel32_StringW);
#endif // _DEBUG
			return false;
		}

		OriginalCreateFileTransactedA = (CreateFileTransactedAPtr)GetProcAddress(Kernel32, CreateFileTransactedA_StringA);

		if (OriginalCreateFileTransactedA == 0)

		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, CreateFileTransactedA_StringW, kernel32_StringW);
#endif // _DEBUG
			return false;
		}

		OriginalCreateFileTransactedW = (CreateFileTransactedWPtr)GetProcAddress(Kernel32, CreateFileTransactedW_StringA);

		if (OriginalCreateFileTransactedW == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, CreateFileTransactedW_StringW, kernel32_StringW);
#endif // _DEBUG
			return false;
		}

		OriginalCreateFile2 = (CreateFile2Ptr)GetProcAddress(Kernel32, CreateFile2_StringA);

		if (OriginalCreateFile2 == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, CreateFile2_StringW, kernel32_StringW);
#endif
			return false;
		}

		OriginalReadFile = (ReadFilePtr)GetProcAddress(Kernel32, ReadFile_StringA);
		if (OriginalReadFile == 0)
		{
#ifdef _DEBUG
			OutputDebugString_GetProcFail(TRUE, ReadFile_StringW, kernel32_StringW);
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

		detour = DetourAttach((PVOID*)&OriginalReadFile, DetouredReadFile);
		if (detour != 0)
		{
#ifdef _DEBUG
			OutputDebugString_ErrorFailedToAttach(detour, L"ReadFile", OriginalReadFile, DetouredReadFile);
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