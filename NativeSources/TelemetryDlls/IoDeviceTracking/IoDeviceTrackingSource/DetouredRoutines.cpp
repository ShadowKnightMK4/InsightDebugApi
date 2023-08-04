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
// Args[EXCEPTION_LAST_ERROR] is context dependant.  For routines detoured that setlasterror, this is a pointer to a 4 byte side that lets a debugger specify the last error value to set. If the routine does not set last error, it's going to be the return value.
#define EXCEPTION_LAST_ERROR 1


/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from CreateFileA/W routine
#define ARG_TYPE_CREATEFILE_NORMAL (1)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from CreateFileTransactedA/W routine
#define ARG_TYPE_CREATEFILE_TRANS (2)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from CloseHandle() routine
#define ARG_TYPE_CLOSE_HANDLE (3)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from NtCreateFile routine
#define ARG_TYPE_NTCREATE_FILE (4)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from NtOpenFile routine. 
#define ARG_TYPE_NTOPEN_FILE (5)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from WriteFile routine. 
#define ARG_TYPE_WRITEFILE (6)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from ReadFile routine. 
#define ARG_TYPE_READFILE (7)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from NtWriteFile routine. 
#define ARG_TYPE_NTWRITEFILE (8)
/// Args[EXCEPTION_ARG_TYPE] will be this for receiving call from NtReadFile routine. 
#define ARG_TYPE_NTREADFILE (9)



// order the exception raised for CreateFileA/W 
// the MSDN location for the routine is at https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew



/// Args[CF_AW_FILENAME] will be a pointer to a unicode string.  
#define CF_AW_FILENAME 2
// Args[CF_AW_FILENAME_CHARCOUNT] will contain how many charactours long the string pointed to by Args[CF_AW_FILENAME] contains
#define CF_AW_FILENAME_CHARCOUNT 3

// Args[CF_AW_DESIREDACCESS] will contain the argument received for dwDesiredAccess which is how the file will be accessed.
#define CF_AW_DESIREDACCESS 4
// Args[CF_AW_SHAREMODE] will contain the argument received for dwShareMode which controls how the file will be shared
#define CT_AW_SHAREMODE 5
// Args[CT_AW_SECURITYATTRIB] will contain the argument recieved for lpSecurityAttributes wich controls security access for the handle.  
#define CT_AW_SECURITYATTRIB 6
/// Args[CT_AW_CREATIONDISPOSITION] will contain the argument received for dwCreationDisposition which also controls how the file is accessed ans some additional actions
#define CT_AW_CREATIONDISPOSITION 7
/// Args[CT_AW_FLAGANDATTRIBUTES] contains the argument received for the dwFlagsAndAttributes argument. 
#define CT_AW_FLAGANDATTRIBUTES 8
/// Args[CT_AW_TEMPLATE_FILE] contains the argument received for the template handle.
#define CT_AW_TEMPLATE_FILE 9
/// Args[CT_AW_OVEERRIDE_HANDLE] contains a pointer to a handle (x86 is 4 bytes while x64 is 8 bytes) that can be ovewriteen by a debuggger/controlling process to replace the original call with a hnadle of your choice \
Note that the handle will need to be either duplicated into the debugged process or be inherited by the spawned process.
#define CT_AW_OVEERRIDE_HANDLE 10
/// Args[CT_AW_LASTERROR] points to the same location as Args[EXCEPTION_LAST_ERROR] and is the same role as it.
#define CT_AW_LASTERROR = 11

// order for Create file transacted
#define CFT_AW_FILENAME 2
#define CFT_AW_FILENAME_CHARCOUNT 3
#define CFT_AW_DESIREDACCESS 4
#define CFT_AW_SHAREMODE 5
#define CFT_AW_SECURITYATTRIB 6
#define CFT_AW_CREATIONDISPOSITION 7
#define CFT_AW_FLAGSANDATTRIBUTES 8
#define CFT_AW_TEMPLATE_FILE 9
#define CFT_AW_TRANSACTION_HANDLE 10
#define CFT_AW_MINVERSION 11
#define CFT_AW_EXTENDARG 12
#define CFT_AW_OVVERIDE_HANDLE 13
#define CFT_AW_LASTERROR = 14


// Argument order for NtCreateFIle

