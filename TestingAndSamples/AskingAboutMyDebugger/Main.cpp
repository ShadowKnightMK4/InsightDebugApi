/* AskingAboutMyDebugger runs thru the AntiDebuggerTelemetry Detoured Routines*/

#include <Windows.h>
#include <winternl.h>
#include <iostream>
#include <process.h>
typedef NTSTATUS(WINAPI* NtSetInformationThreadPtr)(HANDLE, THREADINFOCLASS, PVOID, ULONG);
typedef NTSTATUS(WINAPI* NtQueryInformationThreadPtr)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG);
using namespace std;


#define ThreadHideFromDebugger (0x11)
unsigned WINAPI TestThread(void*)
{
	ULONG Used = 0;
	BYTE large[5000] = { 0 };
	cout << "Now Entering the subthread  ";


	auto HMOD = LoadLibrary(L"NTDLL.DLL");
	NtSetInformationThreadPtr SetMe = (NtSetInformationThreadPtr)GetProcAddress(HMOD, "NtSetInformationThread");
	NtQueryInformationThreadPtr AskMe = (NtQueryInformationThreadPtr)GetProcAddress(HMOD, "NtQueryInformationThread");

	

	if (AskMe(GetCurrentThread(), (THREADINFOCLASS)ThreadHideFromDebugger, &large[0], 5000, &Used) != CMC_STATUS_SUCCESS)
	{
		cout << "Failure!" << endl;
	}
	else
	{
		Used = Used;
		cout << "OK!" << endl;
	}

	auto ret = SetMe(GetCurrentThread(), (THREADINFOCLASS)ThreadHideFromDebugger, 0, 0);

	
	int* NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO = 0;



	int a, b;
	a = *NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO;
	b = 0;
	cout << a / b;

	if (AskMe(GetCurrentThread(), (THREADINFOCLASS)ThreadHideFromDebugger, &large[0], 5000, &Used) != CMC_STATUS_SUCCESS)
	{
		cout << "Failure!" << endl;
	}
	else
	{
		Used = Used;
		cout << "OK!" << endl;
	}


	_endthreadex(0);
	return 0;
}
int main()
{
	HANDLE self;
	BOOL Output = false;
	cout << "IsDebuggerPresent() returns " << IsDebuggerPresent() << endl;

	if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &Output))
	{
		cout << "CheckRemoteProcess(GetCurrentProcess, out) sets the output bool too " << Output;
	}
	else
	{
		cout << "CheckRemoteProcess(GetCurrentProcess, out) failed with last error " << GetLastError();
	}

	self = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	if (CheckRemoteDebuggerPresent(self, &Output))
	{
		cout << "CheckRemoteProcess(Self, out) sets the output bool too " << Output;
	}
	else
	{
		cout << "CheckRemoteProcess(Self, out) failed with last error " << GetLastError();
	}

	HANDLE other = (HANDLE)_beginthreadex(0, 0, TestThread, 0, 0, 0);
	if (other != 0)
	{
		WaitForSingleObject(other, INFINITE);
	}
	CloseHandle(self);
	return 0;
} 