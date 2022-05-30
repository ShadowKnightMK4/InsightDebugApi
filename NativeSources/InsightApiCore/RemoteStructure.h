#pragma once
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

#pragma region UnicodeString

		/// <summary>
		/// We read from the remote. 
		/// </summary>
		/// <param name="Processs"></param>
		/// <param name="DerefFist"></param>
		/// <param name="MemoryLocation">Virtual Memory Location (x64 or x86)</param>
		/// <param name="TargetIs32">Set to true for 32-bit WOW or 64 for non.</param>
		/// <returns>If TargetIs32, then returns UNICODE_STRING32* otherwise UNICODE_STRING64*.  Use  </returns>
		VOID* WINAPI RemoteReadUnicodeString(HANDLE Process, UINT_PTR MemoryLocation, BOOL TargetIs32) noexcept;

		/// <summary>
		/// Free an UNICODE_STRING struct previously allocated by RemoteReadUnicodeString(). You will need to know which type of struct
		/// </summary>
		/// <param name="Str"></param>
		/// <param name="TargetIs32"></param>
		/// <returns></returns>
		BOOL WINAPI RemoteFreeUnicodeString(UNICODE_STRING* Str, bool  TargetIs32);
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
		/// <returns>allocates and returns a Unicode string.  IF the Event contains an ANSI string, it is converted to Unicode before being returned.  Largest String possible returned is 0xfffe long If something goes wrong, nullptr is returned instead</returns>
		LPWSTR WINAPI RemoteReadDebugString(HANDLE Process, const LPDEBUG_EVENT Event) noexcept;


		/// <summary>
		/// Extract a string of arbitrary char length from the remote process.  
		/// </summary>
		/// <param name="Process">Target Process should have PROCESS_VM_READ rights</param>
		/// <param name="RemoteMemoryLocation">Virtual memory in the TARGET Process location</param>
		/// <param name="char_count">how many chars</param>
		/// <returns>returns a pointer to the string or null.  Routine allocates sizeof(wchar_t) *  char_count+1 then null terminates it before returning block</returns>
		LPWSTR WINAPI RemoteReadStringW(HANDLE Process, const wchar_t* RemoteMemoryLocation, size_t char_count);


		/// <summary>
		/// Common code branch between the RemotePpoke routines.  Those are little more than calling this with the correct size.
		/// </summary>
		/// <param name="Process">Process to write too.</param>
		/// <param name="Value">Value to Write</param>
		/// <param name="RemoteLocation">For x86 compiled InsightAPI.dll, this is a 4 byte pointer. For x64 compile InightAPI.dll, this is an 8 byte pointer</param>
		/// <param name="Size"> How many byte (Range is > 0 and less than 9)</param>
		/// <returns>returns FALSE if the write did not work or if Size is not the current size</returns>
		BOOL WINAPI RemotePokeCommon(HANDLE Process, ULONG64 Value, VOID* RemoteLocation, SIZE_T Size) noexcept;

		/// <summary>
		/// write a 4 byte value into the remote memory location with value of Value
		/// </summary>
		/// <param name="Process">Target Process should have at least PROCESS_VM_WRITE and potentially PROCESS_VM_OPERATION access</param>
		/// <param name="Value">4 byte value to write</param>
		/// <param name="RemoteLocation">For x86 compiled InsightAPI.dll, this is a 4 byte pointer. For x64 compile InightAPI.dll, this is an 8 byte pointer</param>		
		/// <returns></returns>
		BOOL WINAPI RemotePoke4(HANDLE Process, DWORD Value, VOID* RemoteLocation) noexcept;
		
		/// <summary>
		/// write an 8 byte value into the remote memory location with value of Value
		/// </summary>
		/// <param name="Process">Target Process should have at least PROCESS_VM_WRITE and potentially PROCESS_VM_OPERATION access</param>
		/// <param name="Value">8 byte value to write</param>
		/// <param name="RemoteLocation">For x86 compiled InsightAPI.dll, this is a 4 byte pointer. For x64 compile InightAPI.dll, this is an 8 byte pointer</param>
		/// <returns></returns>
		BOOL WINAPI RemotePoke8(HANDLE Process, ULONG64 Value, VOID* RemoteLocation) noexcept;
