#pragma once
#include <Windows.h>
#include <winternl.h>

DWORD DetouredGetProcessHeaps(DWORD NumberOfHeaps, PHANDLE ProcessHeaps);
LPVOID DetouredHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
SIZE_T DetouredHeapCompact(HANDLE hHeap, DWORD dwFlags);
HANDLE DetouredHeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
BOOL DetouredHeapDestroy(HANDLE hHeap);
BOOL DetouredHeapLock(HANDLE hHeap);
BOOL DetouredHeapQueryInformation(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass,
    PVOID HeapInformation, SIZE_T HeapInformationLength, PSIZE_T ReturnLength);
LPVOID DetouredHeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes);
BOOL DetouredHeapSetInformation(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass,
    PVOID HeapInformation, SIZE_T HeapInformationLength);
SIZE_T DetouredHeapSize(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);
BOOL DetouredHeapUnlock(HANDLE hHeap);
BOOL DetouredHeapValidate(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);
BOOL DetouredHeapWalk(HANDLE hHeap, LPPROCESS_HEAP_ENTRY lpEntry);