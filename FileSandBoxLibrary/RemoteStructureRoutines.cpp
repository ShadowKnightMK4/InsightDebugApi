#include "RemoteStructure.h"
/*
* Contains routines to read various remote structures into the context of the calling process.
*/

namespace RemoteStructureRoutine
{
	/// <summary>
	/// Allocate a buffer of MaxBuffer, read from the passed process's RemoteMemory allocate and return said buffer
	/// </summary>
	/// <param name="Process">0 is GetCurrentProcess(), True process handle needs PROCESS_VM_READ</param>
	/// <param name="RemoteMemory">Virtual Memory location in the remote process</param>
	/// <param name="MaxBuffer">MaxBuffer to allocate and read (UNICODE_STRING struct has this value)</param>
	/// <returns>a buffer allocated with malloc() of MaxBuffer size with the data read from the remote process. Returns nullptr on error or bad argument</returns>
	PWSTR ReadRemoteUnicodeStringBuffer(HANDLE Process, PWSTR RemoteMemory, USHORT MaxBuffer, USHORT DataSize)
	{
		PWSTR ret = nullptr;
		if (Process == 0)
		{
			Process = GetCurrentProcess();
		}
		if (RemoteMemory == nullptr)
		{
			return nullptr;
		}
		if (MaxBuffer < DataSize)
		{
			return nullptr;
		}
		if (MaxBuffer != 0)
		{
			SIZE_T BytesRead = 0;
			ret = reinterpret_cast<PWSTR>(malloc(MaxBuffer + sizeof(wchar_t)));
			if (ret)
			{
				memset(ret, 0, MaxBuffer + sizeof(wchar_t));
				if (!ReadProcessMemory(Process, RemoteMemory, ret, DataSize, &BytesRead))
				{
					free(ret);
					return nullptr;
				}
				else
				{
					return ret;
				}
			}
		}
		return ret;
	}

	RTL_USER_PROCESS_PARAMETERS* RemoteReadUserProcessParameters(HANDLE Process, LPVOID RemoteMemory)
	{
		RTL_USER_PROCESS_PARAMETERS* ret = nullptr;
		DWORD ProjectedSize = sizeof(RTL_USER_PROCESS_PARAMETERS);
		DWORD BytesRead = 0;
		if (Process == 0)
		{
			Process = GetCurrentProcess();
		}
		if (RemoteMemory == 0)
		{
			return nullptr;
		}

		ret = (RTL_USER_PROCESS_PARAMETERS*)malloc((ProjectedSize));
		if (ret)
		{
			if (!ReadProcessMemory(Process, RemoteMemory, ret, ProjectedSize, &BytesRead) || (BytesRead != ProjectedSize))
			{
				free(ret);
				return nullptr;
			}
			else
			{

				if (ret->CommandLine.Buffer != nullptr)
				{
					if ((ret->CommandLine.Length == 0) || (((VOID*)ret->CommandLine.Buffer) == (VOID*)0xcccccccc))
					{
						ret->CommandLine.Buffer = reinterpret_cast<PWSTR>(malloc(ret->CommandLine.MaximumLength));

						if (ret->CommandLine.Buffer)
						{
							memset(ret->CommandLine.Buffer, 0, ret->CommandLine.MaximumLength);
						}
						else
						{
							free(ret);
							return nullptr;
						}
					}
					else
					{
						ret->CommandLine.Buffer = ReadRemoteUnicodeStringBuffer(Process, ret->CommandLine.Buffer, ret->CommandLine.MaximumLength, ret->CommandLine.Length);
						if (ret->CommandLine.Buffer == nullptr)
						{
							free(ret);
							return nullptr;
						}
					}
				}

				if (ret->ImagePathName.Buffer != nullptr)
				{
					if (ret->ImagePathName.Length == 0)
					{
						ret->ImagePathName.Buffer = reinterpret_cast<PWSTR>(malloc(ret->ImagePathName.MaximumLength));
						if (ret->ImagePathName.Buffer)
						{
							memset(ret->ImagePathName.Buffer, 0, ret->ImagePathName.MaximumLength);
						}
						else
						{
							free(ret->CommandLine.Buffer);
							free(ret);

							return nullptr;
						}
					}
					else
					{
						ret->ImagePathName.Buffer = ReadRemoteUnicodeStringBuffer(Process, ret->ImagePathName.Buffer, ret->ImagePathName.MaximumLength, ret->ImagePathName.Length);
						if (ret->ImagePathName.Buffer == nullptr)
						{
							free(ret->ImagePathName.Buffer);
							if (ret->CommandLine.Buffer != nullptr)
							{
								free(ret->CommandLine.Buffer);
							}
							free(ret);
							return nullptr;
						}
					}
				}
			}
		}
		return ret;
	}



