#include "InsightMemory.h"

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

	SIZE_T InsightMemory_GetPageFault(InsightMemory* that)
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
			return that->GetQuotaPagePoolUsage();
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

	SIZE_T InsightMemory_GetPeakPagePoolUsage(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetPeakNonPagePoolUsage();
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
	}

	PROCESS_MEMORY_COUNTERS_EX* InsightMemory_GetMemoryStatsBulk(InsightMemory* that)
	{
		if (that != nullptr)
		{
			return that->GetMemoryStatsBulk();
		}
		return nullptr;
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