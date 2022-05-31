#include <Windows.h>
#include "detours.h"
#include <winternl.h>
#include <strsafe.h>
#include <string>


/*
0 as PsCreateInitialState;
1 as PsCreateFailOnFileOpen;
2 as PsCreateFailOnSectionCreate;
3 as PsCreateFailExeFormat;
4 as PsCreateFailMachineMismatch;
5 as PsCreateFailExeName;
6 as PsCreateSuccess;
7 as PsCreateMaximumStates.
*/
/* from https://www.geoffchappell.com/studies/windows/km/ntoskrnl/api/ps/psexec/create_info.htm*/
struct PS_CREATE_INFO
{
    ULONG_PTR Size;
    DWORD State;
    union
    {
        struct {

        } InitState;
        struct {
            HANDLE FileHandle;
        } FailSection;
        struct {
            USHORT DllCharactoristics;
        } ExeFormat;
        struct {
            HANDLE IFEOKey;
        } ExeName;;
        struct {

        } SucessState;
    };
};
/*
* So the objective of this thing is replace message boxes with one that has an altered capture
* and propergate the payload when it is spawned to another target.
* 
* typedef BOOL (WINAPI *PDETOUR_CREATE_PROCESS_ROUTINEW)(
*/
typedef NTSTATUS(WINAPI* NtCreateUserProcessPtr)(
    PHANDLE ProcessHandle,
    PHANDLE ThreadHandle,
    ACCESS_MASK ProcessDesiredAccess,
    ACCESS_MASK ThreadDesiredAccess,
    POBJECT_ATTRIBUTES ProcessObjectAttributes,
    POBJECT_ATTRIBUTES ThreadObjectAttributes,
    ULONG ProcessFlags,
    ULONG ThreadFlags,
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
    VOID* CreateInfo,
    VOID* AttributeList    );

HMODULE dll = 0;
NtCreateUserProcessPtr* NtCreateUserProcess = 0;


BOOL WINAPI AllocExeHelper(_Out_ PDETOUR_EXE_HELPER* pHelper,
    _In_ DWORD dwTargetPid,
    _In_ DWORD nDlls,
    _In_reads_(nDlls) LPCSTR* rlpDlls)
{
    PDETOUR_EXE_HELPER Helper = NULL;
    BOOL Result = FALSE;
    _Field_range_(0, cSize - 4) DWORD cOffset = 0;
    DWORD cSize = 4;

    if (pHelper == NULL) {
        goto Cleanup;
    }
    *pHelper = NULL;

    if (nDlls < 1 || nDlls > 4096) {
        SetLastError(ERROR_INVALID_PARAMETER);
        goto Cleanup;
    }

    for (DWORD n = 0; n < nDlls; n++) {
        HRESULT hr;
        size_t cchDest = 0;

        hr = StringCchLengthA(rlpDlls[n], 4096, &cchDest);
        if (!SUCCEEDED(hr)) {
            goto Cleanup;
        }

        cSize += (DWORD)cchDest + 1;
    }

    Helper = (PDETOUR_EXE_HELPER) new BYTE[sizeof(DETOUR_EXE_HELPER) + cSize];
    if (Helper == NULL) {
        goto Cleanup;
    }

    Helper->cb = sizeof(DETOUR_EXE_HELPER) + cSize;
    Helper->pid = dwTargetPid;
    Helper->nDlls = nDlls;

    for (DWORD n = 0; n < nDlls; n++) {
        HRESULT hr;
        size_t cchDest = 0;

        if (cOffset > 0x10000 || cSize > 0x10000 || cOffset + 2 >= cSize) {
            goto Cleanup;
        }

        if (cOffset + 2 >= cSize || cOffset + 65536 < cSize) {
            goto Cleanup;
        }

        _Analysis_assume_(cOffset + 1 < cSize);
        _Analysis_assume_(cOffset < 0x10000);
        _Analysis_assume_(cSize < 0x10000);

        PCHAR psz = &Helper->rDlls[cOffset];

        hr = StringCchCopyA(psz, cSize - cOffset, rlpDlls[n]);
        if (!SUCCEEDED(hr)) {
            goto Cleanup;
        }

        // REVIEW 28020 The expression '1<=_Param_(2)& &_Param_(2)<=2147483647' is not true at this call.
        // REVIEW 28313 Analysis will not proceed past this point because of annotation evaluation. The annotation expression *_Param_(3)<_Param_(2)&&*_Param_(3)<=stringLength$(_Param_(1)) cannot be true under any assumptions at this point in the program.
#pragma warning(suppress:28020 28313)
        hr = StringCchLengthA(psz, cSize - cOffset, &cchDest);
        if (!SUCCEEDED(hr)) {
            goto Cleanup;
        }

        // Replace "32." with "64." or "64." with "32."

        for (DWORD c = (DWORD)cchDest + 1; c > 3; c--) {
#if DETOURS_32BIT
            if (psz[c - 3] == '3' && psz[c - 2] == '2' && psz[c - 1] == '.') {
                psz[c - 3] = '6'; psz[c - 2] = '4';
                break;
            }
#else
            if (psz[c - 3] == '6' && psz[c - 2] == '4' && psz[c - 1] == '.') {
                psz[c - 3] = '3'; psz[c - 2] = '2';
                break;
            }
#endif
        }

        cOffset += (DWORD)cchDest + 1;
    }

    *pHelper = Helper;
    Helper = NULL;
    Result = TRUE;

Cleanup:
    if (Helper != NULL) {
        delete[](PBYTE)Helper;
        Helper = NULL;
    }
    return Result;
}

