#pragma once

#include <Windows.h>

#define FILESANDBOX_AP_DLL_IMPORT 1
#define FILESANDBOX_AP_DLL_EXPORT 2

#ifndef FILESANDBOX_API_DLL
#error Please define FILESANDBOX_API_DLL to FILESANDBOX_AP_DLL_IMPORT (1) or FILESANDBOX_AP_DLL_IMPORT (2)
#else

#if FILESANDBOX_API_DLL==FILESANDBOX_AP_DLL_IMPORT
	#undef FILESANDBOX_API_DLL
	#define FILESANDBOX_API_DLL __declspec(dllimport)
#elif FILESANDBOX_API_DLL==FILESANDBOX_AP_DLL_EXPORT
	#undef FILESANDBOX_API_DLL
	#define FILESANDBOX_API_DLL __declspec(dllexport)
#else
#error Please define FILESANDBOX_API_DLL to IMPORT or EXPORT
#endif // FILESANDBOX_API_DLL=IMPORT

#endif

typedef int(_stdcall DebupEventCallbackApi)(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime, LPVOID CustomArg);
extern "C" {
	/// <summary>
	/// Implements a debugger loop.
	/// </summary>
	/// <param name="client"></param>
	/// <param name="UserRoutine"></param>
	/// <param name="CustomArg"></param>
	/// <returns></returns>
	BOOL FILESANDBOX_API_DLL FSDebugLoop(Client client, DebupEventCallbackApi UserRoutine, LPVOID CustomArg);
	/// <summary>
	/// Wraps WaitForDebugEventEx()
	/// </summary>
	/// <param name="client"></param>
	/// <param name="Event"></param>
	/// <param name="Timer"></param>
	/// <returns></returns>
	BOOL FILESANDBOX_API_DLL FSWaitForDebugEvent(Client client, LPDEBUG_EVENT Event, DWORD Timer);
	/// <summary>
	/// Wraps ContinueDebugEvent()
	/// </summary>
	/// <param name="client"></param>
	/// <param name="Event"></param>
	/// <param name="ContinueStatus"></param>
	/// <returns></returns>
	BOOL FILESANDBOX_API_DLL FSContinueDebugEvent(Client client, LPDEBUG_EVENT Event, DWORD ContinueStatus);
}
