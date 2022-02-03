#include "FileSandBoxHelperDll.h"
#include "ReplacementProcessFunctions.h"
#include "ReplacementStuffCommon.h"
#include "../FilesandboxClientDllProtocol/FileSandboxProtocol.h"
#include "commandments.h"
//#include <ntddk.h>


NtCreateUserProcessPtr OriginalNtCreateUserProcess= 0;



/// <summary>
/// Not intended for a full replacement.  This wraps the createprocessW call located in DetourProcessViaHelperDllsW() of createwith.cpp to the NtCreateUserProcess as the original CreateProcessW will be pointing to *OUR* potential blocker
/// </summary>
/// <param name="lpApplicationName"></param>
/// <param name="lpCommandLine"></param>
/// <param name="lpProcessAttributes"></param>
/// <param name="lpThreadAttributes"></param>
/// <param name="bInheritHandles"></param>
/// <param name="dwCreationFlags"></param>
/// <param name="lpEnvironment"></param>
/// <param name="lpCurrentDirectory"></param>
/// <param name="lpStartupInfo"></param>
/// <param name="lpProcessInformation"></param>
/// <returns></returns>
BOOL WINAPI DetourCreateProcessW_Rundll_stub(
	     LPCWSTR               lpApplicationName,
	 LPWSTR                lpCommandLine,
	    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	              BOOL                  bInheritHandles,
	             DWORD                 dwCreationFlags,
	      LPVOID                lpEnvironment,
	      LPCWSTR               lpCurrentDirectory,
	              LPSTARTUPINFOW        lpStartupInfo,
	             LPPROCESS_INFORMATION lpProcessInformation
)
{
	NTSTATUS result = 0;
	OBJECT_ATTRIBUTES Target;

	RTL_USER_PROCESS_PARAMETERS Arguments;
	
	
	UNICODE_STRING TargetLocation;
	PS_ATTRIBUTE_LIST Test;
	PS_CREATE_INFO CreateInfo;
	
	ZeroMemory(&Test, sizeof(PS_ATTRIBUTE_LIST));
	ZeroMemory(&CreateInfo, sizeof(CreateInfo));
	__try
	{
		Arguments.ImagePathName.Buffer = _wcsdup(lpApplicationName);
		Arguments.ImagePathName.Length = wcslen(lpApplicationName) * sizeof(wchar_t);
		Arguments.ImagePathName.MaximumLength =  Arguments.ImagePathName.Length;

		
		Arguments.CommandLine.Buffer = _wcsdup(lpCommandLine);
		Arguments.CommandLine.Length = wcslen(lpCommandLine) * sizeof(wchar_t);
		Arguments.CommandLine.MaximumLength = Arguments.CommandLine.Length;
		
			TargetLocation.Buffer = _wcsdup(lpApplicationName);
		TargetLocation.Length = wcslen(lpApplicationName) * sizeof(wchar_t);
		TargetLocation.MaximumLength = Arguments.CommandLine.Length;



		
		
		InitializeObjectAttributes(&Target, &TargetLocation, 0, 0, 0);
		result = OriginalNtCreateUserProcess(&lpProcessInformation->hProcess,
											 &lpProcessInformation->hThread,
											  PROCESS_ALL_ACCESS,
											  THREAD_ALL_ACCESS,
											  (POBJECT_ATTRIBUTES) &Target,
											  0,
											  dwCreationFlags,
											  0,
											  &Arguments, 
											  &CreateInfo,
											  &Test);
	}
	__finally
	{
		free(Arguments.ImagePathName.Buffer);
		free(Arguments.CommandLine.Buffer);
		free(TargetLocation.Buffer);
	}
	return result;
}
/// <summary>
/// this sits beteen NtCreateUserProcess() and the rest of the app. This app forces the DLL that contains this code to be loaded into the target
/// </summary>
NTSTATUS NTAPI NtCreateUserProcessCommandment_Handle(

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
	bool AddedSupend = false;
	NTSTATUS retval = 0;
	PROCESS_INFORMATION Faked;
	LPSTR SelfName = 0;
	DWORD SelfSize = 0;
	ZeroMemory(&Faked, sizeof(PROCESS_INFORMATION));


	retval = OriginalNtCreateUserProcess(ProcessHandle, 
										 ThreadHandle,	
										  ProcessDesiredAccess,
										  ThreadDesiredAccess,
		(POBJECT_ATTRIBUTES) ProcessObjectAttributes,
		(POBJECT_ATTRIBUTES)ThreadObjectAttributes,
		ProcessFlags |CREATE_SUSPENDED, ThreadFlags, ProcessParameters, CreateInfo, AttributeList);

	if (ActiveCommandments.ProcessPropergateToTarget)
	{
		Faked.hProcess = ProcessHandle;
		Faked.hThread = ThreadHandle;

		SetLastError(0);
		SelfSize = 512;
		while (true)
		{
			SelfName = new char[SelfSize];
			if (SelfName != 0)
			{
				memset((void*)&SelfName[0], 0, SelfSize);
				SelfSize = GetModuleFileNameA(Self, SelfName, SelfSize);
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					delete[] SelfName;
					SelfSize *= 2;
				}
				else
				{
					if (GetLastError() == 0)
					{
						break;
					}
					else
					{
						CloseHandle(ProcessHandle);
						CloseHandle(ThreadHandle);
						return 5;
					}
				}
			}
		}
		if (!DetourUpdateProcessWithDll(Faked.hProcess, (LPCSTR*)&SelfName, 1) &&
			!DetourProcessViaHelperDllsW(GetProcessId(Faked.hProcess), 1, (LPCSTR*)&SelfName, DetourCreateProcessW_Rundll_stub))
		{
			TerminateProcess(Faked.hProcess, ~0u);
			CloseHandle(Faked.hProcess);
			CloseHandle(Faked.hThread);
			return 5;
		}
	
		if (!(ProcessFlags & CREATE_SUSPENDED))
		{
			ResumeThread(ThreadHandle);
		}
	}
	
		return retval;
}