#pragma endregion








		/// <summary>
		/// Shorthand for grabbing a duplicate handle from the passed process and literal handle into our process
		/// </summary>
		/// <param name="RemoteProcess">Use 0 for GetCurrentProcess().</param>
		/// <param name="TargetHandle">Handle to duplicate</param>
		/// <returns>INVALID_HANDLE_VALUE on fail or the dupliceted handled one ok.</returns>
		HANDLE DuplicateHandleToSelf(HANDLE RemoteProcess, HANDLE TargetHandle) noexcept;

		/// <summary>
		/// Read an arbitrary size of memory from the passed process at Target Location in it's address and return it typecast to a void*.   tl;dr   Deference a pointer in the remote process and return 4 bytes specified by the remote pointer
		/// </summary>
		/// <param name="Process">0 means GetCurrentProcess() here.  A true process handle needs PROCESS_VM_READ permission.</param>
		/// <param name="Target">where in the Process's location to read from.</param>
		/// <param name="pointerSize">Size of the pointer in bytes. Recommand setting to sizeof(VOID*) and also ensuring your pointer size and the remote pointer size match.  This routine FAILs if this is not 4 or 8.</param>
		/// <returns>Returns the value read.  NOTE that this may also be zero if that's the value in the remote process</returns>
		VOID* WINAPI RemoteReadPointer(HANDLE Process, VOID* Target, DWORD pointerSize);



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
		/// Free an OBJECT_ATTRIBUTES struct previously allocated.  Specify if we're freeing a 32-bit version or 64-bit version.
		/// </summary>
		/// <param name="Attrib"></param>
		/// <param name="TargetIs32"></param>
		/// <returns></returns>
		BOOL WINAPI RemoteFreeObjectAttributes(OBJECT_ATTRIBUTES* Attrib, BOOL TargetIs32);

		/// <summary>
		/// Attempt to read 
		/// </summary>
		/// <param name="hProcess"></param>
		/// <param name="RemoteLocation"></param>
		/// <returns></returns>
		VOID* WINAPI RemoteReadObjectAttributes(HANDLE Process, UINT_PTR MemoryLocation, BOOL TargetIs32);
	


#pragma region Misc
/// <summary>
/// Exported as a variety of ways in the module def file.  This wraps C's free() so that users of the DLL have a way to free() memory in the same way it's allocated for them.  Recommended to call the RemoteReadFreeXXXX routine instead of this in case the way it's allocated is changed
/// </summary>
/// <param name="ptr">non null pointer to free</param>
/// <returns>Returns nothing</returns>
		BOOL WINAPI RemoteRead_SimpleFree(VOID* ptr);
#pragma endregion


#pragma region Structures

		/// <summary>
		/// UNICODE_STRING for x86 debugged processes
		/// </summary>
		struct UNICODE_STRING32
		{
			USHORT Length;
			USHORT MaxLength;
			UINT Buffer;
		};

		/// <summary>
		///  UNICODE_STRING for x64 debugged processes
		/// </summary>
		struct UNICODE_STRING64
		{
			USHORT Length;
			USHORT MaxLength;
			INT Padding; // alights buffer to correct location. See the ObjectAttributeOffsets for where I got offsets
			ULONGLONG Buffer;
		};

		/// <summary>
		/// RTL_USER_PROCESS_PARAMETERS for x86 debugged process
		/// </summary>
		struct RTL_USER_PROCESS_PARAMETERS32
		{
			BYTE Reserved1[16];
			UINT32 Reserved2[10];
			UNICODE_STRING32 ImagePathName;
			UNICODE_STRING32 CommandLine;
		};

		/// <summary>
		/// RTL_USER_PROCESS_PARAMETERS  for x64 debugged process.
		/// </summary>
		struct RTL_USER_PROCESS_PARAMETERS64
		{
			BYTE Reserved1[16];
			UINT64 Reserved2[10];
			UNICODE_STRING64 ImagePathName;
			UNICODE_STRING64 CommandLine;
		};

	



		



		/// <summary>
		/// OBJECT_ATTRIBUTES for x86 debugged process.,
		/// </summary>
		struct OBJECT_ATTRIBUTES32
		{
			
			ULONG32 Length;
			ULONG32 RootDirectory;
			ULONG32 ObjectName;
			ULONG32 Attributes;
			ULONG32 SecurityDesc;
			ULONG32 SecurityQoS;
		};

		/// <summary>
		/// OBJECT_ATTRIBUTES for x64 debugged process.
		/// </summary>
		struct OBJECT_ATTRIBUTES64
		{
			ULONG32 Length;
			ULONGLONG RootDirectory;
			ULONGLONG ObjectName;
			ULONG32 Attributes;
			ULONGLONG SecurityDesc;
			ULONGLONG SecurityQoS;
		};



		/// <summary>
		/// pointer may be a Unicode32 or Unicode64 bit. Size of union is 1 pointer.
		/// </summary>
		union MachineDependantObjectAttributes
		{
			OBJECT_ATTRIBUTES32* p32;
			OBJECT_ATTRIBUTES64* p64;
		};

		/// <summary>
		/// pointer may be a Unicode32 or Unicode64 bit. Size of union is 1 pointer.
		/// </summary>
		union MachineDependantUnicodeString
		{
			UNICODE_STRING32* p32;
			UNICODE_STRING64* p64;
		};

		/// <summary>
	/// pointer to either a RTL_USER_PROCESS_PARAMETERS32 or RTL_USER_PROCESS_PARAMETERS64 struct
	/// </summary>
		union MachineDependantUserProcessParameters
		{
			RTL_USER_PROCESS_PARAMETERS32* p32;
			RTL_USER_PROCESS_PARAMETERS64* p64;
		};
#pragma endregion

	}
}