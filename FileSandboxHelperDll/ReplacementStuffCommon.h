#pragma once
#include "FileSandBoxHelperDll.h"

/// if (ExceptionCode == ExceptionMessage) return EXCEPTION_CONTINUE_EXECUTION, else EXCEPTION_CONTINUE_SEARCH
extern int ExceptionCheck(DWORD ExceptionCode, DWORD ExceptionMessage);

#ifndef SELF_CHECK
	#define SELF_CHECK( x,  y )     ((ExceptionCheck( (x),  (y) )))
#endif 

/// <summary>
/// We detour at min this to drop requests to HIDE_THREAD_FROM_DEBUGGER
/// </summary>
typedef NTSTATUS(NTAPI* NtSetInformationThreadPtr)(
	HANDLE          ThreadHandle,
	THREADINFOCLASS ThreadInformationClass,
	PVOID           ThreadInformation,
	ULONG           ThreadInformationLength
	);

typedef NTSTATUS(NTAPI* NtCreateThreadExPtr)(
     PHANDLE ThreadHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes,
     HANDLE ProcessHandle,
     PVOID StartRoutine,
     PVOID Argument,
     ULONG CreateFlags,
     ULONG_PTR ZeroBits,
     SIZE_T StackSize,
     SIZE_T MaximumStackSize,
     PVOID AttributeList
);


/// <summary>
/// Once Dllmain() is called, will contain the hmodule from tjhat.
/// </summary>
extern HMODULE Self;
/// <summary>
/// holds a pointer to NTDLL.DLL NtSetInformatioNThread
/// </summary>
extern NtSetInformationThreadPtr OriginalNtSetInformationThread;
/// <summary>
/// holds poiinter to NTDLL.DLL NtCreateThreadEx
/// </summary>
extern NtCreateThreadExPtr OriginalNtCreateThreadEx;
/// <summary>
/// returns success on called with ThreadSetHideDebugger (17) being passed without actually calling original. Calls original otherwise
/// </summary>
/// <param name="ThreadHandle"></param>
/// <param name="ThreadInformationClass"></param>
/// <param name="ThreadInformation"></param>
/// <param name="ThreadInformationLength"></param>
/// <returns></returns>
NTSTATUS NTAPI NtSetInformationThreadDropDebuggerHideRequestsDetoured(
	HANDLE          ThreadHandle,
	THREADINFOCLASS ThreadInformationClass,
	PVOID           ThreadInformation,
	ULONG           ThreadInformationLength
);

/// <summary>
/// checks if CreateThreadDebuggerFlag is in CreateFlags, strips it if it is and calls the original.
/// </summary>
/// <param name="ThreadHandle"></param>
/// <param name="DesiredAccess"></param>
/// <param name="ObjectAttributes"></param>
/// <param name="ProcessHandle"></param>
/// <param name="StartRoutine"></param>
/// <param name="Argument"></param>
/// <param name="CreateFlags"></param>
/// <param name="ZeroBits"></param>
/// <param name="StackSize"></param>
/// <param name="MaximumStackSize"></param>
/// <param name="AttributeList"></param>
/// <returns></returns>
NTSTATUS NTAPI NtCreateThreadExDropDebuggerFlagDetoured(
	PHANDLE ThreadHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	HANDLE ProcessHandle,
	PVOID StartRoutine,
	PVOID Argument,
	ULONG CreateFlags,
	ULONG_PTR ZeroBits,
	SIZE_T StackSize,
	SIZE_T MaximumStackSize,
	PVOID AttributeList
);