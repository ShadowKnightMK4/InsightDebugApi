#include "OriginalRoutinePts.h"

HANDLE WINAPI DetouredCreateFileA(
	               LPCSTR                lpFileName,
	               DWORD                 dwDesiredAccess,
	               DWORD                 dwShareMode,
	               LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	               DWORD                 dwCreationDisposition,
	               DWORD                 dwFlagsAndAttributes,
	               HANDLE                hTemplateFile
)
{
	return INVALID_HANDLE_VALUE;
}

HANDLE WINAPI DetouredCreateFileW(
	               LPCWSTR               lpFileName,
	               DWORD                 dwDesiredAccess,
	               DWORD                 dwShareMode,
	               LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	               DWORD                 dwCreationDisposition,
	               DWORD                 dwFlagsAndAttributes,
	               HANDLE                hTemplateFile
)
{
	return INVALID_HANDLE_VALUE;
}