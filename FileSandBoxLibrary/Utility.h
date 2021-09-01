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
wchar_t* GetModuleNameViaHandle(HANDLE Process, HMODULE Module);


/// <summary>
/// Allocate a buffer and get the name of the file via the handle. CALLER Frees it
/// </summary>
/// <param name="FileHandle">Handle to a file</param>
/// <returns>returns a buffer allocatedw with malloc().   Use Free() to Free it when done.</returns>
wchar_t* GetFileNameViaHandle(HANDLE FileHandle);
