#pragma once


#include "PS_ProcessInformation.h"

#include <Windows.h>
extern "C" {
	/// <summary>
	/// Open a process handle fit for duplicating a handle for and writing the handle into.
	/// </summary>
	/// <param name="ProcessID">ID of the Process to open</param>
	/// <returns>Returns a handle for PROCESS_DUP_HANDLE, PROCESS_VM_WRITE, PROCESS_VM_OPERATION, </returns>
	HANDLE WINAPI OpenProcessForDuplicatingHandle(DWORD ProcessID) noexcept;

	/// <summary>
	/// Helper to open a process for reading and writing the VM space of that Process. More so for .NET
	/// </summary>
	/// <param name="ProcessID">ID of the Process to open</param>
	/// <returns>Handle to the process with PROCESS_VM_READ and PROCESS_VM_WRITE access if OK, otherwise returns 0</returns>
	HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID) noexcept;

	/// <summary>
	/// Helper to open a process querying some information and reading + writing that process's virtual memory.
	/// </summary>
	/// <param name="ProcessID"></param>
	/// <returns></returns>
	HANDLE WINAPI OpenProcessForQueryReadWrite(DWORD ProcessID) noexcept;


	/// <summary>
	/// Helper to open a process for querying some information PROCESS_QUERY_INFORMATION about that process. More so for .net
	/// </summary>
	/// <param name="ProcessID">ID of the Process to open</param>
	/// <returns></returns>
	HANDLE WINAPI OpenProcesForQueryInformation(DWORD ProcessID) noexcept;

	/// <summary>
	/// Helper to open a process for reading and writing the VM space of that Process. More so for .net
	/// </summary>
	/// <param name="ProcessID">ID of the Process to open</param>
	/// <returns>Handle to the process with PROCESS_VM_READ and PROCESS_VM_WRITE access if OK, otherwise returns 0</returns>
	HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID) noexcept;


	/// <summary>
	/// This one is intended to used when debugged opening process handles. It simply asks for everything possible. (PROCESS_ALL_ACCESS). It is dummied out in Release Builds.
	/// </summary>
	/// <param name="ProcessID"></param>
	/// <returns>For Release Builds: always returns 0.  For Debug Builds returns handle with PROCESS_ALL_ACCESS</returns>
	HANDLE WINAPI OpenProcessNow(DWORD ProcessID) noexcept;

}