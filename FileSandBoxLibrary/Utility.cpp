/*
*	Utility.cpp   Houses code for a few generic routines.    Currently ProcessHandlingInternal.cpp uses the and they are exported for use.
*/
#include "Utility.h"
#include <Psapi.h>


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

BOOL Poke4(DWORD* LocalMemoryLocation, DWORD Value)
{
	if (LocalMemoryLocation == nullptr)
	{
		return FALSE;
	}
	*LocalMemoryLocation = Value;
	return TRUE;
}


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