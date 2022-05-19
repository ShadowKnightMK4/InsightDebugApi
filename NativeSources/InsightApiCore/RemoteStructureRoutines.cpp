#include "RemoteStructure.h"
/*
* RemoteStructureRoutines.CPP
* 
* These routines will typically take a handle (or process id) and read as chunk of memory from it into the caller's process and return it.
*/

namespace RemoteStructureRoutine
{

	/// <summary>
	/// write a dword into the remote memory location with value of Value
	/// </summary>
	/// <param name="Process"></param>
	/// <param name="Value"></param>
	/// <param name="RemoteLocation"></param>
	/// <returns></returns>
	BOOL RemotePoke4(HANDLE Process, DWORD Value, VOID* RemoteLocation)
	{
		DWORD LastError=0;
		if ((RemoteLocation != 0) && (Process != 0))
		{
			SIZE_T BytesWrote = 0;
			if (WriteProcessMemory(Process, RemoteLocation, &Value, sizeof(DWORD), &BytesWrote))
			{
				if (BytesWrote == sizeof(DWORD))
				{
					return TRUE;
				}
			}
			else
			{
#ifdef _DEBUG
				LastError = GetLastError();
				
#endif
			}
		}


		return FALSE;
	}

	/// <summary>
	/// Allocate a buffer of MaxBuffer, read from the passed process's RemoteMemory allocate and return said buffer
	/// </summary>
	/// <param name="Process">Here 0 = GetCurrentProcess(), True process handle needs PROCESS_VM_READ</param>
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
		SIZE_T BytesRead = 0;
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
		INT Padding; // alights buffer to correct location
		ULONGLONG Buffer;
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
	/// Object attrib for x86 debugged 
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
	/// object attrib for x64 debugged process.
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



	VOID* WINAPI RemoteReadUnicodeString(HANDLE Process, UINT_PTR MemoryLocation, BOOL TargetIs32)
	{
		/*
		* Functionally what we do is allocate a local memory chuck sizeof(UNICODE_STRINGNN) (32 or 64 depending on TargetIs32),
		* plus the length of the string and return as one contuous block
		*/
		SIZE_T BytesRead = 0;
		SIZE_T SizeToRead = 0;

		union padding
		{
			UNICODE_STRING32 ret32;
			UNICODE_STRING64 ret64;
		} local;
		MachineDependantUnicodeString ret{};

		memset(&local.ret64, 0, sizeof(UNICODE_STRING64));

		if (TargetIs32)
		{
			SizeToRead = sizeof(UNICODE_STRING32);
		}
		else
		{
			SizeToRead = sizeof(UNICODE_STRING64);
		}


		if (ReadProcessMemory(Process, (LPCVOID)MemoryLocation, &local.ret32, SizeToRead, &BytesRead))
		{
			if (TargetIs32)
			{
				ret.p32 = (UNICODE_STRING32*)malloc(sizeof(UNICODE_STRING32));
				if (ret.p32)
				{
					ZeroMemory(ret.p32, sizeof(UNICODE_STRING32));
					ret.p32->Length = local.ret32.Length;
					ret.p32->MaxLength = local.ret32.MaxLength;
					ret.p32->Buffer = (UINT)malloc(local.ret32.MaxLength);
					if (ret.p32->Buffer != 0)
					{
						ZeroMemory((VOID*)ret.p32->Buffer, local.ret32.MaxLength);

						if (!ReadProcessMemory(Process, (LPCVOID)local.ret32.Buffer, (LPVOID)ret.p32->Buffer, local.ret32.Length, &BytesRead))
						{
							free((VOID*)ret.p32->Buffer);
							free((VOID*)ret.p32);
							return ret.p32;
						}
					}
					return ret.p32;
				}
				
			}
			else
			{
				ret.p64 = (UNICODE_STRING64*)malloc(sizeof(UNICODE_STRING64));
				if (ret.p64)
				{
					ZeroMemory(ret.p64, sizeof(UNICODE_STRING64));
					ret.p64->Length = local.ret64.MaxLength;
					ret.p64->MaxLength = local.ret64.MaxLength;
					ret.p64->Buffer = 0;
					ret.p64->Buffer = (ULONGLONG)malloc(local.ret64.MaxLength);
					if (ret.p64->Buffer != 0)
					{
						ZeroMemory((VOID*)ret.p64->Buffer, local.ret64.MaxLength);

						if (!ReadProcessMemory(Process, (LPCVOID)local.ret64.Buffer, (LPVOID)ret.p64->Buffer, local.ret64.MaxLength, &BytesRead))
						{
							free((VOID*)ret.p64->Buffer);
							free((VOID*)ret.p64);
							return ret.p32;
						}
					}
					return ret.p64;
				}
				

			}
			

		}
		return nullptr;
	}

	BOOL WINAPI RemoteFreeUnicodeString(UNICODE_STRING* Str, bool  TargetIs32)
	{
		MachineDependantUnicodeString help;
		if (Str != nullptr)
		{
			if (TargetIs32)
			{
				help.p32 = (UNICODE_STRING32*)Str;
				if (help.p32->Buffer != 0)
				{
					RemoteRead_SimpleFree((VOID*)help.p32->Buffer);
				}

				return RemoteRead_SimpleFree(help.p32);
			}
			else
			{
				help.p64 = (UNICODE_STRING64*)Str;
				if (help.p64->Buffer != 0)
				{
					RemoteRead_SimpleFree((VOID*)help.p64->Buffer);
				}

				return RemoteRead_SimpleFree(help.p64);
			}
		}
		return FALSE;
	}




