//#include <StaticIncludes.h>
#include <Windows.h>
#include "detours.h"
#include "Utility.h"
/*
* DebugStructExports.cpp
* 
* This exports a small wrapper and allocator using c's malloc in dealing with DEBUG_EVENT structures.
* 
* DotNetSide wrapper DebugEvent.cs uses these plus a pointer to a memory block to implmement access.
* All of these (except DebugEvent_AllocateStructure()) take a pointer to a block of memory and return a pit of data about that memory.
* Naming Scheme is DebugEvent_GetXXXXX where XXXXX is the structure part it returns for example DebugEvent_GetProcessId()
* DebugEvent_XXXXX does something based on structure but does not directly return data - example DebugEvent_IsEventFrom32Bit()
* 
* All routiens routines via WINAPI / _stdcall
*/

union MachineDependantExceptionRecord
{
	EXCEPTION_RECORD32 Except32;
	EXCEPTION_RECORD64 Except64;
};
extern "C"

{



	/// <summary>
	/// Allocate a block of mmoery using libc's malloc() that's big enough to contain a DEBUG_EVENT structure. Also clears it to zero.
	/// </summary>
	/// <returns></returns>
	LPDEBUG_EVENT WINAPI DebugEvent_AllocateStructure()
	{
		LPDEBUG_EVENT ret = (LPDEBUG_EVENT)malloc(sizeof(DEBUG_EVENT));
		if (ret)
		{
			memset(ret, 0, sizeof(DEBUG_EVENT));
		}
		return ret;
	}
	
	// DebugEvent_FreeStructure() is simplemented as a forward to RemoteRead_SimpleFree() which itself call's free().  
	// Currently sufficitent but should the api change where it's notlonger that - implement the routine and remote the DebugEvent_FreeStructure=RemoteRead_SimpleFree in exports.def
	//
	/*
	* DebugEvent_FreeStructure(LPDEBUG_EVENT Ptr) 
	* {
	*	return RemoteRead_SimpleFree(Ptr)
	* }
	* 
	*/
	/// <summary>
	/// Returns if the process id containing within the Ptr is from a 32-bit or 64-bit process using IsTargetProcessID32Bit() contained within Utility.cpp
	/// </summary>
	/// <param name="Ptr">Non Zero Pointer to a block of memory containing a DEBUG_EVENT structure</param>
	/// <returns></returns>
	BOOL WINAPI DebugEvent_IsEventFrom32Bit(const LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			return IsTargetProcessID32Bit(Ptr->dwProcessId);
		}
		return FALSE;
	}

	/// <summary>
	/// Return Procece of the process that this DEBUG_EVENT structure refers too.
	/// </summary>
	/// <param name="Ptr">Non Zero Pointer to a block of memory containing a DEBUG_EVENT structure</param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_GetProcessId(LPDEBUG_EVENT Ptr) 
	{
		if (Ptr)
		{
			return Ptr->dwProcessId;
		}
		return 0;
	}

	/// <summary>
	/// Returns the ThreadID of the thread this DEBUG_EVENT structure refers too.
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_GetThreadId(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			return Ptr->dwThreadId;
		}
		return 0;
	}

	/// <summary>
	/// Returns the event type of the passed DEBUG_EVENT. The event type specifics which union in the struct is valid
	/// </summary>
	/// <param name="Ptr">Pouinter to a DEBUG_EVENT structure</param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_GetEventType(LPDEBUG_EVENT Ptr)
	{
		if (Ptr)
		{
			return Ptr->dwDebugEventCode;
		}
		return 0;
	}


	/// <summary>
	/// Returns the error in the contained Ptr's RIP event. If not a RIP event or a null pointer, returns 0
	/// </summary>
	/// <param name="Ptr">A non-null pointer pointing to a DEBUG_EVENT struct containg a RIP_EVENT</param>
	/// <returns>Returns either the dwError part of the RIP_IFO struct containing within this DEBUG_EVENT. If the event is not a RIP_EVENT or the passed pointer is 0, returns 0</returns>
	DWORD WINAPI DebugEvent_RipGetError(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == RIP_EVENT) )
			{
				return Ptr->u.RipInfo.dwError;
			}
		return 0;
	}

	/// <summary>
	/// Gets the type of error in the DEBUG_EVENT's rip event, otherwise 0
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_RipGetErrorType(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) &&  (Ptr->dwDebugEventCode == RIP_EVENT) )
		{
			return Ptr->u.RipInfo.dwType;
		}
		return 0;
	}

	/// <summary>
	/// Returns the hFile Member if Ptr is not null and the dwEventCode is a create process event.
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	HANDLE WINAPI DebugEvent_CreateProcessGetHFile(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT))
		{
			return Ptr->u.CreateProcessInfo.hFile;
		}
	
		return 0;
	}
