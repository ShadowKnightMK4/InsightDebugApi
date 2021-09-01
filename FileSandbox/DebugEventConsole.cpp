#include "DebugEventConsole.h"

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
DebugEvent::DebugEvent()
{
	LastError = 0;
	ProcessHandle = INVALID_HANDLE_VALUE;
	ProcessId = 0;
}

DebugEvent::DebugEvent(HANDLE ProcessHandle)
{
	LastError = 0;
	if (DuplicateHandle(GetCurrentProcess(), ProcessHandle, GetCurrentProcess(), &this->ProcessHandle, 0, false, DUPLICATE_SAME_ACCESS))
	{
		this->ProcessId = GetProcessId(this->ProcessHandle);
	}
	else
	{
		this->ProcessId = 0;
		this->LastError = GetLocalLastError();
	}
	
}

DebugEvent::DebugEvent(DWORD ProcessID)
{
	this->LastError = 0;
	this->ProcessId = 0;
	this->ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);

	if ((GetLastError() == ERROR_ACCESS_DENIED))
	{
		if (this->AskForDebugPriv(TRUE) == TRUE)
		{
			this->ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
		}
		else 
		{
			this->ProcessHandle = INVALID_HANDLE_VALUE;
		}
	}
}

BOOL DebugEvent::AskForDebugPriv(BOOL Status)
{
	if (EnablePrivOnSelf(SE_DEBUG_NAME, Status))
	{
		return TRUE;
	}
	return FALSE;
}

DWORD DebugEvent::AttachDebugger(DWORD ProcessID)
{
	return 0;
}

BOOL DebugEvent::SetKillOnDebuggerExit(bool NewVal)
{
	return DebugSetProcessKillOnExit(NewVal);
}

BOOL DebugEvent::WaitForEvent(LPDEBUG_EVENT EventPtr, DWORD Timeout)
{
	if (EventPtr != nullptr)
	{
		if (!WaitForDebugEventEx(EventPtr, Timeout))
		{
			this->LastError = GetLastError();
			if (this->LastError == 0)
			{
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			this->LastError = 0;
			return TRUE;
		}
	}
	else
	{
		this->LastError = ERROR_INVALID_ADDRESS;
		return FALSE;
	}

}

BOOL DebugEvent::ContinueEvent(LPDEBUG_EVENT EventPtr, DWORD ContinueState)
{
	if (EventPtr != nullptr)
	{
		if (!ContinueDebugEvent(EventPtr->dwProcessId, EventPtr->dwThreadId, ContinueState))
		{
			this->LastError = GetLastError();
			return FALSE;
		}
		else
		{
			this->LastError = 0;
			return TRUE;
		}
	}
	else
	{
		this->LastError = ERROR_INVALID_ADDRESS;
		return FALSE;

	}
}

/// <summary>
/// Wrapper for ContinueDebugEvent(). 
/// </summary>
/// <param name="ProcessId"></param>
/// <param name="ThreadId"></param>
/// <param name="ContinueState"></param>
/// <returns></returns>
BOOL DebugEvent::ContinueEvent(DWORD ProcessId, DWORD ThreadId, DWORD ContinueState)
{
	if (!ContinueDebugEvent(ProcessId, ThreadId, ContinueState))
	{
		this->LastError = GetLastError();
		return FALSE;
	}
	this->LastError = 0;
	return TRUE;
}

/// <summary>
/// Provides a debug loop to handle debug events. 
/// </summary>
/// <param name="UserRoutine">Your Routine to handle the events</param>
/// <param name="CustomArg">A  value passed to your routine unchanged.</param>
/// <returns>FALSE if UserRoutine is null or an error happened.  Returns TRUE once all known debugged targets are exited (</returns>
BOOL DebugEvent::DebugLoop(DebupEventCallbackApi UserRoutine, LPVOID CustomArg)
{
	DEBUG_EVENT ThisLoop;
	DWORD ContinueState = DBG_CONTINUE;
	DWORD WaitTimer = INFINITE;
	memset(&ThisLoop, 0, sizeof(DEBUG_EVENT));
	if (UserRoutine == nullptr)
	{
		this->LastError = ERROR_INVALID_ADDRESS;
		return FALSE;
		
	}
	else
	{
		int UserDebugRoutineResult = 0;
		while (TRUE)
		{
			if (this->WaitForEvent(&ThisLoop, WaitTimer))
			{
				UserDebugRoutineResult = UserRoutine(&ThisLoop, &ContinueState, &WaitTimer, CustomArg);
				if (UserDebugRoutineResult < 0)
				{
					this->SetKillOnDebuggerExit(TRUE);
					break;
				}
				
				if (UserDebugRoutineResult == 0)
				{
					this->SetKillOnDebuggerExit(FALSE);
					break;
				}
				
				if (UserDebugRoutineResult > 0)
				{
					if (!this->ContinueEvent(&ThisLoop, ContinueState))
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

DWORD DebugEvent::GetLocalLastError()
{
	return this->LastError;
}

/// <summary>
/// Default handler provider when you don't want your own. This Closes the handled passed via CreateProcessDebugEvent
/// </summary>
/// <param name="CurEvent">specifies the event struct to deal with. </param>
/// <param name="ContinueState">If non null, this is set to zero for every event but DEBUG_EXCEPTION_EVENT. It's set to DBG_EXCEPTION_NOT_HANDLED in that case</param>
/// <param name="WaitTime">If non null, this is set to INFINITE</param>
/// <returns>always returns 1 to keep going</returns>
int  __stdcall DebugEvent::DefaultDebugEventHandling(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime)
{
	if (ContinueState != nullptr) {
		*ContinueState = DBG_CONTINUE;
	}
	if (WaitTime != nullptr) {
		*WaitTime = INFINITE;
	}
	
	switch (CurEvent->dwDebugEventCode)
	{
	case CREATE_PROCESS_DEBUG_EVENT:
	{
		if ((CurEvent->u.CreateProcessInfo.hFile != INVALID_HANDLE_VALUE) &&
			(CurEvent->u.CreateProcessInfo.hFile != nullptr))
		{
			CloseHandle(CurEvent->u.CreateProcessInfo.hFile);
		}
		return 1;
	}
	case EXCEPTION_DEBUG_EVENT:
	{
		if (ContinueState != nullptr)	{
			*ContinueState = DBG_EXCEPTION_NOT_HANDLED;
		}
		
		return 1;
	}
	default:
		return 1;
	}
	return 1;
}

void DebugEvent::ZeroSelf()
{
	memset(this, 0, sizeof(DebugEvent));
}
