
/*
* DetourPayloads.cpp
* 
* Handle enumarting/ viewing the detour payloads (if any) and setting payloads used by the helper dll
* 
*/
#include <Windows.h>
//#include <StaticIncludes.h>
#include "detours.h"


// lifed from detours.h
#define MM_ALLOCATION_GRANULARITY 0x10000


/// <summary>
/// WARNING: BROKEN  This code attempts to let someone enumate the modules of the hProcess apssed. Mostly lifted from DetourEnumerateModules
/// </summary>
/// <param name="hProcess"></param>
/// <param name="hModuleLast"></param>
/// <returns></returns>
HMODULE WINAPI DetourEnumerateModulesEx(HANDLE hProcess,_In_opt_ HMODULE hModuleLast)
{
    /*
    PBYTE pbLast = (PBYTE)hModuleLast + MM_ALLOCATION_GRANULARITY;
    IMAGE_DOS_HEADER LocalDosHeader;
    IMAGE_NT_HEADERS LocalNtHeaders;
    if (hProcess == GetCurrentProcess())
    {
        return DetourEnumerateModules(hModuleLast);
    }
    MEMORY_BASIC_INFORMATION mbi;
    ZeroMemory(&mbi, sizeof(mbi));

    // Find the next memory region that contains a mapped PE image.
    //
    for (;; pbLast = (PBYTE)mbi.BaseAddress + mbi.RegionSize) {
        if (VirtualQueryEx(hProcess, pbLast, &mbi, sizeof(mbi)) <= 0) {
            break;
        }

        // Skip uncommitted regions and guard pages.
        //
        if ((mbi.State != MEM_COMMIT) ||
            ((mbi.Protect & 0xff) == PAGE_NOACCESS) ||
            (mbi.Protect & PAGE_GUARD)) {
            continue;
        }

        __try {
            DWORD BytesRead = 0;
            if ( (ReadProcessMemory(hProcess, pbLast, &LocalDosHeader, sizeof(IMAGE_DOS_HEADER), &BytesRead) == TRUE) && (BytesRead == sizeof(IMAGE_DOS_HEADER))) {
                //PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pbLast;
                if (LocalDosHeader.e_magic != IMAGE_DOS_SIGNATURE ||
                    (DWORD)LocalDosHeader.e_lfanew > mbi.RegionSize ||
                    (DWORD)LocalDosHeader.e_lfanew < sizeof(LocalDosHeader)) {
                    continue;
                }

                PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)&LocalDosHeader +
                    LocalDosHeader.e_lfanew);
                if ((ReadProcessMemory(hProcess, pNtHeader, &LocalNtHeaders, sizeof(IMAGE_NT_HEADERS), &BytesRead) &&
                    (BytesRead == sizeof(IMAGE_NT_HEADERS)))) {
                    if (LocalNtHeaders.Signature != IMAGE_NT_SIGNATURE) {
                        continue;
                    }
                }
                else
                {
                    SetLastError(5);
                }

                SetLastError(NO_ERROR);
                return (HMODULE)pbLast;
            }
        }
#pragma prefast(suppress:28940, "A bad pointer means this probably isn't a PE header.")
            __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
                EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
                continue;
            }
        }*/
    return NULL;
}
    


void DetourRemotePayloadEnum()
{
    DetourEnumerateModules(0);
}