#include "InsightMemory.h"
#include "InsightMemory_CWrappers.h"

extern "C" {
	InsightMemory* WINAPI InsightMemory_MakeInstance()
	{
		return new InsightMemory();
	}

	InsightMemory* InsightMemory_DupInstance(InsightMemory* First)
	{
		return new InsightMemory(First);
	}

	BOOL InsightMemory_KillInstance(InsightMemory* that)
	{
		if (that)
		{
			delete that;
			return TRUE;
		}
		return FALSE;
	}

	DWORD InsightMemory_GetTargetProcessID(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetTargetProcessID();
		}
		return 0;
	}
	BOOL InsightMemory_SetTargetProcess(InsightMemory* that, DWORD ProcessOrHandle, BOOL HandleInstead)
	{
		if (that != nullptr)
		{
			if (HandleInstead)
			{
				return that->SetTargetProcess((HANDLE)ProcessOrHandle);
			}
			else
			{
				return that->SetTargetProcess(ProcessOrHandle);
			}
		}
		return FALSE;
	}

	BOOL InsightMemory_UpdateMemoryStats(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->UpdateMemoryStats();
		}
		return FALSE;
	}

	SIZE_T InsightMemory_GetPageFaultCount(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetPageFaultCount();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetPeakWorkingSetSize(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetPeakWorkingSetSize();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetWorkingSetSize(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetWorkingSetSize();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetQuotaPeakPagedPoolUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaPeakPagedPoolUsage();
		}
		return 0;
	}

	//InsightMemory_GetQuotaNonPagedPoolUsage

	SIZE_T InsightMemory_GetQuotaNonPagedPoolUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaNonPagedPoolUsage();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetQuotaPagePoolUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaPagePoolUsage();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetQuotaPeakPagePoolUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaPeakPagePoolUsage();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetPageFileUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetPageFileUsage();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetPeakNonPagePoolUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetPeakNonPagePoolUsage();
		}
		return 0;
	}
	SIZE_T InsightMemory_GetPeakPageFileUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetPeakPageFileUsage();
		}
		return 0;
	}

	SIZE_T InsightMemory_GetPrivateUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetPrivateUsage();
		}
		return 0;
	}

	PROCESS_MEMORY_COUNTERS_EX* InsightMemory_GetMemoryStatsBulk(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetMemoryStatsBulk();
		}
		return nullptr;
	}

	PSAPI_WORKING_SET_EX_INFORMATION* InsightMemory_GetWorkingSetContainerInfo(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetWorkingSetInfo();
		}
		return nullptr;
	}

	PERFORMACE_INFORMATION* InsightMemory_GetPerformanceInfo(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetMemoryPerformanceStatsBulk();
		}
		return nullptr;

	}
	BOOL InsightMemory_FreeWorkingSetContainerInfo(InsightMemory* that, PSAPI_WORKING_SET_EX_INFORMATION* t)
	{
		if (that != nullptr)
		{
			return that->FreeWorkingSetInfo(t);
		}
		return FALSE;
	}
	BOOL InsightMemory_SetAutoRefreshMemoryStats(InsightMemory* that, BOOL Enable)
	{
		if (that != nullptr)
		{
			return that->SetAutoRefreshMemoryStats(Enable);
		}
		return FALSE;
	}


	BOOL InsightMemory_GetAutoRefreshMemoryStats(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetAutoRefreshMemoryStats();
		}
		return FALSE;
	}
}