// the EXCEPTION_LAST_ERROR is a pointer to what you want the function to return to the code lookup NtCreateFile NtCreateFile
#define NTCF_AW_OUTPUTHANDLE 2
#define NTCT_AW_DESIRED_ACCESS 3
#define NTCT_AW_OBJECT_ATTRIBUTES 4
#define NTCT_AW_IOSTATUSBLOCK 5
#define NTCT_AW_ALLOCATION_SIZE 6
#define NTCT_AW_FILEATRIBUTES 7
#define NTCT_AW_SHARE_ACCESS 8
#define NTCT_AW_CREATIONDISPOSITION 9
#define NTCT_AW_CREATEOPTION 10
#define NTCT_AW_EABUFFER 11
#define NTCT_AW_EALENGTH 12
 // as NtCreateFile already has a pointer to handle for output, this is will point to the NTCF_AW_OUTPUTHANDLE
#define NTCT_AW_OVERRIDE_HANDLE 13



// argument order for NtOpenFIle
// the EXCEPTION_LAST_ERROR is a pointer to what you want the function to return to the code NtOpenFile
#define NTOF_AW_OUTPUT_HANDLE 2
#define NTOF_AW_DESIRED_ACCESS 3
#define NTOF_AW_OBJECT_ATTRIBUTES 3
#define NTOF_AW_IOSTATUSBLOCK 4
#define NTOF_AW_SHARE_ACCESS 5
#define NTOF_AW_OPEN_OPTIONS 6
#define NTOF_AW_OVERRIDE_HANDLE 7


DWORD WINAPI __NtCreateFile_alert(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	//VOID* ObjectAttributes,
	POBJECT_ATTRIBUTES ObjectAttributes,
	VOID* IoStatusBlock,
	//PIO_STATUS_BLOCK   IoStatusBlock,
	PLARGE_INTEGER     AllocationSize,
	ULONG              FileAttributes,
	ULONG              ShareAccess,
	ULONG              CreateDisposition,
	ULONG              CreateOptions,
	PVOID              EaBuffer,
	ULONG              EaLength,
	HANDLE*				OverwriteHandle,
	NTSTATUS*			ReturnValue
	)
{
	BOOL DebugDidNotSee = FALSE;
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS] = { 0 };
	
	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_NTCREATE_FILE;

	// Last Error for this is an NTSTATUS value
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG_PTR)ReturnValue;
	ExceptionArgs[NTCF_AW_OUTPUTHANDLE] = (ULONG_PTR)FileHandle;
	ExceptionArgs[NTCT_AW_DESIRED_ACCESS] = (ULONG_PTR)DesiredAccess;
	ExceptionArgs[NTCT_AW_OBJECT_ATTRIBUTES] = (ULONG_PTR)ObjectAttributes;
	ExceptionArgs[NTCT_AW_IOSTATUSBLOCK] = (ULONG_PTR)IoStatusBlock;
	ExceptionArgs[NTCT_AW_ALLOCATION_SIZE] = (ULONG_PTR)AllocationSize;
	ExceptionArgs[NTCT_AW_FILEATRIBUTES] = (ULONG_PTR)FileAttributes;
	ExceptionArgs[NTCT_AW_SHARE_ACCESS] = (ULONG_PTR)ShareAccess;
	ExceptionArgs[NTCT_AW_CREATIONDISPOSITION] = (ULONG_PTR)CreateDisposition;
	ExceptionArgs[NTCT_AW_CREATEOPTION] = (ULONG_PTR)CreateOptions;
	ExceptionArgs[NTCT_AW_EABUFFER] = (ULONG_PTR)EaBuffer;
	ExceptionArgs[NTCT_AW_EALENGTH] = (ULONG_PTR)EaLength;
	ExceptionArgs[NTCT_AW_OVERRIDE_HANDLE] = (ULONG_PTR)OverwriteHandle;


	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, EXCEPTION_MAXIMUM_PARAMETERS, &ExceptionArgs[0]);
	}
	__except (GetExceptionCode() == EXCEPTION_VALUE)
	{
		DebugDidNotSee = TRUE;
	}

	if (!DebugDidNotSee)
	{

	}
	return 0;


}

#define RF_HFILE (2)
#define RF_READBUFFER (3)
#define RF_BYTESTOREAD (4)
#define RF_BYTESREAD (5)
#define RF_OVERLAPPED (6)
#define RF_RETURNVALPTR (7)

