#include "FileSandBoxHelperDll.h"
#include "ReplacementFileFileFunctions.h"
#include <winternl.h>
#include <vector>
#include "../FilesandboxClientDllProtocol/FileSandboxProtocol.h"


int ExceptionCheck(DWORD ExceptionCode, DWORD ExceptionMessage)
{
	if (ExceptionCode == ExceptionMessage)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

#define SELF_CHECK( x,  y )     ((ExceptionCheck( (x),  (y) )))



NtCreateUserProcessPtr NtCreateUserProcessOriginal;



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
	_In_opt_ PPS_ATTRIBUTE_LIST AttributeList)
{
	ULONG_PTR Args[EXCEPTION_MAXIMUM_PARAMETERS];
	FILESANDBOX_API_EXCEPTION_CLASS ExpMessage;
	NTSTATUS result = E_FAIL;
	ULONG_PTR* ArgList;
	memset(&Args, 0, sizeof(Args));


	PHANDLE DupProcessHandle;
	PHANDLE DupThreadHandle;
	ACCESS_MASK DupProcessDesiredAccess;
	ACCESS_MASK DupThreadDesiredAccess;
	POBJECT_ATTRIBUTES DupProcessObjectAttributes;
	POBJECT_ATTRIBUTES DupThreadObjectAttributes;
	ULONG DupProcessFlags;
	ULONG DupThreadFlags;
	PRTL_USER_PROCESS_PARAMETERS DupProcsssParamters;
	PPS_CREATE_INFO DupCreateInfo;
	PPS_ATTRIBUTE_LIST DupAttribList;


	DupProcessHandle = ProcessHandle;
	DupThreadHandle = ThreadHandle;
	DupProcessDesiredAccess = ProcessDesiredAccess;
	DupThreadDesiredAccess = ThreadDesiredAccess;

	DupProcessObjectAttributes = ProcessObjectAttributes;
	DupThreadObjectAttributes = ThreadObjectAttributes;
	DupProcsssParamters = ProcessParameters;
	DupProcessFlags = ProcessFlags;
	DupThreadFlags = ThreadFlags;

    DupAttribList = AttributeList;






	__try
	{ 

		


		
		memset(&Args, 0, sizeof(ULONG_PTR) * EXCEPTION_MAXIMUM_PARAMETERS);
		ExpMessage.PackedArgumentLength = 11;
		ExpMessage.PackedArguments = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * (ExpMessage.PackedArgumentLength+1));
		if (ExpMessage.PackedArguments != 0)
		{
			memset(ExpMessage.PackedArguments, 0, sizeof(ULONG_PTR) * (ExpMessage.PackedArgumentLength + 1));

		}	
		
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSHANDLE] = (ULONG_PTR)&DupProcessHandle;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_THREADHANDLE] = (ULONG_PTR)&DupThreadHandle;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSDESIREDACCESS] = (ULONG_PTR)&DupProcessDesiredAccess;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_THREADDESIREDACCESS] = (ULONG_PTR)&DupThreadDesiredAccess;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSOBJECTATTRIBUTES] = (ULONG_PTR)&DupProcessObjectAttributes;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_THREADOBJECTATTRIBUTES] = (ULONG_PTR)&DupThreadObjectAttributes;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSFLAGS] = (ULONG_PTR)&DupProcessFlags;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_THREADFLAGS] = (ULONG_PTR)&DupThreadFlags;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSPARAMETERS] = (ULONG_PTR)&DupProcsssParamters;
		ExpMessage.PackedArguments[FILESANDBOX_NTCREATEUSERPROCESS_ATTRIBUTELIST] = (ULONG_PTR)&DupAttribList;

		ExpMessage.cb = sizeof(FILESANDBOX_API_EXCEPTION_CLASS);
		ExpMessage.ACK = 0;
		ExpMessage.SpecialHandling = SANDBOX_SPECIAL_CASE_DEFAULT;
		ExpMessage.SpecialHandlingArg = 0;
		ExpMessage.ExeceptionMessage = SANDBOX_API_PROCESS_CREATEUSERPROCESS;

		Args[0] = (ULONG_PTR)&ExpMessage;

		__try
		{
			RaiseException(ExpMessage.ExeceptionMessage, 0, 1, Args);
		}
		__except (SELF_CHECK(GetExceptionCode(), ExpMessage.ExeceptionMessage))
		{
			ExpMessage.ACK = 0;
			;
		}
		
		{
			if (ExpMessage.ACK == 0)
			{
				// Debugger may not have understaood it.
				result = NtCreateUserProcessOriginal(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, ThreadObjectAttributes, ProcessFlags, ThreadFlags, ProcessParameters, CreateInfo, AttributeList);
				__leave;
			}
			else
			{
				switch (ExpMessage.SpecialHandling)
				{
				case SANDBOX_SPECIAL_CASE_SHORTIT:
				{
					result = ExpMessage.SpecialHandlingArg;
					__leave;
				}
				case SANDBOX_SPECIAL_CASE_DEFAULT:
				default:
				{
					result = NtCreateUserProcessOriginal(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, ThreadObjectAttributes, ProcessFlags, ThreadFlags, ProcessParameters, CreateInfo, AttributeList);
					__leave;
				}
				}
			}
		}



	}
	__finally
	{
		if (ExpMessage.PackedArguments != 0)
		{
			free(ExpMessage.PackedArguments);
		}
	}
	return result;
 }