#pragma region  ExceptionStuff TO Add
	/// <summary>
	/// N
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
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
		MachineDependantExceptionRecord Except{ 0 };
		bool Is32Bit = false;
		if ( (Ptr) && (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT))
		{
			Is32Bit = DebugEvent_IsEventFrom32Bit(Ptr);
			CopyMemory(&Except.Except64, &Ptr->u.Exception, sizeof(Except.Except64));
			return Except.Except64.NumberParameters;
			if (Is32Bit)
			{
				//CopyMemory(&Except.Except32, &Ptr->u.Exception, sizeof(Except.Except32));
				
			}
			else
			{
				
				
			}
		}
		/*
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

		}*/
		return 0;
	}

	/// <summary>
	/// Returns of the offset to the Exception32.Exception info or the Exception64.ExceptionInfo. 
	/// </summary>
	/// <param name="Ptr">Non Zero Pointer to a block of memory containing a DEBUG_EVENT structure</param>
	/// <returns></returns>
	DWORD64* WINAPI DebugEvent_ExceptionInfo_GetExceptionInformation(LPDEBUG_EVENT Ptr)
	{
		DWORD64* ret = { 0 };
		union GroupUpWithMe
		{
			EXCEPTION_RECORD Ignore;
			EXCEPTION_RECORD32 Except32;
			EXCEPTION_RECORD64 Except64;
		} ExceptionData{ 0 };
		
		bool Pref32 = false;

		
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				ret = (DWORD64*)malloc(sizeof(DWORD64) * EXCEPTION_MAXIMUM_PARAMETERS);
				if (ret)
				{
					ZeroMemory(ret, sizeof(DWORD64)* EXCEPTION_MAXIMUM_PARAMETERS);
					ZeroMemory(&ExceptionData.Except64, sizeof(EXCEPTION_RECORD64));
					Pref32 = DebugEvent_IsEventFrom32Bit(Ptr);
					if (Pref32)
					{
						for (int step = 0; step < EXCEPTION_MAXIMUM_PARAMETERS; step++)
						{
							ret[step] = ((DWORD64)ExceptionData.Except32.ExceptionInformation[step]);
						}
					}
					else
					{
						for (int step = 0; step < EXCEPTION_MAXIMUM_PARAMETERS; step++)
						{
							ret[step] = ((DWORD64)ExceptionData.Except64.ExceptionInformation[step]);
						}
					}
					return ret;
				}

			}
			return nullptr;

			/*
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

				Ret = (DWORD64*)malloc(sizeof(Except64.ExceptionInformation));
				if (Ret)
				{
					if (Pref32)
					{
						for (int step = 0; step < EXCEPTION_MAXIMUM_PARAMETERS; step++)
						{
							Ret[step] = Except32.ExceptionInformation[step];
						}
					}
					else
					{
						for (int step = 0; step < EXCEPTION_MAXIMUM_PARAMETERS; step++)
						{
							Ret[step] = Except64.ExceptionInformation[step];
						}
					}
					return Ret;
				}
			}*/

		}
		return nullptr;
	}
#pragma endregion



	/// <summary>
	/// Return the value if the exception is as first chance exception or not.
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_ExceptionInfo_GetFirstChance(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT) )
		{
				return Ptr->u.Exception.dwFirstChance;
			}
		return 0;
	}


	/// <summary>
	/// Return the value of hFile handle if the ptr passed belongs to a DEBUG_EVENT load dll structure
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	HANDLE WINAPI DebugEvent_LoadDllInfo_GetHFile(LPDEBUG_EVENT Ptr)
	{
		if ((Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) )
		{
				return Ptr->u.LoadDll.hFile;
		}
		return INVALID_HANDLE_VALUE;
	}
	
	/// <summary>
	/// Get the base of the dll if  the passed DEBUG_EVENT is a LOAD_DLL_DEBUG event
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	LPVOID WINAPI DebugEvent_LoadDllInfo_GetBaseOfDll(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) )
		{
				return Ptr->u.LoadDll.lpBaseOfDll;
		}
		return 0;
	}

	/// <summary>
	/// Get the Offset into the Dll file where its debugg info is located if passed ptr is a LOAD_DLL_DEBUG event
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoOffset(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) &&  (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT))
		{
				return Ptr->u.LoadDll.dwDebugInfoFileOffset;
		}
		return 0;
	}


	/// <summary>
	/// Get how big the Dll's Debug INfo is within the file if the passed ptr is a LOAD_DLL_DEBUG event
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoSize(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) )
		{
				return Ptr->u.LoadDll.nDebugInfoSize;
		}
		return 0;
	}




	/// <summary>
	/// Get a pointer to a string containing the image name of the loaded dll within the LOAD_DLL_DEBUG struct. May be null and may or may not be within the context of the Target debugged process
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	LPVOID WINAPI DebugEvent_LoadDllInfo_GetImageNamge(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT))
		{
				return Ptr->u.LoadDll.lpImageName;
		}
		return 0;
	}

	/// <summary>
	/// return the part of the struct that indicates if the ImageName will be ANSI or Unicode.
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
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

	/// <summary>
	/// Get the base address of the Dll location during a passeded UNLOAD_DLLL_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>

	LPVOID WINAPI DebugEvent_UnloadDllInfo_GetBaseAddress(LPDEBUG_EVENT Ptr)
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == UNLOAD_DLL_DEBUG_EVENT) )
			{
				return Ptr->u.UnloadDll.lpBaseOfDll;
			}
		return nullptr;
	}

	/// <summary>
	/// Get the passed handle for a thread contained within a CREATE_THREAD_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>

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

	/// <summary>
	/// return a pointer to the start address of the passed thread
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
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

	/// <summary>
	/// Get a pointer to the thread's Tls Location for the CREATE_THREAD_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
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

	/// <summary>
	/// Get the exit code obtaine within an EXIT_THREAD_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
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


	/// <summary>
	/// return the exit code objected when a process existed via the EXIT_PROCESS_DEBUG_EVENT 
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
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