#include "StartupInfoStructHelper.h"


StartupInfoWrapper::StartupInfoWrapper()
{
	ZeroMemory(&Struct, sizeof(Struct));
	

	Struct.StartupInfo.cb = sizeof(STARTUPINFOEXW);
}

StartupInfoWrapper::StartupInfoWrapper(const StartupInfoWrapper& other)
{
	CopyMemory(&this->Struct.StartupInfo, &other.Struct.StartupInfo, sizeof(other.Struct));
	if (other.Struct.StartupInfo.lpDesktop != nullptr)
	{
		this->DesktopNameContainer = this->Struct.StartupInfo.lpDesktop;
		this->Struct.StartupInfo.lpDesktop = (LPWSTR)this->DesktopNameContainer.c_str();
	}

	if (other.Struct.StartupInfo.lpTitle != nullptr)
	{
		this->DesktopNameContainer = this->Struct.StartupInfo.lpTitle;
		this->Struct.StartupInfo.lpTitle = (LPWSTR)this->TileNameContainer.c_str();
	}

}

const wchar_t* StartupInfoWrapper::lpDesktop()
{
	if (Struct.StartupInfo.lpDesktop != nullptr)
	{
		this->DesktopNameContainer = Struct.StartupInfo.lpDesktop;
		return this->DesktopNameContainer.c_str();
	}
	else
	{
		return nullptr;
	}
	
}

void StartupInfoWrapper::lpDesktop(const wchar_t* Name)
{
	this->DesktopNameContainer = Name;
	this->Struct.StartupInfo.lpDesktop = (LPWSTR)this->DesktopNameContainer.c_str();
}

const wchar_t* StartupInfoWrapper::lpTitle()
{
	if (Struct.StartupInfo.lpTitle != nullptr)
	{
		this->TileNameContainer = Struct.StartupInfo.lpTitle;
		return this->TileNameContainer.c_str();
	}
	else
	{
		return nullptr;
	}
}

void StartupInfoWrapper::lpTitle(const wchar_t* Title)
{
	this->TileNameContainer = Title;
	this->Struct.StartupInfo.lpTitle = (LPWSTR)this->TileNameContainer.c_str();
}

DWORD StartupInfoWrapper::dwX()
{
	return this->Struct.StartupInfo.dwX;
}

void StartupInfoWrapper::dwX(DWORD NewValue)
{
	this->Struct.StartupInfo.dwX = NewValue;
}

DWORD StartupInfoWrapper::dwY()
{
	return this->Struct.StartupInfo.dwY;
}

void StartupInfoWrapper::dwY(DWORD NewValue)
{
	this->Struct.StartupInfo.dwY = NewValue;
}

DWORD StartupInfoWrapper::dwXSize()
{
	return this->Struct.StartupInfo.dwXSize;
}

void StartupInfoWrapper::dwXSize(DWORD NewValue)
{
	this->Struct.StartupInfo.dwXSize = NewValue;
}

DWORD StartupInfoWrapper::dwYSize()
{
	return this->Struct.StartupInfo.dwYSize;
}

void StartupInfoWrapper::dwYSize(DWORD NewValue)
{
	this->Struct.StartupInfo.dwYSize = NewValue;
}

DWORD StartupInfoWrapper::dwXCountChars()
{
	return this->Struct.StartupInfo.dwXCountChars;
}

void StartupInfoWrapper::dwXCountChars(DWORD NewValue)
{
	this->Struct.StartupInfo.dwXCountChars = NewValue;
}

DWORD StartupInfoWrapper::dwYCountChars()
{
	return this->Struct.StartupInfo.dwYCountChars;
}

void StartupInfoWrapper::dwYCountChars(DWORD NewValue)
{
	this->Struct.StartupInfo.dwYCountChars;
}

DWORD StartupInfoWrapper::dwFillAttribute()
{
	return this->Struct.StartupInfo.dwFillAttribute;
}

void StartupInfoWrapper::dwFillAttribute(DWORD NewValue)
{
	this->Struct.StartupInfo.dwFillAttribute = NewValue;
}

DWORD StartupInfoWrapper::dwFlags()
{
	return this->Struct.StartupInfo.dwFlags;
}

void StartupInfoWrapper::dwFlags(DWORD NewValue)
{
	this->Struct.StartupInfo.dwFlags = NewValue;
}

WORD StartupInfoWrapper::wShowWindow()
{
	return this->Struct.StartupInfo.wShowWindow;
}

void StartupInfoWrapper::wShowWindow(WORD NewValue)
{
	this->Struct.StartupInfo.wShowWindow = NewValue;
}

HANDLE StartupInfoWrapper::hStdInput()
{
	return this->Struct.StartupInfo.hStdInput;
}

void StartupInfoWrapper::hStdInput(HANDLE NewValue)
{
	this->Struct.StartupInfo.hStdInput = NewValue;
}

HANDLE StartupInfoWrapper::hStdOutput()
{
	return this->Struct.StartupInfo.hStdOutput;
}

void StartupInfoWrapper::hStdOutput(HANDLE NewValue)
{
	this->Struct.StartupInfo.hStdOutput = NewValue;
}

HANDLE StartupInfoWrapper::hStdError()
{
	return this->Struct.StartupInfo.hStdError;
}

void StartupInfoWrapper::hStdError(HANDLE NewValue)
{
	this->Struct.StartupInfo.hStdError = NewValue;
}

const STARTUPINFOEXW* StartupInfoWrapper::GetPtr()
{
	return &this->Struct;
}
