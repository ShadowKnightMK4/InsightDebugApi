#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include <sstream>
#include <winternl.h>

using namespace std;
wchar_t* AnsiToUnicode(char* ansi)
{
	if (ansi == nullptr)
		return nullptr;

	DWORD SizeString = MultiByteToWideChar(CP_ACP, 0, ansi, -1, 0, 0);
	wchar_t* UnicodeString = nullptr;



	UnicodeString = new wchar_t[SizeString + 1];

	SizeString = MultiByteToWideChar(CP_ACP, 0, ansi, -1, UnicodeString, SizeString);
	UnicodeString[SizeString] = 0;
	return UnicodeString;

}


/* We throw exception with this value. As long as debugger knows this value
and what the arguments contain, the value matters not. Do note that bit 28 will be cleared by Windows when thrown if 
you happen to change this to another value (for example another Telemetry Project.
*/



#define EXCEPTION_VALUE (0x68ACB7A9)

#define ARG_TYPE_CREATEFILE_NORMAL (1)
#define ARG_TYPE_CREATEFILE_TRANS (2)
#define ARG_TYPE_CLOSE_HANDLE (3)
#define ARG_TYPE_NTCREATE_FILE (4)
#define ARG_TYPE_NTOPEN_FILE (5)


/*
What this does:
We detour the chosen routines,
We pass the arguments.
We pass a pointer to something the debugger can change to modify the functionality of the return value.


On the InsightSheath Side

Add code to InsightSheath.Telemetry.IoDeviceExceptionReader to read (and write) exceptions
*/
/*
* return values:
* 0 = DO NOT CONTINUE call
* 1 = CONTINUE CALL with new arguments
* 2 = Replace Handle (Debugger Provides)
*/

#define EXCEPTION_ARG_TYPE 0
#define EXCEPTION_LAST_ERROR 1

// order for create file routines
#define CF_AW_FILENAME 2
#define CF_AW_FILENAME_CHARCOUNT 3
#define CF_AW_DESIREDACCESS 4
#define CT_AW_SHAREMODE 5
#define CT_AW_SECURITYATTRIB 6
#define CT_AW_CREATIONDISPOSITION 7
#define CT_AW_FLAGANDATTRIBUTES 8
#define CT_AW_TEMPLATE_FILE 9
#define CT_AW_OVEERRIDE_HANDLE 10
#define CT_AW_LASTERROR = 11

// order for Create file transacte
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
#define NTCT_AW_CREATEDISPO 9
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
	ULONG ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));
	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_NTCREATE_FILE;

	// Last Error for this is an NTSTATUS value
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG)ReturnValue;
	ExceptionArgs[NTCF_AW_OUTPUTHANDLE] = (ULONG)FileHandle;
	ExceptionArgs[NTCT_AW_DESIRED_ACCESS] = (ULONG)DesiredAccess;
	ExceptionArgs[NTCT_AW_OBJECT_ATTRIBUTES] = (ULONG)ObjectAttributes;
	ExceptionArgs[NTCT_AW_IOSTATUSBLOCK] = (ULONG)IoStatusBlock;
	ExceptionArgs[NTCT_AW_ALLOCATION_SIZE] = (ULONG)AllocationSize;
	ExceptionArgs[NTCT_AW_FILEATRIBUTES] = (ULONG)FileAttributes;
	ExceptionArgs[NTCT_AW_SHARE_ACCESS] = (ULONG)ShareAccess;
	ExceptionArgs[NTCT_AW_CREATEOPTION] = (ULONG)CreateDisposition;
	ExceptionArgs[NTCT_AW_EABUFFER] = (ULONG)EaBuffer;
	ExceptionArgs[NTCT_AW_EALENGTH] = (ULONG)EaLength;
	ExceptionArgs[NTCT_AW_OVERRIDE_HANDLE] = (ULONG)OverwriteHandle;
	

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
	ULONG ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));
	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_NTOPEN_FILE;
	ExceptionArgs[EXCEPTION_LAST_ERROR] = 0;
	ExceptionArgs[NTOF_AW_OUTPUT_HANDLE] = (ULONG)FileHandle;
	ExceptionArgs[NTOF_AW_DESIRED_ACCESS] = (ULONG)DesiredAccess;
	ExceptionArgs[NTOF_AW_OBJECT_ATTRIBUTES] = (ULONG)ObjectAttributes;
	ExceptionArgs[NTOF_AW_IOSTATUSBLOCK] = (ULONG)IoStatusBlock;
	ExceptionArgs[NTOF_AW_SHARE_ACCESS] = (ULONG)ShareAccess;
	ExceptionArgs[NTOF_AW_OPEN_OPTIONS] = OpenOptions;
	
	ExceptionArgs[NTOF_AW_OVERRIDE_HANDLE] = (ULONG)FileHandle;


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

