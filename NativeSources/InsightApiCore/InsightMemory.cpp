#include "InsightMemory.h"

void InsightMemory::RefreshWorkingSet()
{
	if (this->WorkingSetContainer != nullptr)
	{
		ZeroMemory(WorkingSetContainer, this->WorkingSetContainerSize * sizeof(PSAPI_WORKING_SET_EX_INFORMATION));
	}
	while (true)
	{
		if (QueryWorkingSetEx(ProcessHandle, WorkingSetContainer, WorkingSetContainerSize))
		{
			break;
		}
		else
		{
			if (WorkingSetContainer != nullptr)
			{
				free(WorkingSetContainer);
			}
			WorkingSetContainerSize *= 2;
			WorkingSetContainer = (PSAPI_WORKING_SET_EX_INFORMATION*)malloc(this->WorkingSetContainerSize * sizeof(PSAPI_WORKING_SET_EX_INFORMATION));
			if (WorkingSetContainer == nullptr)
			{
				break;
			}
			else
			{
				ZeroMemory(WorkingSetContainer, this->WorkingSetContainerSize * sizeof(PSAPI_WORKING_SET_EX_INFORMATION));
			}

		}
	}
}

void InsightMemory::UpdatePerfInformation()
{
	ZeroMemory(&this->u2.PerfInformation, sizeof(this->u2));
	u2.PerfInformation.cb = sizeof(u2.PerfInformation);

	if (!GetPerformanceInfo(&u2.PerfInformation, sizeof(u2.PerfInformation)))
	{
		u2.PerfInformation.cb = 0;
	}
}

inline void InsightMemory::InlineRefreshMemoryStats()
{
	if (AutoRefreshMemoryStats)
	{
		UpdateMemoryStats();
	}
}

inline void InsightMemory::InlineUpdatePerfInformation()
{
	if (AutoRefreshMemoryStats)
	{
		UpdatePerfInformation();
	}
}

InsightMemory::InsightMemory()
{
	ZeroMemory(this, sizeof(this));
	this->AutoRefreshMemoryStats = true;
}

InsightMemory::InsightMemory(DWORD ProcessID)
{
	ZeroMemory(this, sizeof(this));
	this->AutoRefreshMemoryStats = true;
	SetTargetProcess(ProcessID);
}

InsightMemory::InsightMemory(HANDLE ProcessHandle)
{
	ZeroMemory(this, sizeof(this));
	this->AutoRefreshMemoryStats = true;
	SetTargetProcess(ProcessHandle);
}

InsightMemory::InsightMemory(const InsightMemory& Other)
{
	ZeroMemory(this, sizeof(this));
	AutoRefreshMemoryStats = Other.AutoRefreshMemoryStats;
	if (DuplicateHandle(GetCurrentProcess(), Other.ProcessHandle, GetCurrentProcess(), &this->ProcessHandle, 0, FALSE, DUPLICATE_SAME_ACCESS))
	{

	}
}

InsightMemory::~InsightMemory()
{
	if ( (this->ProcessHandle != 0) && (this->ProcessHandle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->ProcessHandle);
	}
	
}

DWORD InsightMemory::GetTargetProcessID()
{
	return this->dwProcessID;
}

BOOL InsightMemory::SetTargetProcess(DWORD Process)
{
	HANDLE TempHandle = 0;
	bool ret = false;
	
	__try
	{
		TempHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, Process);
		if (TempHandle)
		{
			ret = SetTargetProcess(TempHandle);
		}
	}
	__finally
	{
		if (TempHandle != 0)
		{
			CloseHandle(TempHandle);
		}
	}
	return ret;
}

BOOL InsightMemory::SetTargetProcess(HANDLE ProcessHandle)
{
	HANDLE PersonalHandle = INVALID_HANDLE_VALUE;
	
	if (DuplicateHandle(GetCurrentProcess(), ProcessHandle, GetCurrentProcess(), &PersonalHandle, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, 0))
	{
		if (this->ProcessHandle != 0)
		{
			CloseHandle(this->ProcessHandle);
		}
		this->ProcessHandle = PersonalHandle;
		return TRUE;
	}
	return FALSE;
}

BOOL InsightMemory::UpdateMemoryStats()
{
	BOOL result = TRUE;
	this->u.MemoryCounter.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
	if (!GetProcessMemoryInfo(this->ProcessHandle, (PPROCESS_MEMORY_COUNTERS)&this->u.MemoryCounter, this->u.MemoryCounter.cb))
	{
		result = FALSE;
	}
	if (!GetPerformanceInfo(&this->u2.PerfInformation, sizeof(this->u2.PerfInformation)))
	{
		result = FALSE;
	}
	return result;
}

SIZE_T InsightMemory::GetPageFaultCount()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.PageFaultCount;
}

SIZE_T InsightMemory::GetPeakWorkingSetSize()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.PeakWorkingSetSize;
}

SIZE_T InsightMemory::GetWorkingSetSize()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.WorkingSetSize;
}

SIZE_T InsightMemory::GetQuotaPeakPagedPoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.QuotaPeakPagedPoolUsage;
}

SIZE_T InsightMemory::GetQuotaPagePoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.QuotaPagedPoolUsage;
}

SIZE_T InsightMemory::GetPeakNonPagePoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.QuotaPeakNonPagedPoolUsage;
}

SIZE_T InsightMemory::InsightMemory_GetPeakPagePoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.QuotaPeakPagedPoolUsage;
}

SIZE_T InsightMemory::GetPageFileUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.PagefileUsage;
}

SIZE_T InsightMemory::GetPeakPageFileUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.PeakPagefileUsage;
}

SIZE_T InsightMemory::GetPrivateUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.PrivateUsage;
}

SIZE_T InsightMemory::GetQuotaNonPagedPoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.QuotaNonPagedPoolUsage;
}

SIZE_T InsightMemory::GetQuotaPeakPagePoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->u.MemoryCounter.QuotaPeakPagedPoolUsage;
}

PROCESS_MEMORY_COUNTERS_EX* InsightMemory::GetMemoryStatsBulk()
{
	if (this->UpdateMemoryStats())
	{
		return &this->u.MemoryCounter;
	}
	return nullptr;
}

PERFORMACE_INFORMATION* InsightMemory::GetMemoryPerformanceStatsBulk()
{
	if (this->UpdateMemoryStats())
	{
		if (this->u2.PerfInformation.cb != 0)
		{
			return &this->u2.PerfInformation;
		}
	}
	return nullptr;
}



BOOL InsightMemory::SetAutoRefreshMemoryStats(BOOL Enable)
{
	BOOL ret = this->AutoRefreshMemoryStats;
	this->AutoRefreshMemoryStats = Enable;
	return ret;
}

BOOL InsightMemory::GetAutoRefreshMemoryStats()
{
	return this->AutoRefreshMemoryStats;
}

PSAPI_WORKING_SET_EX_INFORMATION* InsightMemory::GetWorkingSetInfo()
{
	RefreshWorkingSet();
	return this->WorkingSetContainer;
}

BOOL InsightMemory::FreeWorkingSetInfo(PSAPI_WORKING_SET_EX_INFORMATION* t)
{
	if (t != nullptr)
	{
		free(t);
		return TRUE;
	}
	return FALSE;
}