	/// <summary>
		/// Cleanup a RTL_USER_PROCESS_PARAMETERS struct allocated by <see cref="RemoteReadUserProcessParameters">RemoteReadUserProcessParameters</see> call
		/// </summary>
		/// <param name="ptr"></param>
		/// <returns></returns>
	BOOL RemoteFreeUserProcessParameters(RTL_USER_PROCESS_PARAMETERS* ptr)
	{
		if (ptr)
		{
			if (ptr->CommandLine.Buffer != nullptr)
			{
				free(ptr->CommandLine.Buffer);
			}
			if (ptr->ImagePathName.Buffer != nullptr)
			{
				free(ptr->ImagePathName.Buffer);
			}
			free(ptr);
			return TRUE;
		}
		return FALSE;
	}



	HANDLE DuplicateHandleToSelf(HANDLE RemoteProcess, HANDLE TargetHandle)
	{
		HANDLE ret = INVALID_HANDLE_VALUE;
		if (RemoteProcess == 0)
		{
			RemoteProcess = GetCurrentProcess();
		}
		if (!DuplicateHandle(RemoteProcess, TargetHandle, GetCurrentProcess(), &ret, 0, FALSE, DUPLICATE_SAME_ACCESS))
		{
			return INVALID_HANDLE_VALUE;
		}
		return ret;
	}


	VOID* WINAPI RemoteReadPointer(HANDLE Process, VOID* Target, DWORD pointerSize)
	{
		{
			VOID* Ret = nullptr;
			SIZE_T size = 0;
			if (Target != nullptr)
			{
				if (Process == nullptr)
				{
					Process = GetCurrentProcess();
				}
				if ((pointerSize != 4) && (pointerSize != 8))
				{
					return nullptr;
				}
				if (ReadProcessMemory(Process, Target, &Ret, pointerSize, &size))
				{
					if (size != pointerSize)
					{
						return nullptr;
					}

				}
			}
			return Ret;
		}
	}


	/// <summary>
	/// Reads the UNICODE_STRING structure from the remote process into the calling process.  Duplicates the string from the remmote process into the calling process also and sets pointers correctly.
	/// </summary>
	/// <param name="Process"></param>
	/// <param name="Deref"></param>
	/// <param name="MemoryLocation"></param>
	/// <param name="CallerProcessMemory"></param>
	VOID RemoteReadUnicodeStringIntoStruct(HANDLE Process, BOOL Deref, UNICODE_STRING* MemoryLocation, UNICODE_STRING* CallerProcessMemory)
	{
		UNICODE_STRING ret;
		ret.Buffer = nullptr;
		ret.Length = ret.MaximumLength = 0;

		if (Process == 0)
		{
			Process = GetCurrentProcess();
		}
		
		if (CallerProcessMemory == 0)
		{
			return;
		}

		if (MemoryLocation == 0)
		{
			return;
		}
		else
		{
			DWORD SizeNeeded = sizeof(UNICODE_STRING);
			if (Deref)
			{
				MemoryLocation = reinterpret_cast<UNICODE_STRING*>(RemoteReadPointer(Process, MemoryLocation, sizeof(VOID*)));
			}

			if ((!ReadProcessMemory(Process, MemoryLocation, &ret, sizeof(UNICODE_STRING), &SizeNeeded) ||	(SizeNeeded != sizeof(UNICODE_STRING))))
			{
				CallerProcessMemory->Buffer = nullptr;
				CallerProcessMemory->Length = CallerProcessMemory->MaximumLength;
				return;
			}
			else
			{
				if (ret.Buffer != nullptr)
				{
					PWSTR localBuffer = reinterpret_cast<PWSTR>(malloc(ret.MaximumLength));
					if (localBuffer)
					{
						memset(localBuffer, 0, ret.MaximumLength);
						SizeNeeded = ret.Length;
						if ((!ReadProcessMemory(Process, MemoryLocation, localBuffer, ret.Length, &SizeNeeded)) || (SizeNeeded < ret.Length))
						{
							free(localBuffer);
							CallerProcessMemory->Buffer = nullptr;
							CallerProcessMemory->Length = CallerProcessMemory->MaximumLength;
							return;
						}
						else
						{
							ret.Buffer = localBuffer;
							CallerProcessMemory->Buffer = ret.Buffer;
							CallerProcessMemory->Length = ret.Length;
							CallerProcessMemory->MaximumLength = ret.MaximumLength;
							return;
						}
					}
				}
			}
			CallerProcessMemory->Buffer = nullptr;
			CallerProcessMemory->Length = CallerProcessMemory->MaximumLength;
		}
	}


