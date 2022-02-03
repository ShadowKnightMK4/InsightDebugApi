#pragma once
#include <Windows.h>
#include <map>
#include <string>

typedef HMODULE(*WINAPI LoadLibraryAPtr)(LPCSTR);
typedef HMODULE(*WINAPI LoadLibraryWPtr)(LPCWSTR);

typedef HMODULE(*WINAPI LoadLibraryExAPtr)(LPCSTR, HANDLE, DWORD);
typedef HMODULE(*WINAPI LoadLibraryExWPtr)(LPCWSTR, HANDLE, DWORD);
typedef BOOL(*WINAPI SetDllDirectoryAPtr)(LPCSTR);
typedef BOOL(*WINAPI SetDllDirectoryWPtr)(LPCWSTR);
typedef DLL_DIRECTORY_COOKIE(*WINAPI AddDllDirectoryAPtr)(LPCSTR);
typedef DLL_DIRECTORY_COOKIE(*WINAPI AddDllDirectoryWPtr)(LPCWSTR);
typedef BOOL(*WINAPI RemoveDllDirectoryPtr)(DLL_DIRECTORY_COOKIE);
typedef BOOL(*WINAPI SetDefaultDllDirectoriesPtr)(DWORD);



extern SetDllDirectoryAPtr OriginalSetDllDirectoryA;
extern SetDllDirectoryWPtr OriginalSetDllDirectoryW;
extern AddDllDirectoryAPtr OriginalAddDllDirectoryA;
extern AddDllDirectoryWPtr OriginalAddDllDirectoryW;
extern RemoveDllDirectoryPtr OriginalRemoveDllDirectory;
extern SetDefaultDllDirectoriesPtr OriginalSetDefaultDllDirectories;

/// <summary>
/// Once DLLMain Runs, Will point to Window's LoadLibraryA
/// </summary>
extern LoadLibraryAPtr OriginalLoadLibraryA;
/// <summary>
/// Once DLLMain Runs, Will point to Window's LoadLibraryW
/// </summary>
extern LoadLibraryWPtr OriginalLoadLibraryW;
/// <summary>
/// Once DLLMain Runs, Will point to Window's LoadLibraryExA
/// </summary>
extern LoadLibraryExAPtr OriginalLoadLibraryExA;
/// <summary>
/// Once DLLMain Runs, Will point to Window's LoadLibraryExW
/// </summary>
extern LoadLibraryExWPtr OriginalLoadLibraryExW;


HMODULE WINAPI DetouredLoadLibraryExW(
    LPCWSTR lpLibFileName,
    HANDLE  hFile,
    DWORD   dwFlags
);

HMODULE WINAPI DetouredLoadLibraryExA(
    LPCSTR lpLibFileName,
    HANDLE hFile,
    DWORD  dwFlags
);

HMODULE WINAPI DetouredLoadLibraryA(
    LPCSTR lpLibFileName
);

HMODULE WINAPI DetouredLoadLibraryW(
    LPCWSTR lpLibFileName
);

/// <summary>
/// Deals with the payload that is a list of path;path2;path3; ect....
/// </summary>
/// <param name="PayloadPtr"></param>
/// <param name="PayloadSize"></param>
void ExtractPrioritySearchPathPayload(PVOID PayloadPtr, DWORD PayloadSize);

/// <summary>
/// After reverting the dll search path to default or (before updating it), this ads the entries in  ForceDllSearchPathCookies to be searched
/// </summary>
/// <returns></returns>
void WINAPI AddDllSearchPathBack();