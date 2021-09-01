
#include <Windows.h>
#include "FileSandBoxApi.h"
#include "FilesandboxApi_DllStuff.h"
#include "Client.h"

#include "DebugEventInternal.h"

extern "C" {

	/// <summary>
	/// Enable a priv on self.  NOT INTENDED TO BE EXPORTED. 
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


	BOOL WINAPI FSContinueDebugEvent(DWORD ClientHandle, LPDEBUG_EVENT Event, DWORD ContinueStatus)
	{
		if (ClientHandle == 0)
		{
			return FALSE;
		}
		if (Event == nullptr)
		{
			return FALSE;
		}
		Client* ClientData = FSClientIdToPoint(ClientHandle);
		if (ClientData != nullptr)
		{
			return FSContinueDebugEventInternal(ClientData, Event, ContinueStatus);
		}
		else
		{
			return FALSE;
		}
	}


	BOOL WINAPI FSWaitForDebugEvent(DWORD ClientHandle, LPDEBUG_EVENT Event, DWORD Timer)
	{

		if (ClientHandle == 0)
		{
			return FALSE;
		}
		if (Event == nullptr)
		{
			return FALSE;
		}
		Client* ClientData = FSClientIdToPoint(ClientHandle);
		if (ClientData != nullptr) 
		{
			return FSWaitForDebugEventInternal(ClientData, Event, Timer);
		}
		else
		{
			return FALSE;
		}
	}


	
	/// <summary>
	/// The Public export for FSDebugLoop.   FSDebugLoop implements a single threaded debug loop to process DEBUG_EVENTS.    
	/// </summary>
	/// <param name="ClientIndex">Value returned FSStartup or FSSTartupEx</param>
	/// <param name="UserRoutine">your routine to process the events</param>
	/// <param name="CustomArg">custom arg passed to your routie</param>
	/// <returns>TRUE if process exited with no issues.   DOES NOT RETURN Until process exists.</returns>
	BOOL WINAPI FSDebugLoop(DWORD ClientIndex, DebupEventCallbackApi UserRoutine, LPVOID CustomArg)
	{
		if (UserRoutine == 0)
		{
			return FALSE;
		}
		if (ClientIndex == 0)
		{
			return FALSE;
		}
		Client* UserClient = FSClientIdToPoint(ClientIndex);
		if (UserClient == nullptr)
		{
			return FALSE;
		}
		else
		{
			return FSDebugLoopInternal(UserClient, UserRoutine, CustomArg);
		}
	}
	
}