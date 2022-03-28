#pragma once
#include <Windows.h>

typedef HANDLE (WINAPI*CreateFileAPtr)(
	               LPCSTR                lpFileName,
	               DWORD                 dwDesiredAccess,
	               DWORD                 dwShareMode,
	               LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	               DWORD                 dwCreationDisposition,
			       DWORD                 dwFlagsAndAttributes,
	               HANDLE                hTemplateFile
);


typedef HANDLE (WINAPI* CreateFileWPtr)(
	               LPCWSTR               lpFileName,
	               DWORD                 dwDesiredAccess,
	               DWORD                 dwShareMode,
	               LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	               DWORD                 dwCreationDisposition,
	               DWORD                 dwFlagsAndAttributes,
	               HANDLE                hTemplateFile
);

typedef HANDLE (WINAPI* CreateFileTransactedAPtr)(
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


typedef HANDLE (WINAPI* CreateFileTransactedWPtr)(
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

typedef HANDLE (WINAPI* CreateFile2Ptr)(
	               LPCWSTR                           lpFileName,
	               DWORD                             dwDesiredAccess,
	               DWORD                             dwShareMode,
	               DWORD                             dwCreationDisposition,
	               LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams
);

typedef BOOL(WINAPI* CloseHandlePtr)(HANDLE hObject);



extern CreateFileAPtr OriginalCreateFileA;
extern CreateFileWPtr OriginalCreateFileW;
extern CreateFileTransactedAPtr OriginalCreateFileTransactedA;
extern CreateFileTransactedWPtr OriginalCreateFileTransactedW;
extern CreateFile2Ptr OriginalCreateFile2;
extern CloseHandlePtr OriginalCloseHandle;

