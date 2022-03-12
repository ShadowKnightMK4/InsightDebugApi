#include "OriginalRoutinePtrs.h"

 HANDLE(WINAPI* OriginalGetProcessHeap)();
 DWORD(WINAPI* OriginalGetProcessHeaps)(DWORD, PHANDLE);
 LPVOID(WINAPI* OrignialHeapAlloc)(HANDLE, DWORD, SIZE_T);
 SIZE_T(WINAPI* OriginalHeapCompact)(HANDLE, DWORD);
 HANDLE(WINAPI* OriginalHeapCreate)(DWORD, SIZE_T, SIZE_T);
 BOOL(WINAPI* OriginalHeapFree)(HANDLE, DWORD, LPVOID);
 BOOL(WINAPI* OriginalHeapLock)(HANDLE);
BOOL(WINAPI* OriginalHeapQueryInformation)(HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T);
LPVOID(WINAPI* OriginalHeapRealloc)(HANDLE, DWORD, LPVOID, SIZE_T);
 BOOL(WINAPI* OriginalHeapSetInformation)(HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T);
 SIZE_T(WINAPI* OriginalHeapSize)(HANDLE, DWORD, LPCVOID);
BOOL(WINAPI* OriginalHeapUnLock)(HANDLE);
 BOOL(WINAPI* OriginalHeapValidate)(HANDLE, DWORD, LPCVOID);
BOOL(WINAPI* OriginalHeapWalk)(HANDLE, LPPROCESS_HEAP_ENTRY);





