#include <windows.h>
#include <string>

LPWSTR ConvertAnsiToUnicode(LPSTR Str)
{
	if (Str == 0)
	{
		return nullptr;
	}
	else
	{
		DWORD SizeNeeded = MultiByteToWideChar(CP_ACP, 0, Str, -1, 0, 0);
		if (SizeNeeded != 0)
		{
			LPWSTR ret = (LPWSTR)malloc(sizeof(wchar_t) * (SizeNeeded + 1));
			if (ret)
			{
				SizeNeeded = MultiByteToWideChar(CP_ACP, 0, Str, -1, ret, SizeNeeded);
				ret[SizeNeeded] = 0;
				return ret;
			}
		}
		return nullptr;
	}
}

LPSTR ConvertUnicodeToAnsi(LPWSTR Str)
{
	if (Str == 0)
	{
		return nullptr;
	}
	else
	{
		DWORD SizeNeeded = WideCharToMultiByte(CP_ACP, 0, Str, -1, 0, 0, 0, 0);
		if (SizeNeeded != 0)
		{
			LPSTR ret = (LPSTR)malloc(sizeof(char) * (SizeNeeded + 1));
			if (ret)
			{
				SizeNeeded = WideCharToMultiByte(CP_ACP, 0, Str, -1, ret, -1, 0, 0);
				ret[SizeNeeded] = 0;
				return ret;
			}
		}
		return nullptr;
	}
}

void ConvertAnsiToUnicode(LPSTR Str, std::wstring& Output)
{
	LPWSTR tmp = nullptr;
	__try
	{
		 tmp = ConvertAnsiToUnicode(Str);
		 Output.assign(tmp);
	}
	__finally
	{
		if (tmp != nullptr)
		{
			free(tmp);
		}
	}
}

void ConvertUnicodeToAnsi(LPWSTR Str, std::string& Output)
{
	LPSTR tmp =nullptr;
	__try
	{
		tmp = ConvertUnicodeToAnsi(Str);
		Output.assign(tmp);
	}
	__finally
	{
		if (tmp != nullptr)
		{
			free(tmp);
		}
	}
}
