#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include "TelemetrySupport.h"
#include <winternl.h>

using namespace std;



/* We throw exception with this value. As long as debugger knows this value
and what the arguments contain, the value matters not. Do note that bit 28 will be cleared by Windows when thrown if 
you happen to change this to another value (for example another Telemetry Project.
*/

/*
* ULONG_PTR
*/


//#define EXCEPTION_VALUE (0x68ACB7A9)

// DEBUGGING reading exceptions from WOW into x64
#define EXCEPTION_VALUE (2)




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
// Args[EXCEPTION_LAST_ERROR] is context dependant.  For routines detoured that setlasterror, this is a pointer to a 4 byte side that lets a debugger specify the last error value to set. If the routine does not set last error, it's going to be the return value.
#define EXCEPTION_LAST_ERROR 1

#define LOAD_LIBRARY_EXW_API_CALL (1)

#define LL_FILENAME_PT (2)
#define LL_FILENAME_CHAR_COUNT (3)
#define LL_HFILE_ARG_RESERVED (4)
#define LL_FLAGS (5)
#define LL_OVERRIDE_HMODULE_PTR (6)

/*
*  Debugger can alter execution via changes these variables
* 
*  setting a different last error value
*  forcing a specific return value
*  replacing the loaded file name. (Use VirtualAllocEx()
* 
* 
*  In order
*/


/*
* Debugger gets 3 pointers to modify /change.
* 
* 
* DWORD* lastError;			Debugger can set to change what the last error value is set too
* HMODULE OverrideHandle	If you manually allocate/ load the dll and do not want to use LoadLibrary,
*							Or are just wanting a specific value instead of the loadlibrary return, set this
* 
* LPCWSTR*					Debugger recevies a pointer to a block of memory that can be reallocated with
*							VirtualAlloc() if needed,  to manually point to using a specific dll.
* 
* 
* 
* If you are alterting the code path
* write a block of memory to LPCWSTR to change the target	-> done
* set either a speciic override handle or last error to skip calling Loadlibrasry-> done
* Debugger may also set string to update;.
*/
DWORD WINAPI __LoadLibraryExW_Alert(
	LPCWSTR lpLibFileName,
	HANDLE hFile,
	DWORD Flags,
	HMODULE* OverrideHandle,
	DWORD* LastErrorPtr,
	LPCWSTR* NewFileName)
	
{
	LPWSTR EditableString = 0;
	BOOL DebugDidNotSee = FALSE;
	SIZE_T size = 0;
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS] = { 0 };

	__try
	{
		if (lpLibFileName == nullptr)
		{
			EditableString = 0;
		}
		else
		{
			size = wcslen(lpLibFileName);
			EditableString = (LPWSTR) VirtualAlloc(0, size * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (EditableString != 0)
			{
				CopyMemory(EditableString, lpLibFileName, size * sizeof(wchar_t));
				ExceptionArgs[EXCEPTION_ARG_TYPE] = (ULONG_PTR)LOAD_LIBRARY_EXW_API_CALL;
				ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG_PTR)LastErrorPtr;
				ExceptionArgs[LL_FILENAME_PT] = (ULONG_PTR)EditableString;
				ExceptionArgs[LL_FILENAME_CHAR_COUNT] = size;
				ExceptionArgs[LL_HFILE_ARG_RESERVED] = (ULONG_PTR)hFile;
				ExceptionArgs[LL_FLAGS] = Flags;
				ExceptionArgs[LL_OVERRIDE_HMODULE_PTR] = (ULONG_PTR)OverrideHandle;
				
				__try
				{
					RaiseException(EXCEPTION_VALUE, 0, EXCEPTION_MAXIMUM_PARAMETERS, &ExceptionArgs[0]);
				}
				__except (GetExceptionCode() == EXCEPTION_VALUE)
				{
					DebugDidNotSee = TRUE;
				}

			}
		}
	}
	__finally
	{
		/* this code checks if wcsicmp() says the editable string is equal to the original and 
		* sets the output string to the original if it (freeing the replacement).  
		* 
		* If they don't match, output string is set to editable string and caller of this routine needs
		* toVirutal free it()
		*/
		if (EditableString != 0)
		{
			if (wcsicmp(EditableString, lpLibFileName) == 0)
			{
				VirtualFree(EditableString, 0, MEM_RELEASE);
				*NewFileName = lpLibFileName;
			}
			else
			{
				*NewFileName = EditableString;
			}
		}
	}


	if (!DebugDidNotSee)
	{

	}
	return 0;

}


FARPROC DetouredGetProcAddress(
	HMODULE hModule,
	LPCSTR  lpProcName
) 
{
	return OriginalGetProcAddress(hModule, lpProcName);
}

HMODULE DetouredLoadLibraryExW(
   LPCWSTR lpLibFileName,
   HANDLE  hFile,
   DWORD   dwFlags)
{
	HMODULE retval = (HINSTANCE)-1;
	DWORD LastError = 0;
	LPCWSTR Final_lpLibFileName=0;

	DWORD result = __LoadLibraryExW_Alert(lpLibFileName, hFile, dwFlags, &retval, &LastError, &Final_lpLibFileName);
	
	BOOL Override = 0;

	if (Final_lpLibFileName != lpLibFileName)
	{
		Override = TRUE;
	}
	if (LastError != 0)
	{
		Override = TRUE;
		SetLastError(LastError);
	}

	if (retval != (HINSTANCE)-1)
	{
		Override = TRUE;
	}

	
	if (Override)
	{
		if (retval != (HINSTANCE)-1)
		{
			return retval;
		}
		else
		{
			retval = LoadLibraryExW(Final_lpLibFileName, hFile, dwFlags);
		}
	}
	else
	{
		retval = LoadLibraryExW(lpLibFileName, hFile, dwFlags);
	}


	if (Final_lpLibFileName != lpLibFileName)
	{
		VirtualFree((LPVOID)Final_lpLibFileName, 0, MEM_FREE);
	}
	return retval;
}