DWORD __CloseHandle_Alert(HANDLE hHandle, BOOL * ReturnValue)
{
	BOOL DebugDidNotSee = FALSE;
	ULONG ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));

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
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG)lpLastError;


	ExceptionArgs[CFT_AW_FILENAME] = (ULONG)lpFileName;
	if (lpFileName != nullptr)
	{
		ExceptionArgs[CFT_AW_FILENAME_CHARCOUNT] = wcslen(lpFileName);
	}
	ExceptionArgs[CFT_AW_DESIREDACCESS] = dwDesiredAccess;
	ExceptionArgs[CFT_AW_SHAREMODE] = dwShareMode;
	ExceptionArgs[CFT_AW_CREATIONDISPOSITION] = dwCreationDisposition;
	ExceptionArgs[CFT_AW_FLAGSANDATTRIBUTES] = dwFlagsAndAttributes;
	ExceptionArgs[CFT_AW_TEMPLATE_FILE] = (ULONG)hTemplateFile;
	ExceptionArgs[CFT_AW_TRANSACTION_HANDLE] = (ULONG)hTransaction;
	ExceptionArgs[CFT_AW_MINVERSION] = (ULONG)pusMiniVersion;
	ExceptionArgs[CFT_AW_EXTENDARG] = (ULONG)lpExtendedParameter;
	ExceptionArgs[CFT_AW_OVVERIDE_HANDLE] = (ULONG)hReplacement;

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
	ULONG ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));
	int DebugDidNotSee = 0;

	ExceptionArgs[EXCEPTION_ARG_TYPE] = ARG_TYPE_CREATEFILE_NORMAL;
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG)lpLastError;

	ExceptionArgs[CF_AW_FILENAME] = (ULONG)lpFileName;
	if (lpFileName != nullptr)
	{

		ExceptionArgs[CF_AW_FILENAME_CHARCOUNT] = wcslen(lpFileName);
	}
	ExceptionArgs[CF_AW_DESIREDACCESS] = dwDesiredAccess;
	ExceptionArgs[CT_AW_SHAREMODE] = dwShareMode;
	ExceptionArgs[CT_AW_SECURITYATTRIB] = (ULONG)lpSecurityAttributes;
	ExceptionArgs[CT_AW_CREATIONDISPOSITION] = dwCreationDisposition;
	ExceptionArgs[CT_AW_FLAGANDATTRIBUTES] = dwFlagsAndAttributes;
	ExceptionArgs[CT_AW_TEMPLATE_FILE] = (ULONG)hTemplateFile;
	ExceptionArgs[CT_AW_OVEERRIDE_HANDLE] = (ULONG)hReplacementPointer;

	
	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, 15, (CONST ULONG_PTR*)&ExceptionArgs);
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
HANDLE                hTemplateFile
)

{
BOOL retVal = FALSE;
BOOL Overwritten = FALSE;
DWORD lastErrorRep = 0;
// common think gets a pointer to a string they can replace value with.
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
	OutputDebugStringW(L"\"\r\n");
#endif
	DWORD common_branch = __CreateFileAW_CommmonAlert(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, &hReplacement,&lastErrorRep);
	

	if (hReplacement != 0)
	{
#ifdef _DEBUG
		OutputDebugString(L"Overritten the file handle with the new one");

#ifdef _DEBUG
		std::wstringstream tmp;
		tmp << L"Replaced!!! \r\n" << "Handle ==" << std::hex << hReplacement << L"LastError = " << std::dec << lastErrorRep << L"\r\n";
		OutputDebugString(tmp.str().c_str());
#endif

#endif
		Overwritten = TRUE;
	}

	if (Overwritten)
	{
#ifdef _DEBUG
		OutputDebugString(L"Setting last error to overwritten value and returning new one");
#endif
		SetLastError(lastErrorRep);
		return hReplacement;

	}
	else
	{
#ifdef _DEBUG
		OutputDebugString(L"Call not overwritten\r\n");
#endif
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
	   wstringstream output;
	   output << L"ObjectAttributes->Attributes == " << ObjectAttributes->Attributes << endl;
	   output << L"ObjectAttributes->Length == " << ObjectAttributes->Length << endl;
	   output << L"ObjectAttributes->ObjectName->Buffer == " << ObjectAttributes->ObjectName->Buffer << endl;
	   output << L"ObjectAttributes->RootDirectory ==" << ObjectAttributes->RootDirectory << endl << hex;
	   output << L"ObjectAttributes->SecurityDescriptor  ==" << ObjectAttributes->SecurityDescriptor << endl;
	   output << L"ObjectAttributes->SecurityQualityOfService == " << ObjectAttributes->SecurityQualityOfService << endl;
	   OutputDebugString(output.str().c_str());
#endif


	   DWORD branch = __NtCreateFile_alert(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength, &hReplacement, &Returnvalue);


	   if (hReplacement != 0)
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