/*
* Alert routine for ReadFile.
* This breaks the usually mold because there's more things to interact with.
* Debugger gets the args (as normal).   Debugger has points to perform the read itself, last error value and return value.
* 
* IMPORTANT! Debugger should set return value if wanting to indicate it handled the request. If that's not done, request is handled in the local process
* 
*/
DWORD WINAPI __ReadFile_alert(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD  nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesToRead,
	LPOVERLAPPED lpOverlapped,
	BOOL* ReturnValuePtr,
	DWORD* LastErrorPtr)
{
	DWORD ret = 0;
	BOOL DebugDidNotSee = FALSE;
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS] = { 0 };

	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_READFILE;
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG_PTR)LastErrorPtr;
	ExceptionArgs[RF_HFILE] = (ULONG_PTR)hFile;
	ExceptionArgs[RF_BYTESREAD] = (ULONG_PTR)nNumberOfBytesToRead;
	ExceptionArgs[RF_BYTESREAD] = (ULONG_PTR)lpNumberOfBytesToRead;
	ExceptionArgs[RF_OVERLAPPED] = (ULONG_PTR)lpOverlapped;
	ExceptionArgs[RF_RETURNVALPTR] = (ULONG_PTR)ReturnValuePtr;
	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, EXCEPTION_MAXIMUM_PARAMETERS, &ExceptionArgs[0]);
	}
	__except (GetExceptionCode() == EXCEPTION_VALUE)
	{
		DebugDidNotSee = TRUE;
	}

	if (!DebugDidNotSee)
	{

	}
	return ret;
}
DWORD WINAPI __NtOpenFile_alert(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	VOID* ObjectAttributes,
	//POBJECT_ATTRIBUTES ObjectAttributes,
	VOID* IoStatusBlock,
	//		      PIO_STATUS_BLOCK   IoStatusBlock,
	ULONG              ShareAccess,
	ULONG              OpenOptions,
	HANDLE*				OverrideHandle,
	NTSTATUS*			ReturnValue)
{
	BOOL DebugDidNotSee = FALSE;
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS] = { 0 };
	
	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_NTOPEN_FILE;
	ExceptionArgs[EXCEPTION_LAST_ERROR] = 0;
	ExceptionArgs[NTOF_AW_OUTPUT_HANDLE] = (ULONG_PTR)FileHandle;
	ExceptionArgs[NTOF_AW_DESIRED_ACCESS] = (ULONG_PTR)DesiredAccess;
	ExceptionArgs[NTOF_AW_OBJECT_ATTRIBUTES] = (ULONG_PTR)ObjectAttributes;
	ExceptionArgs[NTOF_AW_IOSTATUSBLOCK] = (ULONG_PTR)IoStatusBlock;
	ExceptionArgs[NTOF_AW_SHARE_ACCESS] = (ULONG_PTR)ShareAccess;
	ExceptionArgs[NTOF_AW_OPEN_OPTIONS] = (ULONG_PTR)OpenOptions;
	
	ExceptionArgs[NTOF_AW_OVERRIDE_HANDLE] = (ULONG_PTR)FileHandle;


	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, EXCEPTION_MAXIMUM_PARAMETERS, &ExceptionArgs[0]);
	}
	__except (GetExceptionCode() == EXCEPTION_VALUE)
	{
		DebugDidNotSee = TRUE;
	}

	if (!DebugDidNotSee)
	{

	}
	return 0;



}

DWORD __CloseHandle_Alert(HANDLE hHandle, BOOL* ReturnValue)
{
	BOOL DebugDidNotSee = FALSE;
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS] = { 0 };
	

	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_CLOSE_HANDLE;
	ExceptionArgs[EXCEPTION_ARG_TYPE] = 0;
	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, EXCEPTION_MAXIMUM_PARAMETERS, (CONST ULONG_PTR*) & ExceptionArgs);
	}
	__except (GetExceptionCode() == EXCEPTION_VALUE)
	{
		DebugDidNotSee = TRUE;
	}

	return 0;
}

