#include <Windows.h>
#include <iostream>
#include <winternl.h>
#include <Psapi.h>


/*
* ObjectAttributesOffset.cpp
* 
* What this does: It outputs to stdout the relative offsets in  a struct and the specific ones.
* How to use:  Pick either x86 or x64 for your target and see if the offsets specificed by the general structure (example UNICODE_STRING) matches
* the same as the specific struct (UNICODE_STRING32 for x86 and UNICODE_STRING64 for x64).
* 
* What does it mean:  If the specific struct version matches the generic struct that you compiled for then when debugging that target, that's the struct
* to assme said target is using.
* 
* Scope:	Change struct and output strings as neeed.
* 
* Stuff used:  cout,  offsetof(),  sizeof()
*/
/*
sizeof(PTR) = 8
sizeof(UNICODE_STRING) =16
sizeof(UNICODE_STRING32) =8
sizeof(UNICODE_STRING64) =16

UNICODE_STRING.Length offset = 0
UNICODE_STRING.Max offset = 2
UNICODE_STRING.Buffer offset = 8

UNICODE_STRING32.Length offset = 0
UNICODE_STRING32.Max offset = 2
UNICODE_STRING32.Buffer offset = 4


UNICODE_STRING64.Length offset = 0
UNICODE_STRING64.Max offset = 2
UNICODE_STRING64.Buffer offset = 8




sizeof(PTR) = 4
sizeof(UNICODE_STRING) =8
sizeof(UNICODE_STRING32) =8
sizeof(UNICODE_STRING64) =16

UNICODE_STRING.Length offset = 0
UNICODE_STRING.Max offset = 2
UNICODE_STRING.Buffer offset = 4

UNICODE_STRING32.Length offset = 0
UNICODE_STRING32.Max offset = 2
UNICODE_STRING32.Buffer offset = 4

UNICODE_STRING64.Length offset = 0
UNICODE_STRING64.Max offset = 2
UNICODE_STRING64.Buffer offset = 8
*/
using namespace std;

typedef struct _PSAPI_WORKING_SET_INFORMATION32 {
	DWORD               NumberOfEntries;
	PSAPI_WORKING_SET_BLOCK WorkingSetInfo[1];
} PSAPI_WORKING_SET_INFORMATION32, * PPSAPI_WORKING_SET_INFORMATION32;

typedef struct _PSAPI_WORKING_SET_INFORMATION64 {
	__int64               NumberOfEntries;
	PSAPI_WORKING_SET_BLOCK WorkingSetInfo[1];
} PSAPI_WORKING_SET_INFORMATION64, * PPSAPI_WORKING_SET_INFORMATION64;