/// <summary>
/// this sits beteen NtCreateUserProcess() and the rest of the app. This raises an exception to let debugger modify code
/// </summary>
/// <param name="ProcessHandle"></param>
/// <param name="ThreadHandle"></param>
/// <param name="ProcessDesiredAccess"></param>
/// <param name="ThreadDesiredAccess"></param>
/// <param name="ProcessObjectAttributes"></param>
/// <param name="ThreadObjectAttributes"></param>
/// <param name="ProcessFlags"></param>
/// <param name="ThreadFlags"></param>
/// <param name="ProcessParameters"></param>
/// <param name="CreateInfo"></param>
/// <param name="AttributeList"></param>
/// <returns></returns>
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
		ExpMessage.PackedArguments = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * (ExpMessage.PackedArgumentLength + 1));
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
		ExpMessage.ExceptionMessage = SANDBOX_API_PROCESS_CREATEUSERPROCESS;

		Args[0] = (ULONG_PTR)&ExpMessage;

		__try
		{
			RaiseException(ExpMessage.ExceptionMessage, 0, 1, Args);
		}
		__except (SELF_CHECK(GetExceptionCode(), ExpMessage.ExceptionMessage))
		{
			ExpMessage.ACK = 0;
			;
		}

		{
			if (ExpMessage.ACK == 0)
			{
				// Debugger may not have understaood it.
				result = OriginalNtCreateUserProcess(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, ThreadObjectAttributes, ProcessFlags, ThreadFlags, ProcessParameters, CreateInfo, AttributeList);
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
					result = OriginalNtCreateUserProcess(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, ThreadObjectAttributes, ProcessFlags, ThreadFlags, ProcessParameters, CreateInfo, AttributeList);
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