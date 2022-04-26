#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
//a044c813 - fbb2 - 4af6 - a354 - 7cbb78acb7a9
/* We throw exception with this value. As long as deubgger knows this value
and what the arguments cvontrain, thet value matters no. Do not the 
bit 28 will be cleared by Windows when thrown*/
#define EXCEPTION_VALUE (0x78ACB7A9)


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

#define CF_AW_FILENAME 0
#define CF_AW_FILENAME_CHARCOUNT 1
#define CF_AW_DESIREDACCESS 2
#define CT_AW_SHAREMODE 3
#define CT_AW_SECURITYATTRIB 4
#define CT_AW_CREATIONDISPOSITION 5
#define CT_AW_FLAGANDATTRIBUTES 6
#define CT_AW_TEMPLATE_FILE 7
#define CT_AW_OVEERRIDE_HANDLE 8

#define CFT_AW_FILENAME 0
#define CFT_AW_FILENAME_CHARCOUNT 1
#define CFT_AW_DESIREDACCESS 3
#define CFT_AW_SHAREMODE 4
#define CFT_AW_SECURITYATTRIB 5
#define CFT_AW_CREATIONDISPOSITION 6
#define CFT_AW_FLAGSANDATTRIBUTES 7
#define CFT_AW_TEMPLATE_FILE 8
#define CFT_AW_TRANSACTION_HANDLE 9
#define CFT_AW_MINVERSION 10
#define CFT_AW_EXTENDARG 11
#define CFT_AW_OVVERIDE_HANDLE 12

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
				   HANDLE*				 hReplacement)
)

DWORD __CreateFileAW_CommmonAlert(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile,
	/* This is an argument that gets passed to debugger (if any)*/
	HANDLE*				  hReplacementPointer)
{
	ULONG ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));
	int DebugDidNotSee = 0;
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
	DWORD common_branch;
	BOOL retVal = FALSE;
	BOOL Overwritten = FALSE;
	// common think gets a pointer to a string they can replace value with.
	HANDLE hReplacement = 0;
	DWORD SizeString = MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, 0, 0);
	wchar_t* UnicodeString = nullptr;
	
	__try
	{
		UnicodeString = new wchar_t[SizeString + 1];
		SizeString = MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, UnicodeString,SizeString );
		common_branch = __CreateFileAW_CommmonAlert(UnicodeString, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, &hReplacement);
		if (hReplacement != 0)
		{
			retVal = TRUE;
			Overwritten = TRUE;
			__leave;
		}
	}
	__finally
	{
		if (UnicodeString)
		{
			delete UnicodeString;
			UnicodeString = nullptr;
		}
	}

	if (Overwritten)
	{
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
	DWORD commom_branch = __CreateFileAW_CommmonAlert(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, &hReplacement);
	if (hReplacement != 0)
	{
		Overwritten = TRUE;
	}

	if (Overwritten)
	{
		return hReplacement;
	}
	else
	{
		return OriginalCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	
}

HANDLE __stdcall DetouredCreateFileTransactedA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID lpExtendedParameter)
{
	return HANDLE();
}

HANDLE __stdcall DetouredCreateFileTransactedW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID lpExtendedParameter)
{
	return HANDLE();
}

HANDLE __stdcall DetouredCreateFile2(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams)
{
	return HANDLE();
}

BOOL __stdcall DetouredCloseHandle(HANDLE hObject)
{
	return 0;
}
