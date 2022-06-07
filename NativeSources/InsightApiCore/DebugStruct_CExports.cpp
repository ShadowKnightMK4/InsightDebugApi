//#include <StaticIncludes.h>
#include <Windows.h>
#include "detours.h"
#include "Utility.h"

#include "DebugStruct_CExports.h"

/*
* DebugStructExports.cpp
* 
* This exports a small wrapper and allocator using c's malloc in dealing with DEBUG_EVENT structures.
* 
* DotNetSide wrapper DebugEvent.cs uses these plus a pointer to a memory block to implement access.
* All of these (except DebugEvent_AllocateStructure()) take a pointer to a block of memory and return a pit of data about that memory.
* Naming Scheme is DebugEvent_GetXXXXX where XXXXX is the structure part it returns for example DebugEvent_GetProcessId()
* DebugEvent_XXXXX does something based on structure but does not directly return data - example DebugEvent_IsEventFrom32Bit()
* 
* All routines routines via WINAPI / _stdcall
*/


extern "C"

{



	
	LPDEBUG_EVENT WINAPI DebugEvent_AllocateStructure()  noexcept
	{
		const LPDEBUG_EVENT ret = (LPDEBUG_EVENT)malloc(sizeof(DEBUG_EVENT));
		if (ret != 0)
		{
			memset(ret, 0, sizeof(DEBUG_EVENT));
		}
		return ret;
	}

	BOOL WINAPI DebugEvent_IsEventFrom32Bit( const LPDEBUG_EVENT Ptr) noexcept
	{
		if (Ptr)
		{
			return IsTargetProcessID32Bit(Ptr->dwProcessId);
		}
		return FALSE;
	}

	
	DWORD WINAPI DebugEvent_GetProcessId(const LPDEBUG_EVENT Ptr) noexcept
	{
		if (Ptr)
		{
			return Ptr->dwProcessId;
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_GetThreadId(const LPDEBUG_EVENT Ptr) noexcept
	{
		if (Ptr)
		{
			return Ptr->dwThreadId;
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_GetEventType(const LPDEBUG_EVENT Ptr) noexcept
	{
		if (Ptr)
		{
			return Ptr->dwDebugEventCode;
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_RipGetError(const LPDEBUG_EVENT Ptr) noexcept
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == RIP_EVENT) )
			{
				return Ptr->u.RipInfo.dwError;
			}
		return 0;
	}

	DWORD WINAPI DebugEvent_RipGetErrorType(const LPDEBUG_EVENT Ptr) noexcept
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
	/// This checks if the target is 32 or 64 bit via DebugEvent_IsEventFrom32Bit() and Copys either EXCEPTION_RECORD32 or EXCEPTION_RECORD64 from the Ptr to the Container. The front ends call this and return the struct member asked for 
	/// </summary>
	/// <param name="Ptr"></param>
	/// <param name="Container"></param>
	/// <returns></returns>
	bool inline _fastcall __DebugEventExceptionFrontEndHelper(const LPDEBUG_EVENT Ptr, VOID *Container, bool* Pref32)
	{
		if (Ptr != nullptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (Container != nullptr)
				{
					if (Pref32 != nullptr)
					{
						if (DebugEvent_IsEventFrom32Bit(Ptr))
						{
							CopyMemory(Container, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
							*Pref32 = true;
						}
						else
						{
							CopyMemory(Container, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
							*Pref32 = false;
						}
						return true;
					}
				}
			}
		}
		return false;
	}
	/// <summary>
	/// Return the Exception code in the contained DEBUG_EVENT struct if it is an EXCEPTION_DEBUG_EVENT 
	/// </summary>
	/// <param name="Ptr">Non null ptr. </param>
	/// <returns>if Ptr is null, returns 0, otherwise checks if event came from a 32 or 64 bit process via DebugEvent_IsEventFrom32Bit() and returns the ExceptionCode </returns>
	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionCode(const LPDEBUG_EVENT Ptr) noexcept
	{
		MachineDependantExceptionRecord ExceptionContainer = { 0 };
		//EXCEPTION_RECORD32 Except32;
		//EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		//memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		//memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (__DebugEventExceptionFrontEndHelper(Ptr, &ExceptionContainer, &Pref32))
		{
			if (Pref32)
			{
				return ExceptionContainer.Except32.ExceptionCode;
			}
			else
			{
				return ExceptionContainer.Except64.ExceptionCode;
			}
		}
		/*
	
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					//CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					CopyMemory(&ExceptionContainer.Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					CopyMemory(&ExceptionContainer.Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
					//CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
				}


				if (Pref32)
				{
					return ExceptionContainer.Except32.ExceptionCode;
				}
				else
				{
					return ExceptionContainer.Except32.ExceptionCode;
				}
			}

		}*/
		return 0;
	}


	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionFlags(const LPDEBUG_EVENT Ptr) noexcept
	{
		MachineDependantExceptionRecord ExceptionContainer = { 0 };
		//EXCEPTION_RECORD32 Except32;
		//EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		//memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		//memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (__DebugEventExceptionFrontEndHelper(Ptr, &ExceptionContainer, &Pref32))
		{
			if (Pref32)
			{
				return ExceptionContainer.Except32.ExceptionFlags;
			}
			else
			{
				return ExceptionContainer.Except64.ExceptionFlags;
			}
		}
		/*
		if (Ptr)
		{
			if (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
			{
				if (DebugEvent_IsEventFrom32Bit(Ptr))
				{
					//CopyMemory(&Except32, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD32));
					Pref32 = true;
				}
				else
				{
					//CopyMemory(&Except64, &Ptr->u.Exception.ExceptionRecord, sizeof(EXCEPTION_RECORD64));
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

		}*/
		return 0;
	}

	DWORD64 WINAPI DebugEvent_ExceptionInfo_GetExceptionRecord(const LPDEBUG_EVENT Ptr) noexcept
	{
		MachineDependantExceptionRecord ExceptionContainer = { 0 };
	//	EXCEPTION_RECORD32 Except32;
		//EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		//memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		//memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (__DebugEventExceptionFrontEndHelper(Ptr, &ExceptionContainer, &Pref32))
		{
			if (Pref32)
			{
				return ExceptionContainer.Except32.ExceptionRecord;
			}
			else
			{
				return ExceptionContainer.Except64.ExceptionRecord;
			}
		}

		/*
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

		}*/
		return 0;
	}


	DWORD64 WINAPI DebugEvent_ExceptionInfo_GetExceptionAddress(const LPDEBUG_EVENT Ptr) noexcept
	{
		MachineDependantExceptionRecord ExceptionContainer = { 0 };
		//	EXCEPTION_RECORD32 Except32;
			//EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		//memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		//memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (__DebugEventExceptionFrontEndHelper(Ptr, &ExceptionContainer, &Pref32))
		{
			if (Pref32)
			{
				return ExceptionContainer.Except32.ExceptionAddress;
			}
			else
			{
				return ExceptionContainer.Except64.ExceptionAddress;
			}
		}



		/*
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

		}*/
		return 0;
	}

	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionArgumentCount(const LPDEBUG_EVENT Ptr) noexcept
	{

		MachineDependantExceptionRecord ExceptionContainer = { 0 };
		//	EXCEPTION_RECORD32 Except32;
			//EXCEPTION_RECORD64 Except64;
		bool Pref32 = false;
		//memset(&Except32, 0, sizeof(EXCEPTION_RECORD32));
		//memset(&Except64, 0, sizeof(EXCEPTION_RECORD64));

		if (__DebugEventExceptionFrontEndHelper(Ptr, &ExceptionContainer, &Pref32))
		{
			if (Pref32)
			{
				return ExceptionContainer.Except32.NumberParameters;
			}
			else
			{
				return ExceptionContainer.Except64.NumberParameters;
			}
		}


		/*
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
		}*/
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

	DWORD64* WINAPI DebugEvent_ExceptionInfo_GetExceptionInformation(LPDEBUG_EVENT Ptr) noexcept
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

	DWORD WINAPI DebugEvent_ExceptionInfo_GetFirstChance(const LPDEBUG_EVENT Ptr) noexcept
	{

		if ((Ptr) && (Ptr->dwDebugEventCode == EXCEPTION_DEBUG_EVENT))
		{
			return Ptr->u.Exception.dwFirstChance;
		}
		return 0;
	}

#pragma endregion






	HANDLE WINAPI DebugEvent_LoadDllInfo_GetHFile(const LPDEBUG_EVENT Ptr) noexcept
	{
		if ((Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) )
		{
				return Ptr->u.LoadDll.hFile;
		}
		return INVALID_HANDLE_VALUE;
	}
	

	LPVOID WINAPI DebugEvent_LoadDllInfo_GetBaseOfDll(const LPDEBUG_EVENT Ptr) noexcept
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) )
		{
				return Ptr->u.LoadDll.lpBaseOfDll;
		}
		return 0;
	}

	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoOffset(const LPDEBUG_EVENT Ptr) noexcept
	{
		if ( (Ptr) &&  (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT))
		{
				return Ptr->u.LoadDll.dwDebugInfoFileOffset;
		}
		return 0;
	}


	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoSize(const LPDEBUG_EVENT Ptr) noexcept
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) )
		{
				return Ptr->u.LoadDll.nDebugInfoSize;
		}
		return 0;
	}




	LPVOID WINAPI DebugEvent_LoadDllInfo_GetImageNamge(const LPDEBUG_EVENT Ptr) noexcept
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT))
		{
				return Ptr->u.LoadDll.lpImageName;
		}
		return 0;
	}
	

	BOOL WINAPI DebugEvent_LoadDllInfo_GetUnicodeFlag(const LPDEBUG_EVENT Ptr) noexcept
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
	/// Get the base address of the Dll location during a passed UNLOAD_DLLL_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>

	DWORD64 WINAPI DebugEvent_UnloadDllInfo_GetBaseAddress(const LPDEBUG_EVENT Ptr) noexcept
	{
		if ( (Ptr) && (Ptr->dwDebugEventCode == UNLOAD_DLL_DEBUG_EVENT) )
			{
				return (DWORD64)Ptr->u.UnloadDll.lpBaseOfDll;
			}
		return 0;
	}

	HANDLE WINAPI DebugEvent_CreateThreadInfo_GetThreadHandle(const LPDEBUG_EVENT Ptr) noexcept
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

	LPTHREAD_START_ROUTINE WINAPI DebugEvent_CreateThreadInfo_GetThreadStartAddress(const LPDEBUG_EVENT Ptr) noexcept
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

	
	LPVOID WINAPI DebugEvent_CreateThreadInfo_GetThreadLocalBase(const LPDEBUG_EVENT Ptr) noexcept
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

	DWORD WINAPI DebugEvent_ExitThreadInfo_GetExitCode(const LPDEBUG_EVENT Ptr) noexcept
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


	DWORD WINAPI DebugEvent_ExitProcessInfo_GetExitCode(const LPDEBUG_EVENT Ptr) noexcept
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