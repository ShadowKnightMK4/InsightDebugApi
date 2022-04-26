#pragma once
#include <Windows.h>
/*
* This header defines protoypes for a few generic routines that aren't directly related to the library.
*/
/// <summary>
/// Utility routine: return a duplicate of the passed string converted to ANSI.  Caller will need to free it.
/// </summary>
/// <param name="Original"></param>
/// <returns>new block of memory containing the converted string or null</returns>
char* WINAPI ConvertUnicodeString(const wchar_t* Original);
/// <summary>
/// utility: return a duplicate of the passed string converted to UNICODE.  Caller will need to free it.
/// </summary>
/// <param name="Original"></param>
/// <returns>new block of memory containing the converted string or null</returns>
wchar_t* WINAPI ConvertANSIString(const char * Original);



/// <summary>
/// Keep calling GetModuleFileNameEx until we get strength big genough.  CALLER frees 
/// </summary>
/// <param name="Process">Process to read from. Needs PROCESS_QUERY_INFORMATION and PROCESS_VM_READ </param>
/// <param name="Module">if nullptr, then the mainmodule, otherwise the name of the dll.</param>
/// <returns></returns>
wchar_t* WINAPI GetModuleNameViaHandle(HANDLE Process, HMODULE Module);


/// <summary>
/// Allocate a buffer and get the name of the file via the handle. CALLER Frees it
/// </summary>
/// <param name="FileHandle">Handle to a file</param>
/// <returns>returns a buffer allocated with malloc().   Use Free() to Free it when done.</returns>
wchar_t* WINAPI GetFileNameViaHandle(HANDLE FileHandle);

/// <summary>
/// Compares results from GetNativeSystemInfo and IsWOW64Process to try to figure it target is 32 or 64-bit that trigged the event. 
/// IF NativeSystem is 32-bit then
///			returns true;
/// IF NativeSystem is 64-bit then
///			if Wow64 is live on the target
///				returns true
///			if Wow64 is NOT live on the target
///				return false
/// IF IsWOW64Process not in kernel32.dll (GetProcAddress fails)
///			return TRUE
/// </summary>
/// <param name="Ptr"></param>
/// <returns></returns>
BOOL WINAPI IsTargetProcessID32Bit(DWORD ProcessID);


/// <summary>
/// Ask for Windows Debug Privilege and return if it was granted.
/// </summary>
/// <returns></returns>
BOOL WINAPI AskForDebugPriv();


/// <summary>
/// Duplicate handle into the target process. Will still need to figure a way to put the handle there.
/// </summary>
/// <param name="CurrentHandle"></param>
/// <param name="Access"></param>
/// <param name="CopyAccess"></param>
/// <param name="RemoteProcess"></param>
/// <param name="Inherit"></param>
/// <returns></returns>
HANDLE WINAPI RemoteHandleDup(HANDLE CurrentHandle, DWORD Access, BOOL CopyAccess, HANDLE RemoteProcess, BOOL Inherit);

/// <summary>
/// Duplicate a handle in the local process to another local handle with the option of access specifying or keeping current access
/// </summary>
/// <returns>0 on failure and the new handle on ok.</returns>
HANDLE WINAPI LocalHandleDup(HANDLE CurrentHandle, DWORD Access, BOOL CopyAccess);

/// accessed in theadcontext and utility. If not zero, points to kernel32.dll
extern HMODULE Kernel32;