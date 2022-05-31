#pragma once

#include <Windows.h>

/*
* Telemetry Support is a generic library intended for laying some
*/

using namespace std;

/// <summary>
/// Convert the passed Ansi String to Unicode and returns a buffer allocated with new to contain it.
/// </summary>
/// <param name="ansi">ANSI string to convert.</param>
/// <returns>returns a buffer allocated with new[] containing the string or null on error.  Also returns null if ansi=null also</returns>
wchar_t* AnsiToUnicode(char* ansi);

/// <summary>
/// Depends on if build with _DEBUG defined or not.  If _DEBUG is defined, it gives a message displaying the failure and the passed unicodestring. if _DEBUG is not defined, this gives message showing the error alone without the message
/// </summary>
/// <param name="val"></param>
/// <param name="telemetryname"></param>
void OutputDebugString_DetourCantCommit(DWORD val, const wchar_t* telemetryname);

/// <summary>
/// Outputs a string indicated that the call to a DetourAttach() failed.
/// </summary>
/// <param name="val">value returned by DetourAttach()</param>
/// <param name="routine_name">name of routine you provide</param>
/// <param name="Ptr">pointer to original routine.</param>
/// <param name="Replacement">Your Replacement routine</param>
void OutputDebugString_ErrorFailedToAttach(DWORD val, const wchar_t* routine_name, LPVOID Ptr, LPVOID Replacement);

/// <summary>
/// Outputs a string indicating that the call to DetourTransaction() failed
/// </summary>
/// <param name="val">return value by DetourTransactionBegin()</param>
void OutputDebugString_DetourTransBeginFail(DWORD val);

/// <summary>
/// Outputs a string indicating that the call to DetourSetThead() failed
/// </summary>
/// <param name="val">the return value from the routine</param>
void OutputDebugString_DetourTargetThreadFail(DWORD val);

/// <summary>
/// Outputs a string indicated that the routine passed was not found via GetProcAddress in the passed dll.   
/// </summary>
/// <param name="IsFatal">If set, the message starts with "FATAL:" otherwise "WARNING:"</param>
/// <param name="routine">name of the routine</param>
/// <param name="dll">dll the routine was attempted to be found in.</param>
void OutputDebugString_GetProcFail(BOOL IsFatal, const wchar_t* routine, const wchar_t* dll);

/// <summary>
/// Output the passed pointer as a hex value if PrefHex is set.
/// </summary>
/// <param name="D">pointer</param>
/// <param name="PrefHex">If set, outputs as hex. Otherwise outputs as decimal</param>
void OutputDebugString(VOID* D, bool PrefHex);

/// <summary>
/// Output the passed pointer has a hex value
/// </summary>
/// <param name="D">pointer</param>
void OutputDebugString(VOID* D);

/// <summary>
/// Output the value a dec to OutputDebugString
/// </summary>
/// <param name="P"></param>
void OutputDebugString(unsigned long long P);
