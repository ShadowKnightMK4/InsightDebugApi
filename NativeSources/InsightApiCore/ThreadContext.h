#pragma once
#include <Windows.h>

/*
* Control and Configure Threads whose dwThread ID / handle you've gotten typically through handling a CREATE_THREAD_DEBUG_EVENT but not limited to that..
* Not strictly built to be part of the stuff BUT useful to track and change settings.
* 
* Why do we copy AddRef()/Release() from COM?
* It is because this lcass is publicly accessable thru the sheath and has a container class also. DEleting a class instance from the contariner list without
* accouting for the .NET side of things leaves a non functional .Net Class instance. 
*/
class ThreadInsight
{
public:

	/// <summary>
	/// The copy constructor needs to ensure the processid, threadid are copied and the new copy gets a dedicated handle to the target thread.
	/// </summary>
	/// <param name="Other"></param>
	ThreadInsight(const ThreadInsight& Other);
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
	CONTEXT* GetTargetThreadContext();
	/// <summary>
	/// Set the thread's new context
	/// </summary>
	/// <param name="NewContext"></param>
	/// <returns></returns>
	BOOL SetTargetThreadContext(CONTEXT* NewContext);

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
	/// Return if the target thread's priority boost is enabled
	/// </summary>
	/// <returns></returns>
	bool GetThreadPriorityBoostValue();
	/// <summary>
	/// Set the priority boost for the target thread
	/// </summary>
	/// <param name="EnablePriorityBoost"></param>
	/// <returns></returns>
	bool SetThreadPriorityBoostValue(bool EnablePriorityBoost);


	

	/// <summary>
	/// Get the thread's exit code.
	/// </summary>
	/// <returns></returns>
	DWORD GetTargetThreadExitCode();

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
	/// <param name="NewThread">Handle to the new thread.  You do not need to keep it open afterwards as the class duplicates it to a private value</param>
	/// <returns>returns true if it worked and false if it failed </returns>
	BOOL SetTargetThread(HANDLE NewThread);
	/// <summary>
	/// Choose a different thread to target by thread if
	/// </summary>
	/// <param name="dwThreadID">thread id for the new thread</param>
	/// <returns>Returns true if it worked and false if it failed</returns>
	BOOL SetTargetThread(DWORD dwThreadID);


	/// <summary>
	/// Suspend the thread this class instance targets by calling SuspendThread() or Wow64SuspendThread() May trigger a LoadLibrary if Wow64Suspend
	/// </summary>
	/// <returns></returns>
	DWORD SuspendThreadHandle();
	/// <summary>
	/// Resume the thread this class targets by calling ResumeThread()
	/// </summary>
	/// <returns></returns>
	DWORD ResumeThreadHandle();

	/// <summary>
	/// Get the context of a wow64 process thread. If it's not a wow64 process, returns null instead
	/// </summary>
	/// <returns>Returns null if not wow64 process and a pointer to a WOW64_CONTEXT if it is </returns>
	WOW64_CONTEXT* Wow64GetContext();
	/// <summary>
	/// Get the context of 64-bit process thread, if not that, 
	/// </summary>
	/// <returns></returns>
	CONTEXT* GetContext();

	/// <summary>
	/// Set the context of a Wow64 process thread. Fails if not a Wow64 process. 
	/// </summary>
	/// <param name="Context"></param>
	/// <returns>TRUE if it worked and false if it did not (If not a wow64 - returns false)</returns>
	BOOL Wow64SetContext(WOW64_CONTEXT* Context);
	/// <summary>
	/// Set the context of a 64-bit thread. Fails if not a that
	/// </summary>
	/// <param name="Context"></param>
	/// <returns></returns>
	BOOL SetContext(CONTEXT* Context);

	/// <summary>
	/// Get the id of the thread that this class is pointing to.
	/// </summary>
	/// <remarks>First calls the windows routine to get the thread of the handle and stores for later. Repeated calls return this value</remarks>
	/// <returns>Thread id of the class this points too</returns>
	DWORD GetTargetThreadId();

	/// <summary>
	/// Get the current ideal processor for the thread without chanigng it.
	/// </summary>
	/// <returns></returns>
	DWORD GetIdealThreadProcessorMask();

	/// <summary>
	/// Set the ideal processor for the passed thread. Returns the old value of the ideal processor or (DWORD)-1 on error
	/// </summary>
	/// <returns></returns>
	DWORD SetIdealThreadProcessorMask(DWORD NewPreferredProcessor);
	/// <summary>
	/// </summary>
	VOID AddRef();
	VOID Release();
private:


	/// <summary>
	/// Handle to the thread we are working on.
	/// </summary>
	HANDLE ThreadHandle;
	DWORD dwProcessID;
	DWORD dwThreadID;

	/// <summary>
	/// Determine if Wow64 or not. Fetches the context (CONTEXT* || WOW64_CONTEXT*) and stashes in a privaae variable. 
	/// </summary>
	/// <returns>returns true if worked and false it if did not</returns>
	BOOL FetchContext();
	/// <summary>
	/// Determine if wow64 or not and call the correct way to assign context. May be either a CONTEXT* or a WOW64_CONTEXT* pointer
	/// </summary>
	BOOL ApplyContext(CONTEXT* Context);
	/// <summary>
	/// Provides memory sharing between the 2 structs for thread context
	/// </summary>
	union ContextUnion
	{
		CONTEXT Win64Context;
		WOW64_CONTEXT Win32Context;
	}; 

	/// <summary>
	/// if Wow64, use Win32Context. Otherwise Use Win64Contect
	/// </summary>
	ContextUnion ThreadContext;
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