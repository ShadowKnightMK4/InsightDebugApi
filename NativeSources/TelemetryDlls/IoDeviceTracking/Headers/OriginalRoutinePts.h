#pragma once
#include <Windows.h>
#include <winternl.h>

typedef BOOL (WINAPI* WriteFilePtr)(
	                    HANDLE       hFile,
	                    LPCVOID      lpBuffer,
	                    DWORD        nNumberOfBytesToWrite,
	                    LPDWORD      lpNumberOfBytesWritten,
	                    LPOVERLAPPED lpOverlapped);

 typedef NTSTATUS (WINAPI* NtWriteFilePtr)(
	               HANDLE           FileHandle,
	               HANDLE           Event,
	               PIO_APC_ROUTINE  ApcRoutine,
	               PVOID            ApcContext,
	               PIO_STATUS_BLOCK IoStatusBlock,
	               PVOID            Buffer,
	               ULONG            Length,
	               PLARGE_INTEGER   ByteOffset,
	               PULONG           Key);

typedef BOOL (WINAPI* ReadFilePtr)(
	                     HANDLE       hFile,
	                     LPVOID       lpBuffer,
	                     DWORD        nNumberOfBytesToRead,
	                     LPDWORD      lpNumberOfBytesRead,
	                     LPOVERLAPPED lpOverlapped);
typedef  NTSTATUS (WINAPI* NtReadFilePtr)(
	               HANDLE           FileHandle,
	               HANDLE           Event,
	               /*PIO_APC_ROUTINE  ApcRoutine*/
				   VOID* ApcRoutine,
	               PVOID            ApcContext,
		           /*PIO_STATUS_BLOCK IoStatusBlock*/
				   VOID* IoStatusBlock,
	               PVOID            Buffer,
	               ULONG            Length,
	               PLARGE_INTEGER   ByteOffset,
	               PULONG           Key);


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


typedef  NTSTATUS(WINAPI* NtCreateFilePtr)(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	VOID* ObjectAttributes,
	//POBJECT_ATTRIBUTES ObjectAttributes,
	VOID* IoStatusBlock,
	//PIO_STATUS_BLOCK   IoStatusBlock,
	PLARGE_INTEGER     AllocationSize,
	ULONG              FileAttributes,
	ULONG              ShareAccess,
	ULONG              CreateDisposition,
	ULONG              CreateOptions,
	PVOID              EaBuffer,
	ULONG              EaLength);

typedef NTSTATUS( WINAPI*  NtOpenFilePtr)(
	       PHANDLE            FileHandle,
		      ACCESS_MASK        DesiredAccess,
			  VOID* ObjectAttributes,
			  //POBJECT_ATTRIBUTES ObjectAttributes,
			  VOID* IoStatusBlock,
//		      PIO_STATUS_BLOCK   IoStatusBlock,
		      ULONG              ShareAccess,
		      ULONG              OpenOptions
	);

extern NtWriteFilePtr OriginalNtWriteFile;
extern NtReadFilePtr OriginalNtReadFile;
extern WriteFilePtr OriginalWriteFile;
extern ReadFilePtr OriginalReadFile;


extern CreateFileAPtr OriginalCreateFileA;
extern CreateFileWPtr OriginalCreateFileW;
extern CreateFileTransactedAPtr OriginalCreateFileTransactedA;
extern CreateFileTransactedWPtr OriginalCreateFileTransactedW;
extern CreateFile2Ptr OriginalCreateFile2;
extern CloseHandlePtr OriginalCloseHandle;
extern NtCreateFilePtr OriginalNtCreateFile;
extern NtOpenFilePtr OriginalNtOpenFile;


