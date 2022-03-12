
#include <Windows.h>
/*
*	DebuggerCommuincation is how these telemetry dlls will talk to your custom debugger. 
* 
*	They do this by packing stuff into an exception_record data struct and Raising the exception to notify the debugger.
*	
*/

/* this const is intended for any teleemtry dll that wants to talk to Insight.  */
#define TELEMEMTRY_MESSAGE 1

/*
* ARGUMENT_LIST[0] = UserMessageClass		<- recommand using the MSG_MAIN_XXXX #defines.    You aren't limited to them as the other values will be grouped by int value.
* ARGUMENT_LIST[13] = ArgCount;
* ARGUMENT_LIST[14] = Args;		<- It is recommended to  ensure the memory args points to is readable and writable.
*/
extern "C" {

	int ExceptionFilter(DWORD value, DWORD check_againt)
	{
		if (value == check_againt)
		{
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
	int MessageDebugger(DWORD BaseMessage, DWORD UserMessageClass, DWORD UserMessageType, DWORD ArgCount, ULONG_PTR* Args)
	{
		ULONG_PTR ARGUMENT_LIST[EXCEPTION_MAXIMUM_PARAMETERS];
		memset( &ARGUMENT_LIST, 0, sizeof(ARGUMENT_LIST));
		bool DebugSaw = false;

		ARGUMENT_LIST[0] = UserMessageClass;
		ARGUMENT_LIST[1] = UserMessageType;
		ARGUMENT_LIST[13] = (ULONG_PTR)ArgCount;
		ARGUMENT_LIST[14] = (ULONG_PTR)Args;


		__try
		{
			RaiseException(TELEMEMTRY_MESSAGE, 0, 15, ARGUMENT_LIST);
		}
		__except (ExceptionFilter(GetExceptionCode(), TELEMEMTRY_MESSAGE))
		{
			// We really don't kneed to do anything but continue on after throwing the xception.
		}

		return 0;
	}
}

