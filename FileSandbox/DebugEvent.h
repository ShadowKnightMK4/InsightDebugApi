#pragma once
#include <Windows.h>
/// <summary>
/// This is the protoype for the DebugLoop() user routine caller. 
/// 
/// return -1 on your routine to stop debugging and forcibly kill the target
/// return 0 on your routine to stop debugging and let the target continue
/// return 1 on your routine to continue handling events
/// OTHER values mean the same as 1.
/// CurEvent is the current event the DebuggerLoop is handling
/// ContinueState is what to pass to ContinueDebugEvent()
/// WaitTime is what to pass for WaitForDebugEventEx()
/// CustomArg is passed unchanged to your routine.
/// </summary>
typedef int (_stdcall DebupEventCallbackApi)(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime, LPVOID CustomArg);
/// <summary>
/// Monitor, log and handle debug events generated. 
/// </summary>
class DebugEvent
{

public:
	DebugEvent();
	DebugEvent(HANDLE ProcessHandle);
	DebugEvent(DWORD ProcessID);

	/// <summary>
	/// Try to enable debug (or disable debug)
	/// </summary>
	/// <param name="Status">If true, will try to enable the SE_DEBUG_PRIV. If False, it will try to disable said priv</param>
	/// <returns>returns TRUE if the call was sucessfully done and false if a problem happened.</returns>
	BOOL AskForDebugPriv(BOOL Status);
	/// <summary>
	/// NOT IMPLMENTED();  Purpose is to attach to passed process it for debugging.
	/// </summary>
	/// <returns>Always returns 0;     0 Means it failed, 1 returns ok.</returns>
	DWORD AttachDebugger(DWORD ProcessID);
	/// <summary>
	/// If NewVal is true then when caller exists, see the MSDN Win32 Api DebugSetProcessKillOnExit()
	/// </summary>
	/// <param name="NewVal"></param>
	/// <returns></returns>
	BOOL SetKillOnDebuggerExit(bool NewVal);
	/// <summary>
	/// Wait for a Unicode Debug Event. Should you pass null (why?) Last error for this class is set to ERROR_INVALID_ADDRESS
	/// </summary>
	/// <param name="EventPtr"></param>
	/// <returns></returns>
	BOOL WaitForEvent(LPDEBUG_EVENT EventPtr, DWORD TimeOut);
	/// <summary>
	/// Continue a debug event after caller has seen it, Sets Local Last eror to ERROR_INVALID_ARG if passed null for eventptr
	/// </summary>
	/// <param name="EventPtr">the ProcessID and ThreadID are pulled from this structure</param>
	/// <param name="ContinueState">status of debug event continue state</param>
	/// <returns>true if the event was ok and false if something went wrong</returns>
	BOOL ContinueEvent(LPDEBUG_EVENT EventPtr, DWORD ContinueState);
	/// <summary>
	/// Continue debug event after caller as seen it.  Sets local last error to error value on value and 0 on ok.
	/// </summary>
	/// <param name="ProcessId"></param>
	/// <param name="ThreadId"></param>
	/// <param name="ContinueState"></param>
	/// <returns>true if event was ok and false if there was en error</returns>
	BOOL ContinueEvent(DWORD ProcessId, DWORD ThreadId, DWORD ContinueState);
	/// <summary>
	/// Provides a debugger loop to start with. lets user specify a callback routine
	/// </summary>
	BOOL DebugLoop(DebupEventCallbackApi UserRoutine, LPVOID CustomArg);
	/// <summary>
	/// Get the last error val used for this class.
	/// </summary>
	/// <returns></returns>
	DWORD GetLocalLastError();

	/// <summary>
	/// Provides a minumume handler for DebugLoop()
	/// </summary>
	/// <param name="CurEvent"></param>
	/// <param name="ContinueState"></param>
	/// <param name="WaitTime"></param>
	/// <returns></returns>
	static int WINAPI DefaultDebugEventHandling(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime);

private:
	void ZeroSelf();
	HANDLE ProcessHandle;
	DWORD ProcessId;
	DWORD LastError;
};

