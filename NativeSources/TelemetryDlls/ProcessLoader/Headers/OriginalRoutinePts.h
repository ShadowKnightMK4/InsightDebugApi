#pragma once
#include <Windows.h>
#include <winternl.h>



typedef BOOL (WINAPI* CreateProcessWPtr)(
	                    LPCWSTR               lpApplicationName,
	                    LPWSTR                lpCommandLine,
	                    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	                    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	                    BOOL                  bInheritHandles,
	                    DWORD                 dwCreationFlags,
	                    LPVOID                lpEnvironment,
	                    LPCWSTR               lpCurrentDirectory,
	                    LPSTARTUPINFOW        lpStartupInfo,
	                    LPPROCESS_INFORMATION lpProcessInformation);

typedef FARPROC(WINAPI* GetProcAddressPtr)(
	HMODULE hModule,
	LPCSTR  lpProcName);

typedef HMODULE(WINAPI* LoadLibraryExWPtr)(
	LPCWSTR lpLibFileName,
	HANDLE  hFile,
	DWORD   dwFlags);

typedef HMODULE(WINAPI* LoadLibraryExAPtr)(
	LPCSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags);


typedef HMODULE(WINAPI* LoadLibraryAPtr)(
	LPCSTR lpLibFileName);

typedef HMODULE(WINAPI* LoadLibraryWPtr)(
	LPCWSTR lpLibFileName);


extern GetProcAddressPtr OriginalGetProcAddress;
extern LoadLibraryExWPtr OriginalLoadLibraryExW;
extern LoadLibraryExAPtr OriginalLoadLibraryExA;

extern LoadLibraryWPtr OriginalLoadLibraryW;
extern LoadLibraryAPtr OriginalLoadLibraryA;
extern CreateProcessWPtr OriginalCreateProcessW;