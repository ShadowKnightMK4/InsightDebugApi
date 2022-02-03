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
 extern NtCreateFilePtr NtCreateFileOriginal;


 NTSTATUS NTAPI NtCreateUserProcesDetoured(
	 _Out_ PHANDLE ProcessHandle,
	 _Out_ PHANDLE ThreadHandle,
	 _In_ ACCESS_MASK ProcessDesiredAccess,
	 _In_ ACCESS_MASK ThreadDesiredAccess,
	 _In_opt_ POBJECT_ATTRIBUTES ProcessObjectAttributes,
	 _In_opt_ POBJECT_ATTRIBUTES ThreadObjectAttributes,
	 _In_ ULONG ProcessFlags, // PROCESS_CREATE_FLAGS_*
	 _In_ ULONG ThreadFlags, // THREAD_CREATE_FLAGS_*
	 _In_opt_ PRTL_USER_PROCESS_PARAMETERS ProcessParameters, // PRTL_USER_PROCESS_PARAMETERS
	 _Inout_ PPS_CREATE_INFO CreateInfo,
	 _In_opt_ PPS_ATTRIBUTE_LIST AttributeList);


 // Vista and above; will contain pointer to NtCreateUserProcess gotten by GetProcAddress()
 extern NtCreateUserProcessPtr NtCreateUserProcessOriginal;
