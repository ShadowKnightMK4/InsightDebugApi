
#include "FileSandBoxHelperDll.h"
#include "DetourFunctionStuff.h"
#include "ReplacementFileFileFunctions.h"
#include "ReplacementLoadLibraryStuff.h"
#include "ReplacementStuffCommon.h"
#include "ReplacementProcessFunctions.h"
#include "../FilesandboxClientDllProtocol/FileSandboxProtocol.h"
#include "../FilesandboxClientDllProtocol/HelperPayloadGuids.h"
#include <string>
#include <sstream>
HMODULE NTDLL = 0;
HMODULE KERNEL32 = 0;

/// <summary>
/// During the Helper DLL alert exception, the debugger receives a copy of this structure to modify.
/// Ad miniumm, it should hit ACK to indicate it understood the exception. 
/// 
/// IMPORTANT!!!!  Ensdure this structyurre in the help dll matchs the 
/// </summary>
/*
struct DebugNotify_HelperStruct
{
	// currently set to 0.  Change this if modifyin struct that adds breaking stuff
	DWORD Version;
	/// <summary>
	/// Debugger should set this to non-zero once execption is finished handling. 
	/// </summary>
	DWORD Ack;
	/// <summary>
	/// When modifying things controled with the UUIDs, set this to singled so the helper dll will actually reload them
	/// </summary>
	HANDLE KickerEvent;
	/// <summary>
	/// HelperDll generates a UUID for this instance and using RaiseException() to pass back to debugger.
	/// Debugger is free to copy information via detours payloads to this guid. We look for  detours thing once back and process data once set
	/// </summary>
	UUID PayLoadInfo;
	/// <summary>
	/// This guid payload lets parent process force some changes to the search path info. These paths will always be desired by the LoadLibraryXXX routines and be added back in after calls reset Search PathInfo.
	///	Path1;Path2; ect...
	/// DON'T use quotes in the path.
	/// Envoirment variables are ok, they will get expanded at runtime as needed
	/// 
	/// </summary>
	UUID DllSearchPathInfo;
	/// <summary>
	/// "A"="B" case insensitive;
	/// should a LoadLibraryXXX() routine get A, use B instead.
	/// </summary>
	UUID DllHardExceptionInfo;
	/// <summary>
	/// Parent process should use this guid and copy 
	/// </summary>
	UUID CommandmentSettings;
};
*/


void RoutineFailededWithErrorCode(std::wstringstream& buffer, LPCWSTR RoutineName, DWORD Val, bool Clear = TRUE)
{
	if (Clear)
	{
		buffer.str().assign(nullptr);
	}
	buffer << L"\"" << RoutineName << L"\" failed with an error code of 0x" << std::hex << Val << std::dec << L" (" << Val << L" )" << std::endl;
	
}
/// <summary>
/// LibraryRoutineName failed to locate \"DlllName\" with an error code of \"Val\"\r\n
/// </summary>
/// <param name="buffer"></param>
/// <param name="LibraryRoutineName"></param>
/// <param name="DllName"></param>
/// <param name="Val"></param>
/// <param name="Clear"></param>
void LoadLibraryFailedMessage(std::wstringstream& buffer, LPCWSTR LibraryRoutineName, LPCWSTR DllName, DWORD Val,bool Clear=TRUE)
{
	if (Clear)
	{
		buffer.str().assign(nullptr);
	}
	buffer << LibraryRoutineName << L"Failed to locate \"" << DllName << L"\" with an error code of \"" << Val << L"\"" << std::endl;
}

