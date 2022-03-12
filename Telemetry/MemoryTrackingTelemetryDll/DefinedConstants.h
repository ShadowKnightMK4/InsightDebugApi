#pragma once


/*
* Resource Constants is a large colleciton of #defines that can be used to indicate what your file is atempting to communcate to the debugger
* 
*/


#define PROCESS_HEAP (0)

// A random gui value.  Value itself matters not as long as telemetry DLLs and insight have the same common value
#define COMMON_TELEMETRY_EXCEPTION_BASE_VAL (0x31FAFED1)
// a value to tell Insight that this is a special exception to handle.  Real value is Args[0]
#define TELEMETRY_EXCEPTION ((COMMON_TELEMETRY_EXCEPTION_BASE_VAL & ~(1 << 29)))

/*
*	Talking to and from telemtry requies ReadProcessMemory  and WritePRoessMemory()
*	
*	Args[0] = Main message id
	Args[1] AckFlag Pointer				<- set to non zero to incidate your debugger understood it. IF YOU DON"T SET this, the rest of the arguments are ignored
	Args[2] RequestReturnValue			<- set to non zero to indicate you want the return value wrote to this location process memory
	ARgs[3] ReturnValueSize				<- if return value is a litteral 0, else size of the block of memory it points too
	Args[3] ProceedWithCall				<- nonzero if call is ok to go.
	args[4] DebuggerID					<- Starts as 0.  Debuger is free to set at and if Request return value is 
	Args[13]  = Number of Arguments (if any)
	Args[14]  = Pointer to array contraining pointers to the argumemnts. 
*/

#define TELEMETRY_MAIN_MESSAGE_ID (0)
#define TELEMETRY_ACK_POINTER (1)
#define TELEMETRY_REQUEST_RETURN (2)
#define TELEMETRY_RETURN_VALUE_SIZE (3)
#define TELEMETRY_PROCEED_WITH_CALL (4)
#define TELEMETRY_DEBUGGER_ID (5)
#define TELEMETRY_ARG_COUNT (13)
#define TELEMTRY_ARG_POINTER (14)


#ifndef TELEMETRY_STRUCT_PTR
#define TELEMETRY_STRUCT_PTR
typedef struct TelemetyContainer
{
	/// <summary>
	/// DEBUGGER: Set this value to non-zero 
	/// </summary>
	DWORD AckFlag;
	/// <summary>
	/// If CallBad is set, contains return value to send back to code.
	/// If CallBac is clear and this is non-zero, return revalue is also written there
	/// </summary>
	DWORD RequestReturn;
	/// <summary>
	/// If callbad is set, IGNORED
	/// if callback is clear, is set to return avalue size
	/// </summary>
	DWORD ReturnValSize;
	/// <summary>
	/// Set to true to drop the call and return the above stuff
	/// </summary>
	DWORD CallBad;
	/// <summary>
	/// Debuger is free to mobid ythis to non-zero.  If it does, Exception is raised again 
	/// </summary>
	DWORD DebuggerOK;
};
#endif

#define TELEMETRY_GETPROCESSHEAP_CALLED (0)
#define TELEMETRY_GETPROCESSHEAP_RETURNED (10)
