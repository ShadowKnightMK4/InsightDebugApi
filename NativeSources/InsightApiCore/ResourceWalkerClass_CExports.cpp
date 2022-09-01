#include "ResourceWalker.h"

extern "C" {

	ResourceWalker* WINAPI ResourceWalker_MakeInstance()
	{
		return new ResourceWalker();
	}

	ResourceWalker* WINAPI ResourceWalker_MakeInstanceTarget(HANDLE )
	{
		return new ResourceWalker();
	}


	BOOL WINAPI ResourceWalker_KillInstance(ResourceWalker* that)
	{
		if (that)
		{
			delete that;
			return TRUE;
		}
		return FALSE;
	}

	ResourceWalker* WINAPI ResourceWalker_DupInstance(const ResourceWalker* original)
	{
		return new ResourceWalker(original);
	}

	BOOL WINAPI ResourceWalker_GetThreadLock(ResourceWalker* that)
	{
		if (that)
		{
			return that->GetThreadLock();
		}
		return FALSE;
	}

	BOOL WINAPI ResourceWalker_SetThreadLock(ResourceWalker* that, BOOL Status)
	{
		if (that)
		{
			return that->SetThreadLock(Status);
		}
		return FALSE;
	}

	BOOL WINAPI ResourceWalker_EnumResourceNameW(ResourceWalker* that, LPCWSTR lpType, ENUMRESNAMEPROCW Callback, LONG_PTR CustomArg, DWORD dwFlags, LANGID LangId)
	{
		if (that)
		{
			return that->EnumerateResourceNamesEx(lpType, Callback, CustomArg, dwFlags, LangId);
		}
		return FALSE;
	}

	
}