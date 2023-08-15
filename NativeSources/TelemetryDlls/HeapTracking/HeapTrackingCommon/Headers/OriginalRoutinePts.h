#pragma once
#include <Windows.h>
#include <winternl.h>
typedef DWORD(WINAPI* GetProcessHeapsPtr)(DWORD NumberOfHeaps, PHANDLE ProcessHeaps);

typedef LPVOID(WINAPI* HeapAllocPtr)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);

typedef SIZE_T(WINAPI* HeapCompactPtr)(HANDLE hHeap, DWORD dwFlags);

typedef HANDLE(WINAPI* HeapCreatePtr)(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);

typedef BOOL(WINAPI* HeapDestroyPtr)(HANDLE hHeap);

typedef BOOL(WINAPI* HeapLockPtr)(HANDLE hHeap);

typedef BOOL(WINAPI* HeapQueryInformationPtr)(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass, PVOID HeapInformation, SIZE_T HeapInformationLength, PSIZE_T ReturnLength);

typedef LPVOID(WINAPI* HeapReAllocPtr)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes);

typedef BOOL(WINAPI* HeapSetInformationPtr)(HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass, PVOID HeapInformation, SIZE_T HeapInformationLength);

typedef SIZE_T(WINAPI* HeapSizePtr)(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);

typedef BOOL(WINAPI* HeapUnlockPtr)(HANDLE hHeap);

typedef BOOL(WINAPI* HeapValidatePtr)(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);

typedef BOOL(WINAPI* HeapWalkPtr)(HANDLE hHeap, LPPROCESS_HEAP_ENTRY lpEntry);



extern GetProcessHeapsPtr OriginalGetProcessHeaps;

extern HeapAllocPtr OriginalHeapAlloc;

extern HeapCompactPtr OriginalHeapCompact;

extern HeapCreatePtr OriginalHeapCreate;

extern HeapDestroyPtr OriginalHeapDestroy;

extern HeapLockPtr OriginalHeapLock;

extern HeapQueryInformationPtr OriginalHeapQueryInformation;

extern HeapReAllocPtr OriginalHeapReAlloc;

extern HeapSetInformationPtr OriginalHeapSetInformation;

extern HeapSizePtr OriginalHeapSize;

extern HeapUnlockPtr OriginalHeapUnlock;

extern HeapValidatePtr OriginalHeapValidate;

extern HeapWalkPtr OriginalHeapWalk;
