#include "StartupInfoStructHelper.h"


StartupInfoWrapper::StartupInfoWrapper() noexcept
{
	ZeroMemory(&Struct, sizeof(STARTUPINFOEXW));
	

	Struct.StartupInfo.cb = sizeof(STARTUPINFOEXW);
}

StartupInfoWrapper::StartupInfoWrapper(const StartupInfoWrapper& other) noexcept
{
	CopyMemory(&Struct, &other.Struct, sizeof(STARTUPINFOEXW));
	if (other.Struct.StartupInfo.lpTitle != nullptr)
	{
		Struct.StartupInfo.lpTitle = _wcsdup(other.Struct.StartupInfo.lpTitle);
	}
	if (other.Struct.StartupInfo.lpDesktop != nullptr)
	{
		Struct.StartupInfo.lpDesktop = _wcsdup(other.Struct.StartupInfo.lpDesktop);
	}

	/*
	
	if (other.Struct.StartupInfo.lpDesktop != nullptr)
	{
		this->DesktopNameContainer = this->Struct.StartupInfo.lpDesktop;
		this->Struct.StartupInfo.lpDesktop = const_cast<LPWSTR>(this->DesktopNameContainer.c_str());
	}

	if (other.Struct.StartupInfo.lpTitle != nullptr)
	{
		this->DesktopNameContainer = this->Struct.StartupInfo.lpTitle;
		this->Struct.StartupInfo.lpTitle = const_cast<LPWSTR>(this->TileNameContainer.c_str());
	}
	*/
}

StartupInfoWrapper::~StartupInfoWrapper()
{
	if (Struct.StartupInfo.lpDesktop != nullptr)
	{
		free(Struct.StartupInfo.lpDesktop);
	}
	if (Struct.StartupInfo.lpTitle != nullptr)
	{
		free(Struct.StartupInfo.lpTitle);
	}

	ZeroMemory(&Struct, sizeof(STARTUPINFOEXW));

}

StartupInfoWrapper& StartupInfoWrapper::operator=(const StartupInfoWrapper& CopyAssign)
{
	// TODO: insert return statement here
	CopyMemory(&this->Struct, &CopyAssign.Struct, sizeof(CopyAssign.Struct));
	
	if (this->Struct.StartupInfo.lpDesktop != 0)
	{
		this->Struct.StartupInfo.lpDesktop = _wcsdup(this->Struct.StartupInfo.lpDesktop);
	}
	if (this->Struct.StartupInfo.lpTitle)
	{
		this->Struct.StartupInfo.lpTitle = _wcsdup(this->Struct.StartupInfo.lpTitle);
	}
	*this;
}

StartupInfoWrapper& StartupInfoWrapper::operator=(StartupInfoWrapper&& MoveAssign)
{
	if (this != &MoveAssign)
	{
		CopyMemory(&this->Struct, &MoveAssign.Struct, sizeof(MoveAssign.Struct));
		ZeroMemory(&MoveAssign.Struct, sizeof(MoveAssign.Struct));;
	}
	return *this;
}

const wchar_t* StartupInfoWrapper::lpDesktop() noexcept
{
	return Struct.StartupInfo.lpDesktop;
	/*
	if (Struct.StartupInfo.lpDesktop != nullptr)
	{
		this->DesktopNameContainer = Struct.StartupInfo.lpDesktop;
		return this->DesktopNameContainer.c_str();
	}
	else
	{
		return nullptr;
	}
	*/
}

void StartupInfoWrapper::lpDesktop(const wchar_t* Name) noexcept
{

	if (Struct.StartupInfo.lpDesktop != nullptr)
	{
#pragma warning(once: 26481)
		delete Struct.StartupInfo.lpDesktop;
	}

	if (Name != nullptr)
	{
		size_t count = wcslen(Name);
		Struct.StartupInfo.lpDesktop = new(std::nothrow) wchar_t[count+1];
		if (Struct.StartupInfo.lpDesktop)
		{
			CopyMemory(Struct.StartupInfo.lpTitle, Name, count * sizeof(wchar_t));
			Struct.StartupInfo.lpDesktop[count] = 0;
		}
	}
	else
	{
		Struct.StartupInfo.lpDesktop = nullptr;
	}
	/*
	if (Name != nullptr)
	{
		this->DesktopNameContainer = Name;
		this->Struct.StartupInfo.lpDesktop = const_cast<LPWSTR>(this->DesktopNameContainer.c_str());
	}
	else
	{
		this->DesktopNameContainer.clear();
		this->Struct.StartupInfo.lpDesktop = nullptr;
	}*/
}