void DetourAttachFailureMessage(std::wstringstream& buffer, LPCWSTR TargetRoutineName, LONG val, LPVOID TargeRoutineLocation = 0, bool Clear = TRUE)
{
	if (Clear)
	{
		buffer.str().assign(nullptr);
	}
	buffer << "DetourAttach encountered problem when attempting to detour ";

	if (TargetRoutineName != 0)
	{
		buffer << L"a routine named \"" << TargetRoutineName << L"\"";
	}

	if (TargeRoutineLocation != 0)
	{
		buffer << " at a location in memory 0x" << std::hex << TargeRoutineLocation << std::dec;
	}

	buffer << " with an error value of " << std::hex << val << std::dec << std::endl;
}
/// <summary>
/// GetProcAddress failed to locate  "RoutineName" in {LibraryName & LibraryName} with error code of \"VAL"
/// </summary>
/// <param name="buffer"></param>
/// <param name="LibraryName"></param>
/// <param name="LibraryHandle"></param>
/// <param name="RoutineName"></param>
/// <param name="Val"></param>
/// <param name="Clear"></param>
void GetProcFailureMessage(std::wstringstream& buffer, LPCWSTR LibraryName, HMODULE LibraryHandle, LPCWSTR RoutineName, DWORD Val, bool Clear = TRUE)
{
	if (Clear)
	{
		buffer.str().assign(nullptr);
	}
	buffer << L"GetProcAddress failed to locate \"" << RoutineName << L"\" ";
	if ((LibraryName == nullptr) && (LibraryHandle == nullptr))
	{
		buffer << L"an unknown library ";
	}
	if ((LibraryName != nullptr) && (LibraryHandle == nullptr))
	{
		buffer << L"A library located from \"" << LibraryName << L"\"";
	}

	if ((LibraryName == nullptr) && (LibraryHandle != nullptr))
	{
		buffer << L"whose handle is " << std::hex << LibraryHandle << std::dec;
	}

	if ((LibraryName != nullptr) && (LibraryHandle != nullptr))
	{
		buffer << L"A library located from \"" << LibraryName << L"\" whose handle is " << std::hex << LibraryHandle << std::dec ;
	}

	buffer << L"with an error code of 0x" << std::hex << Val << std::dec << std::endl;


}
// sourced from ReplacementFileFunctions.cpp
int ExceptionCheck(DWORD ExceptionCode, DWORD ExceptionMessage);

/// <summary>
/// Handle extracting the payloads
/// </summary>
/// <param name="StructData"></param>
bool HandlePayloadParsing()
{
	PVOID Payload;
	DWORD Size;

	/*
	* There are two pay
	*/
	Payload = DetourFindPayloadEx(HelperDll_PriorityLibraryPaths, &Size);
	if (Payload != nullptr)
	{
		// extra load library stuff.  This will be an unicode string that is path;path;path;
		ExtractPrioritySearchPathPayload(Payload, Size);
	}

	Payload = DetourFindPayloadEx(HelperDll_SpecialCaseLibrarySubs, &Size);
	if (Payload != nullptr)
	{
		// extra hardcoded case stuff This will be a unciode string of  input=this;input=this
	}

	Payload = DetourFindPayloadEx(HelperDll_Commandment_Array, &Size);
	if (Payload != nullptr)
	{
		// extract commandment stuff. It's going to be an array of commands to make active.
	}
	return true;

}

