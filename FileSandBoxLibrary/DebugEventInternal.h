#pragma once
typedef int(_stdcall DebupEventCallbackApi)(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime, LPVOID CustomArg);


/// <summary>
/// Implments the FSDebugLoopPublic public facing routine's thing.      The Public facing routine resolves the Passed Index to a Client* and then calls this.
/// </summary>
/// <param name="client">Client structure make from FSStarUp() or FSStartUpEx()</param>
/// <param name="UserRoutine">user routien to call for each received event. Will need to Match DebugEventCallbackApi</param>
/// <param name="CustomArg">The custom arg passed to the event.  This is unmodified.</param>
/// <returns>returns TRUE if the loop was ended ok.   NOTE THAT THIS DOES NOT RETURN until the app being watched exits.</returns>
BOOL WINAPI FSDebugLoopInternal(Client* client, DebupEventCallbackApi UserRoutine, LPVOID CustomArg);
/// <summary>
/// Implement The FSWaitForDebugEvent routine.    This Waits for a debug event event to be sent to the calling thread.  
/// </summary>
/// <param name="client"> This we are currently working with</param>
/// <param name="Event">This Buffer that holds a DEBUG_EVENT struct pointer that receives event info</param>
/// <param name="Timer">This specifies how long do we wait. use the INFINITE (0xFFFFFFF) value to wait until an event happens./</param>
/// <remarks>       Public routine resolves the index to a client* and calls this.  </remarks>
/// <returns>TRUE if an event was FOUDN and false if no.</returns>
BOOL WINAPI FSWaitForDebugEventInternal(Client* client, LPDEBUG_EVENT Event, DWORD Timer);

/// <summary>
/// Implments FSContinueDebugEvent's routine.   After processing the event,  call this to return control to the Windows Debugger system.
/// </summary>
/// <param name="client">This we are currently working with</param>
/// <param name="Event">DEBUG_EVENT strct pointer for the event we just handled</param>
/// <param name="ContinueStatus">There's 3 options.  (DBG_CONTINUE [0x00010002L] tells Windows that your debugger HANDLED the exception. Callings this on an event other than exception just resumes the debugged app)   (DBG_EXCEPTION_NOT_HANDLED [0x80010001L] tells Windows your debugger did NOT handle the exception)  (DBG_REPLY_LATER [0x40010001L] means your debugger will handle it later)  </param>
/// <returns></returns>
BOOL WINAPI FSContinueDebugEventInternal(Client* client, LPDEBUG_EVENT Event, DWORD ContinueStatus);
