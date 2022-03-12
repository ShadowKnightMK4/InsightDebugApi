#include "ThreadContext.h"
#include "Utility.h"
HMODULE KernelBase =0;
 //HMODULE Kernel32 = 0; // ACtually sourcef rom utility.cpp */
typedef HRESULT(WINAPI* SetThreadDescriptionPtr)(HANDLE, LPWSTR);
typedef HRESULT(WINAPI* GetThreadDescriptionPtr)(HANDLE, LPWSTR*);
typedef DWORD(WINAPI* Wow64SuspendThreadPtr)(HANDLE);

SetThreadDescriptionPtr InternalBase_SetThreadDescriptionPointer = nullptr;
GetThreadDescriptionPtr InternalBase_GetThreadDescriptionPointer = nullptr;
Wow64SuspendThreadPtr InternalBase_Wow64SuspendThreadPtr = nullptr;

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
			
			{
				InternalBase_GetThreadDescriptionPointer = (GetThreadDescriptionPtr) GetProcAddress(KernelBase, "GetThreadDescription");
				InternalBase_SetThreadDescriptionPointer = (SetThreadDescriptionPtr) GetProcAddress(KernelBase, "SetThreadDescription");
				if ((InternalBase_GetThreadDescriptionPointer == nullptr) || (InternalBase_SetThreadDescriptionPointer == nullptr))
				{
					FreeLibrary(KernelBase);
					return false;
				}
			}
		}
	}
	if ((InternalBase_GetThreadDescriptionPointer != nullptr) && (InternalBase_SetThreadDescriptionPointer != nullptr))
	{
		return true;
	}
	return false;
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

CONTEXT* ThreadInsight::GetThreadContext()
{
	return nullptr;
}

BOOL ThreadInsight::SetThreadContext(CONTEXT* NewContext)
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
	if (this->ThreadHandle != 0)
	{
		CloseHandle(this->ThreadHandle);
	}
	if (DuplicateHandle(GetCurrentProcess(), NewThread, GetCurrentProcess(), &NewHandle, THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION | THREAD_SET_INFORMATION, FALSE, 0))
	{
		this->ThreadHandle = NewHandle;
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
			return InternalBase_Wow64SuspendThreadPtr(this->ThreadHandle);
		}
	}
	else
	{
		return SuspendThread(this->ThreadHandle);
	}
}

DWORD ThreadInsight::ResumeThreadHandle()
{
	return ResumeThread(ThreadHandle);
}

