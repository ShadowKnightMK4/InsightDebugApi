#pragma once
//#include <StaticIncludes.h>
#include <Windows.h>
#include <winternl.h>
#include "detours.h"

/// <summary>
/// Remote Structure Routines deals with reading and writing blocks of data to and from self and arbitarary processes.
/// </summary>
namespace RemoteStructureRoutine
{
	extern "C" {
#pragma region UserProcessParemeter

		/// <summary>
		/// Read and copy the RTL_USER_PROCESS_PARAMETERS structure from the Target process into the calling process. Use <see cref="RemoteFreeUserProcessParameters">RemoteFreeUserProcessParameters</see> to free
		/// </summary>
		/// <param name="Process">You may use 0 for a GetCurrentProcess() replacement.  If a true Process Handle, it neads at min PROCESS_VM_READ</param>
		/// <param name="Target">Target location in the Passed Process (not yours) Handle's Virtual Memory. </param>
		/// <returns></returns>
		RTL_USER_PROCESS_PARAMETERS* RemoteReadUserProcessParameters(HANDLE Process, LPVOID Target);


		/// <summary>
		/// Cleanup a RTL_USER_PROCESS_PARAMETERS struct allocated by <see cref="RemoteReadUserProcessParameters">RemoteReadUserProcessParameters</see> call
		/// </summary>
		/// <param name="ptr">ptr to a structure previous returned by RemoteReadUserProcessParameters</param>
		/// <returns></returns>
		BOOL RemoteFreeUserProcessParameters(RTL_USER_PROCESS_PARAMETERS* ptr);

#pragma endregion

#pragma region ObjectAttributes
		/// <summary>
		/// Free an OBJECT_ATTRIBUTES structure allocated with RemoteReadObjectAttributes()
		/// </summary>
		/// <param name="ObjAttrib"></param>
		/// <returns></returns>
		BOOL WINAPI RemoteReadFreeObjectAttributes(OBJECT_ATTRIBUTES* ObjAttrib);
#pragma endregion

#pragma region ArraysAndMore

		/// <summary>
		/// Read an array of ULONG_PTR size values the remote process. CALLER (that's you) will need to free the memory after user.
		/// </summary>
		/// <param name="Process">Process is the process to read from (0=GetCurrentProcess()), A true handle to a remote process needs at least PROCESS_VM_READ rights</param>
		/// <param name="TargetMemory">TargetMemomory is a block of virtual memory in the context of the Process whose handle is speciifed.</param>
		/// <param name="ElementCount">ElementCount Number of Elements to read.   </param>
		/// <returns>Pointer to the data read.  use free() when done</returns>
		ULONG_PTR* WINAPI RemoteReadArray(HANDLE Process, LPVOID TargetMemory, DWORD ElementCount);


		/// <summary>
		/// Extract the string the debugged process is trying to pass to the debugger (that's you).  CALLER needs to free string after use.
		/// </summary>
		/// <param name="Process">Process to read from. Handle needs to have PROCESS_VM_READ rights</param>
		/// <param name="Event">Event to extract from.</param>
		/// <returns>allocates and returns a Unicode string.  IF the Event contains an ANSI string, a is converted to Unicode before being returned.  Largested String possible returned is 0xfffe long If something goes wrong, nullptr is returned instead</returns>
		LPWSTR WINAPI RemoteReadDebugString(HANDLE Process, LPDEBUG_EVENT Event);
#pragma endregion








		/// <summary>
		/// Shorthand for grabbing a duplicate handle from the passed process and literal handle into our process
		/// </summary>
		/// <param name="RemoteProcess">Use 0 for GetCurrentProcess().</param>
		/// <param name="TargetHandle">Handle to duplicate</param>
		/// <returns>INVALID_HANDLE_VALUE on fail or the dupliceted handled one ok.</returns>
		HANDLE DuplicateHandleToSelf(HANDLE RemoteProcess, HANDLE TargetHandle);

		/// <summary>
		/// Read an arbitrary size of memory from the passed process at Target Location in it's address and return it typecast to a void*.   tl;dr   Deference a pointer in the remote process and return 4 bytes specified by the remote pointer
		/// </summary>
		/// <param name="Process">0 means GetCurrentProcess() here.  A true process handle needs PROCESS_VM_READ permission.</param>
		/// <param name="Target">where in the Process's location to read from.</param>
		/// <param name="pointerSize">Size of the pointer in bytes. Recommand setting to sizeof(VOID*) and also ensuring your pointer size and the remote pointer size match.  This routine FAILs if this is not 4 or 8.</param>
		/// <returns>Returns the value read.  NOTE that this may also be zero if that's the value in the remote process</returns>
		VOID* WINAPI RemoteReadPointer(HANDLE Process, VOID* Target, DWORD pointerSize);


		/// <summary>
		/// Free the underlying buffer containid within a unicode string. If the UNICODE_STRING is part of a larger structure allocated withOUT being a standalone pointer, use this.
		/// </summary>
		/// <param name="ptr">Pointer to the UNICODE_STRING whose underyling buffer is to be freed</param>
		/// <returns>TRUE on OK and FALSE on FALURE.</returns>
		//BOOL RemoteFreeUnicodeStringPart(UNICODE_STRING* ptr);

		/// <summary>
		/// free the structure and the underlying buffer of the string. If the UNICODE_STRING was a solo read from teh remote process (ReadUnicodeString() call) use this.
		/// </summary>
		/// <param name="ptr">ptr to free. This also frees the underyling buffer.</param>
		/// <returns>TRUE on OK and FALSE on falure.</returns>
		//BOOL RemoteFreeStandaloneUnicodeString(UNICODE_STRING* ptr);


		/// <summary>
		/// Reads sizeof(UNICODE_STRING) bytes from the passed process into a newly allocated buffer big enough to hold the unicode string structure. This also duplicates the buffer that UnicodeString.Buffer is pointing to in the remote process to the calling process (that's you)
		/// </summary>
		/// <param name="Process">If a true process handle, you must have PROCESS_VM_READ at least.  You may use 0 as a sub for GetCurrentProcess()</param>
		/// <param name="SourceLocation">where to read from in the passed'process's virtual memory.</param>
		/// <returns>returns a new block of memory containing a unicode string.  The buffer from the old UnicodeString is copied into the new one also.</returns>
		/// <remarks>This variant of the routine is used when directly reading UNICODE_STRINGS from pointers or structures that pointer to UNICODE_STRINGS.</remarks>
		//UNICODE_STRING* RemoteReadUnicodeString(HANDLE Process, LPVOID SourceLocation);

		/// <summary>
		/// Copys sizeof(UNICODE_STRING) bytes starting at SourceLocation in the Passed Process to The target location in the calling (that's you) process.
		/// </summary>
		/// <param name="Process">Can use 0 for GetCurrentProcess(), True Process Handles must have PROCESS_VM_READ at least</param>
		/// <param name="SourceLocation">This is where to read from in the virtual memory of the passed processs's handle.</param>
		/// <param name="TargetLocation">The results of the read are copied here. Also, the underling buffer in SourceLocation.Buffer is duplicating into TargetLocation as a seperate buffer.</param>
		/// <returns>True if sucess and false if failure.</returns>
		/// <remarks>This varient of the routine is used when reading UNICODE_STRINGS as part of a more complex data struvture that are baked into the structure as pointers to UNICODE_STRINGS</remarks>
		//BOOL  RemoteReadUnicodeString(HANDLE Process, LPVOID SourceLocation, UNICODE_STRING* TargetLocation);


		/// <summary>
		/// Free a UNICODE_STRING* allocated by RemoteReadUnicodeString().   Memory leaks/ double free() errors may happen if you use this to free a UNICODE_STRING that's stored as part of a larger buffer rather than standalone.    The exception is if the larger structure stores it as a memory pointer to a UNICODE_STRING
		/// </summary>
		/// <param name="Str"></param>
		BOOL WINAPI RemoteFreeUnicodeString(UNICODE_STRING* Str);

		/// <summary>
		/// Read the OBJECT_ATTRIBUTES from the passed location in the remote process DOES NOT COPY THE SECURITY DESCRIPTORS to calling processs. The pointers are still relevent in the remote process  We do allocate an extra 4 bytes to store if we'll need to free handles, ect.. also
		/// </summary>
		/// <param name="Process"></param>
		/// <param name="TargetMemory"></param>
		/// <param name="DupHandles">if passed, current process will get a duplicate of the handles (if any). IF you don't need the handles, leaave this false. You'll get the handle value themselfs BUT they will be in the context of the Process whose handle you specified earlier. You wont be able to use them if you aren't copying from yourself</param>
		/// <returns> a pointer to the new OBJECT_STRIBUTES structure (or 0 on FAILURE to read)</returns>
		OBJECT_ATTRIBUTES* WINAPI RemoteReadObjectAttributes(HANDLE Process, LPVOID TargetMemory, BOOL DuplHandles);

		/// <summary>
		/// Free an OBJECT_ATTRIBUTES data struct allocated by RemoteReadObjectAttributes(). Since we allocate a bit of memory after the struct itself to store HOW it was allocated,  you risk errors passing OBJECT_ATTRIBUTES* to this routine Not allocated by the RemoteReadObjectAtributes() routine
		/// </summary>
		/// <param name="ptr">Pointer to a block of memory previously returned by RemoteReadObjectAttributes</param>
		/// <returns></returns>
		BOOL WINAPI RemoteFreeObjectAttributes(OBJECT_ATTRIBUTES* ptr);


#pragma region Misc
/// <summary>
/// Exported as a variety of ways in the module def file.  This wraps C's free() so that users of the dll have a way to free() memory in the same way it's allocated for them.  Recommanded to call the RmoteReadFreeXXXX routine instead of this in case the way it's allocated is changed
/// </summary>
/// <param name="ptr">non null pointer to free</param>
/// <returns>Returns nothing</returns>
		VOID WINAPI RemoteRead_SimpleFree(VOID* ptr);
#pragma endregion

	}
}