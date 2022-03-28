#pragma once
#include <Windows.h>

HANDLE WINAPI GetProcessHeapDetour();

DWORD WINAPI GetProcessHeapsDetour(DWORD NumberOfHeaps, PHANDLE ProcessHeap);


LPVOID WINAPI HeapAllocDetour(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);

LPVOID WINAPI HeapCompactDetour(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);

HANDLE WINAPI HeapCreateDetour(DWORD flOptions, SIZE_T dwStartSize, SIZE_T dwMaxSize);

BOOL WINAPI HeapDestroyDetour(HANDLE hHeap);

BOOL WINAPI HeapFreeDetour(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);


BOOL WINAPI HeapLockDetour(HANDLE hHeap);

BOOL WINAPI HeapQueryInformationDetour(HANDLE HeapHandle,
	HEAP_INFORMATION_CLASS HeapInformationClass,
	PVOID                  HeapInformation,
	SIZE_T                 HeapInformationLength,
	PSIZE_T                ReturnLength
);

LPVOID HeapReAllocDetour(
	 HANDLE                 hHeap,
	 DWORD                  dwFlags,
	 _Frees_ptr_opt_ LPVOID lpMem,
	SIZE_T                 dwBytes
);

BOOL WINAPI HeapSetnformationDetour(HANDLE HeapHandle,
	HEAP_INFORMATION_CLASS HeapInformationClass,
	PVOID                  HeapInformation,
	SIZE_T                 HeapInformationLength,
	PSIZE_T                ReturnLength
);

BOOL WINAPI HeapUnlockDetour(HANDLE hHeap);

LPVOID WINAPI HeapValidateDetour(
	HANDLE                 hHeap,
	DWORD                  dwFlags,
	LPVOID lpMem
);

BOOL WINAPI HeapWalkDetour(
	HANDLE                 hHeap,
	LPPROCESS_HEAP_ENTRY lpEntry
);
