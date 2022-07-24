#pragma once
#include <windows.h>
#include <vector>
#include <string>

#include <gsl>



/*
* Encapsulates a startupinfoexW pointer with some memory management
*/
class StartupInfoWrapper
{
public:
	StartupInfoWrapper() noexcept;
	StartupInfoWrapper(const StartupInfoWrapper& other) noexcept;
	~StartupInfoWrapper();

	const wchar_t* lpDesktop() noexcept;
	void lpDesktop(const wchar_t* Name) noexcept;

	const wchar_t* lpTitle() noexcept;
	void lpTitle(const wchar_t* Title) noexcept;

	DWORD dwX() noexcept;
	void dwX(DWORD NewValue) noexcept;

	DWORD dwY() noexcept;
	void dwY(DWORD NewValue) noexcept;

	DWORD dwXSize() noexcept;
	void dwXSize(DWORD NewValue) noexcept;


	DWORD dwYSize() noexcept;
	void dwYSize(DWORD NewValue) noexcept;

	DWORD dwXCountChars() noexcept;
	void dwXCountChars(DWORD NewValue) noexcept;

	DWORD dwYCountChars() noexcept;
	void dwYCountChars(DWORD NewValue) noexcept;

	DWORD dwFillAttribute() noexcept;
	void dwFillAttribute(DWORD NewValue) noexcept;

	DWORD dwFlags() noexcept;
	void dwFlags(DWORD NewValue) noexcept;

	WORD wShowWindow() noexcept;
	void wShowWindow(WORD NewValue) noexcept;

	HANDLE hStdInput() noexcept;
	void hStdInput(HANDLE NewValue) noexcept;

	HANDLE hStdOutput() noexcept;
	void hStdOutput(HANDLE NewValue) noexcept;

	HANDLE hStdError() noexcept;
	void hStdError(HANDLE NewValue) noexcept;


	const STARTUPINFOEXW* GetPtr() noexcept;

private:
	
	STARTUPINFOEXW Struct;

	
	//std::wstring DesktopNameContainer;
	//std::wstring TileNameContainer;

};