#include "FileSandBoxHelperDll.h"
#include "ReplacementStuffCommon.h"
#include "ReplacementProcessFunctions.h"
#include <winternl.h>

#include <windows.h>

/*
* This file contains the common stuff between the ReplacementXXXX stuff.
* This mostly just replaces NtCreateThreadEx() + NtSetThreadInfo so that calls to turn off debug
* events are either stripped or fake succeeed
* 
*/

int ExceptionCheck(DWORD ExceptionCode, DWORD ExceptionMessage)
{
	if (ExceptionCode == ExceptionMessage)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
// from https://ntquery.wordpress.com/2014/03/29/anti-debug-ntcreatethreadex/
#define CreateThreadDebuggerFlag 0x00000004
// https://stackoverflow.com/questions/57118618/anti-debugger-technique-how-to-hide-a-thread-from-the-debugger-using-vb-net
#define ThreadHideFromDebugger 17



/// <summary>
/// holds a pointer to NTDLL.DLL NtSetInformatioNThread
/// </summary>
  NtSetInformationThreadPtr OriginalNtSetInformationThread =0 ;
/// <summary>
/// holds poiinter to NTDLL.DLL NtCreateThreadEx
/// </summary>
  NtCreateThreadExPtr OriginalNtCreateThreadEx = 0;



/// <summary>
/// this detour fuction just checks and drops the HIDE_THREAD_FROMDEBUGGER before passing it on.
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
)
{
	if (ThreadInformationClass == ThreadHideFromDebugger)
	{
		if ((ThreadInformation == 0) && (ThreadInformationLength == 0))
		{
			// TODO: RaiseException to notify thread attepting to thide itself to the debugger.
			// the C# 'InsightDebugger' will track  aflag if the thread was 'hidden' for user convirence
			SetLastError(0);
			return 0;
		}
	}
	return OriginalNtSetInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
}

/// <summary>
/// this detoure function just checks if CreateThreadDebuggerFlag is set and removes the flag betour forwarding to original
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
) {
	if ((CreateFlags & CreateThreadDebuggerFlag) == CreateThreadDebuggerFlag)
	{
		CreateFlags &= ~CreateThreadDebuggerFlag;
	}
	return OriginalNtCreateThreadEx(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, StartRoutine, Argument, CreateFlags, ZeroBits, StackSize, MaximumStackSize, AttributeList);
}