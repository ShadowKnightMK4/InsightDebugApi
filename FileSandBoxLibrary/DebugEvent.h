#pragma once

#include "FilesandboxApi_DllStuff.h"
extern "C" {
	/// <summary>
/// Callback for FSDebugLoop
/// CurEvent is event to process. Your routine gets this .
///  IF NOT NULL, ContinueState is a pointer to a 4 byte dword. Use the Win32 Api ContinueDebugEvent() argument constants.  
/// IF NOT NULL, Waittime is a pointer to a 4 byte dword. Specifies how long to wait with WaitForeDebugEventEx().  Use INFINITE to Wait until an event triggers
/// CustomArg is something You'll be passing to FSDebugLoop().  This is passed unaltered to your routine.
/// </summary>
	typedef int(_stdcall DebupEventCallbackApi)(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime, LPVOID CustomArg);


	/// <summary>
	/// Implements a debugger loop.
	/// </summary>
	/// <param name="client"></param>
	/// <param name="UserRoutine"></param>
	/// <param name="CustomArg"></param>
	/// <returns></returns>
	BOOL FILESANDBOX_API_DLL FSDebugLoop(Client* client, DebupEventCallbackApi UserRoutine, LPVOID CustomArg);
	/// <summary>
	/// Wraps WaitForDebugEventEx()
	/// </summary>
	/// <param name="client"></param>
	/// <param name="Event"></param>
	/// <param name="Timer"></param>
	/// <returns></returns>
	BOOL FILESANDBOX_API_DLL FSWaitForDebugEvent(Client* client, LPDEBUG_EVENT Event, DWORD Timer);
	/// <summary>
	/// Wraps ContinueDebugEvent()
	/// </summary>
	/// <param name="client"></param>
	/// <param name="Event"></param>
	/// <param name="ContinueStatus"></param>
	/// <returns></returns>
	BOOL FILESANDBOX_API_DLL FSContinueDebugEvent(Client* client, LPDEBUG_EVENT Event, DWORD ContinueStatus);
}