/// <summary>
/// Raise the alert for CreateFileTransasctedA/W and return 0.
/// </summary>
/// <param name="lpFileName"></param>
/// <param name="dwDesiredAccess"></param>
/// <param name="dwShareMode"></param>
/// <param name="lpSecurityAttributes"></param>
/// <param name="dwCreationDisposition"></param>
/// <param name="dwFlagsAndAttributes"></param>
/// <param name="hTemplateFile"></param>
/// <param name="hTransaction"></param>
/// <param name="pusMiniVersion"></param>
/// <param name="lpExtendedParameter"></param>
/// <param name="hReplacement"></param>
/// <param name="lpLastError"></param>
/// <returns></returns>
DWORD __CreateFileTransactedAW_CommonAlert(
	LPCWSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile,
	HANDLE                hTransaction,
	PUSHORT               pusMiniVersion,
	PVOID                 lpExtendedParameter,
	HANDLE* hReplacement,
	DWORD* lpLastError)
{
	BOOL DebugDidNotSee = FALSE;
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));

	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_CREATEFILE_TRANS;
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG_PTR)lpLastError;


	ExceptionArgs[CFT_AW_FILENAME] = (ULONG_PTR)lpFileName;
	if (lpFileName != nullptr)
	{
		ExceptionArgs[CFT_AW_FILENAME_CHARCOUNT] = wcslen(lpFileName);
	}
	ExceptionArgs[CFT_AW_DESIREDACCESS] = dwDesiredAccess;
	ExceptionArgs[CFT_AW_SHAREMODE] = dwShareMode;
	ExceptionArgs[CFT_AW_CREATIONDISPOSITION] = dwCreationDisposition;
	ExceptionArgs[CFT_AW_FLAGSANDATTRIBUTES] = dwFlagsAndAttributes;
	ExceptionArgs[CFT_AW_TEMPLATE_FILE] = (ULONG_PTR)hTemplateFile;
	ExceptionArgs[CFT_AW_TRANSACTION_HANDLE] = (ULONG_PTR)hTransaction;
	ExceptionArgs[CFT_AW_MINVERSION] = (ULONG_PTR)pusMiniVersion;
	ExceptionArgs[CFT_AW_EXTENDARG] = (ULONG_PTR)lpExtendedParameter;
	ExceptionArgs[CFT_AW_OVVERIDE_HANDLE] = (ULONG_PTR)hReplacement;

	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, EXCEPTION_MAXIMUM_PARAMETERS, (CONST ULONG_PTR*) & ExceptionArgs);
	}
	__except (GetExceptionCode() == EXCEPTION_VALUE)
	{
		DebugDidNotSee = TRUE;
	}

	if (!DebugDidNotSee)
	{

	}
	return 0;

	
}



/// <summary>
/// Raise the alert for CreateFileA/W and return 0;
/// </summary>
/// <param name="lpFileName"></param>
/// <param name="dwDesiredAccess"></param>
/// <param name="dwShareMode"></param>
/// <param name="lpSecurityAttributes"></param>
/// <param name="dwCreationDisposition"></param>
/// <param name="dwFlagsAndAttributes"></param>
/// <param name="hTemplateFile"></param>
/// <param name="hReplacementPointer"></param>
/// <returns></returns>
DWORD __CreateFileAW_CommmonAlert(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile,
	/* This is an argument that gets passed to debugger (if any)*/
	HANDLE*				  hReplacementPointer,
	DWORD*				  lpLastError)
{
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));
	int DebugDidNotSee = 0;

	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_CREATEFILE_NORMAL;
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG_PTR)lpLastError;

	ExceptionArgs[CF_AW_FILENAME] = (ULONG_PTR)lpFileName;

	if (lpFileName != nullptr)
	{

		ExceptionArgs[CF_AW_FILENAME_CHARCOUNT] = wcslen(lpFileName);
	}
	ExceptionArgs[CF_AW_DESIREDACCESS] = dwDesiredAccess;
	ExceptionArgs[CT_AW_SHAREMODE] = dwShareMode;
	ExceptionArgs[CT_AW_SECURITYATTRIB] = (ULONG_PTR)lpSecurityAttributes;
	ExceptionArgs[CT_AW_CREATIONDISPOSITION] = dwCreationDisposition;
	ExceptionArgs[CT_AW_FLAGANDATTRIBUTES] = dwFlagsAndAttributes;
	ExceptionArgs[CT_AW_TEMPLATE_FILE] = (ULONG_PTR)hTemplateFile;
	ExceptionArgs[CT_AW_OVEERRIDE_HANDLE] = (ULONG_PTR)hReplacementPointer;

	
	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, 15, &ExceptionArgs[0]);
	}
	__except (GetExceptionCode() == EXCEPTION_VALUE)
	{
		DebugDidNotSee = 1;
	}
	return 0;

}
HANDLE WINAPI DetouredCreateFileA(
LPCSTR                lpFileName,
DWORD                 dwDesiredAccess,
DWORD                 dwShareMode,
LPSECURITY_ATTRIBUTES lpSecurityAttributes,
DWORD                 dwCreationDisposition,
DWORD                 dwFlagsAndAttributes,
HANDLE                hTemplateFile)

