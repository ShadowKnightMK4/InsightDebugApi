#include <Windows.h>
/// <summary>
/// DebugNotifyHelperStructWrappers
/// During the Helper DLL alert exception, the debugger receives a copy of this structure to modify.
/// Ad miniumm, it should hit ACK to indicate it understood the exception. 
/// 
/// IMPORTANT!!!!  Ensdure this structyurre in the help dll matchs the 
/// </summary>
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

extern "C" {
	DWORD WINAPI DebugNotify_HelperStruct_GetVersionData(HANDLE TargetProcess, DebugNotify_HelperStruct* ptr)
	{
		DebugNotify_HelperStruct Remote;
		DWORD BytesRead=0;
		if (ptr != nullptr)
		{
			if (TargetProcess == GetCurrentProcess())
			{
				return ptr->Version;
			}
			else
			{
				if ((ReadProcessMemory(TargetProcess, ptr, &Remote, sizeof(DebugNotify_HelperStruct), &BytesRead) == TRUE) &&
					(BytesRead == sizeof(DebugNotify_HelperStruct)))
				{
					return Remote.Version;
				}
			}
		}
		return 0;
	}

	HANDLE WINAPI DebugNotify_HelperStruct_GetKickerHandle(HANDLE TargetProcess, DebugNotify_HelperStruct* ptr, BOOL CopyToSelf)
	{
		DebugNotify_HelperStruct Remote;
		DWORD BytesRead = 0;
		HANDLE MyHandle = INVALID_HANDLE_VALUE;
		if (TargetProcess == GetCurrentProcess())
		{
			return ptr->KickerEvent;
		}
		else
		{
			if ((ReadProcessMemory(TargetProcess, ptr, &Remote, sizeof(DebugNotify_HelperStruct), &BytesRead) == TRUE) &&
				(BytesRead == sizeof(DebugNotify_HelperStruct)))
			{
				if (CopyToSelf == FALSE)
				{
					return Remote.KickerEvent;
				}
				else
				{

				}
			}
		}
		return INVALID_HANDLE_VALUE;
	}
}