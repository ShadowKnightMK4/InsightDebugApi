#pragma once
#include <Windows.h>

/*
* Control and Configure Threads whose dwThread ID / handle you've gotten typically through handling a CREATE_THREAD_DEBUG_EVENT but not limited to that..
* Not strictly built to be part of the stuff BUT useful to track and change settings.
*/
class ThreadInsight
{
public:
	/// <summary>
	/// Create an instance of this class without it pointing to any specific thread. You will need to call <see also name="SetTargetThread"> to specify a thread to work with</see also>
	/// </summary>
	ThreadInsight();
	/// <summary>
	/// Create an instance of this class and point it to the thread specified by dwThreadId.    
	/// </summary>
	/// <param name="dwThreadID"></param>
	ThreadInsight(DWORD dwThreadID);
	/// <summary>
	/// Create an instance of this class and point it to the thread specified by the handle.   You do not need to keep the handle open as it duplicates it to a private one.
	/// </summary>
	/// <param name="HandleToThread">handle to the thread to point this class too</param>
	ThreadInsight(HANDLE HandleToThread);
	
	/// <summary>
	/// Destructor
	/// </summary>
	~ThreadInsight();

	/// <summary>
	/// Set the description for the thread Note: Will trigger LoadLibraryW() calls for Kernel32 / KernelBase the first time you call this
	/// </summary>
	/// <param name="Description"></param>
	/// <returns></returns>
	BOOL SetThreadDescriptionW(const wchar_t* Description);
	/// <summary>
	/// get the description for a thread. Note: Will trigger LoadLibraryW() calls for Kernel32 / KernelBase when you first call this.
	/// </summary>
	/// <returns></returns>
	const wchar_t* GetThreadDescriptionW();

	/// <summary>
	/// Get a pointer to the generic thread context of the main thread
	/// </summary>
	/// <returns></returns>
	CONTEXT* GetThreadContext();
	/// <summary>
	/// Set the thread's new context
	/// </summary>
	/// <param name="NewContext"></param>
	/// <returns></returns>
	BOOL SetThreadContext(CONTEXT* NewContext);

	/// <summary>
	/// Get the thread's priority
	/// </summary>
	/// <returns>Priority for the thread</returns>
	int GetHandleThreadPriority();

	/// <summary>
	/// Assign the thread priority
	/// </summary>
	/// <param name="NewPriority">new priority</param>
	/// <returns>true if it worked and false if it didn't</returns>
	bool SetHandleThreadPriority(int NewPriority);


	/// <summary>
	/// Updates the FILETIME private variables and returns a pointer to the one specifying the exit time (if thread appears active still, exit time will be set to 0)
	/// </summary>
	/// <returns>Returns pointer to FILETIME struct indicating the exit time. (If thread appears active still i.e. GetThreadExitCode() returns STILL_ACTIVE (259), exit value is set to zero before returning pinter)</returns>
	FILETIME* GetExitTime();
	/// <summary>
	/// Updates the FILETIME private variables and returns a pointer to the one specifying the amount of time the thread's executed in user code.
	/// </summary>
	/// <returns>Returns pointer to a FILETIME struct indicating how much time the thread's executed in user mode.</returns>
	FILETIME* GetUserTime();
	/// <summary>
	/// Updates the FILETIME private variables and returns a pointer to the one specifying when the thread was created in local time
	/// </summary>
	/// <returns>Returns pointer to a FILETIME struct indicating the time the thread was created in local time.</returns>
	FILETIME* GetCreationTime();
	/// <summary>
	/// Updates the FILETIME private variables and returns a pointer to the one specifying the amount of time the thread's executed in kernel / protected code.
	/// </summary>
	/// <returns>Returns pointer to a FILETIME struct indicating how much time the thread's executed in kernel / protected mode.</returns>
	FILETIME* GetKernelTime();

	/// <summary>
	/// Choose a different thread to target by handle
	/// </summary>
	/// <param name="NewThread">Handle to the new thread.  You do not need to keep it open afterwards</param>
	/// <returns>returns true if it worked and false if it failed </returns>
	BOOL SetTargetThread(HANDLE NewThread);
	/// <summary>
	/// Choose a different thread to target by thread if
	/// </summary>
	/// <param name="dwThreadID">thread id for the new thread</param>
	/// <returns>Returns true if it worked and false if it failed</returns>
	BOOL SetTargetThread(DWORD dwThreadID);


	/// <summary>
	/// Suspend the thread this class instance targets by calling SuspendThread() or Wow64SuspendThread()
	/// </summary>
	/// <returns></returns>
	DWORD SuspendThreadHandle();
	/// <summary>
	/// Resume the thread this class targets by calling ResumeThread()
	/// </summary>
	/// <returns></returns>
	DWORD ResumeThreadHandle();
private:
	/// <summary>
	/// Handle to the thread we are working on.
	/// </summary>
	HANDLE ThreadHandle;
	DWORD dwProcessID;
	DWORD dwThreadID;

	CONTEXT* TheadContext;

	/// <summary>
	/// Contains last update of the thread exit time (or 0 if thread appears to still be running)
	/// </summary>
	FILETIME lpExitTime;
	/// <summary>
	/// Contains the last update of the thread's creation time
	/// </summary>
	FILETIME lpCreationTime;
	/// <summary>
	/// Contains the last update of the amount of time the thread's been in user mode
	/// </summary>
	FILETIME lpUserTime;
	/// <summary>
	/// Contains the last update of the amount of time the th read's been in kernel mode.
	/// </summary>
	FILETIME lpKernelTime;

	/// <summary>
	/// Holds a copy of the thread description once gotton that can be safely returned as neeed. 
	/// </summary>
	const wchar_t* ThreadDescription;

};