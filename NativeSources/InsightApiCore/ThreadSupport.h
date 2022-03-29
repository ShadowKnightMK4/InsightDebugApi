#pragma once
#include <vector>
#include <map>
#include "ThreadContext.h"
/// <summary>
/// Manage and separate threads received from debug events. DOES NOT actually know of threads it does not receive the correct notification from 
/// </summary>
class ThreadContainer
{
public:
	/// <summary>
	/// Pass a Create thread debug event to this class and start tracking said thread.
	/// </summary>
	/// <param name="Event">the event to read from</param>
	/// <returns>Returns true if OK and false if Event=null, invalid thread handle or Not a CREATE_DEBUG_THREAD_EVENT </returns>
	bool ProcessCreateThreadDebugEvent(LPDEBUG_EVENT Event);
	/// <summary>
	/// handle an exit thread event
	/// </summary>
	/// <param name="Event"></param>
	/// <returns></returns>
	bool ProcessExitThreadDebugEvent(LPDEBUG_EVENT Event);
	/// <summary>
	/// Handle a process quitting event.
	/// </summary>
	/// <param name="Event"></param>
	/// <returns></returns>
	bool ProcessExitProcessDebugEvent(LPDEBUG_EVENT Event);

	/// <summary>
	/// Specify what do to when handling dead threads
	/// </summary>
	/// <param name="Purge">If true, we remove the thread from our list. If False, Thread is kept in the list until this object is finelly deleted</param>
	/// <returns></returns>
	bool SetDeadThreadPreference(BOOL Purge);

	/// <summary>
	/// Option to Purge all process dead threads in the list;
	/// </summary>
	void ManualPurge();
	/// <summary>
	/// Option to Purge this process's dead threads in our list.
	/// </summary>
	/// <param name="dwProcessID"></param>
	void ManualPurge(DWORD dwProcessID);

	/// <summary>
	/// Remove this process (and all threads tracked in) from our list.
	/// </summary>
	/// <param name="dwProcessID"></param>
	void RemoveSingleProcess(DWORD dwProcessID);
	/// <summary>
	/// Remove a single thread whose process ID is thsi and thread IF is that.
	/// </summary>
	/// <param name="dwProcessID"></param>
	/// <param name="dwThreadID"></param>
	void RemoveSingleThread(DWORD dwProcessID, DWORD dwThreadID);

	
	ThreadInsight* GetThreadInsightPtr(DWORD dwProcessID, DWORD dwThreadID);
	/// <summary>
	/// Return how many processes in DB have an entry for threads
	/// </summary>
	/// <returns></returns>
	DWORD ProcessCount();

	/// <summary>
	/// How many threads are known for this process.
	/// </summary>
	/// <param name="ProcessId"></param>
	/// <returns></returns>
	DWORD ThreadCount(DWORD ProcessId);
private:
	/// <summary>
	/// DWORD = Process ID.
	/// vector of threadinsights = known threads received viea CREATE_THREAD_DEBUG_EVENT (may not be all threads> - see NtSetInformation HideFromDebugger
	/// </summary>
	std::map<DWORD, std::vector<ThreadInsight*>> View;
	/// <summary>
	/// If set, clear threads from our list.
	/// </summary>
	BOOL PurgePreference;
};