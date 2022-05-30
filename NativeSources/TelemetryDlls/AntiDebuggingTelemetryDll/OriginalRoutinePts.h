#pragma once
#include <Windows.h>
#include <winternl.h>
typedef BOOL(WINAPI* IsDebuggerPresentPtr)();
typedef BOOL(WINAPI* CheckRemoteDebuggerPresentPtr)(HANDLE, PBOOL);
typedef NTSTATUS(WINAPI* NtSetInformationThreadPtr)(HANDLE, THREADINFOCLASS, PVOID, ULONG);
typedef NTSTATUS(WINAPI* NtQueryInformationThreadPtr)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG);
typedef NTSTATUS(WINAPI* NtCreateThreadExPtr)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE, PVOID, PVOID, ULONG, ULONG_PTR, SIZE_T, SIZE_T, PVOID);

extern IsDebuggerPresentPtr OriginalIsDebuggerPresent;
extern CheckRemoteDebuggerPresentPtr OriginalCheckRemoteDebuggerPresent;
extern NtSetInformationThreadPtr OriginalNtSetInformationThread;
extern NtQueryInformationThreadPtr OriginalNtQueryInformationThread;
extern NtCreateThreadExPtr OriginalNtCreateThreadEx;