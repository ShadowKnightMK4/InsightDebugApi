#pragma once
#include <Windows.h>


extern HANDLE(WINAPI* OriginalGetProcessHeap)();
extern DWORD(WINAPI* OriginalGetProcessHeaps)(DWORD, PHANDLE);
extern LPVOID(WINAPI* OrignialHeapAlloc)(HANDLE, DWORD, SIZE_T);
extern SIZE_T(WINAPI* OriginalHeapCompact)(HANDLE, DWORD);
extern HANDLE(WINAPI* OriginalHeapCreate)(DWORD, SIZE_T, SIZE_T);
extern BOOL(WINAPI* OriginalHeapFree)(HANDLE, DWORD, LPVOID);
extern BOOL(WINAPI* OriginalHeapLock)(HANDLE);
extern BOOL(WINAPI* OriginalHeapQueryInformation)(HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T);
extern LPVOID(WINAPI* OriginalHeapRealloc)(HANDLE, DWORD, LPVOID, SIZE_T);
extern BOOL(WINAPI* OriginalHeapSetInformation)(HANDLE, HEAP_INFORMATION_CLASS, PVOID,SIZE_T);
extern SIZE_T(WINAPI* OriginalHeapSize)(HANDLE, DWORD, LPCVOID);
extern BOOL(WINAPI* OriginalHeapUnLock)(HANDLE);
extern BOOL(WINAPI* OriginalHeapValidate)(HANDLE, DWORD, LPCVOID);
extern BOOL(WINAPI* OriginalHeapWalk)(HANDLE, LPPROCESS_HEAP_ENTRY);



