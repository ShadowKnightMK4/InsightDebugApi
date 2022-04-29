


#include "ProcessHandling.h"
//#include <StaticIncludes.h>

/// <summary>
/// Open a process handle fit for duplicating a handle for and writing the handle into.
/// </summary>
/// <param name="ProcessID">ID of the Process to open</param>
/// <returns></returns>
HANDLE WINAPI OpenProcessForDuplicatingHandle(DWORD ProcessID)
{
	return OpenProcess(PROCESS_DUP_HANDLE | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, ProcessID);
}


/// <summary>
/// This one is used when debugged opening process handles. It simply asks for everything possible. (PROCESS_ALL_ACCESS)
/// </summary>
/// <param name="ProcessID"></param>
/// <returns></returns>
HANDLE WINAPI OpenProcessNow(DWORD ProcessID)
{
#ifdef _DEBUG
	OutputDebugString(L"Debug Warning Warning OpenProcessWith ALL ACCESS\r\n");
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
#else
	OutputDebugString(L"Debug Warning. OpenProcessNow() is dummied out in release builds");
#endif // _DEBUG
	return 0;
}
/*
* ProcessHandling.CPP.
* 
* Some shorthand for opening Windows for various access reasons/
* 
* Most are exported as UtilXXXXX where XXXX is the  name - for example UtilOpenProcessForMemory()
*/

HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID) 
{
	return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, ProcessID);
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