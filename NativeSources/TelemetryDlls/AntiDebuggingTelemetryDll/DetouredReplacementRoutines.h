#pragma once
#include <Windows.h>
#include <winternl.h>


BOOL WINAPI DetouredCheckRemoteDebuggerPresent(HANDLE Process, PBOOL pbDebuggerPresent);
BOOL WINAPI DetouredIsDebuggerPresent();


NTSTATUS WINAPI DetouredNtQueryInformationThread(
                    HANDLE          ThreadHandle,
	                THREADINFOCLASS ThreadInformationClass,
	                PVOID           ThreadInformation,
	                ULONG           ThreadInformationLength,
	                PULONG          ReturnLength);

    NTSTATUS WINAPI DetouredNtSetInformationThread(
	     HANDLE          ThreadHandle,
	     THREADINFOCLASS ThreadInformationClass,
	     PVOID           ThreadInformation,
	     ULONG           ThreadInformationLength);

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
		PVOID                AttributeList);

