


#include "ProcessHandling.h"
/*
* ProcessHandling.CPP.
*
* Some shorthand for opening Windows for various access reasons/
*
* Most are exported as UtilXXXXX where XXXX is the  name - for example UtilOpenProcessForMemory()
*/

extern "C" {

	HANDLE WINAPI OpenProcessForDuplicatingHandle(DWORD ProcessID) noexcept
	{
		return OpenProcess(PROCESS_DUP_HANDLE | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, ProcessID);
	}


	HANDLE WINAPI OpenProcessNow(DWORD ProcessID) noexcept
	{
#ifdef _DEBUG
		OutputDebugString(L"Debug Warning Warning OpenProcessWith ALL ACCESS\r\n");
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
#else
		OutputDebugString(L"Debug Warning. OpenProcessNow() is dummied out in release builds");
#endif // _DEBUG
		return 0;
	}

	HANDLE WINAPI OpenProcessForMemory(DWORD ProcessID) noexcept
	{
		return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, ProcessID);
	}

	/// <summary>
	/// Helper to open a process for querying some information about that process. More so for .NET
	/// </summary>
	/// <param name="ProcessID">ID of the Process to open</param>
	/// <returns></returns>
	HANDLE WINAPI OpenProcesForQueryInformation(DWORD ProcessID) noexcept
	{
		return OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
	}

	/// <summary>
	/// Helper to open a process for reading from its virtual memory and querying information about that process. More so for .NET
	/// </summary>
	/// <param name="ProcessID">ID of the Process to open</param>
	/// <returns></returns>

	HANDLE WINAPI OpenProcessForQueryRead(DWORD ProcessID) noexcept
	{
		return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
	}

	HANDLE WINAPI OpenProcessForQueryReadWrite(DWORD ProcessID) noexcept
	{
		return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE, FALSE, ProcessID);
	}

}