
#include <Windows.h>
#include "OriginalRoutinePtrs.h"
#include "DefinedConstants.h"

// this defines the template vars not including the return container
#define COMMON_VARS_NOT_RET_DECLARE ULONG_PTR Args[EXCEPTION_MAXIMUM_PARAMETERS]; \
						    TelemetyContainer DataPtr;
// This clears the common template vars not including the return value contrainer
#define BLANK_SLATE_COMMON_VARS_NOT_RET ZeroMemory(&Args, sizeof(ULONG_PTR) * EXCEPTION_MAXIMUM_PARAMETERS); ZeroMemory(&DataPtr, sizeof(TelemetyContainer));
#define COMMON_VARS_SET_POINTERS_NO_MESSAGE_SET Args[TELEMETRY_ACK_POINTER] = (ULONG_PTR)&DataPtr.AckFlag; \
												Args[TELEMETRY_REQUEST_RETURN] = (ULONG_PTR)&DataPtr.RequestReturn; \
												Args[TELEMETRY_RETURN_VALUE_SIZE] = (ULONG_PTR)&DataPtr.ReturnValSize; \
												Args[TELEMETRY_PROCEED_WITH_CALL] = (ULONG_PTR)&DataPtr.CallBad; \
												Args[TELEMETRY_DEBUGGER_ID] = (ULONG_PTR)&DataPtr.DebuggerOK;
// this raises an exception and sets the common AckFlag declared above 
#define GENERIC_RAISE_EXCEPTION_ACK_CLEAR __try  { \
											RaiseException(TELEMETRY_EXCEPTION, 0, EXCEPTION_MAXIMUM_PARAMETERS, (const ULONG_PTR*)&Args); \
										  }\
										  __except (GetExceptionCode() == TELEMETRY_EXCEPTION)  { \
										    DataPtr.AckFlag = 0; \
										  } 

// this raises the exception in response to debugger asking for routine value
/*
#define GENERIC_RAISE_RESPONSE_DEBUG_FLAG(ReturnValueType, MessageType) \
if (DataPtr.DebuggerOK != 0)  \
{ \
Args[TELEMETRY_MAIN_MESSAGE_ID] = ((MessageType));  \
__try {
	\
		RaiseException(TELEMETRY_EXCEPTION, 0, EXCEPTION_MAXIMUM_PARAMETERS, (ULONG_PTR*)Args);  \
}   \
__except (GetExceptionCode() == TELEMETRY_EXCEPTION) {
		\
			DataPtr.AckFlag = 0;   \
	} \
		return ((ReturnValueType)) DataPtr.RequestReturn;  \
} \
else {
	\
		return ((ReturnValueType)) DataPtr.RequestReturn;  \
}  \


*/



#define COMMON_VARS_PROLOG COMMON_VARS_NOT_RET_DECLARE BLANK_SLATE_COMMON_VARS_NOT_RET
HANDLE WINAPI GetProcessHeapDetour()
{
	COMMON_VARS_PROLOG
	HANDLE Ret = 0;

	Args[TELEMETRY_MAIN_MESSAGE_ID] = TELEMETRY_GETPROCESSHEAP_CALLED;
	COMMON_VARS_SET_POINTERS_NO_MESSAGE_SET
	GENERIC_RAISE_EXCEPTION_ACK_CLEAR

	if (DataPtr.AckFlag)
	{
		if (DataPtr.CallBad)
		{
//			GENERIC_RAISE_RESPONSE_DEBUG_FLAG((HANDLE), (TELEMETRY_GETPROCESSHEAP_RETURNED));
			
		}

		}
	}
	else
	{
		return OriginalGetProcessHeap();
	}
	
}

DWORD WINAPI GetProcessHeapsDetour(DWORD NumberOfHeaps, PHANDLE ProcessHeap)
{
	return 0;
}

LPVOID WINAPI HeapAllocDetour(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
	return 0;
}


LPVOID WINAPI HeapCompactDetour(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
	return 0;
}

HANDLE WINAPI HeapCreateDetour(DWORD flOptions, SIZE_T dwStartSize, SIZE_T dwMaxSize)
{

}

BOOL WINAPI HeapDestroyDetour(HANDLE hHeap) 
{

}

BOOL WINAPI HeapFreeDetour(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
	return FALSE;
}


BOOL WINAPI HeapLockDetour(HANDLE hHeap)
{
	return FALSE;
}

BOOL WINAPI HeapQueryInformationDetour(HANDLE HeapHandle,
HEAP_INFORMATION_CLASS HeapInformationClass,
PVOID                  HeapInformation,
SIZE_T                 HeapInformationLength,
 PSIZE_T                ReturnLength
)
{
	return FALSE;
}
LPVOID HeapReAllocDetour(
	[in] HANDLE                 hHeap,
	[in] DWORD                  dwFlags,
	[in] _Frees_ptr_opt_ LPVOID lpMem,
	[in] SIZE_T                 dwBytes
) 
{
	return 0;
}

BOOL WINAPI HeapSetnformationDetour(HANDLE HeapHandle,
	HEAP_INFORMATION_CLASS HeapInformationClass,
	PVOID                  HeapInformation,
	SIZE_T                 HeapInformationLength,
	PSIZE_T                ReturnLength
)
{
	return FALSE;
}


BOOL WINAPI HeapUnlockDetour(HANDLE hHeap)
{
	return FALSE;
}

LPVOID WINAPI HeapValidateDetour(
	HANDLE                 hHeap,
	DWORD                  dwFlags,
	LPVOID lpMem
)
{
	return 0;
}

BOOL WINAPI HeapWalkDetour(
	HANDLE                 hHeap,
	LPPROCESS_HEAP_ENTRY lpEntry
)
{
	return FALSE;
}