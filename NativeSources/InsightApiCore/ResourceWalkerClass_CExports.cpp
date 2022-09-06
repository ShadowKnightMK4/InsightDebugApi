#include "ResourceWalker.h"

extern "C" {

	ResourceWalker* WINAPI ResourceWalker_MakeInstance()
	{
		//return new ResourceWalker();
		return nullptr;
	}

	ResourceWalker* WINAPI ResourceWalker_MakeInstanceTarget(LPCWSTR ExeName,  DWORD Access, DWORD SharePermissions)
	{
		return new ResourceWalker(ExeName, Access, SharePermissions);
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
		//return new ResourceWalker(original);
		return nullptr;
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

	BOOL WINAPI ResourceWalker_EnumResourceTypesW(ResourceWalker* that, ENUMRESTYPEPROCW callback, LONG_PTR CustomArg, DWORD flags, LANGID LangId)
	{
		if (that)
		{
			return that->EnumerateResourceTypesExW(callback, CustomArg, flags, LangId);
		}
		return FALSE;
	}
	BOOL WINAPI ResourceWalker_EnumResourceNamesW(ResourceWalker* that, LPCWSTR lpType, ENUMRESNAMEPROCW Callback, LONG_PTR CustomArg, DWORD dwFlags, LANGID LangId)
	{
		if (that)
		{
	//		IS_INTRESOURCE()
			return that->EnumerateResourceNamesEx(lpType, Callback, CustomArg, dwFlags, LangId);
		}
		return FALSE;
	}

	
}