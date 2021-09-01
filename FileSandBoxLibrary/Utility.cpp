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


wchar_t* GetModuleNameViaHandle(HANDLE Process, HMODULE Module)
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


wchar_t* GetFileNameViaHandle(HANDLE FileHandle)
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
