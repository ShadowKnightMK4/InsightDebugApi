#include "FileSandBoxHelperDll.h"
#include "ReplacementStuffCommon.h"
#include "ReplacementFileFileFunctions.h"
#include <vector>
#include "../FilesandboxClientDllProtocol/FileSandboxProtocol.h"




 NtOpenFilePtr OriginalNtOpenFile;
 NTSTATUS WINAPI NtOpenFileDetoured(
	  PHANDLE            FileHandle,
	   ACCESS_MASK        DesiredAccess,
	   POBJECT_ATTRIBUTES ObjectAttributes,
	  PIO_STATUS_BLOCK   IoStatusBlock,
	   ULONG              ShareAccess,
	  ULONG              OpenOptions
 )
 {
	 ULONG_PTR Args[EXCEPTION_MAXIMUM_PARAMETERS];
	 FILESANDBOX_API_EXCEPTION_CLASS ExpMessage;
	 NTSTATUS Results = 0;
	 memset(&Args, 0, sizeof(Args));
	 __try
	 {
		 ExpMessage.cb = sizeof(ExpMessage);
		 ExpMessage.ExceptionMessage = SANDBOX_API_FILE_NTOPENFILE;
		 ExpMessage.PackedArgumentLength = 6;

		 ExpMessage.PackedArguments = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * ExpMessage.PackedArgumentLength + 1);
		 if (ExpMessage.PackedArguments != 0)
		 {
			 memset(ExpMessage.PackedArguments, 0, sizeof(ULONG_PTR) * ExpMessage.PackedArgumentLength + 1);
			 ExpMessage.PackedArguments[FILESANDBOX_NTOPENFILE_FILEHANDLE] = (ULONG_PTR)&FileHandle;
			 ExpMessage.PackedArguments[FILESANDBOX_NTOPENFILE_DESIREDACCESS] = (ULONG_PTR)&DesiredAccess;
			 ExpMessage.PackedArguments[FILESANDBOX_NTOPENFILE_OBJECTATTRIBUTES] = (ULONG_PTR)&ObjectAttributes;
			 ExpMessage.PackedArguments[FILESANDBOX_NTOPENFILE_IOSTATUSBLOCK] = (ULONG_PTR)&IoStatusBlock;
			 ExpMessage.PackedArguments[FILESANDBOX_NTOPENFILE_SHAREACCESS] = (ULONG_PTR)&ShareAccess;
			 ExpMessage.PackedArguments[FILESANDBOX_NTOPENFILE_OPENOPTIONS] = (ULONG_PTR)&OpenOptions;
		 }

		 ExpMessage.ACK = 0;
		 ExpMessage.SpecialHandling = SANDBOX_SPECIAL_CASE_DEFAULT;


		 Args[0] = (ULONG_PTR)&ExpMessage;
		 __try
		 {

			 RaiseException(ExpMessage.ExceptionMessage, 0, 1, Args);
		 }
		 __except (ExceptionCheck(GetExceptionCode(), ExpMessage.ExceptionMessage))
		 {
			 ExpMessage.ACK = 0;
		 }

		 if (ExpMessage.ACK == 0)
		 {
			 Results = OriginalNtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
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
				{
					Results = OriginalNtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
					__leave;
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

	 return Results;
 }

NtCreateFilePtr OriginalNtCreateFile;
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
		 ExpMessage.ExceptionMessage = SANDBOX_API_FILE_NTCREATEFILE;
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

			 RaiseException(ExpMessage.ExceptionMessage, 0, 1, Args);
		 }
		 __except (ExceptionCheck( GetExceptionCode(), ExpMessage.ExceptionMessage ))
		 {
			 ExpMessage.ACK = 0;
		 }
		 
		 {
			 if (ExpMessage.ACK == 0)
			 {
				 // debugger may not understand mesage. just call original
				 
				 Results=  OriginalNtCreateFile(DupFileHandle,
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
					 Results = OriginalNtCreateFile(DupFileHandle,
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
	return  OriginalNtCreateFile(FileHandle,
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