/// <summary>
/// Called after stuff is detoured.  Raises an exception to tell the debugger the dll was loaded. 
/// </summary>
/// <returns></returns>
bool NotifyDebugger_OfHelperDll()
{
	DebugNotify_HelperStruct StructData;
	memset(&StructData, 0, sizeof(StructData));
	StructData.Version = 1;
	UuidCreateSequential(&StructData.PayLoadInfo);
	UuidCreateSequential(&StructData.CommandmentSettings);
	UuidCreateSequential(&StructData.DllHardExceptionInfo);
	UuidCreateSequential(&StructData.DllSearchPathInfo);


	StructData.KickerEvent = CreateEvent(nullptr, TRUE, TRUE, 0);
	
	__try
	{

		RaiseException(SANDBOX_API_PROCESS_HAS_HELPERDLL, 0, 1, (const ULONG_PTR*) &StructData);
	}
	__except (ExceptionCheck(GetExceptionCode(), SANDBOX_API_PROCESS_HAS_HELPERDLL))
	{
		StructData.Ack = 0;
	}
	if (StructData.Ack)
	{
		/*
		* Now we see if we've indeed gotton a payload module inserted by the debugger
		*/
		
		
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool DetourKernelLoadLibraryRoutines()
{
	std::wstringstream Buffer;
	LONG DetourRetVal;
	if (KERNEL32 == 0)
	{
		KERNEL32 = LoadLibraryW(L"kernel32.dll");
	}

	if (KERNEL32 == 0)
	{
		return false;
	}
	else
	{

		OriginalLoadLibraryA = (LoadLibraryAPtr) GetProcAddress(KERNEL32, "LoadLibraryA");
		OriginalLoadLibraryW = (LoadLibraryWPtr)GetProcAddress(KERNEL32, "LoadLibraryW");
		OriginalLoadLibraryExA = (LoadLibraryExAPtr)GetProcAddress(KERNEL32, "LoadLibraryExA");
		OriginalLoadLibraryExW = (LoadLibraryExWPtr)GetProcAddress(KERNEL32, "LoadLibraryExW");
		OriginalSetDllDirectoryA = (SetDllDirectoryAPtr)GetProcAddress(KERNEL32, "SetDllDirectoryA");
		OriginalSetDllDirectoryW = (SetDllDirectoryWPtr)GetProcAddress(KERNEL32, "SetDllDirectoryW");
		OriginalAddDllDirectoryA = (AddDllDirectoryAPtr)GetProcAddress(KERNEL32, "AddDllDirectoryA");
		OriginalAddDllDirectoryW = (AddDllDirectoryWPtr)GetProcAddress(KERNEL32, "AddDllDirectoryW");

		DetourRetVal = DetourTransactionBegin();


		DetourRetVal = DetourUpdateThread(GetCurrentThread());
		if (DetourRetVal != NO_ERROR)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			//Buffer << L"DetourUpdateThread() failed with error code " << DetourRetVal << std::endl;
			RoutineFailededWithErrorCode(Buffer, L"DetourUpdateThread", DetourRetVal);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return false;
		}

		if (DetourRetVal == 0)
		{
			if (OriginalLoadLibraryA != 0)
			{
				DetourRetVal = DetourAttach((PVOID*)&OriginalLoadLibraryA, DetouredLoadLibraryA);
				if (DetourRetVal != 0)
				{
					DetourTransactionAbort();
					return false;
				}
			}

			if (OriginalLoadLibraryW != 0)
			{
				DetourRetVal = DetourAttach((PVOID*)&OriginalLoadLibraryW, DetouredLoadLibraryW);
				if (DetourRetVal != 0)
				{
					DetourTransactionAbort();
					return false;
				}
			}


			if (OriginalLoadLibraryExA != 0)
			{
				DetourRetVal = DetourAttach((PVOID*)&OriginalLoadLibraryExA, DetouredLoadLibraryExA);
				if (DetourRetVal != 0)
				{
					DetourTransactionAbort();
					return false;
				}
			}

			if (OriginalLoadLibraryExW != 0)
			{
				DetourRetVal = DetourAttach((PVOID*)&OriginalLoadLibraryExW, DetouredLoadLibraryExW);
				if (DetourRetVal != 0)
				{
					DetourTransactionAbort();
					return false;
				}
			}
		}

		DetourRetVal = DetourTransactionCommit();
		return (DetourRetVal == 0);
	}
}

bool DetourNtFileRoutines()
{
	LONG fallback;
	if (NTDLL == 0)
	{
		NTDLL = LoadLibraryW(L"NTDLL.DLL");
	}
	if (NTDLL == 0)
	{
		
		return FALSE;
	}
	else
	{
		OriginalNtCreateFile = (NtCreateFilePtr)GetProcAddress(NTDLL, "NtCreateFile");
		OriginalNtCreateUserProcess = (NtCreateUserProcessPtr)GetProcAddress(NTDLL, "NtCreateUserProcess");
		OriginalNtOpenFile = (NtOpenFilePtr)GetProcAddress(NTDLL, "NtOpenFile");
		
		fallback = DetourTransactionBegin();
		fallback = DetourUpdateThread(GetCurrentThread());
		if (fallback != 0)
		{
			OutputDebugString(L"Failed At Detours Begin()");
			return FALSE;
		}
		if (OriginalNtCreateFile == 0)
		{
			OutputDebugString(L"Unable to locate ntdll's NtCreateFile. File logging ability reduced");
		}
		else
		{
			fallback = DetourAttach((PVOID*)&OriginalNtCreateFile, NtCreateFileDetoured);
			if (fallback != 0)
			{
				OutputDebugString(L"Unable to detour NtCreateFile()");
				DetourTransactionAbort();
				return FALSE;
			}
		}

		if (OriginalNtOpenFile == 0)
		{
			OutputDebugString(L"Unable to locate ntdll's NtOpenFile. File logging ability reduced");
		}
		else
		{
			fallback = DetourAttach((PVOID*)&OriginalNtOpenFile, NtOpenFileDetoured);
			if (fallback != 0)
			{
				OutputDebugString(L"Unable to detour NtOpenFile");
				DetourTransactionAbort();
				return FALSE;
			}
		}

		if (OriginalNtCreateUserProcess != 0)
		{
			fallback = DetourAttach((PVOID*)&OriginalNtCreateUserProcess, NtCreateUserProcesDetoured);
			if (fallback != 0)
			{
				OutputDebugString(L"Unable to detour NtCreateCreateUserProcess()");
				DetourTransactionAbort();
				return FALSE;
			}
		}
		//else
		{
			//OutputDebugString(L"Unable to locate ntdll's NtCreateUserProcess. Ability to propergate Filesandboxhelper.dll to spawned processes is not usable.");
		}
		if (DetourTransactionCommit() != 0)
		{
			DetourTransactionAbort();
			return FALSE;
		}

		return true;
	}

}


bool DetourCommonRoutines()
{
	LONG DetourRetVal=0;
	if (NTDLL == 0) {
		NTDLL = LoadLibrary(L"NTDLL.DLL");
	}

	if (KERNEL32 == 0) {
		KERNEL32 = LoadLibraryW(L"Kernel32.dll");
	}
	if (NTDLL == 0)
	{
#ifdef VERBOSE_DEBUG_LINK_OUTPUT
		OutputDebugString(L"Failed to load NTDLL.DLL. Why?!\r\n");
#endif
		return false;
	}

	if (KERNEL32 == 0)
	{
#ifdef VERBOSE_DEBUG_LINK_OUTPUT
		OutputDebugString(L"Failed to load NTDLL.DLL. Why?!\r\n");
#endif
		return false;
	}

	if (OriginalNtCreateUserProcess == 0)
	{
		OriginalNtCreateUserProcess = (NtCreateUserProcessPtr)GetProcAddress(NTDLL, "NtCreateUserProcess");
	}


	if (OriginalNtCreateUserProcess == 0)
	{
		// TODO: Do a pre Vista check
#ifdef VERBOSE_DEBUG_LINK_OUTPUT
		OutputDebugString(L"Failed to Locate NtCreateUserProcess in NTDLL.DLL\r\n");
#endif
		return false; 
	}

	if (OriginalNtSetInformationThread == 0)
	{
		OriginalNtSetInformationThread = (NtSetInformationThreadPtr)GetProcAddress(NTDLL, "NtSetInformationThread");
	}

	if (OriginalNtSetInformationThread == 0)
	{
#ifdef VERBOSE_DEBUG_LINK_OUTPUT
		OutputDebugString(L"Failed to Locate NtSetInformationThread in NTDLL.DLL\r\n");
#endif
		return false;
	}

	if (OriginalNtCreateThreadEx == 0)
	{
		OriginalNtCreateThreadEx = (NtCreateThreadExPtr)GetProcAddress(NTDLL, "NtCreateThreadEx");
	}


	if (OriginalNtCreateThreadEx == 0)
	{
#ifdef VERBOSE_DEBUG_LINK_OUTPUT
		OutputDebugString(L"Failed to Locate NtCreateThreadEx in NTDLL.DLL\r\n");
#endif
		return false;
	}
	{
#ifdef VERBOSE_DETOUR_OUTPUT
		std::wstringstream Buffer;
#endif
		DetourRetVal = DetourTransactionBegin();
		if (DetourRetVal != NO_ERROR)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			RoutineFailededWithErrorCode(Buffer, L"DetourTransactionBegin", DetourRetVal);
			//Buffer << L"DetourTransactionBegin() failed with error code " << DetourRetVal << std::endl;
			OutputDebugString(Buffer.str().c_str());
#endif
			return false;
		}

		DetourRetVal = DetourUpdateThread(GetCurrentThread());
		if (DetourRetVal != NO_ERROR)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			//Buffer << L"DetourUpdateThread() failed with error code " << DetourRetVal << std::endl;
			RoutineFailededWithErrorCode(Buffer, L"DetourUpdateThread", DetourRetVal);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return false;
		}

		DetourRetVal = DetourAttach(&(PVOID&)OriginalNtSetInformationThread, NtSetInformationThreadDropDebuggerHideRequestsDetoured);

		if (DetourRetVal != NO_ERROR)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			DetourAttachFailureMessage(Buffer, L"NtSetInformationThread", DetourRetVal, OriginalNtSetInformationThread);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return false;
		}

		DetourRetVal = DetourAttach(&(PVOID&)OriginalNtCreateThreadEx, NtCreateThreadExDropDebuggerFlagDetoured);

		if (DetourRetVal != NO_ERROR)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			DetourAttachFailureMessage(Buffer, L"NtCreateThreadEx", DetourRetVal, OriginalNtCreateThreadEx);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return false;
		}

		DetourRetVal = DetourTransactionCommit();
		if (DetourRetVal != NO_ERROR)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			RoutineFailededWithErrorCode(Buffer, L"DetourTransactionCommit", DetourRetVal);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return false;
		}
		return true;
	}
}