typedef struct _PROCESS_MEMORY_COUNTERS_EX32 {
	DWORD cb;
	DWORD PageFaultCount;
	DWORD PeakWorkingSetSize;
	DWORD WorkingSetSize;
	DWORD QuotaPeakPagedPoolUsage;
	DWORD QuotaPagedPoolUsage;
	DWORD QuotaPeakNonPagedPoolUsage;
	DWORD QuotaNonPagedPoolUsage;
	DWORD PagefileUsage;
	DWORD PeakPagefileUsage;
	DWORD PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX32;


typedef struct _PROCESS_MEMORY_COUNTERS_EX64 {
	DWORD cb;
	DWORD PageFaultCount;
	__int64 PeakWorkingSetSize;
	__int64 WorkingSetSize;
	__int64 QuotaPeakPagedPoolUsage;
	__int64 QuotaPagedPoolUsage;
	__int64 QuotaPeakNonPagedPoolUsage;
	__int64 QuotaNonPagedPoolUsage;
	__int64 PagefileUsage;
	__int64 PeakPagefileUsage;
	__int64 PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX64;



struct UNICODE_STRING32
{
	USHORT Length;
	USHORT MaximumLength;
	UINT Buffer;
};

struct UNICODE_STRING64
{
	USHORT Length;
	USHORT MaximumLength;
	INT Padding; // aliggns buffer ok.
	ULONGLONG Buffer;
};

struct RTL_USER32
{
	BYTE Reserved1[16];
	UINT32 Reserved2[10];
	UNICODE_STRING32 ImagePathName;
	UNICODE_STRING32 CommandLine;
};
struct RTL_USER64
{
	BYTE Reserved1[16];
	UINT64 Reserved2[10];
	UNICODE_STRING64 ImagePathName;
	UNICODE_STRING64 CommandLine;
};
int main()
{
	UNICODE_STRING Buffer;
	PROCESS_MEMORY_COUNTERS_EX Buffer2;
	


	EXCEPTION_NONCONTINUABLE
	
	cout << "Usage:  Know if you're compiling for x64 or x86.  Compare the given offsets for the general struct for example UNICODE_STRING and see if your custom struct - for example UNICODE_STRING32 matches the generic struct. If so then it should work when reading from a debugged process. If not, try again." << endl;
	
	cout << "sizeof(PTR) = " << sizeof(VOID*) << endl;
	cout << "sizeof(SIZE_T) " << sizeof(SIZE_T) << endl;

	

	cout << "BEGIN GENERIC PSAPI_WORKING_SET_INFORMATION" << endl;
	cout << "sizeof(PSAPI_WORKING_SET_INFORMATION)" << sizeof(PSAPI_WORKING_SET_INFORMATION) << endl;
	cout << "offsetof(PSAPI_WORKING_SET_INFORMATION, NumberOfEntries)" << offsetof(PSAPI_WORKING_SET_INFORMATION, NumberOfEntries) << endl;
	cout << "offsetof(PSAPI_WORKING_SET_INFORMATION, WorkingSetInfo)" << offsetof(PSAPI_WORKING_SET_INFORMATION, WorkingSetInfo) << endl;
	cout << endl << endl;

	cout << "BEGIN GENERIC PSAPI_WORKING_SET_INFORMATION32" << endl;
	cout << "sizeof(PSAPI_WORKING_SET_INFORMATION32)" << sizeof(PSAPI_WORKING_SET_INFORMATION32) << endl;
	cout << "offsetof(PSAPI_WORKING_SET_INFORMATION32, NumberOfEntries)" << offsetof(PSAPI_WORKING_SET_INFORMATION32, NumberOfEntries) << endl;
	cout << "offsetof(PSAPI_WORKING_SET_INFORMATION32, WorkingSetInfo)" << offsetof(PSAPI_WORKING_SET_INFORMATION32, WorkingSetInfo) << endl;
	cout << endl << endl;


	cout << "BEGIN GENERIC PSAPI_WORKING_SET_INFORMATION64" << endl;
	cout << "sizeof(PSAPI_WORKING_SET_INFORMATIO64)" << sizeof(PSAPI_WORKING_SET_INFORMATION64) << endl;
	cout << "offsetof(PSAPI_WORKING_SET_INFORMATION64, NumberOfEntries)" << offsetof(PSAPI_WORKING_SET_INFORMATION64, NumberOfEntries) << endl;
	cout << "offsetof(PSAPI_WORKING_SET_INFORMATION64, WorkingSetInfo)" << offsetof(PSAPI_WORKING_SET_INFORMATION64, WorkingSetInfo) << endl;
	cout << endl << endl;



	cout << "BEGIN GENERIC STRUCT: PROCESS_MEMORY_COUNTERS_EX  (cb == " << sizeof(PROCESS_MEMORY_COUNTERS_EX) << ")" << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, cb)" << offsetof(PROCESS_MEMORY_COUNTERS_EX, cb) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, PageFaultCount) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, PageFaultCount) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, PeakWorkingSetSize) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, PeakWorkingSetSize) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, WorkingSetSize) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, WorkingSetSize) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaPeakPagedPoolUsage) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaPeakPagedPoolUsage) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaPagedPoolUsage) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaPagedPoolUsage) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaPeakNonPagedPoolUsage) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaPeakNonPagedPoolUsage) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaNonPagedPoolUsage) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, QuotaNonPagedPoolUsage) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, PagefileUsage) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, PagefileUsage) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, PeakPagefileUsage) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, PeakPagefileUsage) << endl;
	cout << "offsetof(PROCESS_MEMORY_COUNTERS_EX, PrivateUsage) " << offsetof(PROCESS_MEMORY_COUNTERS_EX, PrivateUsage) << endl;
	cout << endl << endl;

	cout << "BEGIN 64-bit _PROCESS_MEMORY_COUNTERS_EX64 (cb == " << sizeof(PROCESS_MEMORY_COUNTERS_EX64) << ")" << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, cb)" << offsetof(PROCESS_MEMORY_COUNTERS_EX64, cb) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PageFaultCount)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PageFaultCount) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PeakWorkingSetSize)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PeakWorkingSetSize) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, WorkingSetSize)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, WorkingSetSize) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaPeakPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaPeakPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaPeakNonPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaPeakNonPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaNonPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, QuotaNonPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PagefileUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PagefileUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PeakPagefileUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PeakPagefileUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PrivateUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX64, PrivateUsage) << endl;
	cout << endl << endl;

	cout << "BEGIN 32-bit _PROCESS_MEMORY_COUNTERS_EX32 (cb == " << sizeof(_PROCESS_MEMORY_COUNTERS_EX32) << ")" << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, cb)" << offsetof(PROCESS_MEMORY_COUNTERS_EX32, cb) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PageFaultCount)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PageFaultCount) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PeakWorkingSetSize)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PeakWorkingSetSize) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, WorkingSetSize)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, WorkingSetSize) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaPeakPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaPeakPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaPeakNonPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaPeakNonPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaNonPagedPoolUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, QuotaNonPagedPoolUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PagefileUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PagefileUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PeakPagefileUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PeakPagefileUsage) << endl;
	cout << "offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PrivateUsage)" << offsetof(_PROCESS_MEMORY_COUNTERS_EX32, PrivateUsage) << endl;
	cout << endl << endl;




	cout << "Offsets for RTL_USER64, RTL_USER32, RTL_USER_PROCESS_PARAMETERS based on <wintintern>" << endl;

	cout << "sizeof(RTL_USER_PROCESS_PARAMETERS) == " << sizeof(RTL_USER_PROCESS_PARAMETERS) << endl;
	cout << "sizeof(RTL_USER_PROCESS_PARAMETERS32) == " << sizeof(RTL_USER32) << endl;
	cout << "sizeof(RTL_USER_PROCESS_PARAMETERS64) == " << sizeof(RTL_USER64) << endl;
	cout << endl << endl;

	cout << "offsetof(RTL_USER_PROCESS_PARAMETERS, Reserved1) " << offsetof(RTL_USER_PROCESS_PARAMETERS, Reserved1) << endl;
	cout << "offsetof(RTL_USER_PROCESS_PARAMETERS, Reserved2) " << offsetof(RTL_USER_PROCESS_PARAMETERS, Reserved2) << endl;
	cout << "offsetof(RTL_USER_PROCESS_PARAMETERS, ImagePathName) " << offsetof(RTL_USER_PROCESS_PARAMETERS, ImagePathName) << endl;
	cout << "offsetof(RTL_USER_PROCESS_PARAMETERS, CommandLine) " << offsetof(RTL_USER_PROCESS_PARAMETERS, CommandLine) << endl;
	cout << endl << endl;

	cout << "offsetof(RTL_USER32, Reserved1) " << offsetof(RTL_USER32, Reserved1) << endl;
	cout << "offsetof(RTL_USER32, Reserved2) " << offsetof(RTL_USER32, Reserved2) << endl;
	cout << "offsetof(RTL_USER32, ImagePathName) " << offsetof(RTL_USER32, ImagePathName) << endl;
	cout << "offsetof(RTL_USER32, CommandLine) " << offsetof(RTL_USER32, CommandLine) << endl;
	cout << endl << endl;
	cout << "offsetof(RTL_USER64, Reserved1) " << offsetof(RTL_USER64, Reserved1) << endl;
	cout << "offsetof(RTL_USER64, Reserved2) " << offsetof(RTL_USER64, Reserved2) << endl;
	cout << "offsetof(RTL_USER64, ImagePathName) " << offsetof(RTL_USER64, ImagePathName) << endl;
	cout << "offsetof(RTL_USER64, CommandLine) " << offsetof(RTL_USER64, CommandLine) << endl;
	cout << endl << endl;

	cout << "Offsets for UNICODE_STRING, UNICODE_STRING32, UNICODE_STRING64" << endl;
	cout << "sizeof(UNICODE_STRING) =" << sizeof(UNICODE_STRING) << endl;
	cout << "sizeof(UNICODE_STRING32) =" << sizeof(UNICODE_STRING32) << endl;
	cout << "sizeof(UNICODE_STRING64) =" << sizeof(UNICODE_STRING64) << endl;
	cout << endl << endl;

	cout << "UNICODE_STRING.Length offset = " << offsetof(UNICODE_STRING, Length) << endl;
	cout << "UNICODE_STRING.Max offset = " << offsetof(UNICODE_STRING, MaximumLength) << endl;
	cout << "UNICODE_STRING.Buffer offset = " << offsetof(UNICODE_STRING, Buffer) << endl;

	cout << endl << endl;
	cout << "UNICODE_STRING32.Length offset = " << offsetof(UNICODE_STRING32, Length) << endl;
	cout << "UNICODE_STRING32.Max offset = " << offsetof(UNICODE_STRING32, MaximumLength) << endl;
	cout << "UNICODE_STRING32.Buffer offset = " << offsetof(UNICODE_STRING32, Buffer) << endl;

	cout << endl << endl;
	
	cout << "UNICODE_STRING64.Length offset = " << offsetof(UNICODE_STRING64, Length) << endl;
	cout << "UNICODE_STRING64.Max offset = " << offsetof(UNICODE_STRING64, MaximumLength) << endl;
	cout << "UNICODE_STRING64.Buffer offset = " << offsetof(UNICODE_STRING64, Buffer) << endl;

	return 0;
}