
#include <windows.h>
#include "detours.h"
#include <Psapi.h>

extern "C" {
    BOOL DetourEnumerateImportsExRemote(
        _In_opt_ HMODULE hModule,
        _In_opt_ PVOID pContext,
        _In_opt_ PF_DETOUR_IMPORT_FILE_CALLBACK pfImportFile,
        _In_opt_ PF_DETOUR_IMPORT_FUNC_CALLBACK_EX pfImportFunc,
        _In_ DWORD dwProcessID)
    {
        IMAGE_DOS_HEADER dos_header = { 0 };
        IMAGE_NT_HEADERS WindowsHeader = { 0 };
        BOOL ret = FALSE;
        HANDLE Process = 0;
        SIZE_T bytesRead = 0;
        HINSTANCE remotemain = 0;

        __try
        {
            Process = OpenProcess(dwProcessID, FALSE, PROCESS_VM_READ | PROCESS_VM_OPERATION);
            if (Process == 0)
            {
                SetLastError(ERROR_ACCESS_DENIED);
                __leave;
            }
            EnumProcessModules(Process, &remotemain, 4, (LPDWORD)&bytesRead);
            {
                if (ReadProcessMemory(Process, (LPCVOID)remotemain, &dos_header, sizeof(IMAGE_DOS_HEADER), &bytesRead))
                {
                    if (dos_header.e_magic != IMAGE_DOS_SIGNATURE)
                    {
                        SetLastError(ERROR_BAD_EXE_FORMAT);
                        __leave;
                    }
                    if (ReadProcessMemory(Process, remotemain + dos_header.e_lfanew, & WindowsHeader, sizeof(WindowsHeader),  &bytesRead))
                    {
                        if (WindowsHeader.FileHeader.SizeofOptionalHeader == 0)
                    }

                }
            }
        }
        __finally
        {
            if (Process != 0)
            {
                CloseHandle(Process);
            }
        }
        return ret;
    }
}