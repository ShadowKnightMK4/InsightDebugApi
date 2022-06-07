#pragma once
#include <Windows.h>

HMODULE WINAPI DetouredLoadLibraryW(
	     LPCWSTR lpLibFileName);

HMODULE WINAPI DetouredLoadLibraryA(
	LPCSTR lpLibFileName);

FARPROC WINAPI DetouredGetProcAddress(
	HMODULE hModule,
	LPCSTR  lpProcName);

HMODULE WINAPI DetouredLoadLibraryExW(
	LPCWSTR lpLibFileName,
	HANDLE  hFile,
	DWORD   dwFlags);

HMODULE WINAPI DetouredLoadLibraryExA(
	LPCSTR lpLibFileName,
	HANDLE  hFile,
	DWORD   dwFlags);

BOOL  WINAPI DetouredCreateProcessW(
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

