#include "StartupInfoStructHelper.h"

extern "C" {

	StartupInfoWrapper* WINAPI StartupInfo_MakeInstance()
	{
		return new StartupInfoWrapper();
	}


	StartupInfoWrapper* WINAPI StartupInfo_DupInstance(StartupInfoWrapper* other)
	{
		if (other) 
		{
			return new StartupInfoWrapper(*other);
		}
		return nullptr;
	}

	void WINAPI StartupInfo_DeleteInstance(StartupInfoWrapper* that)
	{
		if (that != nullptr)
		{
			delete that;
		}
	}


	const  wchar_t* WINAPI StartupInfo_GetlpDesktopW(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->lpDesktop();
		}
		return nullptr;
	}

	BOOL WINAPI StartupInfo_SetlpDesktopW(StartupInfoWrapper* that, const wchar_t* NewDesktopName)
	{
		if (that)
		{
			that->lpDesktop(NewDesktopName);
			return TRUE;
		}
		return FALSE;
	}


	const wchar_t* WINAPI StartupUpInfo_GetlpTitleW(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->lpTitle();
		}
		return nullptr;
	}

	BOOL WINAPI StartUpInfo_SetlpTitleW(StartupInfoWrapper* that, const wchar_t* NewTitle)
	{
		if (that)
		{
			that->lpTitle(NewTitle);
			return TRUE;
		}
		return FALSE;
	}


	DWORD WINAPI StartupInfo_GetdwX(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwX();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwX(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwX(NewValue);
			return TRUE;
		}
		return FALSE;
	}



	DWORD WINAPI StartupInfo_GetdwY(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwY();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwY(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwY(NewValue);
			return TRUE;
		}
		return FALSE;
	}




	DWORD WINAPI StartupInfo_GetdwXCountChars(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwXCountChars();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwXCountChars(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwXCountChars(NewValue);
			return TRUE;
		}
		return FALSE;
	}


	DWORD WINAPI StartupInfo_GetdwXSize(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwXSize();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwXSize(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwXSize(NewValue);
			return TRUE;
		}
		return FALSE;
	}

	const STARTUPINFOEXW* StartupInfo_GetPointer(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->GetPtr();
		}
		return nullptr;
	}
	DWORD WINAPI StartupInfo_GetdwYSize(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwYSize();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwYSize(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwYSize(NewValue);
			return TRUE;
		}
		return FALSE;
	}


	DWORD WINAPI StartupInfo_GetdwYCountChars(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwYCountChars();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwYCountChars(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwYCountChars(NewValue);
			return TRUE;
		}
		return FALSE;
	}

	DWORD WINAPI StartupInfo_GetdwFillAttributes(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwFillAttribute();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwFillAttribute(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwFillAttribute(NewValue);
			return TRUE;
		}
		return FALSE;
	}





	DWORD WINAPI StartupInfo_GetdwFlags(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->dwFlags();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetdwFlags(StartupInfoWrapper* that, DWORD NewValue)
	{
		if (that)
		{
			that->dwFlags(NewValue);
			return TRUE;
		}
		return FALSE;
	}






	WORD WINAPI StartupInfo_GetwShowWindow(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->wShowWindow();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SetwShowWindow(StartupInfoWrapper* that, WORD NewValue)
	{
		if (that)
		{
			that->wShowWindow(NewValue);
			return TRUE;
		}
		return FALSE;
	}




	HANDLE WINAPI StartupInfo_GethStdInput(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->hStdInput();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SethStdInput(StartupInfoWrapper* that, HANDLE NewValue)
	{
		if (that)
		{
			that->hStdInput(NewValue);
			return TRUE;
		}
		return FALSE;
	}



	HANDLE WINAPI StartupInfo_GethStdOutput(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->hStdOutput();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SethStdOutput(StartupInfoWrapper* that, HANDLE NewValue)
	{
		if (that)
		{
			that->hStdOutput(NewValue);
			return TRUE;
		}
		return FALSE;
	}





	HANDLE WINAPI StartupInfo_GethStdError(StartupInfoWrapper* that)
	{
		if (that)
		{
			return that->hStdError();
		}
		return 0;
	}

	BOOL WINAPI StartupInfo_SethStdError(StartupInfoWrapper* that, HANDLE NewValue)
	{
		if (that)
		{
			that->hStdError(NewValue);
			return TRUE;
		}
		return FALSE;
	}



}