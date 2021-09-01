#pragma once

#include "FilesandboxApi_DllStuff.h"

/// <summary>
/// Callback for FSDebugLoop
/// CurEvent is event to process. Your routine gets this .
///  IF NOT NULL, ContinueState is a pointer to a 4 byte dword. Use the Win32 Api ContinueDebugEvent() argument constants.  
/// IF NOT NULL, Waittime is a pointer to a 4 byte dword. Specifies how long to wait with WaitForeDebugEventEx().  Use INFINITE to Wait until an event triggers
/// CustomArg is something You'll be passing to FSDebugLoop().  This is passed unaltered to your routine.
/// </summary>
typedef int(_stdcall DebupEventCallbackApi)(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime, LPVOID CustomArg);


extern "C" {


	/// <summary>
	/// Public facing routine.   This calls an internal routine that implements a single threaded debugger loop.
	/// </summary>
	/// <param name="ClientIndex">Index of the client from FSStartup()</param>
	/// <param name="UserRoutine">user routien to call for each received event. Will need to Match DebugEventCallbackApi</param>
	/// <param name="CustomArg">The custom arg passed to the event.  This is unmodified.</param>
	/// <returns>returns TRUE if the loop was ended ok.   NOTE THAT THIS DOES NOT RETURN until the app being watched exits.</returns>
	BOOL WINAPI FSDebugLoop(DWORD ClientIndex, DebupEventCallbackApi UserRoutine, LPVOID CustomArg);


	/// <summary>
	/// Public facing routine.  This calls an internal routine that waits for a debug event event to be sent to the calling thread.  
	/// </summary>
	/// <param name="ClientIndex"> This we are currently working with</param>
	/// <param name="Event">This Buffer that holds a DEBUG_EVENT struct pointer that receives event info</param>
	/// <param name="Timer">This specifies how long do we wait. use the INFINITE (0xFFFFFFF) value to wait until an event happens./</param>
	/// <remarks>       Public routine resolves the index to a client* and calls this.  </remarks>
	/// <returns>TRUE if an event was FOUDN and false if no.</returns>
	BOOL WINAPI FSWaitForDebugEvent(DWORD ClientIndex, LPDEBUG_EVENT Event, DWORD Timer);


	/// <summary>
	/// Public facing routine.   After processing the event,  call this to return control to the Windows Debugger system.
	/// </summary>
	/// <param name="client">This we are currently working with</param>
	/// <param name="Event">DEBUG_EVENT strct pointer for the event we just handled</param>
	/// <param name="ContinueStatus">There's 3 options.  (DBG_CONTINUE [0x00010002L] tells Windows that your debugger HANDLED the exception. Callings this on an event other than exception just resumes the debugged app)   (DBG_EXCEPTION_NOT_HANDLED [0x80010001L] tells Windows your debugger did NOT handle the exception)  (DBG_REPLY_LATER [0x40010001L] means your debugger will handle it later)  </param>
	/// <returns></returns>
	BOOL WINAPI FSContinueDebugEvent(DWORD ClientIndex, LPDEBUG_EVENT Event, DWORD ContinueStatus);
}
