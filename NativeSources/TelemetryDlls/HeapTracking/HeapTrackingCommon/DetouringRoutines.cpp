#include <Windows.h>

#include <detours.h>
#include <string>
#include <sstream>


#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include "TelemetrySupport.h"
#include "Headers/StringConstants.h"


HMODULE Kernel32;



bool DetourTargetRoutines()
{
	
	LONG detour = 0;
	Kernel32 = LoadLibraryExW(kernel32_StringW, 0, 0);

	if (Kernel32 != nullptr)
	{
        OriginalGetProcessHeaps = (GetProcessHeapsPtr)GetProcAddress(Kernel32, GetProcessHeaps_StringA);
        if (OriginalGetProcessHeaps == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, GetProcessHeaps_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapAlloc = (HeapAllocPtr)GetProcAddress(Kernel32, HeapAlloc_StringA);
        if (OriginalHeapAlloc == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapAlloc_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapCompact = (HeapCompactPtr)GetProcAddress(Kernel32, HeapCompact_StringA);
        if (OriginalHeapCompact == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapCompact_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapCreate = (HeapCreatePtr)GetProcAddress(Kernel32, HeapCreate_StringA);
        if (OriginalHeapCreate == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapCreate_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapDestroy = (HeapDestroyPtr)GetProcAddress(Kernel32, HeapDestroy_StringA);
        if (OriginalHeapDestroy == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapDestroy_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapLock = (HeapLockPtr)GetProcAddress(Kernel32, HeapLock_StringA);
        if (OriginalHeapLock == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapLock_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapQueryInformation = (HeapQueryInformationPtr)GetProcAddress(Kernel32, HeapQueryInformation_StringA);
        if (OriginalHeapQueryInformation == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapQueryInformation_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapReAlloc = (HeapReAllocPtr)GetProcAddress(Kernel32, HeapReAlloc_StringA);
        if (OriginalHeapReAlloc == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapReAlloc_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapSetInformation = (HeapSetInformationPtr)GetProcAddress(Kernel32, HeapSetInformation_StringA);
        if (OriginalHeapSetInformation == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapSetInformation_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapSize = (HeapSizePtr)GetProcAddress(Kernel32, HeapSize_StringA);
        if (OriginalHeapSize == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapSize_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapUnlock = (HeapUnlockPtr)GetProcAddress(Kernel32, HeapUnlock_StringA);
        if (OriginalHeapUnlock == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapUnlock_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapValidate = (HeapValidatePtr)GetProcAddress(Kernel32, HeapValidate_StringA);
        if (OriginalHeapValidate == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapValidate_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        OriginalHeapWalk = (HeapWalkPtr)GetProcAddress(Kernel32, HeapWalk_StringA);
        if (OriginalHeapWalk == 0)
        {
#ifdef _DEBUG
            OutputDebugString_GetProcFail(TRUE, HeapWalk_StringW, kernel32_StringW);
#endif // _DEBUG
            return false;
        }

        detour = DetourTransactionBegin();
        if (detour != 0)
        {
#ifdef _DEBUG
            OutputDebugString_DetourTransBeginFail(detour);
#endif
            return false;
        }

        detour = DetourUpdateThread(GetCurrentThread());
        if (detour != 0)
        {
#ifdef _DEBUG
            OutputDebugString_DetourTargetThreadFail(detour);
#endif
            return false;
        }

        if (OriginalGetProcessHeaps != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalGetProcessHeaps, DetouredGetProcessHeaps);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, GetProcessHeaps_StringW, OriginalGetProcessHeaps, DetouredGetProcessHeaps);
            }
            else
            {
                OutputDebugString(GetProcessHeaps_StringW);
                OutputDebugString(L"GetProcessHeaps Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to GetProcessHeaps. Can't Detour");
#endif
        }

        if (OriginalHeapAlloc != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapAlloc, DetouredHeapAlloc);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapAlloc_StringW, OriginalHeapAlloc, DetouredHeapAlloc);
            }
            else
            {
                OutputDebugString(HeapAlloc_StringW);
                OutputDebugString(L"HeapAlloc Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapAlloc. Can't Detour");
#endif
        }

        if (OriginalHeapCompact != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapCompact, DetouredHeapCompact);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapCompact_StringW, OriginalHeapCompact, DetouredHeapCompact);
            }
            else
            {
                OutputDebugString(HeapCompact_StringW);
                OutputDebugString(L"HeapCompact Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapCompact. Can't Detour");
#endif
        }

        if (OriginalHeapCreate != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapCreate, DetouredHeapCreate);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapCreate_StringW, OriginalHeapCreate, DetouredHeapCreate);
            }
            else
            {
                OutputDebugString(HeapCreate_StringW);
                OutputDebugString(L"HeapCreate Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapCreate. Can't Detour");
#endif
        }


        if (OriginalHeapDestroy != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapDestroy, DetouredHeapDestroy);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapDestroy_StringW, OriginalHeapDestroy, DetouredHeapDestroy);
            }
            else
            {
                OutputDebugString(HeapDestroy_StringW);
                OutputDebugString(L"HeapDestroy Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapDestroy. Can't Detour");
#endif
        }

        if (OriginalHeapLock != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapLock, DetouredHeapLock);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapLock_StringW, OriginalHeapLock, DetouredHeapLock);
            }
            else
            {
                OutputDebugString(HeapLock_StringW);
                OutputDebugString(L"HeapLock Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapLock. Can't Detour");
#endif
        }
        if (OriginalHeapQueryInformation != 0)
{
    detour = DetourAttach((PVOID*)&OriginalHeapQueryInformation, DetouredHeapQueryInformation);
#ifdef _DEBUG
    if (detour != 0)
    {
        OutputDebugString_ErrorFailedToAttach(detour, HeapQueryInformation_StringW, OriginalHeapQueryInformation, DetouredHeapQueryInformation);
    }
    else
    {
        OutputDebugString(HeapQueryInformation_StringW);
        OutputDebugString(L"HeapQueryInformation Detoured!");
    }
#endif
}
else
{
#ifdef _DEBUG
    OutputDebugString(L"Failed to grab pointer to HeapQueryInformation. Can't Detour");
#endif
}

        if (OriginalHeapQueryInformation != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapQueryInformation, DetouredHeapQueryInformation);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapQueryInformation_StringW, OriginalHeapQueryInformation, DetouredHeapQueryInformation);
            }
            else
            {
                OutputDebugString(HeapQueryInformation_StringW);
                OutputDebugString(L"HeapQueryInformation Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapQueryInformation. Can't Detour");
#endif
        }

        if (OriginalHeapReAlloc != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapReAlloc, DetouredHeapReAlloc);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapReAlloc_StringW, OriginalHeapReAlloc, DetouredHeapReAlloc);
            }
            else
            {
                OutputDebugString(HeapReAlloc_StringW);
                OutputDebugString(L"HeapReAlloc Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapReAlloc. Can't Detour");
#endif
        }

        if (OriginalHeapSetInformation != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapSetInformation, DetouredHeapSetInformation);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapSetInformation_StringW, OriginalHeapSetInformation, DetouredHeapSetInformation);
            }
            else
            {
                OutputDebugString(HeapSetInformation_StringW);
                OutputDebugString(L"HeapSetInformation Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapSetInformation. Can't Detour");
#endif
        }

        if (OriginalHeapSize != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapSize, DetouredHeapSize);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapSize_StringW, OriginalHeapSize, DetouredHeapSize);
            }
            else
            {
                OutputDebugString(HeapSize_StringW);
                OutputDebugString(L"HeapSize Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapSize. Can't Detour");
#endif
        }

        if (OriginalHeapUnlock != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapUnlock, DetouredHeapUnlock);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapUnlock_StringW, OriginalHeapUnlock, DetouredHeapUnlock);
            }
            else
            {
                OutputDebugString(HeapUnlock_StringW);
                OutputDebugString(L"HeapUnlock Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapUnlock. Can't Detour");
#endif
        }

        if (OriginalHeapValidate != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapValidate, DetouredHeapValidate);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapValidate_StringW, OriginalHeapValidate, DetouredHeapValidate);
            }
            else
            {
                OutputDebugString(HeapValidate_StringW);
                OutputDebugString(L"HeapValidate Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapValidate. Can't Detour");
#endif
        }

        if (OriginalHeapWalk != 0)
        {
            detour = DetourAttach((PVOID*)&OriginalHeapWalk, DetouredHeapWalk);
#ifdef _DEBUG
            if (detour != 0)
            {
                OutputDebugString_ErrorFailedToAttach(detour, HeapWalk_StringW, OriginalHeapWalk, DetouredHeapWalk);
            }
            else
            {
                OutputDebugString(HeapWalk_StringW);
                OutputDebugString(L"HeapWalk Detoured!");
            }
#endif
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString(L"Failed to grab pointer to HeapWalk. Can't Detour");
#endif
        }



	}
	return false;
}