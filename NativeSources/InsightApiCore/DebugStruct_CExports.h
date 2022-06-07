#pragma once
extern "C" {
	/// <summary>
	/// Allocate a block of memory using libc's malloc() that's big enough to contain a DEBUG_EVENT structure. Also clears it to zero.
	/// </summary>
	/// <returns>returns a block of memory big enough to hold a DEBUG_EVENT struct with values already set to zero</returns>
	LPDEBUG_EVENT WINAPI DebugEvent_AllocateStructure()  noexcept;


	/// <summary>
	/// Returns if the process id containing within the Ptr is from a 32-bit or 64-bit process by using IsTargetProcessID32Bit() contained within Utility.cpp
	/// </summary>
	/// <param name="Ptr">Non Zero Pointer to a block of memory containing a DEBUG_EVENT structure</param>
	/// <returns>Returns the result of Calling IsTargetProcessID32Bit() against the process ID contained within the passed struct. If Ptr is null, returns FALSE</returns>
	BOOL WINAPI DebugEvent_IsEventFrom32Bit(const LPDEBUG_EVENT Ptr) noexcept;

	/// <summary>
	/// Return the ID of the process that this DEBUG_EVENT structure refers too.
	/// </summary>
	/// <param name="Ptr">Non Zero Pointer to a block of memory containing a DEBUG_EVENT structure</param>
	/// <returns>If Ptr is 0, returns 0 otherwise returns the DEBUG_EVENT->dwProcessID Member</returns>
	DWORD WINAPI DebugEvent_GetProcessId(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Returns the ThreadID of the thread this DEBUG_EVENT structure refers too.
	/// </summary>
	/// <param name="Ptr">Non Zero Pointer to a block of memory containing a DEBUG_EVENT structure</param>
	/// <returns>If Ptr is 0, returns 0 otherwise returns the DEBUG_EVENT->dwThreadID member</returns>
	DWORD WINAPI DebugEvent_GetThreadId(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Returns the event type of the passed DEBUG_EVENT. The event type specifics which union in the struct is valid
	/// </summary>
	/// <param name="Ptr">Pointer to a DEBUG_EVENT structure</param>
	/// <returns>If Ptr is 0, returns 0 otherwise returns the DEBUG_EVENT->dwEventCode member</returns>
	DWORD WINAPI DebugEvent_GetEventType(const LPDEBUG_EVENT Ptr) noexcept;

	/// <summary>
	/// Returns the error in the contained Ptr's RIP event. If not a RIP event a null pointer is passed, returns 0 instead
	/// </summary>
	/// <param name="Ptr">A non-null pointer pointing to a DEBUG_EVENT struct containing a RIP_EVENT</param>
	/// <returns>Returns either the DEBUG_EVENT->u.RipInfo.dwError part of the RIP_IFO struct containing within this DEBUG_EVENT. If the event is not a RIP_EVENT or the passed pointer is null, returns 0</returns>
	DWORD WINAPI DebugEvent_RipGetError(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Returns the type of error in the contained Ptr's RIP event. If not a RIP event a null pointer is passed, returns 0 instead
	/// </summary>
	/// <param name="Ptr">A non-null pointer pointing to a DEBUG_EVENT struct containing a RIP_EVENT</param>
	/// <returns>Returns either the DEBUG_EVENT->u.RipInfo.dwType part of the RIP_IFO struct containing within this DEBUG_EVENT. If the event is not a RIP_EVENT or the passed pointer is null, returns 0</returns>
	DWORD WINAPI DebugEvent_RipGetErrorType(const LPDEBUG_EVENT Ptr) noexcept;

	/// <summary>
	/// Return the Exception code in the contained DEBUG_EVENT struct if it is an EXCEPTION_DEBUG_EVENT 
	/// </summary>
	/// <param name="Ptr">Non null ptr. </param>
	/// <returns>if Ptr is null, returns 0, otherwise checks if event came from a 32 or 64 bit process via DebugEvent_IsEventFrom32Bit() and returns the ExceptionCode </returns>
	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionCode(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Return the Exception Flags in the contained DEBUG_EVENT struct if it is an EXCEPTION_DEBUG_EVENT 
	/// </summary>
	/// <param name="Ptr">Non null ptr. </param>
	/// <returns>if Ptr is null, returns 0, otherwise checks if event came from a 32 or 64 bit process via DebugEvent_IsEventFrom32Bit() and returns the ExceptionFlags member </returns>
	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionFlags(const LPDEBUG_EVENT Ptr) noexcept;
	

	/// <summary>
	/// Return this Exception Record value which may pointer to another EXCEPTION_RECORD in the contained DEBUG_EVENT struct if it is an EXCEPTION_DEBUG_EVENT 
	/// </summary>
	/// <param name="Ptr">Non null ptr. </param>
	/// <returns>if Ptr is null, returns 0. The returned pointer may also be 0. This returns a pointer to the Exception Record contained within this struct.  x86 debugged Processes should not see a size larger than DWORD_MAX / 4 bytes.  x64 machines will potentially see that. The DWORD64 return value means it should be big enough to hold either when compiled from the same source.</returns>
	DWORD64 WINAPI DebugEvent_ExceptionInfo_GetExceptionRecord(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Returns the address the contained exception triggered from. 
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns>if Ptr is null, returns 0. The returned pointer may also be 0. This returns a pointer to the ExceptionAddress contained within this struct.  x86 debugged Processes should not see a size larger than DWORD_MAX / 4 bytes.  x64 machines will potentially see that. The DWORD64 return value means it should be big enough to hold either when compiled from the same source.</returns>
	DWORD64 WINAPI DebugEvent_ExceptionInfo_GetExceptionAddress(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Return the number of arguments that came with the exception.  These will vary depending on what type of exception.
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_ExceptionInfo_GetExceptionArgumentCount(const LPDEBUG_EVENT Ptr) noexcept;

	/// <summary>
	/// Return the value if the exception is as first chance exception or not.
	/// </summary>
	/// <param name="Ptr"></param>
	/// <returns></returns>
	DWORD WINAPI DebugEvent_ExceptionInfo_GetFirstChance(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Allocates a block of memory large enough to contain an EXCEPTION_MAXIMUM_PARAMETERS (currently 15) sized array of DWORD64 values and copies the contents of the contained array there.  x86 debugged process values are promoted from DWORD (4 bytes) to DWORD64 (8 byte) sized values with the upper half always being zero.
	/// </summary>
	/// <param name="Ptr">Non Zero Pointer to a block of memory containing a DEBUG_EVENT structure</param>
	/// <returns>returns a buffer allocated to be (EXCEPTION_MAXIMUM_PAMETERS* sizeof(DWORD64)) and copies contents of the event there.  Caller will need to free. </returns>
	DWORD64* WINAPI DebugEvent_ExceptionInfo_GetExceptionInformation(LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Return if the exception is a first chance or not.
	/// </summary>
	/// <param name="Ptr">Non null Ptr to a DEBUG_EVENT struct containing an EXCEPTION_DEBUG_EVENT</param>
	/// <returns>If Ptr is null or not an EXCEPTION_DEBUG_EVENT, returns 0. Otherwise returns the Exception.dwFirstChange value which is true or false.</returns>
	DWORD WINAPI DebugEvent_ExceptionInfo_GetFirstChance(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Return the value of hFile handle if the ptr passed belongs to a DEBUG_EVENT load DLL structure
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a LOAD_DLL_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a LOAD_DLL_DEBUG_EVENT, returns INVALID_HANDLE_VALUE (x86 callers this is 0xffffffff (4 bytes) and x64 callers this is 0xffffffffffffffff (8 bytes) due to HANDLES being the size of pointers)</returns>
	HANDLE WINAPI DebugEvent_LoadDllInfo_GetHFile(const LPDEBUG_EVENT Ptr) noexcept;

	/// <summary>
	/// Get the base of the DLL if  the passed DEBUG_EVENT is a LOAD_DLL_DEBUG event
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a LOAD_DLL_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a LOAD_DLL_DEBUG_EVENT, returns 0, otherwise returns the LoadDll.lpBaseOfDll. </returns>
	LPVOID WINAPI DebugEvent_LoadDllInfo_GetBaseOfDll(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Get the Offset into the DLL file where its debug info is located if passed ptr is a LOAD_DLL_DEBUG event
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a LOAD_DLL_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a LOAD_DLL_DEBUG_EVENT, returns 0, otherwise returns the LoadDll.dwDebugInfoFileOffset. </returns>
	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoOffset(const LPDEBUG_EVENT Ptr) noexcept;

	/// <summary>
	/// Get how big the DLL's Debug Info is within the file if the passed ptr is a LOAD_DLL_DEBUG event
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a LOAD_DLL_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a LOAD_DLL_DEBUG_EVENT, returns 0, otherwise returns LoadDll.nDebugInfoSize</returns>
	DWORD WINAPI DebugEvent_LoadDllINfo_GetDebugInfoSize(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Get a pointer to a string containing the image name of the loaded DLL within the LOAD_DLL_DEBUG struct. May be null and may or may not be within the context of the Target debugged process
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a LOAD_DLL_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a LOAD_DLL_DEBUG_EVENT, returns 0, otherwise returns LoadDll.lpImageName. Note Pointer is likely only relevant in the process you're debugging. Aka, ReadProcessMemory() should likely be called. There are alternatives such as GetModuleNameViaHandle(), GetFileNameViaHandle() in the utility.cpp routines</returns>
	LPVOID WINAPI DebugEvent_LoadDllInfo_GetImageNamge(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// return the part of the struct that indicates if the ImageName member (DebugEvent_LoadDllInfo_GetImageNamge())  will be an ANSI or Unicode string.
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a LOAD_DLL_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a LOAD_DLL_DEBUG_EVENT, returns 0, otherwise returns the u.LoadDll.fUnicode value</returns>
	BOOL WINAPI DebugEvent_LoadDllInfo_GetUnicodeFlag(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Get the base address of the DLL location during a passed ed UNLOAD_DLLL_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a LOAD_DLL_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a UNLOAD_DLL_DEBUG_EVENT, returns 0. Do note that x86 processes will only use the lower DWORD part of this return value. x64 will use it all</returns>
	DWORD64 WINAPI DebugEvent_UnloadDllInfo_GetBaseAddress(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Get the passed handle for a thread contained within a CREATE_THREAD_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a CREATE_THREAD_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a CREATE_THREAD_DEBUG_EVENT, returns 0. Otherwise returns CreateThread.uThread</returns>
	HANDLE WINAPI DebugEvent_CreateThreadInfo_GetThreadHandle(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// return a pointer to the start address of the passed thread
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a CREATE_THREAD_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a CREATE_THREAD_DEBUG_EVENT, returns 0. Otherwise returns CreateThread.lpStartAddress</returns>
	LPTHREAD_START_ROUTINE WINAPI DebugEvent_CreateThreadInfo_GetThreadStartAddress(const LPDEBUG_EVENT Ptr) noexcept;
	

	/// <summary>
	/// Get a pointer to the thread's TLS Location for the CREATE_THREAD_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a CREATE_THREAD_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a CREATE_THREAD_DEBUG_EVENT, returns 0. Otherwise returns CreateThread.lpThreadLocalBase</returns>
	LPVOID WINAPI DebugEvent_CreateThreadInfo_GetThreadLocalBase(const LPDEBUG_EVENT Ptr) noexcept;


	/// <summary>
	/// Get the exit code obtained within an EXIT_THREAD_DEBUG_EVENT
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a EXIT_THREAD_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a EXIT_THREAD_DEBUG_EVENT, returns 0. Otherwise returns ExitThread.dwExitCode</returns>
	DWORD WINAPI DebugEvent_ExitThreadInfo_GetExitCode(const LPDEBUG_EVENT Ptr) noexcept;

	/// <summary>
	/// return the exit code objected when a process existed via the EXIT_PROCESS_DEBUG_EVENT 
	/// </summary>
	/// <param name="Ptr">Non null ptr to a DEBUG_EVENT struct containing a EXIT_THREAD_DEBUG_EVENT</param>
	/// <returns>If Ptr is 0 or not a EXIT_PROCESS_DEBUG_EVENT, returns 0. Otherwise returns ExitProcess.dwExitCode</returns>
	DWORD WINAPI DebugEvent_ExitProcessInfo_GetExitCode(const LPDEBUG_EVENT Ptr) noexcept;


}

/// <summary>
/// This is used in the c level routines when the contained exception 32 or 64 needs to be defined.
/// </summary>
union MachineDependantExceptionRecord
{
	EXCEPTION_RECORD32 Except32;
	EXCEPTION_RECORD64 Except64;
};