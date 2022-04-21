#include "ThreadContext.h"
#include "Utility.h"
HMODULE KernelBase =0;
 //HMODULE Kernel32 = 0; // ACtually source from utility.cpp */
typedef HRESULT(WINAPI* SetThreadDescriptionPtr)(HANDLE, LPWSTR);
typedef HRESULT(WINAPI* GetThreadDescriptionPtr)(HANDLE, LPWSTR*);
typedef DWORD(WINAPI* Wow64SuspendThreadPtr)(HANDLE);
typedef BOOL(WINAPI* Wow64GetThreadContextPtr)(HANDLE, PWOW64_CONTEXT);
typedef BOOL(WINAPI* Wow64SetThreadContextPtr)(HANDLE, const PWOW64_CONTEXT);

SetThreadDescriptionPtr InternalBase_SetThreadDescriptionPointer = nullptr;
GetThreadDescriptionPtr InternalBase_GetThreadDescriptionPointer = nullptr;
Wow64SuspendThreadPtr InternalBase_Wow64SuspendThreadPtr = nullptr;
Wow64GetThreadContextPtr InternalBase_Wow64GetThreadContextPtr = nullptr;
Wow64SetThreadContextPtr InternalBase_Wow64SetThreadContextPtr = nullptr;

/* IN the interest of attempting to run on systems that don't have wow64,
we try loading and return false if we can't find the*/;
bool LinkWowThreadContext()
{
	if (Kernel32 == 0)
	{
		Kernel32 = LoadLibrary(L"kernel32.dll");
		if (Kernel32 == 0)
		{
			return FALSE;
		}
	}
		
	InternalBase_Wow64GetThreadContextPtr = (Wow64GetThreadContextPtr) GetProcAddress(Kernel32, "Wow64GetThreadContext");
	InternalBase_Wow64SetThreadContextPtr = (Wow64SetThreadContextPtr) GetProcAddress(Kernel32, "Wow64SetThreadContext");

	if ((InternalBase_Wow64GetThreadContextPtr == 0) || (InternalBase_Wow64SetThreadContextPtr == 0))
	{
		return FALSE;

	}
	return TRUE;
}

