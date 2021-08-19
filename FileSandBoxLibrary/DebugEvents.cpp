
#include <Windows.h>
#include "FileSandBoxApi.h"
#include "FilesandboxApi_DllStuff.h"
extern "C" {

	/// <summary>
	/// Enable a priv on self.  NOT INTENDED TO BE EXPORTED
	/// </summary>
	/// <param name="PrivNaem"></param>
	/// <param name="Enable"></param>
	/// <returns></returns>
	bool EnablePrivOnSelf(LPCTSTR PrivNaem, BOOL Enable)
	{

		// try asking for the debug priv and go gain.
		TOKEN_PRIVILEGES priv;
		LUID luid;
		HANDLE SelfAccessHandle = INVALID_HANDLE_VALUE;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &SelfAccessHandle))
		{
			if (LookupPrivilegeValue(nullptr, PrivNaem, &luid))
			{
				priv.PrivilegeCount = 1;
				priv.Privileges[0].Luid = luid;
				if (Enable)
				{
					priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				}
				else
				{
					priv.Privileges[0].Attributes = 0;
				}
				if (AdjustTokenPrivileges(SelfAccessHandle, false, &priv, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr))
				{
					CloseHandle(SelfAccessHandle);
					return TRUE;
				}
			}
		}
		if (SelfAccessHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(SelfAccessHandle);
		}
		return FALSE;
	}


	BOOL FILESANDBOX_API_DLL FSDebugLoop(Client* client, DebupEventCallbackApi UserRoutine, LPVOID CustomArg)
	{
		DEBUG_EVENT ThisLoop;
		DWORD ContinueState = DBG_CONTINUE;
		DWORD WaitTimer = INFINITE;
		memset(&ThisLoop, 0, sizeof(DEBUG_EVENT));
		if (UserRoutine == nullptr)
		{
			SetLastError(ERROR_INVALID_ADDRESS);
			return FALSE;

		}
		else
		{
			int UserDebugRoutineResult = 0;
			while (TRUE)
			{
				if (FSWaitForDebugEvent(client, &ThisLoop, WaitTimer))
				{
					UserDebugRoutineResult = UserRoutine(&ThisLoop, &ContinueState, &WaitTimer, CustomArg);
					if (UserDebugRoutineResult < 0)
					{
						DebugSetProcessKillOnExit(TRUE);
						break;
					}

					if (UserDebugRoutineResult == 0)
					{
						DebugSetProcessKillOnExit(FALSE);
						break;
					}

					if (UserDebugRoutineResult > 0)
					{
						if (!FSContinueDebugEvent(client, &ThisLoop, ContinueState))
						{
							break;
						}
					}



				}
				else
				{
					return FALSE;
				}
			}
			return TRUE;
		}
	}
	BOOL FILESANDBOX_API_DLL FSWaitForDebugEvent(Client* client, LPDEBUG_EVENT Event, DWORD Timer)
	{
		if (client == nullptr)
		{
			return FALSE;
		}
		if (Event == nullptr)
		{
			return FALSE;
		}
		return WaitForDebugEventEx(Event, Timer);
	}

	BOOL FILESANDBOX_API_DLL FSContinueDebugEvent(Client* client, LPDEBUG_EVENT Event, DWORD ContinueStatus)
	{
		if (client == nullptr)
		{
			return FALSE;
		}
		if (Event == nullptr)
		{
			return FALSE;
		}
		return ContinueDebugEvent(Event->dwProcessId, Event->dwThreadId, ContinueStatus);
	}
	
}