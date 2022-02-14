


#include "ProcessHandling.h"
#include <StaticIncludes.h>

/*
* ProcessHandling.CPP.
* 
* Some shorthand for opening proceses for access.
* 
* Most are exported as UtilXXXXX where XXXX is the  name - for example UtilOpenProcessForMemory()
*/

/// <summary>
/// Helper to open a process for reading and wrting the VM space. More so for .net
/// </summary>
/// <param name="ProcessID"></param>
/// <returns></returns>
HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID) 
{
	return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, ProcessID);
}

HANDLE WINAPI OpenProcesForQueryInformation(DWORD ProcessID)
{
	return OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
}

HANDLE WINAPI OpenProcessForQueryRead(DWORD ProcessID)
{
	return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
}

HANDLE WINAPI OpenProcessForQueryReadWrite(DWORD ProcessID)
{
	return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE, FALSE, ProcessID);
}