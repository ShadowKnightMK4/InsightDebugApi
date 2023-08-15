#include <Windows.h>
#include "TelemetrySupport.h"
#include <sstream>

/*
* Telemetry Support is a generic library intended for laying some groundwork to assist debugging. 
* 
* There are several canned OutputDebugString message prototypes related to detours messages and 
* we use C++'s Polymorphism() to add some additional OutputDebugString() routines to the namespace
* that take advantage of the OutputDebugString and C++'s iostream
*/

using namespace std;

char* UnicodeToAnsi(const wchar_t* unc)
{
	if (unc == nullptr)
	{
		return nullptr;
	}
	DWORD StringSize = WideCharToMultiByte(CP_ACP, 0, unc, -1, 0, 0, 0, 0);
	char* AnsiString = nullptr;
	AnsiString = new (std::nothrow) char[StringSize + (size_t)1];
	if (AnsiString != nullptr)
	{
		StringSize = WideCharToMultiByte(CP_ACP, 0, unc, -1, AnsiString, StringSize + 1, 0, 0);
		if (StringSize != 0)
		{
			AnsiString[StringSize] = 0;
		}
		else
		{
			delete[] AnsiString;
			return nullptr;
		}
	}
	return AnsiString;

}
wchar_t* AnsiToUnicode(const char* ansi)
{
	if (ansi == nullptr)
	{
		return nullptr;
	}

	DWORD SizeString = MultiByteToWideChar(CP_ACP, 0, ansi, -1, 0, 0);
	wchar_t* UnicodeString = nullptr;



	UnicodeString = new (std::nothrow) wchar_t[SizeString + (size_t)1];
	if (UnicodeString != nullptr)
	{
		
		SizeString = MultiByteToWideChar(CP_ACP, 0, ansi, -1, UnicodeString, SizeString);
		if (SizeString != 0)
		{
			UnicodeString[SizeString] = 0;
		}
		else
		{
			delete[] UnicodeString;
			return nullptr;
		}
	}
	return UnicodeString;

}

void OutputDebugString_DetourCantCommit(DWORD val, const wchar_t* telemetryname)
{
	std::wstringstream output;
#ifdef _DEBUG

	output << "DetourTransactionCommit() failed with code " << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << "for the detour telemetry dll named " << telemetryname << std::endl;
#else
	output << "Unable to apply detours to relevant routines from the telemetry dll named " << telemetryname << std::endl;
#endif
	OutputDebugString(output.str().c_str());
}
void OutputDebugString_ErrorFailedToAttach(DWORD val, const wchar_t* routine_name, LPVOID Ptr, LPVOID Replacement)
{
#ifdef _DEBUG
	std::wstringstream output;
	output << "DetourAttach() Failed with error code" << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << L" for routine named " << routine_name << L" at local memory" << std::hex << "0x" << Ptr << std::dec;
	OutputDebugString(output.str().c_str());
#endif
}
void OutputDebugString_DetourTransBeginFail(DWORD val)
{
#ifdef _DEBUG
	std::wstringstream output;
	DWORD LastError = GetLastError();
	output << "DetourTransactionBegin() failed with error code " << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << std::endl;
	OutputDebugString(output.str().c_str());
#endif
}

void OutputDebugString_DetourTargetThreadFail(DWORD val)
{
#ifdef _DEBUG
	std::wstringstream output;
	output << "DetourSetTargetTHread() failed with error code " << std::hex << L"(" << val << L")" << std::dec << L"(" << val << L")" << std::endl;
	OutputDebugString(output.str().c_str());
#endif

}
void OutputDebugString_GetProcFail(BOOL IsFatal, const char* routine, const wchar_t* dll)
{
#ifdef _DEBUG
	const wchar_t* buf = AnsiToUnicode(routine);
	__try
	{
		OutputDebugString_GetProcFail(IsFatal, buf, dll);

	}
	__finally
	{
		if (buf != 0)
		{
			delete[] buf;
		}
	}

#endif
}

void OutputDebugString_GetProcFail(BOOL IsFatal, const wchar_t* routine, const wchar_t* dll)
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



void OutputDebugString(unsigned long long P)
{
	std::wstringstream tmp;
	tmp << P;
	OutputDebugString(tmp.str().c_str());
}
void OutputDebugString(VOID* D)
{
	OutputDebugString(D, TRUE);
}

void OutputDebugString(VOID* D, bool HexOrDec)
{
	std::wstringstream tmp;
	if (HexOrDec)
	{
		tmp << hex;
	}
	else
	{
		tmp << dec;
	}
	tmp << D;
	OutputDebugString(tmp.str().c_str());
}
