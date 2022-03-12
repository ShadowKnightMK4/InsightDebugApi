


#include "ProcessHandling.h"
#include <StaticIncludes.h>

/*
* ProcessHandling.CPP.
* 
* Some shorthand for opening Windows for various access reasons/
* 
* Most are exported as UtilXXXXX where XXXX is the  name - for example UtilOpenProcessForMemory()
*/

HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID) 
{
	return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, ProcessID);
}

/// <summary>
/// Helper to open a process for querying some information about that process. More so for .NET
/// </summary>
/// <param name="ProcessID">ID of the Process to open</param>
/// <returns></returns>
HANDLE WINAPI OpenProcesForQueryInformation(DWORD ProcessID)
{
	return OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
}

/// <summary>
/// Helper to open a process for reading from its virtual memory and querying information about that process. More so for .NET
/// </summary>
/// <param name="ProcessID">ID of the Process to open</param>
/// <returns></returns>

HANDLE WINAPI OpenProcessForQueryRead(DWORD ProcessID)
{
	return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
}

HANDLE WINAPI OpenProcessForQueryReadWrite(DWORD ProcessID)
{
	return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE, FALSE, ProcessID);
}