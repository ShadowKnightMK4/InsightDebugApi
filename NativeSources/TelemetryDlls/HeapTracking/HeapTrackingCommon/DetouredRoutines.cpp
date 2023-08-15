#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include "TelemetrySupport.h"
#include <winternl.h>

using namespace std;

/*
* HeapTrack32/64 DetouredRoutine Common
* 
* 
* Plan is to eventually let debugger/sheath deal with being man in the middle
* to handle memory allocations, track them and let the debugger inspect.
*/

/* We throw exception with this value. As long as debugger knows this value
and what the arguments contain, the value matters not. Do note that bit 28 will be cleared by Windows when thrown if 
you happen to change this to another value (for example another Telemetry Project.
*/

/*
* ULONG_PTR
*/

						   
#define EXCEPTION_VALUE (0x60ACB7A9)

// DEBUGGING reading exceptions from WOW into x64
//#define EXCEPTION_VALUE (2)



/*
What this does:
We detour the chosen routines,
We pass the arguments.
We pass a pointer to something the debugger can change to modify the functionality of the return value.


On the Insight Sheath Side

Add code to InsightSheath.Telemetry.IoDeviceExceptionReader to read (and write) exceptions
*/

/*
* To make this a little easier to interpret. Let ARGS be the array we present
* to RaiseException(). This comment also written to be a bit of an intro to what this DLL does.
* 
* 
* What we generally do is pack each argument (or a pointer to it if that's the output) plus 
* a last error pointer into a ULONG_PTR array and use RaiseException() to pass this to a debugger.
* The detoured routines then check to see if last error or the Handle value have been overwritten and
* return that instead.
* 
* A couple of key points:
* You're going to want to keep the constants between these defines synched with the code that interpets them.
* between this and your code that extracts the data to present to the debugger user code.
* 
* Pointers presented to your debugger (EXCEPTION_LAST_ERROR and CT_AW_OVEERRIDE_HANDLE for example)
* will be valid in the context of the process that your debugger received the exception from.
* 
* You're going to need to interpret the data and eventually call ReadProcessMemory() to get at the data
* they're pointing too.
* 
* WriteProcessMemory() can be used to write to the target.
* 
* To Get a process handle with correct access
* 
* ReadProcessMemory() -> use OpenProcess with (PROCESS_VM_READ (and maybe) PROCESS_VM_OPERATION),
* WriteProcessMemory() -> use OpenProcess with (PROCESS_VM_WRITE (and maybe) PROCESS_VM_OPERATION)
* 
* To duplicate a handle into your debugged process.
* DuplicateHandle()	-> use OpenProcess with (PROCESS_DUP_HANDLE)
* 
* You do not need to keep access requests isolated. For example
* PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION|PROCESS_DUP_HANDLE.
* 
* Duplicating the INVALID_HANDLE_VALUE into remote process
does *NOT* need PROCESS_DUP_HANDLE access.


IMPORTANT!!!!
You will need to know how big of a handle the debugged process hosting this DLL expects.

HANDLES are VOID* types and are either 8 bytes for x64 or 4 bytes for x86.

x64 INVALID_HANDLE_VALUE -> 0xffffffffffffffff
x86 INVALID_HANDLE_VALUE -> 0xffffffff

Once the correct handle size is known, use WriteProcessMemory into the 
debugged process to set the handle to that process's INVALID_HANDLE_VALUE.
Finally, you're likely going to want to set last error to something other than
0 (for example ACCESS_DENIED resolves to '5') to give process a reason why the file d
could not be accessed.



For fun:
As an exercise for the reader, add a wrapper in your debugger to handling 
redirecting access to a ZIP file if the path is correct. For example:

Debugger spawns Process with IODEVICETRACKINGNN.DLL

Process accesses
	%cd%\Coolthing.dat	
	Debugger receives exception, checks against a container zip file.
	Debugger extras %cd%\Coolthing.dat to temp file and returns handle to that instead
Process carries on as if it is using %cd%\coolthing.dat


*/

/*
Common to all the routines detoured by this.
*/

// Args[EXCEPTION_ARG_TYPE] contains a value indicating what routine we're passing. Each routine on the .NET Telemetry Side has a specific struct/ handler for it.
#define EXCEPTION_ARG_TYPE 0
// Args[EXCEPTION_LAST_ERROR] is context dependant.  For routines detoured 
//that setlasterror, this is a pointer to a 4 byte side that lets a 
//debugger specify the last error value to set. If the routine does not set 
//last error, it's going to be the return value.
#define EXCEPTION_LAST_ERROR 1

DWORD DetouredGetProcessHeaps(DWORD NumberOfHeaps, PHANDLE ProcessHeaps) 
{
	return 0;
}

LPVOID DetouredHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) 
{
	return NULL;
}

SIZE_T DetouredHeapCompact(HANDLE hHeap, DWORD dwFlags) 
{
	return 0;
}

HANDLE DetouredHeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) 
{
	return NULL; 
}

BOOL DetouredHeapDestroy(HANDLE hHeap)
{
	return FALSE;
}

BOOL DetouredHeapLock(HANDLE hHeap) 
{
	return FALSE; 
}

BOOL DetouredHeapQueryInformation(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass,
	PVOID HeapInformation, SIZE_T HeapInformationLength, PSIZE_T ReturnLength) 
{
	return FALSE;
}

LPVOID DetouredHeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes)
{
	return NULL;
}

BOOL DetouredHeapSetInformation(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass,
	PVOID HeapInformation, SIZE_T HeapInformationLength)
{
	return FALSE;
}

SIZE_T DetouredHeapSize(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem)
{
	return 0;
}

BOOL DetouredHeapUnlock(HANDLE hHeap) 
{
	return FALSE;
}

BOOL DetouredHeapValidate(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem) 
{
	return FALSE;
}

BOOL DetouredHeapWalk(HANDLE hHeap, LPPROCESS_HEAP_ENTRY lpEntry) 
{
	return FALSE; 
}
