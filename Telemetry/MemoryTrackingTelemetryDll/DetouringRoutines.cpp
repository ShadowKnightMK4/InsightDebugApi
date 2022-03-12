#include "OriginalRoutinePtrs.h"
#include "DetouredRoutineSource.h"
#include "..\..\Detours-master\src\detours.h"

#include <corecrt_wstring.h>
#include <sstream>
static HMODULE Kernel32 = 0;


/// <summary>
/// Output to debug string "Detour Failed to Attach to Routinename with code (dec=code , hex=code)\r\n"
/// </summary>
/// <param name="RoutineName"></param>
/// <param name="code"></param>
inline void DetourFail(LPCWSTR RoutineName, LONG code)
{
	std::wstringstream help;
	help << L"Detour Failed when detouring " << RoutineName << L"With code (dec=" << std::dec << code << L", hex =" << std::hex << code << std::hex << L")";
	OutputDebugString(help.str().c_str());
}

bool DetourRoutines()
{
	/*
	* Assigning the original routines directly to the pointer contain
	* means our DLL will be statically Dependant 
	* 
	* Dynamic linking and Getprocaddress() is an option also.
	*/
	OriginalGetProcessHeap = GetProcessHeap;
	OriginalGetProcessHeaps = GetProcessHeaps;
	OriginalHeapCompact = HeapCompact;
	OriginalHeapCreate = HeapCreate;
	OriginalHeapFree = HeapFree;
	OriginalHeapLock = HeapLock;
	OriginalHeapQueryInformation = HeapQueryInformation;
	OriginalHeapRealloc = HeapReAlloc;
	OriginalHeapSetInformation = HeapSetInformation;
	OriginalHeapSize = HeapSize;
	OriginalHeapUnLock = HeapUnlock;
	OriginalHeapValidate = HeapValidate;
	OriginalHeapWalk = HeapWalk;

	LONG result = DetourTransactionBegin();
	if (result != NO_ERROR)
	{
		SetLastError(result);
		return FALSE;
	}
	else
	{
		result = DetourUpdateThread(GetCurrentThread());
		if (result != NO_ERROR)
		{
			SetLastError(result);
			return FALSE;
		}
		else
		{
		
			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}


			result = DetourAttach((PVOID*)&OriginalGetProcessHeaps, GetProcessHeapsDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetours", result);
			}

			result = DetourAttach((PVOID*)&OriginalHeaAll, HeapAllocDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}

			result = DetourAttach((PVOID*)&OriginalGetProcessHeap, GetProcessHeapDetour);
			if (result != NO_ERROR)
			{
				SetLastError(result);
				DetourFail(L"GetProcessHeapDetour", result);
			}


		}
	}
	result = DetourTransactionCommit();
	if (result != 0)
	{
		return FALSE;
	}
}