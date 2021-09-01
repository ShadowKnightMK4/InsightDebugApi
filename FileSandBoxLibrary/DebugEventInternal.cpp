#include <Windows.h>
#include "FileSandBoxApi.h"
#include "FilesandboxApi_DllStuff.h"
#include "Client.h"
#include "DebugEventInternal.h"

/*
* DebugEventInternal.cpp implments the code that the frontend (DebugEvent.CPP) uses.
*/

BOOL WINAPI FSWaitForDebugEventInternal(Client* client, LPDEBUG_EVENT Event, DWORD Timer)
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

BOOL WINAPI FSContinueDebugEventInternal(Client* client, LPDEBUG_EVENT Event, DWORD ContinueStatus)
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

BOOL WINAPI FSDebugLoopInternal(Client* client, DebupEventCallbackApi UserRoutine, LPVOID CustomArg)
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
			if (FSWaitForDebugEventInternal(client, &ThisLoop, WaitTimer))
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
					if (!FSContinueDebugEventInternal(client, &ThisLoop, ContinueState))
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
