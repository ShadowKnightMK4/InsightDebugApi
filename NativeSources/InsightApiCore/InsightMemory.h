#pragma once
#include <Windows.h>
#include <Psapi.h>
/// <summary>
/// Get Memory Stats about a specific process and its current working set. Target must be openable for PROCESS_VM_READ and PROCESS_QUERY_INFORMATION
/// </summary>
class InsightMemory
{
public:
	InsightMemory();
	InsightMemory(DWORD ProcessID);
	InsightMemory(HANDLE ProcessHandle);
	InsightMemory(const InsightMemory& Other);
	~InsightMemory();
	
	/// <summary>
	/// Choose which process we ask about via processId
	/// </summary>
	/// <param name="Process"></param>
	/// <returns></returns>
	BOOL SetTargetProcess(DWORD Process);
	/// <summary>
	/// Choose which process asked about via a handle. You don't need to keep handle open afterwards. Process handle must be abl;e to be duplicated with PROCESS_QUERY_INFORMATION and PROCESS_VM_READ
	/// </summary>
	/// <param name="ProcessHandle"></param>
	/// <returns>True if it worked and false if it did not</returns>
	BOOL SetTargetProcess(HANDLE ProcessHandle);

	/// <summary>
	/// Refresh the memory stats. 
	/// </summary>
	BOOL UpdateMemoryStats();
	/// <summary>
	/// Get page fault value. May trigger a Refresh if Auto Refresh is set otherwise you'll need to call UpdateMemoryFirst() to get the update value.
	/// </summary>
	/// <returns></returns>
	SIZE_T GetPageFaultCount();

	/// <summary>
	/// Get Peak working set memory usage for the target process. 
	/// </summary>
	/// <returns></returns>
	SIZE_T GetPeakWorkingSetSize();
	/// <summary>
	/// Get Working set Size for the target process
	/// </summary>
	/// <returns></returns>
	SIZE_T GetWorkingSetSize();
	/// <summary>
	/// Get the Quota Peak Paged Pool Usage
	/// </summary>
	/// <returns></returns>
	SIZE_T GetQuotaPeakPagedPoolUsage();
	/// <summary>
	/// Get the Quota Page Pool Usage
	/// </summary>
	/// <returns></returns>
	SIZE_T GetQuotaPagePoolUsage();
	/// <summary>
	/// Get the Peak Non Paged Pool Usage
	/// </summary>
	/// <returns></returns>
	SIZE_T GetPeakNonPagePoolUsage();

	/// <summary>
	/// Get the peaked page Pool usage
	/// </summary>
	/// <returns></returns>
	SIZE_T InsightMemory_GetPeakPagePoolUsage();
	/// <summary>
	/// Get Page File Usage
	/// </summary>
	/// <returns></returns>
	SIZE_T GetPageFileUsage();
	/// <summary>
	/// Get Peak Paged File Usage
	/// </summary>
	/// <returns></returns>
	SIZE_T GetPeakPageFileUsage();
	/// <summary>
	/// Get Private Memory Usage
	/// </summary>
	/// <returns></returns>
	SIZE_T GetPrivateUsage();
	
	/// <summary>
	/// Get the quota non paged pool usage
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	SIZE_T GetQuotaNonPagedPoolUsage();
	/// <summary>
	///  Get peak paged pool usage
	/// </summary>
	/// <returns></returns>
	SIZE_T GetQuotaPeakPagePoolUsage();
	/// <summary>
	/// Get a pointer to a struct containing memory counter data. Will always be refreshed before returned.
	/// </summary>
	/// <returns>Returns pointer containing the memory stats. Returns null if it can't get them. Don't free returned pointer.</returns>
	PROCESS_MEMORY_COUNTERS_EX* GetMemoryStatsBulk();
	/// <summary>
	/// Indicate if you want the Get Routines to refresh PROCESS_MEMORY_COUNTERS_EX routines with each call or only via a call via UpdateMemoryStats().
	/// </summary>
	/// <param name="Enable"></param>
	/// <returns>returns old value</returns>
	BOOL SetAutoRefreshMemoryStats(BOOL Enable);
	/// <summary>
	/// Return if each individual call to GetXXXUSage() gets an updated value. Disabling can inprovate performance provided you put a call to UpdateMemoryStats() before you call the GetXXXRoutines()
	/// </summary>
	/// <returns>returns if iit workedor oto</returns>
	BOOL GetAutoRefreshMemoryStats();

	
private:
	DWORD dwProcessID;
	HANDLE ProcessHandle;
	PROCESS_MEMORY_COUNTERS_EX MemoryStats;
	bool AutoRefreshMemoryStats;

	void RefreshWorkingSet();

	/// <summary>
	/// In line helper to refresh memory stats if AutoRefshEnabled
	/// </summary>
	inline void InlineRefreshMemoryStats();

	PSAPI_WORKING_SET_EX_INFORMATION* WorkingSetContainer;
	DWORD WorkingSetContainerSize;
	
};
