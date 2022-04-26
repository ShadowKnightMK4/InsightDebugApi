/*
*	Utility.cpp   Houses code for a few generic routines.   

	Currently mostly for ProcessHandlingInternal.cpp uses the and they are exported for use. Most are self contained.
*/
#include "Utility.h"
#include <Psapi.h>

/// <summary>
/// Enable a chosen priv on self.  NOT INTENDED TO BE EXPORTED.  We really use this to just enable debug priv
/// </summary>
/// <param name="PrivNaem">Name of the Privilege</param>
/// <param name="Enable">TRUE to enable, FALSE to turn off.</param>
/// <returns>True if it worked and false if it did nt</returns>
bool EnablePrivOnSelf(LPCWSTR PrivNaem, BOOL Enable)
{

	// try asking for the debug priv and go gain.
	TOKEN_PRIVILEGES priv;
	LUID luid;
	HANDLE SelfAccessHandle = INVALID_HANDLE_VALUE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &SelfAccessHandle))
	{
		if (LookupPrivilegeValue(nullptr, PrivNaem, &luid))
		{
			priv.PrivilegeCount = 1;
			priv.Privileges[0].Luid = luid;
			if (Enable)
			{
				priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			}
			else
			{
				priv.Privileges[0].Attributes = 0;
			}
			if (AdjustTokenPrivileges(SelfAccessHandle, false, &priv, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr))
			{
				CloseHandle(SelfAccessHandle);
				return TRUE;
			}
		}
	}
	if (SelfAccessHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(SelfAccessHandle);
	}
	return FALSE;
}

/// <summary>
/// Ask for the SeDebugPriv. I'm on the fence for exporting this as this is  aready made option for just loading the dll and asking for the priv.
/// </summary>
/// <returns></returns>
BOOL WINAPI AskForDebugPriv()
{
	return EnablePrivOnSelf(SE_DEBUG_NAME, TRUE);
}


HANDLE WINAPI RemoteHandleDup(HANDLE CurrentHandle, DWORD Access, BOOL CopyAccess, HANDLE RemoteProcess, BOOL Inherit)
{
	HANDLE ret = 0;
	DWORD Arg = 0;
	if (CopyAccess)
	{
		Arg = DUPLICATE_SAME_ACCESS;
	}
	if (DuplicateHandle(GetCurrentProcess(), CurrentHandle, RemoteProcess, &ret, Access, Inherit, Arg))
	{
		return ret;
	}
	return 0;
}

HANDLE WINAPI LocalHandleDup(HANDLE CurrentHandle, DWORD Access, BOOL CopyAccess)
{
	HANDLE Ret = 0;
	DWORD Arg = 0;
	if (CopyAccess)
	{
		Arg = DUPLICATE_SAME_ACCESS;
	}
	if (DuplicateHandle(GetCurrentProcess(), CurrentHandle, GetCurrentProcess(), &Ret, Access, FALSE, Arg))
	{
		return Ret;
	}
	return 0;
}

char* WINAPI ConvertUnicodeString(const wchar_t* Original)
{
	char* ret = nullptr;
	if (Original == nullptr)
	{
		return nullptr;
	}
	else
	{
		int size = wcslen(Original);
		int SizeNeeded = 0;
		SizeNeeded = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, Original, size, nullptr, 0, 0, nullptr);
		SizeNeeded++;
		ret = reinterpret_cast<char*>(malloc(SizeNeeded));
		if (ret != nullptr)
		{
			ret[SizeNeeded - 1] = 0;
			SizeNeeded--;

			SizeNeeded = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, Original, size, ret, SizeNeeded, 0, NULL);

			if (SizeNeeded == 0)
			{
				free(ret);
				return nullptr;
			}
		}
		return ret;
	}
}



wchar_t* WINAPI ConvertANSIString(const char * Original)
{
	wchar_t* ret = nullptr;

	if (Original == nullptr)
	{
		// NULL is as NULL does.
		return nullptr;
	}
	else
	{
		int size = strlen(Original);
		int sizeNeeded = 0;
		if (size == 0)
		{
			ret = reinterpret_cast<wchar_t*>(malloc(0));
			if (ret != nullptr)
			{
				return ret;
			}
			return nullptr;
		}
		else
		{
			sizeNeeded = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Original, size + 1, nullptr, 0);

			sizeNeeded += 1;// the null char(s)
			sizeNeeded *= sizeof(wchar_t); // this is because the returned size neede is in chars.
			if (sizeNeeded == 0)
			{
				return nullptr;
			}
			else
			{
				ret = reinterpret_cast<wchar_t*>(malloc(sizeNeeded));
				if (ret != nullptr)
				{
					memset(ret, 0, sizeNeeded);
					sizeNeeded--; //  let the null terminated be alone
					size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Original, size, ret, sizeNeeded);
				}
				return ret;
			}
		}
	}
}


/// <summary>
/// Allocate a buffer big enough to contain the specified hmodule belonging to the specified process. CALLER will need to free.
/// </summary>
/// <param name="Process">Handle to process to read from. Will need PROCESS_QUERY_INFORMATION || PROCESS_QUERY_LIMITED_INFORMATION access rights</param>
/// <param name="Module">hmodule in question</param>
/// <returns>returns memory allocated in the local process containing a unicode string contraining the module's name</returns>
wchar_t* WINAPI GetModuleNameViaHandle(HANDLE Process, HMODULE Module)
{
	wchar_t* Buffer = nullptr;
	size_t BufferSize = 1024;
	DWORD CalResult = 0;
	while (true)
	{
		Buffer = reinterpret_cast<wchar_t*>(malloc(BufferSize));

		if (Buffer == nullptr)
		{
			return nullptr;
		}
		else
		{
			memset(Buffer, 0, BufferSize);
		}
		SetLastError(0);
		CalResult = GetModuleFileNameExW(Process, Module, Buffer, BufferSize / sizeof(wchar_t));
		if (CalResult >= BufferSize)
		{
			BufferSize *= 2;
			free(Buffer);
		}
		else
		{
			DWORD err = GetLastError();
			if (err != 0)
			{
				free(Buffer);
				Buffer = nullptr;
			}
			break;
		}
	}
	return Buffer;
}


