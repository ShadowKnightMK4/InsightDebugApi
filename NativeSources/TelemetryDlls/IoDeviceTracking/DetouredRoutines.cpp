#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include <sstream>
//a044c813 - fbb2 - 4af6 - a354 - 7cbb78acb7a9
/* We throw exception with this value. As long as deubgger knows this value
and what the arguments cvontrain, thet value matters no. Do note the 
bit 28 will be cleared by Windows when thrown*/


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
	ULONG ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
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
	return OriginalCloseHandle(hObject);
}
