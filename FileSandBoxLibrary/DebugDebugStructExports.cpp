#include <StaticIncludes.h>
#include "Utility.h"
extern "C"

{



	LPDEBUG_EVENT WINAPI DebugEvent_AllocateStructure()
	{
		return (LPDEBUG_EVENT)malloc(sizeof(DEBUG_EVENT));
	}
	/// <summary>
	/// Returns if the process id containing within the Ptr is from a 32-bit or 64-bit process using IsTargetProcessID32Bit()
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	BOOL WINAPI DebugEvent_IsEventFrom32Bit(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			return IsTargetProcessID32Bit(Ptr->dwProcessId);
		}
		return FALSE;
	}

	DWORD WINAPI DebugEvent_GetProcessId(LPDEBUG_EVENT Ptr) 
	{
		if (Ptr)
		{
			return Ptr->dwProcessId;
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_GetThreadId(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			return Ptr->dwThreadId;
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_GetEventType(LPDEBUG_EVENT PTR)
	{
		if (PTR)
		{
			return PTR->dwDebugEventCode;
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_RipGetError(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == RIP_EVENT)
			{
				return Ptr->u.RipInfo.dwError;
			}
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_RipGetErrorType(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == RIP_EVENT)
			{
				return Ptr->u.RipInfo.dwType;
			}
		}
		return 0;
	}

	// these NOT EXPORTED yet.
	HANDLE WINAPI DebugEvent_CreateProcessGetHFile(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT)
			{
				return Ptr->u.CreateProcessInfo.hFile;
			}
		}
		return 0;
	}
#pragma region  ExceptionStuff TO Add
	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionCode(LPDEBUG_EVENT Ptr)
	{
		EXCEPTION_RECORD32 Except32;
		EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
				}


				if (Pref32)
				{
					return Except32.ExceptionCode;
				}
				else
				{
					return Except64.ExceptionCode;
				}
			}

		}
		return 0;
	}


	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionFlags(LPDEBUG_EVENT Ptr)
	{
		EXCEPTION_RECORD32 Except32;
		EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
				}


				if (Pref32)
				{
					return Except32.ExceptionFlags;
				}
				else
				{
					return Except64.ExceptionFlags;
				}
			}

		}
		return 0;
	}

	DWORD64 WINAPI DebugEvent_ExceptionInfo_GetExceptionRecord(LPDEBUG_EVENT Ptr)
	{
		EXCEPTION_RECORD32 Except32;
		EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
				}


				if (Pref32)
				{
					return Except32.ExceptionRecord;
				}
				else
				{
					return Except64.ExceptionRecord;
				}
			}

		}
		return 0;
	}


	DWORD64 WINAPI DebugEvent_ExceptionInfo_GetExceptionAddress(LPDEBUG_EVENT Ptr)
	{
		EXCEPTION_RECORD32 Except32;
		EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
				}


				if (Pref32)
				{
					return Except32.ExceptionAddress;
				}
				else
				{
					return Except64.ExceptionAddress;
				}
			}

		}
		return 0;
	}

	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionArgumentCount(LPDEBUG_EVENT Ptr)
	{
		EXCEPTION_RECORD32 Except32;
		EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
				}


				if (Pref32)
				{
					return Except32.NumberParameters;
				}
				else
				{
					return Except64.NumberParameters;
				}
			}

		}
		return 0;
	}

	/// <summary>
	/// Returns of the offset to the Exception32.Exception info or the Exception64.ExceptionInfo. 
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD64* WINAPI DebugEvent_ExceptionInfo_GetExceptionInformation(LPDEBUG_EVENT Ptr)
	{
		EXCEPTION_RECORD32 Except32;
		EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
				}


				if (Pref32)
				{
					return (DWORD64*)&Except32.ExceptionInformation[0];
				}
				else
				{
					return &Except64.ExceptionInformation[0];
				}
			}

		}
		return 0;
	}
#pragma endregion




	DWORD WINAPI DebugEvent_ExceptionInfo_GetFirstChance(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				return Ptr->u.Exception.dwFirstChance;
			}
		}
		return 0;
	}


	HANDLE WINAPI DebugEvent_LoadDllInfo_GetHFile(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
			{
				return Ptr->u.LoadDll.hFile;
			}
		}
		return INVALID_HANDLE_VALUE;
	}
	
	LPVOID WINAPI DebugEvent_LoadDllInfo_GetBaseOfDll(LPDEBUG_EVENT Ptr)
	{
		if (Ptr) 
		{
			if (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
			{
				return Ptr->u.LoadDll.lpBaseOfDll;
			}
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoOffset(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
			{
				return Ptr->u.LoadDll.dwDebugInfoFileOffset;
			}
		}
		return 0;
	}


	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoSize(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
			{
				return Ptr->u.LoadDll.nDebugInfoSize;
			}
		}
		return 0;
	}




	LPVOID WINAPI DebugEvent_LoadDllInfo_GetImageNamge(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
			{
				return Ptr->u.LoadDll.lpImageName;
			}
		}
		return 0;
	}

	BOOL WINAPI DebugEvent_LoadDllInfo_GetUnicodeFlag(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
			{
				if (Ptr->u.LoadDll.fUnicode != 0)
				{
					return TRUE;
				}
				return FALSE;
			}
		}
		return FALSE;
	}


	LPVOID WINAPI DebugEvent_UnloadDllInfo_GetBaseAddress(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == UNLOAD_DLL_DEBUG_EVENT)
			{
				return Ptr->u.UnloadDll.lpBaseOfDll;
			}
		}
		return nullptr;
	}

	HANDLE WINAPI DebugEvent_CreateThreadInfo_GetThreadHandle(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT)
			{
				return Ptr->u.CreateThread.hThread;
			}
		}
		return INVALID_HANDLE_VALUE;
	}

	LPTHREAD_START_ROUTINE WINAPI DebugEvent_CreateThreadInfo_GetThreadStartAddress(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT)
			{
				return Ptr->u.CreateThread.lpStartAddress;
			}
		}
		return 0;
	}

	LPVOID WINAPI DebugEvent_CreateThreadInfo_GetThreadLocalBase(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT)
			{
				return Ptr->u.CreateThread.lpThreadLocalBase;
			}
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_ExitThreadInfo_GetExitCode(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXIT_THREAD_DEBUG_EVENT)
			{
				return Ptr->u.ExitThread.dwExitCode;
			}
		}
		return 0;
	}


	DWORD WINAPI DebugEvent_ExitProcessInfo_GetExitCode(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
			{
				return Ptr->u.ExitProcess.dwExitCode;
			}
		}
		return 0;
	}


}