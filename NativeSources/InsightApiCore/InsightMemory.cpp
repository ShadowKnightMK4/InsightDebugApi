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
			free(WorkingSetContainer);
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

inline void InsightMemory::InlineRefreshMemoryStats()
{
	if (AutoRefreshMemoryStats)
	{
		UpdateMemoryStats();
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

BOOL InsightMemory::SetTargetProcess(DWORD Process)
{
	HANDLE TempHandle = 0;
	bool ret = false;
	
	{
		TempHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, Process);
		if (TempHandle)
		{
			ret = SetTargetProcess(TempHandle);
		}
	}
	
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
	this->MemoryStats.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
	if (GetProcessMemoryInfo(this->ProcessHandle, (PPROCESS_MEMORY_COUNTERS)&this->MemoryStats, this->MemoryStats.cb))
	{
		return TRUE;
	}
	return FALSE;
}

SIZE_T InsightMemory::GetPageFaultCount()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.PageFaultCount;
}

SIZE_T InsightMemory::GetPeakWorkingSetSize()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.PeakWorkingSetSize;
}

SIZE_T InsightMemory::GetWorkingSetSize()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.WorkingSetSize;
}

SIZE_T InsightMemory::GetQuotaPeakPagedPoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.QuotaPeakPagedPoolUsage;
}

SIZE_T InsightMemory::GetQuotaPagePoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.QuotaPagedPoolUsage;
}

SIZE_T InsightMemory::GetPeakNonPagePoolUsage()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.QuotaPeakNonPagedPoolUsage;
}

SIZE_T InsightMemory::GetPageFileUsage()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.PagefileUsage;
}

SIZE_T InsightMemory::GetPeakPageFileUsage()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.PeakPagefileUsage;
}

SIZE_T InsightMemory::GetPrivateUsage()
{
	this->InlineRefreshMemoryStats();
	return this->MemoryStats.PrivateUsage;
}

PROCESS_MEMORY_COUNTERS_EX* InsightMemory::GetMemoryStatsBulk()
{
	if (this->UpdateMemoryStats())
	{
		return &this->MemoryStats;
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
