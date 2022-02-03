#pragma once

/*
*	The routines prototyped here each detour a part of the windows api.
* 
*	All take NO arguments and return TRUE or FALSE if the Detour went off without a hitch.
* 
*  Two global #defines (project settings) effect output from these
* VERBOSE_DEBUG_LINK_OUTPUT		-> define to get OutputDebugStringW() info about linking to dlls and getting original imports
*  VERBOSE_DETOUR_OUTPUT		-> define to get OutputDebugStringW() info about where we are able to detour something
*/
/// <summary>
/// Detore the functiions that catch the NtCreate /NtOpenFile routines.
/// </summary>
/// <returns></returns>
bool DetourNtFileRoutines();
/// <summary>
/// Raise an Exception() that makes the HelperDLL Precense known
/// </summary>
/// <returns></returns>
bool NotifyDebugger_OfHelperDll();
/// <summary>
/// DEtour the functions that Deal with LoadLibraryXXX() stuff
/// </summary>
/// <returns></returns>
bool DetourKernelLoadLibraryRoutines();

/// <summary>
/// consolates the other detoursd to this for reasier use
/// </summary>
/// <returns></returns>
bool DetourTheWorld();

/// <summary>
/// Extract the payloads copcuied  ao this process ad dmofidy tem,
/// </summary>
bool HandlePayloadParsing();

/// <summary>
/// Detours NtCreateThreadEx() and NtSetThreadInfo() to make it a little harderd to NOT turn off debug events. Detours the routines that need the commandent enabled also.
/// </summary>
/// <returns></returns>
bool DetourCommonRoutines();

/// <summary>
/// Process the search paths that will get checked first before sending along the dll search path.
/// </summary>
void  PayloadProcessDllSearchPathInfo(LPVOID PayloadLocation, DWORD PayloadSize);