VOID WINAPI FreeExeHelper(PDETOUR_EXE_HELPER* pHelper)
{
    if (*pHelper != NULL) {
        delete[](PBYTE)* pHelper;
        *pHelper = NULL;
    }
}


BOOL WINAPI MyDetourProcessViaHelperDllsW(_In_ DWORD dwTargetPid,
    _In_ DWORD nDlls,
    _In_reads_(nDlls) LPCSTR* rlpDlls,
    _In_ PDETOUR_CREATE_PROCESS_ROUTINEW pfCreateProcessW)
{
    BOOL Result = FALSE;
    PROCESS_INFORMATION pi;
    STARTUPINFOW si;
    WCHAR szExe[MAX_PATH];
    WCHAR szCommand[MAX_PATH];
    PDETOUR_EXE_HELPER helper = NULL;
    HRESULT hr;
    WCHAR szDllName[MAX_PATH];
    int cchWrittenWideChar;
    DWORD nLen = GetEnvironmentVariableW(L"WINDIR", szExe, ARRAYSIZE(szExe));

    
    if (nDlls < 1 || nDlls > 4096) {
        SetLastError(ERROR_INVALID_PARAMETER);
        goto Cleanup;
    }
    if (!AllocExeHelper(&helper, dwTargetPid, nDlls, rlpDlls)) {
        goto Cleanup;
    }

    if (nLen == 0 || nLen >= ARRAYSIZE(szExe)) {
        goto Cleanup;
    }

#if DETOURS_OPTION_BITS
#if DETOURS_32BIT
    hr = StringCchCatW(szExe, ARRAYSIZE(szExe), L"\\sysnative\\rundll32.exe");
#else // !DETOURS_32BIT
    hr = StringCchCatW(szExe, ARRAYSIZE(szExe), L"\\syswow64\\rundll32.exe");
#endif // !DETOURS_32BIT
#else // DETOURS_OPTIONS_BITS
    hr = StringCchCatW(szExe, ARRAYSIZE(szExe), L"\\system32\\rundll32.exe");
#endif // DETOURS_OPTIONS_BITS
    if (!SUCCEEDED(hr)) {
        goto Cleanup;
    }

    //for East Asia languages and so on, like Chinese, print format with "%hs" can not work fine before user call _tsetlocale(LC_ALL,_T(".ACP"));
    //so we can't use "%hs" in format string, because the dll that contain this code would inject to any process, even not call _tsetlocale(LC_ALL,_T(".ACP")) before

    cchWrittenWideChar = MultiByteToWideChar(CP_ACP, 0, &helper->rDlls[0], -1, szDllName, ARRAYSIZE(szDllName));
    if (cchWrittenWideChar >= ARRAYSIZE(szDllName) || cchWrittenWideChar <= 0) {
        goto Cleanup;
    }
    hr = StringCchPrintfW(szCommand, ARRAYSIZE(szCommand),
        L"rundll32.exe \"%s\",#1", szDllName);
    if (!SUCCEEDED(hr)) {
        goto Cleanup;
    }

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);


    RTL_USER_PROCESS_PARAMETERS Args;
    ZeroMemory(&Args, sizeof(RTL_USER_PROCESS_PARAMETERS));
    Args.ImagePathName.Buffer = szExe;
    Args.ImagePathName.Length = std::wcslen(szExe);
    Args.ImagePathName.MaximumLength = Args.ImagePathName.Length+2;


    Args.CommandLine.Buffer = szCommand;
    Args.CommandLine.Length = std::wcslen(szCommand);
    Args.CommandLine.MaximumLength = Args.CommandLine.Length + 2;

    if (pfCreateProcessW(szExe, szCommand, NULL, NULL, FALSE, CREATE_SUSPENDED,
        NULL, NULL, &si, &pi)){
   
        if (!DetourCopyPayloadToProcess(pi.hProcess,
            DETOUR_EXE_HELPER_GUID,
            helper, helper->cb)) {
            
            TerminateProcess(pi.hProcess, ~0u);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            goto Cleanup;
        }

        ResumeThread(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD dwResult = 500;
        GetExitCodeProcess(pi.hProcess, &dwResult);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (dwResult != 0) {
        
            goto Cleanup;
        }
        Result = TRUE;
    }
    else {
        
        goto Cleanup;
    }

Cleanup:
    FreeExeHelper(&helper);
    return Result;
}