NtCreateFilePtr NtCreateFileOriginal;
 /// <summary>
 /// Our Detoured Varient. We do a deep copy of memory buffers so that the debugger can ideally modify them without causing access violation exceptions and then throw an SEH to notify debugger of the call.  The debugger modifies the struct pointed too in the debugged process's space and then send the SEH back as handled
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
)
{
	 ULONG_PTR Args[EXCEPTION_MAXIMUM_PARAMETERS];
	 FILESANDBOX_API_EXCEPTION_CLASS ExpMessage;
	 NTSTATUS Results = 0;
	 
	 // First duplicate the pointers
	 PHANDLE DupFileHandle = FileHandle;

	 DWORD DupDesiredAccess = DesiredAccess;
	 OBJECT_ATTRIBUTES DupObjectAttributes; 
	 UNICODE_STRING DupObjAttributesObjectName;
	 SECURITY_DESCRIPTOR DupObjAttributesSecurity;

	 PIO_STATUS_BLOCK DupIoStatusBlock = IoStatusBlock;
	 PLARGE_INTEGER DupAllocationSize = AllocationSize;
	 ULONG DupFileAttributes = FileAttributes;
	 ULONG DupShareAccess = ShareAccess;
	 ULONG DupCreationDisposition = CreateDisposition;
	 ULONG DupCreateOptions = CreateOptions;
	 PVOID DupEaBuffer = EaBuffer;
	 ULONG DupEaLength = EaLength;

	 
	 memset(&Args, 0, sizeof(Args));
	 __try
	 {
		 memset(&DupObjectAttributes, 0, sizeof(OBJECT_ATTRIBUTES));
		 memcpy(&DupObjectAttributes, ObjectAttributes, sizeof(OBJECT_ATTRIBUTES));
		 DupObjAttributesObjectName.Length = ObjectAttributes->ObjectName->Length;
		 DupObjAttributesObjectName.MaximumLength = ObjectAttributes->ObjectName->MaximumLength;
		 DupObjAttributesObjectName.Buffer = (PWSTR)malloc(DupObjAttributesObjectName.MaximumLength * sizeof(wchar_t));
		 if (DupObjAttributesObjectName.Buffer != 0)
		 {
			 memset(DupObjAttributesObjectName.Buffer, 0, DupObjAttributesObjectName.MaximumLength * sizeof(wchar_t));
			 memcpy(DupObjAttributesObjectName.Buffer, ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length);
			 DupObjectAttributes.ObjectName = &DupObjAttributesObjectName;
		 }
		 else
		 {
			 DupObjAttributesObjectName.Buffer = nullptr;
			 DupObjAttributesObjectName.Length = DupObjAttributesObjectName.MaximumLength = 0;
		 }

		 if (DupEaLength != 0)
		 {
			 DupEaBuffer = malloc(DupEaLength);
			 if (DupEaBuffer != 0)
			 {
				 memset(DupEaBuffer, 0, DupEaLength);
				 memcpy(DupEaBuffer, EaBuffer, DupEaLength);
			 }
		 }

		 // setup the exception arg for our messsage
		 ExpMessage.cb = sizeof(FILESANDBOX_API_EXCEPTION_CLASS);
		 ExpMessage.ExeceptionMessage = SANDBOX_API_FILE_NTCREATEFILE;
		 // rolling my own
		 {
			 ExpMessage.PackedArgumentLength = 11;
			 ExpMessage.PackedArguments = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * ExpMessage.PackedArgumentLength + 1);
			 if (ExpMessage.PackedArguments != 0)
			 {
				 memset(ExpMessage.PackedArguments, 0, sizeof(ULONG_PTR) * ExpMessage.PackedArgumentLength + 1);
			 }
			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_FILEHANDLE] = (ULONG_PTR)&DupFileHandle;
			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_DESIREDACCESS] = (ULONG_PTR)&DupDesiredAccess;
			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_OBJECTATTRIBUTES] = (ULONG_PTR)&DupObjectAttributes;

			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_FILEATTRIBUTES] = (ULONG_PTR)&DupFileAttributes;
			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_SHAREACCESS] = (ULONG_PTR)&DupShareAccess;
			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_CREATIONDISPOSITION] = (ULONG_PTR)&DupCreationDisposition;
			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_EABUFFER] = (ULONG_PTR)&DupEaBuffer;
			 ExpMessage.PackedArguments[FILESANDBOX_NTCREATEFILE_EALENGTH] = (ULONG_PTR)&DupEaLength;
			 
		 }


		 ExpMessage.ACK = 0;
		 ExpMessage.SpecialHandling = SANDBOX_SPECIAL_CASE_DEFAULT;
		 

		 Args[0] = (ULONG_PTR)&ExpMessage;
		 __try
		 {

			 RaiseException(ExpMessage.ExeceptionMessage, 0, 1, Args);
		 }
		 __except (ExceptionCheck( GetExceptionCode(), ExpMessage.ExeceptionMessage ))
		 {
			 ExpMessage.ACK = 0;
		 }
		 
		 {
			 if (ExpMessage.ACK == 0)
			 {
				 // debugger may not understand mesage. just call original
				 
				 Results=  NtCreateFileOriginal(DupFileHandle,
					 DupDesiredAccess,
					 &DupObjectAttributes,
					 DupIoStatusBlock,
					 DupAllocationSize,
					 DupFileAttributes,
					 DupShareAccess,
					 DupCreationDisposition,
					 DupCreateOptions,
					 DupEaBuffer,
					 DupEaLength);
				 __leave;
			 }
			 else
			 {
				 switch (ExpMessage.SpecialHandling)
				 {
				 case SANDBOX_SPECIAL_CASE_SHORTIT:
				 {
					 Results = ExpMessage.SpecialHandlingArg;
					 __leave;
				 }
				 case SANDBOX_SPECIAL_CASE_DEFAULT:
				 default:
				 {
					 Results = NtCreateFileOriginal(DupFileHandle,
							 DupDesiredAccess,
							 &DupObjectAttributes,
							 DupIoStatusBlock,
							 DupAllocationSize,
							 DupFileAttributes,
							 DupShareAccess,
							 DupCreationDisposition,
							 DupCreateOptions,
							 DupEaBuffer,
							 DupEaLength);
					 __leave;

				 }
				 }
			 }
		 }

	 }
	 __finally
	 {
		 // cleanup of allocated buffers here.
		 if (DupObjAttributesObjectName.Buffer != nullptr)
		 {
			 memset(DupObjAttributesObjectName.Buffer, 0, DupObjAttributesObjectName.Length);
			 free(DupObjAttributesObjectName.Buffer);
		 }
		 // clean up the dup eabuffer if it was needed
		 if (DupEaBuffer != nullptr)
		 {
			 free(DupEaBuffer);
		 }

		 if (ExpMessage.PackedArguments != 0)
		 {
			 free(ExpMessage.PackedArguments);
		 }
	 }
	 return Results;
	return  NtCreateFileOriginal(FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		AllocationSize,
		FileAttributes,
		ShareAccess,
		CreateDisposition,
		CreateOptions,
		EaBuffer,
		EaLength);
}


