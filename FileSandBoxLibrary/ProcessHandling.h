#pragma once


#include "PS_ProcessInformation.h"

#include <Windows.h>

/// <summary>
/// Helper to open a process for reading and writing the VM space of that Process. More so for .NET
/// </summary>
/// <param name="ProcessID">ID of the Process to open</param>
/// <returns>Handle to the process with PROCESS_VM_READ and PROCESS_VM_WRITE access if OK, otherwise returns 0</returns>
HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID);

/// <summary>
/// Helper to open a process querying some information and reading + writing that process's virtual memory.
/// </summary>
/// <param name="ProcessID"></param>
/// <returns></returns>
HANDLE WINAPI OpenProcessForQueryReadWrite(DWORD ProcessID);


/// <summary>
/// Helper to open a process for querying some information PROCESS_QUERY_INFORMATION about that process. More so for .net
/// </summary>
/// <param name="ProcessID">ID of the Process to open</param>
/// <returns></returns>
HANDLE WINAPI OpenProcesForQueryInformation(DWORD ProcessID);

/// <summary>
/// Helper to open a process for reading and writing the VM space of that Process. More so for .net
/// </summary>
/// <param name="ProcessID">ID of the Process to open</param>
/// <returns>Handle to the process with PROCESS_VM_READ and PROCESS_VM_WRITE access if OK, otherwise returns 0</returns>
HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID);
