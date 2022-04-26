#include <Windows.h>
#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include <detours.h>
#include <string>
#include <sstream>
HMODULE Kernel32;

const char* kernel32_string = "kernel32.dll";
const char* CloseHandle_string = "CloseHandle";
const char* CreateFileA_string = "CreateFileA";
const char* CreateFileW_string = "CreateFileW";
const char* CreateFile2_string = "CreateFile2";
const char* CreateFileTransactedA_string = "CreateFileTransactedA";
const char* CreateFileTransactedW_string = "CreateFileTransactedW";
void error_unabletocommit_justdating(DWORD val, const wchar_t* telemetryname)
{
	std::wstringstream output;
#ifdef _DEBUG
	
	output << "DetourTransactionCommit() failed with code " << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << "for the detour telemetry dll named " << telemetryname << std::endl;
#else
	output << "Unable to apply detours to relevant routines from the telemetry dll named " << telemetryname << std::endl;
#endif
	
}
void error_detourattachfail(DWORD val, const wchar_t* routine_name, LPVOID Ptr, LPVOID Replacement)
{
#ifdef _DEBUG
	std::wstringstream output;
	output << "DetourAttach() Failed with error code" << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << L" for routine named " << routine_name << L" at local memory" << std::hex << "0x" << Ptr << std::dec;
	OutputDebugString(output.str().c_str());
#endif
}
void error_detourbeginfail(DWORD val)
{
#ifdef _DEBUG
	std::wstringstream output;
	DWORD LastError = GetLastError();
	output << "DetourTransactionBegin() failed with error code " << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << std::endl;
	OutputDebugString(output.str().c_str());
#endif
}

void error_detoursettargetthread(DWORD val)
{
#ifdef _DEBUG
	std::wstringstream output;
	output << "DetourSetTargetTHread() failed with error code " << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << std::endl;
	OutputDebugString(output.str().c_str());
#endif

}
void error_getproc(BOOL IsFatal, const wchar_t* routine, const wchar_t* dll)
{
#ifdef _DEBUG
	std::wstringstream Output;
	DWORD LastError = GetLastError();
	if (IsFatal)
	{
		Output << L"Fatal:  ";
	}
	else
	{
		Output << L"Warning ";
	}
	Output << L"Unable to load " << routine << L"from dll " << dll << ". GetProcAddress Failed with error " << std::hex << L"\"" << LastError << L"\"" << std::dec << L"\"" << LastError << L"\"" << std::endl;


	if (!IsFatal)
	{
		Output << L"This may cause issues in proceeding with loading the telemetry ok" << std::endl;
	}
	else
	{
		Output << L"This will prevent the target from loading the telemetry ok." << std::endl;
	}


	OutputDebugString(Output.str().c_str());
#endif
}
bool DetourTargetRoutines()
{
	LONG detour = 0;

	Kernel32 = LoadLibraryA(kernel32_string);
	if (Kernel32 != nullptr)
	{
		OriginalCloseHandle =  (CloseHandlePtr) GetProcAddress(Kernel32, CloseHandle_string);
		if (OriginalCloseHandle == 0)
		{
#ifdef _DEBUG
			error_getproc(TRUE, L"CloseHandle", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}


		OriginalCreateFileW = (CreateFileWPtr)GetProcAddress(Kernel32, CreateFileW_string);
		if (OriginalCreateFileW == 0)
		{
#ifdef _DEBUG
			error_getproc(TRUE, L"CreateFileW", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}


		OriginalCreateFileA = (CreateFileAPtr)GetProcAddress(Kernel32, CreateFileA_string);
		if (OriginalCreateFileA == 0)
		{
#ifdef _DEBUG
			error_getproc(TRUE, L"CreateFileA", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}

		OriginalCreateFileTransactedA = (CreateFileTransactedAPtr)GetProcAddress(Kernel32, CreateFileTransactedA_string);

		if (OriginalCreateFileTransactedA == 0)

		{
#ifdef _DEBUG
			error_getproc(TRUE, L"CreateFileTransactedW", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}

		OriginalCreateFileTransactedW = (CreateFileTransactedWPtr)GetProcAddress(Kernel32, CreateFileTransactedW_string);

		if (OriginalCreateFileTransactedW == 0)
		{
#ifdef _DEBUG
			error_getproc(TRUE, L"CreateFileTransactedW", L"kernel32.dll");
#endif // _DEBUG
			return false;
		}

		OriginalCreateFile2 = (CreateFile2Ptr)GetProcAddress(Kernel32, CreateFile2_string);

		if (OriginalCreateFile2 == 0)
		{
#ifdef _DEBUG
			error_getproc(TRUE, L"CreateFile2", L"kernel32.dll");
#endif
			return false;
		}

		detour = DetourTransactionBegin();
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detourbeginfail(detour);
#endif
			return false;
		}
		detour = DetourUpdateThread(GetCurrentThread());
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detoursettargetthread(detour);
#endif
			return false;
		}

		detour = DetourAttach((PVOID*)&OriginalCreateFileA, DetouredCreateFileA);
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detourattachfail(detour, L"CreateFileA", OriginalCreateFileA, DetouredCreateFileA);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFileW, DetouredCreateFileW);
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detourattachfail(detour, L"CreateFileW", OriginalCreateFileW, DetouredCreateFileW);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFileTransactedA, DetouredCreateFileTransactedA);
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detourattachfail(detour, L"CreateFileTransactedA", CreateFileTransactedA, DetouredCreateFileTransactedA);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFileTransactedW, DetouredCreateFileTransactedW);
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detourattachfail(detour, L"CreateFileTransactedW", CreateFileTransactedW, DetouredCreateFileTransactedW);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCreateFile2, DetouredCreateFile2);
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detourattachfail(detour, L"CreateFile2", CreateFile2, DetouredCreateFile2);
#endif
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalCloseHandle, DetouredCloseHandle);
		if (detour != 0)
		{
#ifdef _DEBUG
			error_detourattachfail(detour, L"CloseHandle", OriginalCloseHandle, DetouredCloseHandle);
#endif
			return false;
		}
		detour = DetourTransactionCommit();
		if (detour != 0)
		{
			error_unabletocommit_justdating(detour, L"IoDeviceTrackingTelemetry");
		}
		return (detour == 0);
	}
	return false;
}