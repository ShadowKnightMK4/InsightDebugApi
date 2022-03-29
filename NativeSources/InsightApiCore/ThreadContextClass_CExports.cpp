#include <Windows.h>
#include "ThreadContext.h"
#include "Utility.h"
/*
* C level wrapper for ThreadInsight.cpp class
*/
extern "C" {

#define THEADCONTEXT_DEFAULT (0)
#define THREADCONTEXT_USETHREADID (1)

	ThreadInsight* WINAPI ThreadContext_DupInstance(ThreadInsight* that)
	{
		return nullptr;
	}

	DWORD WINAPI ThreadContext_SuspendThread(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->SuspendThreadHandle();
		}
		return 0;
	}

	DWORD WINAPI ThreadContext_ResumeThread(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->ResumeThreadHandle();
		}
		return 0;
	}
	int WINAPI ThreadContext_GetPriority(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetHandleThreadPriority();
		}
		return 0;
	}

	BOOL WINAPI ThreadContext_SetPriority(ThreadInsight* that, int NewPriority)
	{
		if (that != nullptr)
		{
			return that->SetHandleThreadPriority(NewPriority);
		}
		return FALSE;
	}

	/// <summary>
	/// Wrapper to call ThreadInsight->SetThreadDescriptonW() which sets the thread's description (if supported) to NewDescripton
	/// </summary>
	/// <param name="that">non null instance of ThreadInsight</param>
	/// <param name="NewDescription">New description.  May also be null.</param>
	/// <remarks>The underlying routine "SetThreadDescription" on MSDN requires Windows 10, version 1607 / Windows Server 2016 or later.</remarks>
	/// <returns>True if it worked and false if it did not work (or the required DLL does not contain SetThreadDescription</returns>
	BOOL WINAPI ThreadContext_SetThreadDescriptionW(ThreadInsight* that, const wchar_t* NewDescription)
	{
		if (that != nullptr)
		{
			return that->SetThreadDescriptionW(NewDescription);
		}
		return FALSE;
	}

	/// <summary>
	/// Wrapper to call ThreadInsight->GetThreadDescriptionW() which returns the current description (if any and system supported)
	/// </summary>
	/// <param name="that">non null instance of ThreadInsight</param>
	/// <remarks>The underlying routine "GetThreadDescription" on MSDN states it requires Windows 10 LTSB 2016, Windows Server 2016 or Windows 10 version 1607 and later.  You also may need to call LocalFree() but I've not currently tested fully.</remarks>
	/// <returns>Unicode code string containing the thread description if OK and null if something went wrong.</returns>
	const wchar_t* WINAPI ThreadContext_GetThreadDescriptionW(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetThreadDescriptionW();
		}
		return nullptr;
	}

	/// <summary>
	/// Create an instance of ThreadInsight / ThreadContext that points to the passed thread
	/// </summary>
	/// <param name="ThreadHandle">Either a thread id or a handle depending of the flags</param>
	/// <param name="Flags">If this is 0x0,  ThreadHandle is a Handle to the thread.  If this is 0x1, ThreadHandle is a ThreadId</param>
	/// <returns></returns>
	ThreadInsight* WINAPI ThreadContext_CreateInstance(HANDLE ThreadHandle, DWORD Flags)
	{
		DWORD ThreadId;
		ThreadInsight* ret = nullptr;
		if (Flags == THEADCONTEXT_DEFAULT)
		{
			ret = new ThreadInsight(ThreadHandle);
		}
		if (Flags == THREADCONTEXT_USETHREADID)
		{
			ret = new ThreadInsight((DWORD)ThreadHandle);
		}
		return ret;
	}

	/// <summary>
	/// clearnup / delete an instance of ThreadInsight/ThreadContext
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	BOOL WINAPI ThreadContext_KillInstance(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			delete that;
			return TRUE;
		}
		return FALSE;
	}

	FILETIME* WINAPI ThreadContext_GetThreadTimeCreationTime(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetCreationTime();
		}
		return nullptr;
	}

	FILETIME* WINAPI ThreadContext_GetThreadTimeExitTime(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetExitTime();
		}
		return nullptr;
	}

	FILETIME* WINAPI ThreadContext_GetThreadTimeUserTime(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetKernelTime();
		}
		return nullptr;
	}


	FILETIME* WINAPI ThreadContext_GetThreadTimeKernelTime(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetUserTime();
		}
		return nullptr;
	}


	DWORD WINAPI ThreadContext_GetTargetThreadId(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetTargetThreadId();
		}
	}

	DWORD WINAPI ThreadContext_SetIdealProcessor(ThreadInsight* that, DWORD NewProcessor)
	{
		if (that != nullptr)
		{
			return that->SetIdealThreadProcessorMask(NewProcessor);
		}
		return (DWORD)-1;
	}
	DWORD WINAPI ThreadContext_GetIdealProcessor(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetIdealThreadProcessorMask();
		}
		return (DWORD)-1;
	}
	
	BOOL WINAPI ThreadContext_GetThreadPriorityBoostSetting(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetThreadPriorityBoostValue();
		}
		return FALSE;
	}

	BOOL WINAPI ThreadContext_SetThreadPriorityBoostSetting(ThreadInsight* that, BOOL NewValue)
	{
		if (that != nullptr)
		{
			return that->SetThreadPriorityBoostValue(NewValue);
		}
		return FALSE;
	}
	DWORD WINAPI ThreadContext_GetThreadProcessAffinityMask(ThreadInsight* that)
	{
		if (that != nullptr)
		{
			return that->GetIdealThreadProcessorMask();
		}
		return 0;
	}

	DWORD WINAPI ThreadContext_SetThreadProcessAffinityMask(ThreadInsight* that, DWORD NewAffinityMask)
	{
		if (that != nullptr)
		{
			return that->SetIdealThreadProcessorMask(NewAffinityMask);
		}
		return 0;
	}



	BOOL WINAPI ThreadContext_SetTargetThread(ThreadInsight* that, HANDLE Thread, DWORD Flags)
	{
		if (that != nullptr)
		{
			switch (Flags)
			{
			case THREADCONTEXT_USETHREADID:
			{
				return that->SetTargetThread((DWORD)Thread);
			}
			case THEADCONTEXT_DEFAULT:
			{
				return that->SetTargetThread(Thread);
			}
			}
		}
		return FALSE;
	}

}