{
	BOOL retVal = FALSE;
	BOOL Overwritten = FALSE;
	DWORD lastErrorRep = 0;
	// common thinkg gets a pointer to a string they can replace value with.
	HANDLE hReplacement = 0;

wchar_t* UnicodeString = AnsiToUnicode((char*)lpFileName);

DWORD common_branch = __CreateFileAW_CommmonAlert(UnicodeString, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, &hReplacement, &lastErrorRep);
if (!UnicodeString)
{
	free(UnicodeString);
}
	if (Overwritten)
	{
		SetLastError(lastErrorRep);
		return hReplacement;
	}
	else
	{
		return OriginalCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}



}

HANDLE WINAPI DetouredCreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
)
{
	HANDLE hReplacement = 0;
	BOOL Overwritten = FALSE;
	DWORD lastErrorRep = 0;
#ifdef _DEBUG
	OutputDebugString(L"CreatedCreateFIleW for \"");
	OutputDebugString(lpFileName);
	OutputDebugString(L"\"\r\n");
#endif
	DWORD common_branch = __CreateFileAW_CommmonAlert(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, &hReplacement,&lastErrorRep);
	

	if (hReplacement != 0)
	{
		Overwritten = TRUE;
	}

	if (Overwritten)
	{
		SetLastError(lastErrorRep);
		return hReplacement;

	}
	else
	{

		return OriginalCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

}

HANDLE __stdcall DetouredCreateFileTransactedA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID lpExtendedParameter)
{
	HANDLE hReplacement = 0;
	BOOL Overritten = FALSE;
	DWORD lastErrorRep = 0;
	wchar_t* Unicode = AnsiToUnicode((char*)lpFileName);
	DWORD common_branch = __CreateFileTransactedAW_CommonAlert(Unicode, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, hTransaction, pusMiniVersion, lpExtendedParameter, &hReplacement, &lastErrorRep);
	if (hReplacement != 0)
	{
		Overritten = TRUE;
	}

	if (Overritten)
	{
		SetLastError(lastErrorRep);
		return hReplacement;
	}
	else
	{
		return OriginalCreateFileTransactedA(lpFileName,dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, hTransaction, pusMiniVersion, lpExtendedParameter);
	}
}

HANDLE __stdcall DetouredCreateFileTransactedW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID lpExtendedParameter)
{
	HANDLE hReplacement = 0;
	BOOL Overritten = FALSE;
	DWORD lastErrorRep = 0;
	DWORD common_branch = __CreateFileTransactedAW_CommonAlert(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, hTransaction, pusMiniVersion, lpExtendedParameter, &hReplacement,&lastErrorRep);

	if (hReplacement != 0)
	{
		Overritten = TRUE;
	}

	if (Overritten)
	{
		SetLastError(lastErrorRep);
		return hReplacement;
	}
	else
	{
		return OriginalCreateFileTransactedW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, hTransaction, pusMiniVersion, lpExtendedParameter);
	}
}

HANDLE __stdcall DetouredCreateFile2(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams)
{
	SetLastError(ERROR_NOT_SUPPORTED);
	return INVALID_HANDLE_VALUE;
}



