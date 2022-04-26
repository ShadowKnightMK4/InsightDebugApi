#pragma once
#include <Windows.h>


HANDLE WINAPI DetouredCreateFileA(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	);


 HANDLE WINAPI DetouredCreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	);

 HANDLE WINAPI DetouredCreateFileTransactedA(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile,
	HANDLE                hTransaction,
	PUSHORT               pusMiniVersion,
	PVOID                 lpExtendedParameter
	);


HANDLE WINAPI DetouredCreateFileTransactedW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile,
	HANDLE                hTransaction,
	PUSHORT               pusMiniVersion,
	PVOID                 lpExtendedParameter
	);

 HANDLE WINAPI DetouredCreateFile2(
	LPCWSTR                           lpFileName,
	DWORD                             dwDesiredAccess,
	DWORD                             dwShareMode,
	DWORD                             dwCreationDisposition,
	LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams
	);

 BOOL WINAPI DetouredCloseHandle(HANDLE hObject);

