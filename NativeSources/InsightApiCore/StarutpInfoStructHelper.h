#pragma once
#include <windows.h>
#include <vector>
#include <string>
/*
* Encapsulates a startupinfoexW pointer with some memory management
*/
class StartupInfoWrapper
{
public:
	StartupInfoWrapper();

	StartupInfoWrapper(const StartupInfoWrapper& other);
	const wchar_t* lpDesktop();
	void lpDesktop(const wchar_t* Name);

	const wchar_t* lpTitle();
	void lpTitle(const wchar_t* Title);

	DWORD dwX();
	void dwX(DWORD NewValue);

	DWORD dwY();
	void dwY(DWORD NewValue);

	DWORD dwXSize();
	void dwXSize(DWORD NewValue);


	DWORD dwYSize();
	void dwYSize(DWORD NewValue);

	DWORD dwXCountChars();
	void dwXCountChars(DWORD NewValue);

	DWORD dwYCountChars();
	void dwYCountChars(DWORD NewValue);

	DWORD dwFillAttribute();
	void dwFillAttribute(DWORD NewValue);

	DWORD dwFlags();
	void dwFlags(DWORD NewValue);

	WORD wShowWindow();
	void wShowWindow(WORD NewValue);

	HANDLE hStdInput();
	void hStdInput(HANDLE NewValue);

	HANDLE hStdOutput();
	void hStdOutput(HANDLE NewValue);

	HANDLE hStdError();
	void hStdError(HANDLE NewValue);


	const STARTUPINFOEXW* GetPtr();

private:
	
	STARTUPINFOEXW Struct;
	std::wstring DesktopNameContainer;
	std::wstring TileNameContainer;

};