/// <summary>
/// This first tries to link to SetThreadDescription and GetThreadScription Dynamically in KernalBase.dll first then Kernel32.dll and returns true if at least one had routines exported with those names
/// </summary>
/// <returns></returns>
bool LinkThreadDescriptionRoutines()
{
	/*
	*	we first see if the two pointers are null.
	*/
	if ((InternalBase_GetThreadDescriptionPointer == nullptr) || (InternalBase_SetThreadDescriptionPointer == nullptr))
	{
		if (KernelBase == nullptr)
		{
			KernelBase = LoadLibrary(L"KernelBase.Dll");
			if (KernelBase == nullptr)
			{
				Kernel32 = LoadLibrary(L"Kernel32.dll");
				if (Kernel32 == 0)
				{
					return false;
				}
				else
				{ 
					KernelBase = Kernel32;
				}
			}
			
			
				InternalBase_GetThreadDescriptionPointer = (GetThreadDescriptionPtr) GetProcAddress(KernelBase, "GetThreadDescription");
				InternalBase_SetThreadDescriptionPointer = (SetThreadDescriptionPtr) GetProcAddress(KernelBase, "SetThreadDescription");
				if ((InternalBase_GetThreadDescriptionPointer == nullptr) || (InternalBase_SetThreadDescriptionPointer == nullptr))
				{
					FreeLibrary(KernelBase);
					return false;
				}
			
		}
	}
	if ((InternalBase_GetThreadDescriptionPointer != nullptr) && (InternalBase_SetThreadDescriptionPointer != nullptr))
	{
		return true;
	}
	return false;
}
ThreadInsight::ThreadInsight(const ThreadInsight& Other)
{
	ZeroMemory(this, sizeof(ThreadInsight));
	this->dwProcessID = Other.dwProcessID;
	this->dwThreadID = Other.dwThreadID;
	if ((Other.ThreadHandle != 0) && (Other.ThreadHandle != INVALID_HANDLE_VALUE))
	{
		DuplicateHandle(GetCurrentProcess(), Other.ThreadHandle, GetCurrentProcess(), &this->ThreadHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
	}
}
ThreadInsight::ThreadInsight()
{
	ZeroMemory(this, sizeof(ThreadInsight));
}

ThreadInsight::ThreadInsight(DWORD dwThreadID)
{
	ZeroMemory(this, sizeof(ThreadInsight));
	this->SetTargetThread(dwThreadID);
}

ThreadInsight::ThreadInsight(HANDLE HandleToThread)
{
	ZeroMemory(this, sizeof(ThreadInsight));
	this->SetTargetThread(HandleToThread);
}

ThreadInsight::~ThreadInsight()
{

	if (this->ThreadDescription != nullptr)
	{
#ifdef _DEBUG
		if (LocalFree((HLOCAL)this->ThreadDescription) != 0)
		{
			DWORD lastErr = GetLastError();
			DebugBreak();
		}
#else
		LocalFree((HLOCAL)this->ThreadDescription);
#endif
	}
	ThreadDescription = nullptr;
	if ( (ThreadHandle != 0) && (ThreadHandle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(ThreadHandle);
	}
}

BOOL ThreadInsight::SetThreadDescriptionW(const wchar_t* Description)
{
	if (!LinkThreadDescriptionRoutines())
	{
		return FALSE;
	}
	if (ThreadHandle != INVALID_HANDLE_VALUE)
	{
		/*
		if (this->ThreadDescription != 0)
		{
			free((void*)this->ThreadDescription);
			this->ThreadDescription = 0;
		}

		if (Description != 0)
		{
			this->ThreadDescription = _wcsdup(Description);
		}*/
		if (FAILED(InternalBase_SetThreadDescriptionPointer(this->ThreadHandle, (LPWSTR)Description)))
		{
			return FALSE;
		}
		return TRUE;
		
	}
	return FALSE;
}


const wchar_t* ThreadInsight::GetThreadDescriptionW()
{
	LPWSTR tmp = nullptr;
	if (!LinkThreadDescriptionRoutines())
	{
		return nullptr;
	}

	if (!FAILED(InternalBase_GetThreadDescriptionPointer(this->ThreadHandle, &tmp)))
	{
		if (this->ThreadDescription != nullptr)
		{
#ifdef _DEBUG
			/* what apppens when we don't free.*/
			/*
			if (LocalFree((HLOCAL)this->ThreadDescription) != 0)
			{
				DWORD lastErr = GetLastError();
				DebugBreak();
			}
			*/
			
#else
			LocalFree((HLOCAL)this->ThreadDescription);
#endif // _DEBUG

			
			this->ThreadDescription = nullptr;
		}
		this->ThreadDescription = tmp;
	}
	return this->ThreadDescription;
}

CONTEXT* ThreadInsight::GetTargetThreadContext()
{
	WOW64_CONTEXT t;
	return nullptr;
}

BOOL ThreadInsight::SetTargetThreadContext(CONTEXT* NewContext)
{
	return 0;
}

int ThreadInsight::GetHandleThreadPriority()
{
	return GetThreadPriority(this->ThreadHandle);
}

bool ThreadInsight::SetHandleThreadPriority(int NewPriority)
{
	return SetThreadPriority(this->ThreadHandle, NewPriority);
}

bool ThreadInsight::GetThreadPriorityBoostValue()
{
	BOOL ret = FALSE;
	if (GetThreadPriorityBoost(this->ThreadHandle, &ret))
	{
		return ret;
	}
	return FALSE;
}

bool ThreadInsight::SetThreadPriorityBoostValue(bool EnablePriorityBoost)
{
	return (SetThreadPriorityBoost(this->ThreadHandle, EnablePriorityBoost));
}

DWORD ThreadInsight::GetTargetThreadExitCode()
{
	DWORD ret = 0;
	if (this->ThreadHandle != INVALID_HANDLE_VALUE)
	{
		GetExitCodeThread(this->ThreadHandle, &ret);
	}
	return ret;
}

FILETIME* ThreadInsight::GetExitTime()
{
	if (GetThreadTimes(this->ThreadHandle, &this->lpCreationTime, &this->lpExitTime, &this->lpKernelTime, &this->lpUserTime))
	{
		DWORD exitCode = 0;
		GetExitCodeThread(this->ThreadHandle, &exitCode);
		if (exitCode == STILL_ACTIVE)
		{
			lpExitTime.dwLowDateTime = lpExitTime.dwHighDateTime = 0;
		}
		return &this->lpExitTime;
	}
	return nullptr;
}

FILETIME* ThreadInsight::GetUserTime()
{
	if (GetThreadTimes(this->ThreadHandle, &this->lpCreationTime, &this->lpExitTime, &this->lpKernelTime, &this->lpUserTime))
	{
		return &this->lpUserTime;
	}
	return nullptr;
}

FILETIME* ThreadInsight::GetCreationTime()
{
	if (GetThreadTimes(this->ThreadHandle, &this->lpCreationTime, &this->lpExitTime, &this->lpKernelTime, &this->lpUserTime))
	{
		return &this->lpCreationTime;
	}
	return nullptr;
}

FILETIME* ThreadInsight::GetKernelTime()
{
	if (GetThreadTimes(this->ThreadHandle, &this->lpCreationTime, &this->lpExitTime, &this->lpKernelTime, &this->lpUserTime))
	{
		return &this->lpKernelTime;
	}
	return nullptr;
}

BOOL ThreadInsight::SetTargetThread(HANDLE NewThread)
{
	HANDLE NewHandle;
	if ( (this->ThreadHandle != 0) && (this->ThreadHandle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->ThreadHandle);
	}
	if (DuplicateHandle(GetCurrentProcess(), NewThread, GetCurrentProcess(), &NewHandle, THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION | THREAD_SET_INFORMATION, FALSE, 0))
	{
		this->ThreadHandle = NewHandle;
		this->dwThreadID = GetThreadId(this->ThreadHandle);
		this->dwProcessID = GetProcessIdOfThread(this->ThreadHandle);
		return TRUE;
	}
	return FALSE;
}

BOOL ThreadInsight::SetTargetThread(DWORD dwThreadID)
{
	BOOL Ret = FALSE;
	HANDLE NewThread;
	NewThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION | THREAD_SET_INFORMATION, FALSE, dwThreadID);
	if (NewThread == nullptr)
	{
		return FALSE;
	}
	else
	{
		Ret = SetTargetThread(NewThread);
		return Ret;
	}
}

DWORD ThreadInsight::SuspendThreadHandle()
{
	if (IsTargetProcessID32Bit(this->dwProcessID))
	{
		if (InternalBase_Wow64SuspendThreadPtr == nullptr)
		{
			if (Kernel32 == nullptr)
			{
				Kernel32 = LoadLibrary(L"Kernel32.dll");
				if (Kernel32 == 0)
				{
					return (DWORD)-1;
				}
				else
				{
					InternalBase_Wow64SuspendThreadPtr = (Wow64SuspendThreadPtr) GetProcAddress(Kernel32, "Wow64SuspendThread");
					if (InternalBase_Wow64SuspendThreadPtr == 0)
					{
						return (DWORD)-1;
					}
				}
			}
		}

		if (InternalBase_Wow64SuspendThreadPtr == nullptr)
		{
			return (DWORD)-1;
		}
		return InternalBase_Wow64SuspendThreadPtr(this->ThreadHandle);
	}
	else
	{
		return SuspendThread(this->ThreadHandle);
	}
}

DWORD ThreadInsight::ResumeThreadHandle()
{
	return ResumeThread(this->ThreadHandle);
}

WOW64_CONTEXT* ThreadInsight::Wow64GetContext()
{
	if (FetchContext())
	{
		return &this->ThreadContext.Win32Context;
	}
	return nullptr;
}

CONTEXT* ThreadInsight::GetContext()
{
	if (FetchContext())
	{
		return &this->ThreadContext.Win64Context;
	}
	return nullptr;
}

BOOL ThreadInsight::Wow64SetContext(WOW64_CONTEXT* Context)
{
	if (Context != nullptr)
	{
		return ApplyContext((CONTEXT*)Context);
	}
	return FALSE;
}

BOOL ThreadInsight::SetContext(CONTEXT* Context)
{
	if (Context != nullptr)
	{
		return ApplyContext(Context);
	}
	return FALSE;
}

DWORD ThreadInsight::GetTargetThreadId()
{
	if (dwThreadID == 0)
	{
		dwThreadID = GetThreadId(this->ThreadHandle);
	}
	return dwThreadID;
}

DWORD ThreadInsight::GetIdealThreadProcessorMask()
{
	return SetIdealThreadProcessorMask(MAXIMUM_PROCESSORS);
}

DWORD ThreadInsight::SetIdealThreadProcessorMask(DWORD NewPreferredProcessor)
{
	DWORD ret;
	ret = SetThreadIdealProcessor(this->ThreadHandle, NewPreferredProcessor);
	return ret;
}

BOOL ThreadInsight::FetchContext()
{
	ZeroMemory(&this->ThreadContext, sizeof(ThreadContext));
	DWORD ProcessID;
	ProcessID = GetProcessIdOfThread(ThreadHandle);
	ZeroMemory(&this->ThreadContext, sizeof(this->ThreadContext));
	if (IsTargetProcessID32Bit(ProcessID))
	{
		if (LinkWowThreadContext())
		{
			if (InternalBase_Wow64GetThreadContextPtr(ThreadHandle, &this->ThreadContext.Win32Context))
			{
				return TRUE;
			}
		}
	}
	else
	{
		if (!GetThreadContext(ThreadHandle, &this->ThreadContext.Win64Context))
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ThreadInsight::ApplyContext(CONTEXT* Context)
{
	DWORD processId;
	if (Context != nullptr)
	{
		processId = GetProcessIdOfThread(this->ThreadHandle);
		if (IsTargetProcessID32Bit(processId))
		{
			if (!LinkWowThreadContext())
			{
				return FALSE;
			}
			else
			{
				if (InternalBase_Wow64SetThreadContextPtr(ThreadHandle, (const PWOW64_CONTEXT)&Context))
				{
					return TRUE;
				}
			}
		}
		else
		{
			if (SetThreadContext(ThreadHandle, (const CONTEXT*)Context))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}



