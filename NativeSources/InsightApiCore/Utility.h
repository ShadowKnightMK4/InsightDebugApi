#pragma once
#include <Windows.h>
/*
* This header defines prototypes  for a few generic routines that aren't directly related to the library but are
* usefully enough to use through both the Sheath .NET wrapper and the native DLL.  It's kind of a catch all.
*/

extern "C" {
	/// <summary>
	/// Utility routine: return a duplicate of the passed string converted to ANSI.  Caller will need to free it.
	/// </summary>
	/// <param name="Original"></param>
	/// <returns>new block of memory containing the converted string or null</returns>
	char* WINAPI ConvertUnicodeString(const wchar_t* Original) noexcept;
	/// <summary>
	/// utility: return a duplicate of the passed string converted to UNICODE.  Caller will need to free it.
	/// </summary>
	/// <param name="Original"></param>
	/// <returns>new block of memory containing the converted string or null</returns>
	wchar_t* WINAPI ConvertANSIString(const char* Original) noexcept;


	/// <summary>
	/// Utility routine:  Keep calling GetModuleFileNameEx until we get strength big enough.  CALLER frees 
	/// </summary>
	/// <param name="Process">Process to read from. Needs PROCESS_QUERY_INFORMATION and PROCESS_VM_READ </param>
	/// <param name="Module">if nullptr, then the routine gets the main module's (EXE) filename, otherwise it gets the name of the DLL.</param>
	/// <returns></returns>
	wchar_t* WINAPI GetModuleNameViaHandle(HANDLE Process, HMODULE Module) noexcept;


	/// <summary>
	/// Utility routine:  Allocate a buffer and get the name of the file via the handle. CALLER Frees it
	/// </summary>
	/// <param name="FileHandle">Handle to a file</param>
	/// <returns>returns a buffer allocated with malloc().   Use Free() to Free it when done.</returns>
	wchar_t* WINAPI GetFileNameViaHandle(HANDLE FileHandle) noexcept;

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
	BOOL WINAPI IsTargetProcessID32Bit(DWORD ProcessID) noexcept;


	/// <summary>
	/// Maps the passed DLL/EXE file into our memory and fetches the IMAGE_NT_HEADER->FileHeader.Machine value before finishing. Recommended to call once and save results.
	/// </summary>
	/// <param name="Target">Unicode String pointing to the file to check.</param>
	/// <returns>Returns the Machine entry for the struct as told at https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-image_file_header. If you get something other than those or get zero, something went wrong.  </returns>
	/// <remarks>This does involve calling the ImageNtHeader DebugHelp() without taking care to sync calls. TODO: Add that. </remarks>
	DWORD WINAPI GetPEMachineTypeW(LPCWSTR Target) noexcept;

	/// <summary>
	/// Ask for Windows Debug Privilege and return if it was granted.
	/// </summary>
	/// <returns></returns>
	BOOL WINAPI AskForDebugPriv() noexcept;


	/// <summary>
	/// Duplicate handle into the target process. Will still need to figure a way to put the handle there.
	/// </summary>
	/// <param name="CurrentHandle">Source handle to duplicate. Probably wont' work if not a valid handle.</param>
	/// <param name="Access">Access mask specifying your desired access.</param>
	/// <param name="CopyAccess">set to true to use the handle's existing exist</param>
	/// <param name="RemoteProcess">Process to copy too. Will need access flags (PROCESS_DUP_HANDLE | PROCESS_VM_WRITE | PROCESS_VM_OPERATION) at minimum. For example use the OpenProcessForDuplicatingHandle() will attempting opening a handle with those access points routine in ProcessHandling.cpp</param>
	/// <param name="Inherit"></param>
	/// <returns></returns>
	HANDLE WINAPI RemoteHandleDup(HANDLE CurrentHandle, DWORD Access, BOOL CopyAccess, HANDLE RemoteProcess, BOOL Inherit) noexcept;

	/// <summary>
	/// Duplicate a handle in the local process to another local handle with the option of access specifying or keeping current access. 
	/// </summary>
	/// <param name="CurrentHandle"></param>
	/// <param name="Access"></param>
	/// <param name="CopyAccess"></param>
	/// <returns>0 on failure and the new handle on ok.</returns>
	HANDLE WINAPI LocalHandleDup(HANDLE CurrentHandle, DWORD Access, BOOL CopyAccess) noexcept;

	/// <summary>
	/// Write an 8 byte value to the specified local memory location.
	/// </summary>
	/// <param name="MemoryLocation">Local virtual memory location to write too.</param>
	/// <param name="value">value to write</param>
	/// <remarks>If getting into the x86/x64 code usage take care to note that MemoryLocation's size is depending on what machine one targets InsightApi for.</remarks>
	/// <returns>Returns true if MemoryLocation is not null and the write was OK.</returns>
	BOOL WINAPI Poke8(const unsigned long long* MemoryLocation, unsigned long long value) noexcept;

	/// <summary>
	/// Read an 8 byte (QWORD sized) value from a specific location in the local virtual memory
	/// </summary>
	/// <param name="MemoryLocation">Non null location  indicating where to read from</param>
	/// <returns>If you pass 0 for memory location, returns 0 otherwise returns the contents of the location</returns>
	unsigned long long WINAPI Peek8(unsigned long long* MemoryLocation) noexcept;

	/// <summary>
	/// Write a 4 byte (DWORD) sized value to the passed non null location
	/// </summary>
	/// <param name="MemoryLocation">non null value pointing where to write too</param>
	/// <param name="Value">value to write</param>
	/// <returns>true if it worked and false if you pass null</returns>
	BOOL WINAPI Poke4(DWORD* LocalMemoryLocation, DWORD Value) noexcept;
	
	/// <summary>
	/// Read a 4 byte (DWORD sized) value from a specific location in the local virtual memory
	/// </summary>
	/// <param name="MemoryLocation">Non null location  indicating where to read from</param>
	/// <returns>If you pass 0 for memory location, returns 0 otherwise returns the contents of the location</returns>
	DWORD WINAPI Peek4(const DWORD* LocalMemoryLocation) noexcept;




	/// accessed in theadcontext and utility. If not zero, points to kernel32.dll
	extern HMODULE Kernel32;

}