	/// <summary>
	/// Copies 
	/// </summary>
	/// <param name="Processs"></param>
	/// <param name="DerefFist"></param>
	/// <param name="MemoryLocation"></param>
	/// <returns></returns>
	UNICODE_STRING* RemoteReadUnicodeString(HANDLE Process, BOOL DerefFist, UNICODE_STRING* MemoryLocation)
	{
		UNICODE_STRING* ret = nullptr;
		if (Process == 0)
		{
			Process = GetCurrentProcess();
		}
		if (MemoryLocation == nullptr)
		{
			return nullptr;
		}
		else
		{
			ret = reinterpret_cast<UNICODE_STRING*>(malloc(sizeof(UNICODE_STRING)));
			if (ret)
			{
				RemoteReadUnicodeStringIntoStruct(Process, DerefFist, MemoryLocation, ret);
				if ((ret->Buffer == 0))
				{
					free(ret);
					ret = nullptr;
				}
			}
		}
		return ret;
	}

	BOOL WINAPI RemoteFreeUnicodeString(UNICODE_STRING* Str)
	{
		if (Str != nullptr)
		{
			if (Str->Buffer != nullptr)
			{
				free(Str->Buffer);
			}
			free(Str);
			return TRUE;
		}
		return FALSE;
	}




	OBJECT_ATTRIBUTES* WINAPI RemoteReadObjectAttributes(HANDLE Process, LPVOID TargetMemory, BOOL DuplHandles)
	{
		DWORD SizeNeeded = sizeof(OBJECT_ATTRIBUTES);
		OBJECT_ATTRIBUTES* ret;
		if (Process == 0)
		{
			Process = GetCurrentProcess();
		}
		if (TargetMemory == nullptr)
		{
			return nullptr;
		}
		ret = reinterpret_cast<OBJECT_ATTRIBUTES*>(malloc(sizeof(OBJECT_ATTRIBUTES) + sizeof(BOOL)) );
		if (ret)
		{
			memset(ret, 0, SizeNeeded);
			ret->Length = SizeNeeded;
			*((BOOL*)ret + offsetof(OBJECT_ATTRIBUTES, SecurityQualityOfService) + sizeof(PVOID)) = DuplHandles;
			if ((!ReadProcessMemory(Process, TargetMemory, ret, SizeNeeded, &SizeNeeded)) ||
				(SizeNeeded != sizeof(OBJECT_ATTRIBUTES)))
			{
				free(ret);
				return nullptr;
			}
			else
			{
				if (ret->ObjectName != nullptr)
				{
					ret->ObjectName = RemoteReadUnicodeString(Process, FALSE, ret->ObjectName);
				}

				/* TODO copy the security descripter stuff*/
				return ret;
			}


		}
		return ret;

	}

	
	BOOL WINAPI RemoteFreeObjectAttributes(OBJECT_ATTRIBUTES* ptr)
	{
		BOOL DupHandles;
		if (ptr != nullptr)
		{
			DupHandles = *((BOOL*)ptr + offsetof(OBJECT_ATTRIBUTES, SecurityQualityOfService) + sizeof(PVOID));

			if (DupHandles)
			{

			}


			if (ptr->ObjectName != nullptr)
			{
				// OK here because the structure defines the UNICODE_STRING member as pointer to it.
				RemoteFreeUnicodeString(ptr->ObjectName);
			}

			return TRUE;
		}
		return FALSE;
	}