const wchar_t* StartupInfoWrapper::lpTitle()  noexcept
{
	return Struct.StartupInfo.lpTitle;
	/*
	if (Struct.StartupInfo.lpTitle != nullptr)
	{
		this->TileNameContainer = Struct.StartupInfo.lpTitle;
		return this->TileNameContainer.c_str();
	}
	else
	{
		return nullptr;
	}*/
}

void StartupInfoWrapper::lpTitle(const wchar_t* Title) noexcept
{
	if (Struct.StartupInfo.lpTitle != nullptr)
	{
		free(Struct.StartupInfo.lpTitle);
	}
	if (Title != nullptr)
	{
		Struct.StartupInfo.lpTitle = _wcsdup(Title);
	}
	else
	{
		Struct.StartupInfo.lpTitle = nullptr;
	}
	/*
	if (Title != nullptr)
	{
		this->TileNameContainer = Title;
		this->Struct.StartupInfo.lpTitle = const_cast<LPWSTR>(this->TileNameContainer.c_str());
	}
	else
	{
		this->TileNameContainer.clear();
		this->Struct.StartupInfo.lpTitle = nullptr;
	}*/
}

DWORD StartupInfoWrapper::dwX() noexcept
{
	return Struct.StartupInfo.dwX;
}

void StartupInfoWrapper::dwX(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwX = NewValue;
}

DWORD StartupInfoWrapper::dwY() noexcept
{
	return Struct.StartupInfo.dwY;
}

void StartupInfoWrapper::dwY(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwY = NewValue;
}

DWORD StartupInfoWrapper::dwXSize() noexcept
{
	return Struct.StartupInfo.dwXSize;
}

void StartupInfoWrapper::dwXSize(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwXSize = NewValue;
}

DWORD StartupInfoWrapper::dwYSize() noexcept
{
	return Struct.StartupInfo.dwYSize;
}

void StartupInfoWrapper::dwYSize(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwYSize = NewValue;
}

DWORD StartupInfoWrapper::dwXCountChars() noexcept
{
	return Struct.StartupInfo.dwXCountChars;
}

void StartupInfoWrapper::dwXCountChars(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwXCountChars = NewValue;
}

DWORD StartupInfoWrapper::dwYCountChars() noexcept
{
	return Struct.StartupInfo.dwYCountChars;
}

void StartupInfoWrapper::dwYCountChars(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwYCountChars = NewValue;
}


DWORD StartupInfoWrapper::dwFillAttribute() noexcept
{
	return Struct.StartupInfo.dwFillAttribute;
}

void StartupInfoWrapper::dwFillAttribute(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwFillAttribute = NewValue;
}

DWORD StartupInfoWrapper::dwFlags() noexcept
{
	return Struct.StartupInfo.dwFlags;
}

void StartupInfoWrapper::dwFlags(DWORD NewValue) noexcept
{
	Struct.StartupInfo.dwFlags = NewValue;
}

WORD StartupInfoWrapper::wShowWindow() noexcept
{
	return Struct.StartupInfo.wShowWindow;
}

void StartupInfoWrapper::wShowWindow(WORD NewValue) noexcept
{
	Struct.StartupInfo.wShowWindow = NewValue;
}

HANDLE StartupInfoWrapper::hStdInput() noexcept
{
	return Struct.StartupInfo.hStdInput;
}

void StartupInfoWrapper::hStdInput(HANDLE NewValue) noexcept
{
	Struct.StartupInfo.hStdInput = NewValue;
}

HANDLE StartupInfoWrapper::hStdOutput() noexcept
{
	return Struct.StartupInfo.hStdOutput;
}

void StartupInfoWrapper::hStdOutput(HANDLE NewValue) noexcept
{
	Struct.StartupInfo.hStdOutput = NewValue;
}

HANDLE StartupInfoWrapper::hStdError() noexcept
{
	return Struct.StartupInfo.hStdError;
}

void StartupInfoWrapper::hStdError(HANDLE NewValue) noexcept
{
	Struct.StartupInfo.hStdError = NewValue;
}

const STARTUPINFOEXW* StartupInfoWrapper::GetPtr() noexcept
{
	return &Struct;
}
