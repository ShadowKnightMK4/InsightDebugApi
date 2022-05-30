#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include <sstream>
//#include <winternl.h>

using namespace std;
/// <summary>
/// routine to already convert Ansi string to a more suitable Unicode string for .NET. Your replacement routine will need to call delete[] aftwards
/// </summary>
/// <param name="ansi"></param>
/// <returns></returns>
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


/*
Replace this with a different value and put some extension wrappers to DebugStruct in the sheath that deal with it
This is the value for the project this is based on - AntiDebuggerTelemtry
*/
#define EXCEPTION_VALUE (0x68ACB7AA)


/*
What this does:
We detour the chosen routines,
We pass the arguments.
We pass a pointer to something the debugger can change to modify the functionality of the return value.


On the InsightSheath Side

Add code as an extension to DebugEvent to read.
*/
/*
* 
*/

#define EXCEPTION_ARG_TYPE 0
#define EXCEPTION_LAST_ERROR 1


// not actually used currently but reserved for when the detoured routine raises exceptions
#define AD_ISDEBUGGERPRESENT (0)

#define AD_CHECKREMOTEDEBUGGERPRESENT (1)
#define ADNT_CREATETHREADEX (2)
#define ADNT_QUERYTHREADINFO (3)
#define ADNT_SETTHREADINFO (4)


/* consts specify check remote debugger */


#define CRD_PROCESSHANDLE (2)
#define CRD_OUTPUTBOOL (3)
#define CRD_RETURNVALUE (4)

DWORD WINAPI __CheckRemoteDebugger_Alert(HANDLE Process, PBOOL pbDebuggerPresent, PBOOL ReturnValue, DWORD* LastError)
{
	BOOL DebugDidNotSee = FALSE;
	ULONG_PTR ExceptionArgs[EXCEPTION_MAXIMUM_PARAMETERS];
	ZeroMemory(&ExceptionArgs, sizeof(ExceptionArgs));
	ExceptionArgs[EXCEPTION_ARG_TYPE] = AD_CHECKREMOTEDEBUGGERPRESENT;
	ExceptionArgs[EXCEPTION_LAST_ERROR] = (ULONG_PTR)LastError;
	ExceptionArgs[CRD_PROCESSHANDLE] = (ULONG_PTR)Process;
	ExceptionArgs[CRD_OUTPUTBOOL] = (ULONG_PTR)pbDebuggerPresent;
	ExceptionArgs[CRD_RETURNVALUE] = (ULONG_PTR)ReturnValue;
	__try
	{
		RaiseException(EXCEPTION_VALUE, 0, EXCEPTION_MAXIMUM_PARAMETERS, &ExceptionArgs[0]);
	}
	__except (GetExceptionCode() == EXCEPTION_VALUE)
	{
		DebugDidNotSee = TRUE;
	}

	return DebugDidNotSee;
}

BOOL WINAPI DetouredIsDebuggerPresent()
{
	return FALSE;
}

BOOL WINAPI DetouredCheckRemoteDebuggerPresent(HANDLE Process, PBOOL pbDebuggerPresent)
{
	BOOL ReturnValue = FALSE;
	DWORD LastError = 0;

	DWORD DebugDidNotSee = __CheckRemoteDebugger_Alert(Process, pbDebuggerPresent, &ReturnValue, &LastError);
	if (DebugDidNotSee)
	{
		return OriginalCheckRemoteDebuggerPresent(Process, pbDebuggerPresent);
	}
	else
	{
		return ReturnValue;
	}
}
// sourced from the codeproject anti debugging article
NTSTATUS WINAPI DetouredNtCreateThreadEx(
	         PHANDLE              ThreadHandle,
	         ACCESS_MASK          DesiredAccess,
	         POBJECT_ATTRIBUTES   ObjectAttributes,
	         HANDLE               ProcessHandle,
	         PVOID                StartRoutine,
	         PVOID                Argument,
	         ULONG                CreateFlags,
	         ULONG_PTR            ZeroBits,
	         SIZE_T               StackSize,
	         SIZE_T               MaximumStackSize,
	         PVOID                AttributeList)
{
	return OriginalNtCreateThreadEx(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, StartRoutine, Argument, CreateFlags, ZeroBits, StackSize, MaximumStackSize, AttributeList);
}
NTSTATUS WINAPI DetouredNtSetInformationThread(
	     HANDLE          ThreadHandle,
	     THREADINFOCLASS ThreadInformationClass,
	     PVOID           ThreadInformation,
	     ULONG           ThreadInformationLength
)
{
	return OriginalNtSetInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
}
NTSTATUS WINAPI DetouredNtQueryInformationThread(
	HANDLE          ThreadHandle,
	THREADINFOCLASS ThreadInformationClass,
	PVOID           ThreadInformation,
	ULONG           ThreadInformationLength,
	PULONG          ReturnLength)
{
	return OriginalNtQueryInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength);
}