	ULONG_PTR* WINAPI RemoteReadArray(HANDLE Process, LPVOID TargetMemory, DWORD ElementCount)
	{
		DWORD BytesRead = 0;
		ULONG_PTR* ret = nullptr;
		if (Process == 0)
		{
			Process = GetCurrentProcess();
		}
		if (TargetMemory == 0)
		{
			return nullptr;
		}
		if (ElementCount == 0)
		{
			return nullptr;
		}

		ret = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * ElementCount);
		if (ret)
		{
			memset(ret, 0, sizeof(ULONG_PTR) * ElementCount);

			if (!ReadProcessMemory(Process, TargetMemory, ret, sizeof(ULONG_PTR) * ElementCount, &BytesRead) || (BytesRead != sizeof(ULONG_PTR) * ElementCount))
			{
				free(ret);
				return nullptr;
			}
			else
			{

			}
		}


		return ret;
	}

	LPWSTR WINAPI RemoteReadDebugString(HANDLE Process, LPDEBUG_EVENT Event)
	{
		LPWSTR Ret = nullptr;
		DWORD StringSize = 0;
		SIZE_T BytesRead = 0;
		BOOL BigMode = false;
		if (Event == nullptr)
		{
			return nullptr;
		}
		else
		{
			if (Event->dwDebugEventCode == OUTPUT_DEBUG_STRING_EVENT)
			{
				StringSize = Event->u.DebugString.nDebugStringLength;
				if (StringSize == 65536)
				{
					BigMode = true;
				}

				if (Event->u.DebugString.fUnicode)
				{
					StringSize *= 2;
				}

				
				
				Ret = (LPWSTR)malloc((StringSize + sizeof(wchar_t)));
				
			
				if (Ret != nullptr)
				{
					memset(Ret, 0, StringSize + sizeof(wchar_t));
					if (!ReadProcessMemory(Process, Event->u.DebugString.lpDebugStringData, Ret, StringSize, &BytesRead))
					{
						free(Ret);
						return nullptr;
					}
					if (Event->u.DebugString.fUnicode == FALSE)
					{
						LPWSTR ActualString;
						DWORD NeededSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)Ret, Event->u.DebugString.nDebugStringLength, 0, 0);
						ActualString = (LPWSTR)malloc(sizeof(wchar_t) * (NeededSize + 1));
						if (ActualString == nullptr)
						{
							free(Ret);
							return nullptr;
						}
						else
						{
							memset(ActualString, 0, sizeof(wchar_t) * (NeededSize + 1));
							NeededSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)Ret, Event->u.DebugString.nDebugStringLength, ActualString, NeededSize);
							free(Ret);
							Ret = ActualString;
						}
					}
					if (BigMode)
					{
						// ensure string is null terminated.
						Ret[StringSize-1] = 0;
					}
				}

			}

		}
		return Ret;
	}
}