BOOL __stdcall DetouredCloseHandle(HANDLE hObject)
{
	BOOL Ret = FALSE;
	BOOL Overwrite = FALSE;
	
	DWORD Branch = __CloseHandle_Alert(&hObject, &Ret);

	if (Overwrite)
	{
		return Ret;
	}
	else
	{
		return OriginalCloseHandle(hObject);
	}
}


   NTSTATUS WINAPI DetouredNtCreateFile(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	//VOID* ObjectAttributes,
	POBJECT_ATTRIBUTES ObjectAttributes,
	VOID* IoStatusBlock,
	//PIO_STATUS_BLOCK   IoStatusBlock,
	PLARGE_INTEGER     AllocationSize,
	ULONG              FileAttributes,
	ULONG              ShareAccess,
	ULONG              CreateDisposition,
	ULONG              CreateOptions,
	PVOID              EaBuffer,
	ULONG              EaLength)
   {
	   HANDLE hReplacement = 0;
	   NTSTATUS Returnvalue = 0;
	   BOOL Overritten = FALSE;
#ifdef _DEBUG
	   /*
	   wstringstream output;
	   output << L"ObjectAttributes->Attributes == " << ObjectAttributes->Attributes << endl;
	   output << L"ObjectAttributes->Length == " << ObjectAttributes->Length << endl;
	   output << L"ObjectAttributes->ObjectName->Buffer == " << ObjectAttributes->ObjectName->Buffer << endl;
	   output << L"ObjectAttributes->RootDirectory ==" << ObjectAttributes->RootDirectory << endl << hex;
	   output << L"ObjectAttributes->SecurityDescriptor  ==" << ObjectAttributes->SecurityDescriptor << endl;
	   output << L"ObjectAttributes->SecurityQualityOfService == " << ObjectAttributes->SecurityQualityOfService << endl;
	   OutputDebugString(output.str().c_str());*/
#endif


	   DWORD branch = __NtCreateFile_alert(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength, &hReplacement, &Returnvalue);


	   if ( (hReplacement != 0) || (Returnvalue != 0))
	   {
		   Overritten = true;
	   }

	   if (Overritten)
	   {
		   *FileHandle = hReplacement;
		   return Returnvalue;
	   }
	   else
	   {
		   return OriginalNtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
	   }
   }
 
   BOOL WINAPI DetouredReadFile(
	   HANDLE       hFile,
	   LPVOID       lpBuffer,
	   DWORD        nNumberOfBytesToRead,
	   LPDWORD      lpNumberOfBytesRead,
	   LPOVERLAPPED lpOverlapped)
   {
	   ULONGLONG ReturnValue = 0xffffffff00020002; /* pretending to be a bool*/
	   DWORD LastError = 0;
	   BOOL Overritten = FALSE;

	   if (lpNumberOfBytesRead != nullptr)
	   {
		   *lpNumberOfBytesRead = 0;
	   }

	   DWORD branch = __ReadFile_alert(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped, (BOOL*)&ReturnValue, &LastError);
	  
	   /* DEAR FUTURE ME:  This if statement should check if the return value's been set (not the 2 value) or the number of bytes read is not null and has been set*/
	
	   if ((lpNumberOfBytesRead != nullptr))
	   {
		   if (*lpNumberOfBytesRead != 0)
		   {
			   Overritten = true;
		   }
	   }

	   if ((INT)ReturnValue != 20002)
	   {
		   Overritten = true;
	   }

	   if (Overritten)
	   {
		   SetLastError(LastError);
		   return (BOOL)ReturnValue;
	   }
	   else
	   {
		   return OriginalReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	   }
  

   }
   NTSTATUS WINAPI DetouredNtOpenFile(
	   PHANDLE            FileHandle,
	   ACCESS_MASK        DesiredAccess,
	   VOID* ObjectAttributes,
	   //POBJECT_ATTRIBUTES ObjectAttributes,
	   VOID* IoStatusBlock,
	   //		      PIO_STATUS_BLOCK   IoStatusBlock,
	   ULONG              ShareAccess,
	   ULONG              OpenOptions)
   {
	   HANDLE hReplacement = 0;
	   NTSTATUS Returnvalue = 0;
	   BOOL Overritten = FALSE;

	   DWORD branch = __NtOpenFile_alert(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions, &hReplacement, &Returnvalue);

	   if (hReplacement != 0)
	   {
		   Overritten = TRUE;
	   }

	   if (Overritten)
	   {
		   *FileHandle = hReplacement;
		   return Returnvalue;
	   }
	   else
	   {
		   return OriginalNtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
	   }
   }