#pragma once
/// <summary>
/// Callback for NtCreateFile
/// </summary>
typedef NTSTATUS(WINAPI* NtCreateFilePtr)(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK   IoStatusBlock,
	PLARGE_INTEGER     AllocationSize,
	ULONG              FileAttributes,
	ULONG              ShareAccess,
	ULONG              CreateDisposition,
	ULONG              CreateOptions,
	PVOID              EaBuffer,
	ULONG              EaLength
	);

typedef NTSTATUS(WINAPI* NtOpenFilePtr)(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK   IoStatusBlock,
	ULONG              ShareAccess,
	ULONG              OpenOptions
);


NTSTATUS WINAPI NtOpenFileDetoured(
	 PHANDLE            FileHandle,
	  ACCESS_MASK        DesiredAccess,
	  POBJECT_ATTRIBUTES ObjectAttributes,
	 PIO_STATUS_BLOCK   IoStatusBlock,
	  ULONG              ShareAccess,
	  ULONG              OpenOptions
);

 /// <summary>
 /// Prototype for our Detour NtCreateFile
 /// </summary>
 /// <param name="FileHandle"></param>
 /// <param name="DesiredAccess"></param>
 /// <param name="ObjectAttributes"></param>
 /// <param name="IoStatusBlock"></param>
 /// <param name="AllocationSize"></param>
 /// <param name="FileAttributes"></param>
 /// <param name="ShareAccess"></param>
 /// <param name="CreateDisposition"></param>
 /// <param name="CreateOptions"></param>
 /// <param name="EaBuffer"></param>
 /// <param name="EaLength"></param>
 /// <returns></returns>
 NTSTATUS WINAPI NtCreateFileDetoured(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK   IoStatusBlock,
	PLARGE_INTEGER     AllocationSize,
	ULONG              FileAttributes,
	ULONG              ShareAccess,
	ULONG              CreateDisposition,
	ULONG              CreateOptions,
	PVOID              EaBuffer,
	ULONG              EaLength
);



 /// <summary>
 /// Reference for our pointer to NtCreateFile received via GetProcAddress()
 ///  </summary> 
 extern NtCreateFilePtr OriginalNtCreateFile;


 extern NtOpenFilePtr OriginalNtOpenFile;