/// <summary>
/// Exported as a viarety of ways in the module def file.  This wraps free() and is intended for use for DLL users to be a simple way to free() memory Allocated by certain routines that only need to  call free()
/// </summary>
/// <param name="ptr"></param>
/// <returns></returns>
VOID WINAPI RemoteRead_SimpleFree(VOID* ptr)
{
	if (ptr != nullptr)
	{
		free(nullptr);
	}
}
/*

OBJECT_ATTRIBUTES* RemoteStructures::RemoteReadObjectAttributes(HANDLE Process, LPVOID TargetMemory)
{
	OBJECT_ATTRIBUTES* Ret = (OBJECT_ATTRIBUTES*)malloc(sizeof(OBJECT_ATTRIBUTES));
	UNICODE_STRING* ObjectName;
	SIZE_T BytesRead = 0;
	if (Ret)
	{
		memset(Ret, 0, sizeof(OBJECT_ATTRIBUTES));
		if (!ReadProcessMemory(Process, TargetMemory, Ret, sizeof(OBJECT_ATTRIBUTES), &BytesRead))
		{
			free(Ret);
			return nullptr;
		}
		else
		{
			if (( BytesRead != sizeof(OBJECT_ATTRIBUTES)) || (Ret->Length != sizeof(OBJECT_ATTRIBUTES)))
			{

				free(Ret);
				return nullptr;
			}
			if (Ret->ObjectName != nullptr)
			{
				Ret->ObjectName = RemoteReadUnicodeString(Process, Ret->ObjectName);
				if (Ret->ObjectName == nullptr)
				{
					free(Ret);
					return nullptr;
				}
			}
		}
	}
	return Ret;
}

BOOL  RemoteStructures::RemoteReadUnicodeString(HANDLE Process, LPVOID TargetMemory, UNICODE_STRING* Target)
{

	SIZE_T BytesRead;
	if (Target != nullptr)
	{
		if ((Process != INVALID_HANDLE_VALUE) && (Process != 0))
		{
			if (TargetMemory != nullptr)
			{
				if (Target != nullptr)
				{
					memset(Target, 0, sizeof(UNICODE_STRING));
					if (ReadProcessMemory(Process, TargetMemory, Target, sizeof(UNICODE_STRING), &BytesRead))
					{
						if (!(BytesRead == sizeof(UNICODE_STRING)))
						{
							return FALSE;
						}
						PWSTR NewBuffer = (PWSTR)malloc(Target->MaximumLength);
						if (Target->Buffer)
						{
							if (NewBuffer != 0)
							{
								memset(NewBuffer, 0, Target->MaximumLength);
								if (!ReadProcessMemory(Process, Target->Buffer, NewBuffer, Target->Length, &BytesRead))
								{
									free(NewBuffer);
									return FALSE;
								}
								else
								{
									Target->Buffer = NewBuffer;
									Target->Length = BytesRead;
									return TRUE;
								}
							}
							else
							{

								NewBuffer = 0;
								memset(Target, 0, sizeof(UNICODE_STRING));
								free(Target);

							}
						}
						else
						{
							memset(TargetMemory, 0, sizeof(UNICODE_STRING));
							free(NewBuffer);
						}

					}
					else
					{

						memset(TargetMemory, 0, sizeof(UNICODE_STRING));
						return FALSE;
					}
				}
			}
		}
	}
	return FALSE;
}
/// <summary>
/// Read and allocate a UNICODE_STRING from a remote process into ours
/// </summary>
/// <param name="Process"></param>
/// <param name="TargetMemory"></param>
/// <remarks>This allocates the UNICODE_STRING struct, copies it to our process, allocates a LPWSTR of length (in bytes) equal to MaximumLength and then sets our copy to that buffer. Finally this buffer receives the string from the remote process equal to Length</remarks>
/// <returns></returns>
UNICODE_STRING* RemoteStructures::RemoteReadUnicodeString(HANDLE Process, LPVOID TargetMemory)
{
	UNICODE_STRING* Ret = (UNICODE_STRING*)malloc(sizeof(UNICODE_STRING));
	SIZE_T BytesRead;
	if (Ret)
	{
		if (RemoteReadUnicodeString(Process, TargetMemory, Ret))
		{
			return Ret;
		}
		else
		{
			return nullptr;
		}
		/*
		memset(Ret, 0, sizeof(UNICODE_STRING));
		if (!ReadProcessMemory(Process, TargetMemory, Ret, sizeof(UNICODE_STRING), &BytesRead))
		{
			free(Ret);
			return nullptr;
		}
		else
		{
			LPWSTR LocalBuffer = (LPWSTR)malloc(Ret->MaximumLength);
			if (LocalBuffer == nullptr)
			{
				free(Ret);
				return nullptr;
			}
			memset(LocalBuffer, 0, Ret->MaximumLength);
			if (!ReadProcessMemory(Process, Ret->Buffer, LocalBuffer, Ret->Length, &BytesRead))
			{
				free(Ret);
				free(LocalBuffer);
				return nullptr;
			}
			else
			{
				Ret->Buffer = LocalBuffer;
			}
		}

	}
	return Ret;
}



	/// <summary>
	/// Free UNICODE_STRING.  NOT INTENDED FOR GENERIC use outside of this function as it deals with the underylign implementation of how we allocate UNICODE_STRINGs as read from remote processes.
	/// </summary>
	/// <param name="ptr">ptr</param>
	/// <param name="FreePtr">if TRUE, we free(ptr). If False we free(ptr->Buffer) only.</param>
	/// <returns>true on ok and false if something went fooy</returns>
	/// <remarks>For unicode strings that begun list as a pointer, set TreePtr to true.  For UnicodeStrings() that are containting within a larger structure that ARE NOT POIINTERS, set this to false. /<remarks>
	BOOL RemoteFreeUnicodeString(UNICODE_STRING* ptr, BOOL FreePtr)
	{
		if (ptr != nullptr) {
			if (ptr->Buffer != nullptr)
			{
				free(ptr->Buffer);
			}
			if (FreePtr)
			{
				free(ptr);
			}
		}
		return FALSE;
	}


	BOOL RemoteFreeUnicodeStringPart(UNICODE_STRING* ptr)
	{
		return RemoteFreeUnicodeString(ptr, FALSE);
	}

	BOOL RemoteFreeStandaloneUnicodeString(UNICODE_STRING* ptr)
	{
		return RemoteFreeUnicodeString(ptr, TRUE);
	}


	UNICODE_STRING* RemoteReadUnicodeString(HANDLE Process, LPVOID SourceLocation)
	{
		UNICODE_STRING* ret = 0;
		if ((SourceLocation == nullptr))
		{
			return ret;
		}
		if ((Process == 0))
		{
			Process = GetCurrentProcess();
		}
		ret = (UNICODE_STRING*)malloc(sizeof(UNICODE_STRING));
		if (ret)
		{
			if (!RemoteStructureRoutine::RemoteReadUnicodeString(Process, SourceLocation, ret))
			{
				free(ret);
				ret = 0;
			}
		}
		return ret;
	}

	/// <summary>
	/// Copys sizeof(UNICODE_STRING) bytes starting at SourceLocation in the Passed Process to The target location in the calling (that's you) process.
	/// </summary>
	/// <param name="Process">Can use 0 for GetCurrentProcess(), True Process Handles must have PROCESS_VM_READ at least</param>
	/// <param name="SourceLocation">This is where to read from in the virtual memory of the passed processs's handle.</param>
	/// <param name="TargetLocation">The results of the read are copied here. Also, the underling buffer in SourceLocation.Buffer is duplicating into TargetLocation as a seperate buffer.</param>
	/// <returns>True if sucess and false if failure.</returns>
	/// <remarks>This varient of the routine is used when reading UNICODE_STRINGS as part of a more complex data struvture..</remarks>
	BOOL  RemoteReadUnicodeString(HANDLE Process, LPVOID SourceLocation, UNICODE_STRING* TargetLocation)
	{
		PWSTR MyBuffer = 0;
		if ((SourceLocation == nullptr) || (TargetLocation == nullptr))
		{
			return FALSE;
		}
		if (Process == 0)
		{
			Process = GetCurrentProcess();
		}
		SIZE_T BytesRead = 0;
		if ((!ReadProcessMemory(Process, SourceLocation, TargetLocation, sizeof(UNICODE_STRING), &BytesRead)) || (BytesRead != sizeof(UNICODE_STRING)))
		{
			ZeroMemory(TargetLocation, sizeof(UNICODE_STRING));
			return FALSE;
		}
		else
		{

			if (Process == GetCurrentProcess())
			{
				MyBuffer = _wcsdup(TargetLocation->Buffer);
				if (MyBuffer)
				{
					TargetLocation->Buffer = MyBuffer;
					return TRUE;
				}
				else
				{
					ZeroMemory(TargetLocation, sizeof(UNICODE_STRING));
					return FALSE;
				}

			}
			else
			{
				MyBuffer = (PWSTR)malloc(TargetLocation->MaximumLength);
				if (MyBuffer)
				{
					ZeroMemory(MyBuffer, TargetLocation->MaximumLength);
					BytesRead = 0;
					if (!ReadProcessMemory(Process, TargetLocation->Buffer, MyBuffer, TargetLocation->Length, &BytesRead) || (BytesRead != TargetLocation->Length))
					{
						free(MyBuffer);
						ZeroMemory(TargetLocation, sizeof(UNICODE_STRING));
						return FALSE;
					}
					else
					{
						TargetLocation->Buffer = MyBuffer;
						if (TargetLocation->Length == TargetLocation->MaximumLength)
						{
							// a safety guard to ensure the string is null terminated
							TargetLocation->Buffer[TargetLocation->Length - 1] = 0;
						}
						return TRUE;
					}
				}
				return FALSE;
			}
		}
	}
*/
