#include <Windows.h>
#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include <detours.h>
#include <string>
#include <sstream>
HMODULE Kernel32;
HMODULE Ntdll;
const char* ntdll_string = "ntdll.dll";
const char* kernel32_string = "kernel32.dll";


void OutputDebugString_unabletocommit(DWORD val, const wchar_t* telemetryname)
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
	Ntdll = LoadLibraryA(ntdll_string);

	if (Ntdll)
	{
		OriginalNtQueryInformationThread = (NtQueryInformationThreadPtr)GetProcAddress(Ntdll, "NtQueryThreadInformation");
		if (OriginalNtCreateThreadEx == 0)
		{
			error_getproc(FALSE, L"NtQueryThreadInformation", L"ntdll.dll");
			return false;
		}
		OriginalNtSetInformationThread = (NtSetInformationThreadPtr)GetProcAddress(Ntdll, "NtSetInformationThread");
		if (OriginalNtCreateThreadEx == 0)
		{
			error_getproc(FALSE, L"NtSetInformationThread", L"ntdll.dll");
			return false;
		}
		OriginalNtCreateThreadEx = (NtCreateThreadExPtr)GetProcAddress(Ntdll, "NtCreateThreadEx");
		if (OriginalNtCreateThreadEx == 0)
		{
			error_getproc(FALSE, L"NtCreateThreadEx", L"ntdll.dll");
			return false;
		}
	}

	if (Kernel32)
	{
		OriginalIsDebuggerPresent = (IsDebuggerPresentPtr) GetProcAddress(Kernel32, "IsDebuggerPresent");
		if (OriginalIsDebuggerPresent == 0)
		{
			error_getproc(TRUE, L"IsDebuggerPresent", L"Kernel32.dll");
			return false;
		}

		OriginalCheckRemoteDebuggerPresent = (CheckRemoteDebuggerPresentPtr)GetProcAddress(Kernel32, "CheckRemoteDebuggerPresent");
		if (OriginalCheckRemoteDebuggerPresent == 0)
		{
			error_getproc(TRUE, L"CheckRemoteDebuggerPresent", L"Kernel32.dll");
			return false;
		}
	}

	detour = DetourTransactionBegin();
	if (detour != 0)
	{
		error_detourbeginfail(detour);
		return false;
	}

	detour = DetourUpdateThread(GetCurrentThread());
	if (detour != 0)
	{
		error_detoursettargetthread(0);
		return false;
	}

	if (Kernel32 != 0)
	{
		detour = DetourAttach((PVOID*)&OriginalCheckRemoteDebuggerPresent, DetouredCheckRemoteDebuggerPresent);

		if (detour != 0)
		{
			error_detourattachfail(detour, L"CheckRemoteDebuggerPresent", OriginalCheckRemoteDebuggerPresent, DetouredCheckRemoteDebuggerPresent);
			return false;
		}
		detour = DetourAttach((PVOID*)&OriginalIsDebuggerPresent, DetouredIsDebuggerPresent);

		if (detour != 0)
		{
			error_detourattachfail(detour, L"IsDebuggerPresent", OriginalIsDebuggerPresent, DetouredIsDebuggerPresent);
			return false;
		}
	}

	if (Ntdll != 0)
	{
		if (OriginalNtCreateThreadEx != 0)
		{
			detour = DetourAttach((PVOID*)&OriginalNtCreateThreadEx, DetouredNtCreateThreadEx);
			if (detour != 0)
			{
				error_detourattachfail(detour, L"NtCreateThreadEx", OriginalNtCreateThreadEx, DetouredNtCreateThreadEx);
			}
		}

		if (OriginalNtQueryInformationThread != 0)
		{
			if (detour != 0)
			{
				error_detourattachfail(detour, L"NtQueryInformationThread", OriginalNtQueryInformationThread, DetouredNtQueryInformationThread);
			}
		}

		if (OriginalNtSetInformationThread != 0)
		{
			if (detour != 0)
			{
				error_detourattachfail(detour, L"NtSetInformationThread", OriginalNtSetInformationThread, DetouredNtSetInformationThread);
			}
		}
	}

	detour = DetourTransactionCommit();
	if (detour != 0)
	{
		OutputDebugString_unabletocommit(detour, L"AntiDebuggingTelemetry");
	}
	return (detour == 0);

}