wchar_t* WINAPI GetFileNameViaHandle(HANDLE FileHandle)
{
	wchar_t* Buffer = nullptr;
	DWORD BufferSize = 0;
	BufferSize = (GetFinalPathNameByHandle(FileHandle, Buffer, 0, VOLUME_NAME_DOS));
	BufferSize += sizeof(TCHAR);
	Buffer = (wchar_t*)malloc(sizeof(TCHAR) * BufferSize);
	if (Buffer)
	{
		memset(Buffer, 0, sizeof(TCHAR) * BufferSize);
		BufferSize -= sizeof(TCHAR);
		BufferSize = GetFinalPathNameByHandle(FileHandle, Buffer, BufferSize, VOLUME_NAME_DOS);

	}
	return Buffer;
}

/// <summary>
/// Write an 8 byte (QWORD sized) value to a specific location in the local virtual memory
/// </summary>
/// <param name="MemoryLocation">non null value pointing where to write too</param>
/// <param name="value">value to write</param>
/// <returns>true if it worked and false if you pass null</returns>
BOOL Poke8(unsigned long long* MemoryLocation, long long value)
{
	if (MemoryLocation == nullptr)
	{
		return FALSE;
	}
	else
	{
		*MemoryLocation = value;
		return TRUE;
	}
}

/// <summary>
/// Read an 8 byte (QWORD sized) value from a specific location in the local virtual memory
/// </summary>
/// <param name="MemoryLocation">Non null location  indicating where to read from</param>
/// <returns>If you pass 0 for memory location, returns 0 otherwise returns the contents of the location</returns>
unsigned long long Peek8(unsigned long long* MemoryLocation)
{
	if (MemoryLocation == nullptr)
	{
		return FALSE;
	}
	return *MemoryLocation;
}

/// <summary>
/// Write a 4 byte (DWORD) sized value to the passed non null location
/// </summary>
/// <param name="MemoryLocation">non null value pointing where to write too</param>
/// <param name="Value"></param>
/// <returns>true if it worked and false if you pass null</returns>
BOOL Poke4(DWORD* LocalMemoryLocation, DWORD Value)
{
	if (LocalMemoryLocation == nullptr)
	{
		return FALSE;
	}
	*LocalMemoryLocation = Value;
	return TRUE;
}

/// <summary>
/// Read a 4 byte (DWORD sized) value from a specific location in the local virtual memory
/// </summary>
/// <param name="MemoryLocation">Non null location  indicating where to read from</param>
/// <returns>If you pass 0 for memory location, returns 0 otherwise returns the contents of the location</returns>
DWORD Peek4(DWORD* LocalMemoryLocation)
{
	if (LocalMemoryLocation == 0)
	{
		return 0;
	}
	else
	{
		return *LocalMemoryLocation;
	}
}


SYSTEM_INFO Wow64_poss;
SYSTEM_INFO Nativ_poss;
/// <summary>
/// thank you IsWow64Process
/// </summary>
typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS Callback_toWOW64 = 0;
HMODULE Kernel32 = 0;


BOOL WINAPI IsTargetProcessID32Bit(DWORD dwProcessID)
{
	BOOL Checkme = FALSE;
	bool is64Os = false;
	static BOOL SysInfoCalled = false;
	static BOOL SysNatCalled = false;
	
	if (dwProcessID == 0)
	{
		return FALSE;
	}
	if (SysInfoCalled == false)
	{
		GetSystemInfo(&Wow64_poss);
		SysInfoCalled = true;
	}

	if (SysNatCalled == false)
	{
		GetNativeSystemInfo(&Nativ_poss);
		SysNatCalled = true;
	}


	if (Kernel32 == 0)
	{
		Kernel32 = LoadLibrary(L"kernel32.dll");
		if (Kernel32 == 0)
		{
			return 0; // propaly not a good idea
		}
		else
		{
			if (Callback_toWOW64 == 0)
			{
				Callback_toWOW64 = (LPFN_ISWOW64PROCESS)GetProcAddress(Kernel32, "IsWow64Process");
			}

			/// <summary>
			///  we should in theory only need to check the natie system since wow is not existing on the current ssytem
			/// </summary>
			if (Callback_toWOW64 == 0)
			{
				switch (Nativ_poss.wProcessorArchitecture)
				{
				case PROCESSOR_ARCHITECTURE_ARM64:
				case PROCESSOR_ARCHITECTURE_AMD64:
				case PROCESSOR_ARCHITECTURE_IA64:
					return FALSE;
				default:
					return TRUE;
				}
			}
		}
	}
	switch (Nativ_poss.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_ARM64:
	case PROCESSOR_ARCHITECTURE_AMD64:
	case PROCESSOR_ARCHITECTURE_IA64:
		is64Os = true;
	}

	if (!is64Os)
	{
		return true;
	}
	else
	{

		HANDLE Check;
		if (dwProcessID != 0)
		{
			Check = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwProcessID);
			if (Check)
			{
				if (Callback_toWOW64(Check, &Checkme))
				{
					if (Checkme == FALSE)
					{
						if (is64Os)
						{
							// CheckMe = false;
						}
						else
						{
							Checkme = TRUE;
						}
					}
					else
					{
						Checkme = TRUE;
					}
				}
				CloseHandle(Check);
			}

		}
	}
	return Checkme;
}