const char* rlpDlls[] = { "C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x64\\program\\Samples\\DummyDll6464.dll" };
DWORD nDlls = 1;

NTSTATUS
NTAPI
DetouredNtCreateUserProcess(
      PHANDLE ProcessHandle,
      PHANDLE ThreadHandle,
      ACCESS_MASK ProcessDesiredAccess,
      ACCESS_MASK ThreadDesiredAccess,
      POBJECT_ATTRIBUTES ProcessObjectAttributes,
      POBJECT_ATTRIBUTES ThreadObjectAttributes,
      ULONG ProcessFlags,
      ULONG ThreadFlags,
      PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
      VOID* CreateInfo,
      PS_CREATE_INFO* AttributeList
      //BOOL DISABLE_DETOURS /* Extra flag in which turn off detours */
)
{
    LONG result = 0;
    ULONG SuspendedFlags = ProcessFlags | CREATE_SUSPENDED;
    /**ProcessHandle = 0;
    *ThreadHandle = 0;
   */

  
    


    result = (*NtCreateUserProcess)(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, ThreadObjectAttributes, SuspendedFlags, ThreadFlags, ProcessParameters, CreateInfo, AttributeList);

    if (!0)
    {
        if ((result != 0) && (ThreadHandle != 0))
        {
            if (!DetourUpdateProcessWithDll(*ProcessHandle, rlpDlls, nDlls) &&
                !MyDetourProcessViaHelperDllsW(GetProcessId(*ProcessHandle),
                    nDlls,
                    rlpDlls,
                    0)) {

                TerminateProcess(*ProcessHandle, ~0u);
                CloseHandle(*ProcessHandle);
                CloseHandle(*ThreadHandle);
                return FALSE;
            }

            if (!(ProcessFlags & CREATE_SUSPENDED)) {
                ResumeThread(*ThreadHandle);
            }

        }
    }
    else
    {
        ResumeThread(*ThreadHandle);
    }
    return result;
}

int main()
{
    dll = LoadLibrary(L"ntdll.dll");
    NtCreateUserProcess = (NtCreateUserProcessPtr*)GetProcAddress(dll, "NtCreateUserProcess");

    /*DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)NtCreateUserProcess, DetouredNtCreateUserProcess);
    DetourTransactionCommit();
    */
    
    (*((INT_PTR*)NtCreateUserProcess)) = ((INT_PTR)&DetouredNtCreateUserProcess);

    PROCESS_INFORMATION pif;
	STARTUPINFO info = { 0 };
	info.cb = sizeof(info);
	info.wShowWindow = SW_NORMAL;
	return DetourCreateProcessWithDllEx(L"C:\\WINDOWS\\system32\\notepad.exe", nullptr, nullptr, nullptr, FALSE, 0, 0, 0, &info, &pif, "C:\\Users\\Thoma\\source\\repos\\InsightAPI\\code\\Debug\\x64\\program\\Samples\\DummyDll6464.dll", nullptr);


}