	BOOL WINAPI RemoteFreeObjectAttributes(OBJECT_ATTRIBUTES* Attrib, BOOL TargetIs32)
	{
		union ret_padding
		{
			OBJECT_ATTRIBUTES32* p32;
			OBJECT_ATTRIBUTES64* p64;
		} help{};
		
		if (Attrib != nullptr)
		{
			if (TargetIs32)
			{
				help.p32 = (OBJECT_ATTRIBUTES32*)Attrib;
				if ((help.p32->RootDirectory != 0) && (help.p32->RootDirectory != ((ULONG)(-1))))
				{
					CloseHandle(help.p32);
				}
				if (help.p32->RootDirectory != 0)
				{
					RemoteFreeUnicodeString((UNICODE_STRING*)help.p32->ObjectName, TargetIs32);
					help.p32->ObjectName = 0;
				}
			}
			else
			{
				help.p64 = (OBJECT_ATTRIBUTES64*)Attrib;
				if ((help.p64->RootDirectory != 0) && (help.p64->RootDirectory != ((ULONGLONG)(-1))))
				{
					CloseHandle(help.p64);
				}
				if (help.p64->RootDirectory != 0)
				{
					RemoteFreeUnicodeString((UNICODE_STRING*)help.p64->ObjectName, TargetIs32);
					help.p64->ObjectName = 0;
				}
			}

			

			free(Attrib);
		}
		return FALSE;
	}

	VOID* WINAPI RemoteReadObjectAttributes(HANDLE Process, UINT_PTR MemoryLocation, BOOL TargetIs32)
	{
		SIZE_T BytesRead;
		SIZE_T SizeToRead;
		
		union padding 
		{
			OBJECT_ATTRIBUTES32 ret32;
			OBJECT_ATTRIBUTES64 ret64;
		} local;
		union ret_padding
		{
			OBJECT_ATTRIBUTES32* p32;
			OBJECT_ATTRIBUTES64* p64;
		} ret;

		if (TargetIs32)
		{
			SizeToRead = sizeof(OBJECT_ATTRIBUTES32);
		}
		else
		{
			SizeToRead = sizeof(OBJECT_ATTRIBUTES64);
		}
		
		memset(&local.ret64, 0, sizeof(OBJECT_ATTRIBUTES64));
		if (ReadProcessMemory(Process, (LPCVOID)MemoryLocation, &local.ret32, SizeToRead, &BytesRead))
		{
			ret.p32 = (OBJECT_ATTRIBUTES32*)malloc(SizeToRead);
			if (ret.p32 != nullptr)
			{
				ZeroMemory(ret.p32, SizeToRead );


				
				if (TargetIs32)
				{
					ret.p32->Length = local.ret32.Length;
					if (ret.p32->RootDirectory != 0)
					{
						ret.p32->RootDirectory = (ULONG)DuplicateHandleToSelf(Process, (HANDLE)local.ret32.RootDirectory);
					}
					else
					{
						ret.p32->RootDirectory = 0;
					}

					if (local.ret32.ObjectName != 0)
					{
						ret.p32->ObjectName = (ULONG)RemoteReadUnicodeString(Process, ret.p32->ObjectName, TargetIs32);
					}
					else
					{
						ret.p32->ObjectName = 0;
					}

					ret.p32->Attributes = local.ret32.Attributes;
					ret.p32->SecurityDesc = local.ret32.SecurityDesc;
					ret.p32->SecurityQoS = local.ret32.SecurityQoS;
				}
				else
				{
					ret.p64->Length = local.ret64.Length;
					if (ret.p64->RootDirectory != 0)
					{
						ret.p64->RootDirectory = (ULONGLONG)DuplicateHandleToSelf(Process, (HANDLE)local.ret64.RootDirectory);
					}
					else
					{
						ret.p64->RootDirectory = 0;
					}

					if (local.ret64.ObjectName != 0)
					{
						ret.p64->ObjectName = (ULONGLONG)RemoteReadUnicodeString(Process, local.ret64.ObjectName, TargetIs32);
					}
					else
					{
						ret.p64->ObjectName = 0;
					}

					ret.p64->Attributes = local.ret64.Attributes;
					ret.p64->SecurityDesc = local.ret64.SecurityDesc;
					ret.p64->SecurityQoS = local.ret64.SecurityQoS;
				}
				return ret.p64;
			}
		}
		return nullptr;
	}



	

	ULONG_PTR* WINAPI RemoteReadArray(HANDLE Process, LPVOID TargetMemory, DWORD ElementCount)
	{
		SIZE_T BytesRead = 0;
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

		ret = (SIZE_T*)malloc(sizeof(ULONG_PTR) * ElementCount);
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

	LPWSTR WINAPI RemoteReadStringW(HANDLE Process, const wchar_t* RemoteMemoryLocation, size_t char_count)
	{
		DWORD StringSize = 0;
		LPWSTR ret = nullptr;
		SIZE_T BytesRead = 0;
		if (RemoteMemoryLocation == nullptr)
		{
			return nullptr;
		}
		else
		{
			StringSize = char_count+1;
			StringSize *= sizeof(wchar_t);

			
			// fix to try NOT curropting heap
			ret = (LPWSTR)malloc(StringSize * 2);
			if (ret != nullptr)
			{
				ZeroMemory(ret, StringSize * 2);
				StringSize -= sizeof(wchar_t);
				if (!ReadProcessMemory(Process, RemoteMemoryLocation, ret, StringSize, &BytesRead))
				{
					free(ret);
					ret = nullptr;
				}
				else
				{
					
				}

			}
			return ret;
		}
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


	BOOL WINAPI RemoteRead_SimpleFree(VOID* ptr)
	{
		if (ptr != nullptr)
		{
			free(ptr);
			return TRUE;
		}
		return FALSE;
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