/// <summary>
/// detour everything under one routine
/// </summary>
/// <returns></returns>
bool DetourTheWorld()
{
	long DetourRetVal = 0;
	std::wstringstream Buffer;
	if (NTDLL == 0)
	{
		NTDLL = LoadLibraryW(L"NTDLL");
	}
	if (KERNEL32 == 0)
	{
		KERNEL32 = LoadLibraryW(L"KERNEL32.DLL");
	}


	if (OriginalNtSetInformationThread == 0)
	{
		OriginalNtSetInformationThread = (NtSetInformationThreadPtr)GetProcAddress(NTDLL, "NtSetInformationThread");
	}

	if (OriginalNtCreateThreadEx == 0)
	{
		OriginalNtCreateThreadEx = (NtCreateThreadExPtr)GetProcAddress(NTDLL, "NtCreateThreadEx");
	}



	if (OriginalNtCreateFile == 0)
	{
		OriginalNtCreateFile = (NtCreateFilePtr)GetProcAddress(NTDLL, "NtCreateFile");
	}

	if (OriginalNtOpenFile == 0)
	{
		OriginalNtOpenFile = (NtOpenFilePtr)GetProcAddress(NTDLL, "NtOpenFile");
	}

	
	if (OriginalNtCreateUserProcess == 0)
	{
		OriginalNtCreateUserProcess = (NtCreateUserProcessPtr)GetProcAddress(NTDLL, "NtCreateUserProcess");
	}

	if (OriginalLoadLibraryA == 0)
	{
		OriginalLoadLibraryA = (LoadLibraryAPtr)GetProcAddress(KERNEL32, "LoadLibraryA");
	}

	if (OriginalLoadLibraryW == 0)
	{
		OriginalLoadLibraryW = (LoadLibraryWPtr)GetProcAddress(KERNEL32, "LoadLibraryW");
	}

	if (OriginalLoadLibraryA == 0)
	{
		OriginalLoadLibraryExA = (LoadLibraryExAPtr)GetProcAddress(KERNEL32, "LoadLibraryExA");
	}

	if (OriginalLoadLibraryA == 0)
	{
		OriginalLoadLibraryExW = (LoadLibraryExWPtr)GetProcAddress(KERNEL32, "LoadLibraryExW");
	}

	DetourRetVal = DetourTransactionBegin();
	if (DetourRetVal != 0)
	{
#ifdef VERBOSE_DETOUR_OUTPUT
		RoutineFailededWithErrorCode(Buffer, L"DetourTransactionBegin", DetourRetVal);
		//Buffer << L"DetourTransactionBegin() failed with error code " << DetourRetVal << std::endl;
		OutputDebugString(Buffer.str().c_str());
#endif
		return false;
	}


	DetourRetVal = DetourUpdateThread(GetCurrentThread());
	if (DetourRetVal != NO_ERROR)
	{
#ifdef VERBOSE_DETOUR_OUTPUT
		//Buffer << L"DetourUpdateThread() failed with error code " << DetourRetVal << std::endl;
		RoutineFailededWithErrorCode(Buffer, L"DetourUpdateThread", DetourRetVal);
		OutputDebugString(Buffer.str().c_str());
#endif
		DetourTransactionAbort();
		return false;
	}

	DetourRetVal = DetourAttach(&(PVOID&)OriginalNtSetInformationThread, NtSetInformationThreadDropDebuggerHideRequestsDetoured);

	if (DetourRetVal != NO_ERROR)
	{
#ifdef VERBOSE_DETOUR_OUTPUT
		DetourAttachFailureMessage(Buffer, L"NtSetInformationThread", DetourRetVal, OriginalNtSetInformationThread);
		OutputDebugString(Buffer.str().c_str());
#endif
		DetourTransactionAbort();
		return false;
	}

	/// <summary>
	/// This detour of NTCreateTheadEx() Drops the debugger flag
	/// </summary>
	/// <returns></returns>
	DetourRetVal = DetourAttach(&(PVOID&)OriginalNtCreateThreadEx, NtCreateThreadExDropDebuggerFlagDetoured);

	if (DetourRetVal != NO_ERROR)
	{
#ifdef VERBOSE_DETOUR_OUTPUT
		DetourAttachFailureMessage(Buffer, L"NtCreateThreadEx", DetourRetVal, OriginalNtCreateThreadEx);
		OutputDebugString(Buffer.str().c_str());
#endif
		DetourTransactionAbort();
		return false;
	}

	if (OriginalNtCreateFile == 0)
	{
		OutputDebugString(L"Unable to locate ntdll's NtCreateFile. File logging ability reduced");
		DetourTransactionAbort();
		return false;
	}
	else
	{
		DetourRetVal = DetourAttach((PVOID*)&OriginalNtCreateFile, NtCreateFileDetoured);
		if (DetourRetVal != 0)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			DetourAttachFailureMessage(Buffer, L"NtCreateFile", DetourRetVal, OriginalNtCreateFile);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return FALSE;
		}
	}

	if (OriginalNtOpenFile == 0)
	{
		OutputDebugString(L"Unable to locate ntdll's NtOpenFile. File logging ability reduced");
	}
	else
	{
		DetourRetVal = DetourAttach((PVOID*)&OriginalNtOpenFile, NtOpenFileDetoured);
		if (DetourRetVal != 0)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			DetourAttachFailureMessage(Buffer, L"NtOpenFile", DetourRetVal, OriginalNtOpenFile);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return FALSE;
		}
	}


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!
	// FUTURE READERS:
	// Important ! !!!
	//See the DetourChain Sample for an idea of what this is doing
	//	We first detour the original with the Commandment handler and detour the handler to the original.
	// This lets the cmdment one be the last routine called before we actualy call NtCreateUserProcess()
	// Should additional routines do this, it will be marked in comments here,
	// !!!!!!!!!!!!!!!!!!!!!

	if (OriginalNtCreateUserProcess == 0)
	{
		OutputDebugString(L"Unable to locate ntdll's NtCreateUserProcess");
		// TODO: Fallback to pre vista
		return false;
	}
	else
	{
		DetourRetVal = DetourAttach((PVOID*)&OriginalNtCreateUserProcess, NtCreateUserProcessCommandment_Handle);
		if (DetourRetVal != 0)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			DetourAttachFailureMessage(Buffer, L"NtCreateUserProcess (cmdment handler)", DetourRetVal, NtCreateUserProcessCommandment_Handle);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return FALSE;
		}

		DetourRetVal = DetourAttach((PVOID*)&NtCreateUserProcessCommandment_Handle, NtCreateUserProcesDetoured);
		if (DetourRetVal != 0)
		{
#ifdef VERBOSE_DETOUR_OUTPUT
			DetourAttachFailureMessage(Buffer, L"NtCreateUserProcess", DetourRetVal, NtCreateUserProcesDetoured);
			OutputDebugString(Buffer.str().c_str());
#endif
			DetourTransactionAbort();
			return FALSE;
		}
	}


	return TRUE;



}