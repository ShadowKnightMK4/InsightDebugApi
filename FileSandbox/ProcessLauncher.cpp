#include "ProcessLauncher.h"
#include <string.h>
#include "RemoteStructure.h"
/// <summary>
/// Try to add the passed priv to this.
/// </summary>
/// <param name="SourceToken"></param>
/// <param name="PRIV"></param>
/// <returns></returns>
BOOL AddTokenPriv(HANDLE Source, const wchar_t* PRIV)
{
	bool Result = false;
	HANDLE Self = INVALID_HANDLE_VALUE;
	LUID  Priv;
	TOKEN_PRIVILEGES NewPriv;
	
	if (Source == INVALID_HANDLE_VALUE)
	{
		if (!OpenProcessToken(GetCurrentProcess(), PROCESS_ALL_ACCESS, &Self))
		{
			return FALSE;
		}
	}
	else
	{
		Self = Source;
	}

	{
		if (LookupPrivilegeValueW(nullptr, PRIV, &Priv))
		{
			NewPriv.PrivilegeCount = 1;
			NewPriv.Privileges[0].Luid = Priv;
			NewPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			
			if (!AdjustTokenPrivileges(Self, false, &NewPriv, sizeof(NewPriv), nullptr, nullptr))
			{
				Result = false;
			}
			else
			{
				Result = true;
			}
		}
		if (Source == INVALID_HANDLE_VALUE)
			CloseHandle(Self);
	}
	return false;
	
}

#pragma region Constructors

/// <summary>
/// Default constructor.
/// </summary>
ProcessLauncher::ProcessLauncher()
{
	SelfZero();
}

/// <summary>
/// Coyp Constructor. Will throw exception if Handles are valid and can't be Duplicated()
/// </summary>
/// <param name="Other"></param>
ProcessLauncher::ProcessLauncher(ProcessLauncher& Other)
{
	SelfZero();
	if (Other.Arguments != nullptr)
	{
		this->Arguments = _wcsdup(Other.Arguments);
	}

	if (Other.DetoursDll != nullptr)
	{
		this->DetoursDll = _strdup(Other.DetoursDll);
	}

	if (Other.Name != nullptr)
	{
		this->Name = _wcsdup(Other.Name);
	}

	if (Other.Password != nullptr)
	{
		this->Password = _wcsdup(Other.Password);
	}

	if ((Other.ProcessInfo.hProcess != INVALID_HANDLE_VALUE) && (Other.ProcessInfo.hProcess != 0))
	{
		if (DuplicateHandle(GetCurrentProcess(), Other.ProcessInfo.hProcess, GetCurrentProcess(), &this->ProcessInfo.hProcess, 0, false, DUPLICATE_SAME_ACCESS) == false)
		{
			throw;
		}
	}

	if ((Other.ProcessInfo.hThread != INVALID_HANDLE_VALUE) && (Other.ProcessInfo.hThread != 0))
	{
		if (DuplicateHandle(GetCurrentProcess(), Other.ProcessInfo.hThread, GetCurrentProcess(), &this->ProcessInfo.hThread, 0, false, DUPLICATE_SAME_ACCESS) == false)
		{
			throw;
		}
	}

	if ((Other.StartInfo.dwFlags & STARTF_USESTDHANDLES) == (STARTF_USESTDHANDLES))
	{
		if (Other.StartInfo.hStdError != INVALID_HANDLE_VALUE)
		{
			if (DuplicateHandle(GetCurrentProcess(), Other.StartInfo.hStdError, GetCurrentProcess(), &this->StartInfo.hStdError, 0, false, DUPLICATE_SAME_ACCESS) == false)
			{
				throw;
			}
		}
		if (Other.StartInfo.hStdInput != INVALID_HANDLE_VALUE)
		{
			if (DuplicateHandle(GetCurrentProcess(), Other.StartInfo.hStdInput, GetCurrentProcess(), &this->StartInfo.hStdInput, 0, false, DUPLICATE_SAME_ACCESS) == false)
			{
				throw;
			}
		}
		if (Other.StartInfo.hStdOutput != INVALID_HANDLE_VALUE)
		{
			if (DuplicateHandle(GetCurrentProcess(), Other.StartInfo.hStdOutput, GetCurrentProcess(), &this->StartInfo.hStdOutput, 0, false, DUPLICATE_SAME_ACCESS) == false)
			{
				throw;
			}
		}
	}
	


}

ProcessLauncher::~ProcessLauncher()
{
	if (this->Arguments != nullptr)
	{
		free(this->Arguments);
	}
	if (this->DetoursDll != nullptr)
	{
		free(this->DetoursDll);
	}

	if ((this->DomainName != nullptr))
	{
		free(this->DomainName);
	}
	if (this->Name != nullptr)
	{
		free(this->Name);
	}

	if (this->Password != nullptr)
	{
		RtlSecureZeroMemory(this->Password, wcslen(this->Password) * sizeof(wchar_t));
		free(this->Password);
	}
	if (this->UserName != nullptr)
	{
		free(this->UserName);
	}
	if ( (this->UserToken != INVALID_HANDLE_VALUE) && (this->UserName != 0))
	{
		CloseHandle(this->UserToken);
	}

	if ( (this->ProcessInfo.hProcess != INVALID_HANDLE_VALUE) )
	{
		CloseHandle(this->ProcessInfo.hProcess);
	}
	if (this->ProcessInfo.hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->ProcessInfo.hThread);
	}

	if ((this->StartInfo.dwFlags & STARTF_USESTDHANDLES) == (STARTF_USESTDHANDLES))
	{
		if (this->StartInfo.hStdError != INVALID_HANDLE_VALUE)
		{
			CloseHandle(this->StartInfo.hStdError);
		}
		if (this->StartInfo.hStdInput != INVALID_HANDLE_VALUE)
		{
			CloseHandle(this->StartInfo.hStdInput);
		}
		if (this->StartInfo.hStdOutput != INVALID_HANDLE_VALUE)
		{
			CloseHandle(this->StartInfo.hStdOutput);
		}
	}
}

#pragma endregion


#pragma region CreateProcessSettings

#pragma region SpecifyProcessTarget
/// <summary>
/// Set the Process to Launch's Name.
/// </summary>
/// <param name="name">name to use. Null means first name on the Application Argument list</param>
/// <returns>true if the duplicate went ok.</returns>
bool ProcessLauncher::SetApplicationName(const wchar_t* name)
{
	return CommonSettingRoutine(name, &this->Name, false);
}
/// <summary>
/// Set the name of the Process to Lauch
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
bool ProcessLauncher::SetApplicationName(const char* name)
{
	return CommonSettingRoutine(name, &this->Name, false);
}

/// <summary>
/// If Target is nullptr, returns bytes needed to contain the ApplicationName. If Target is not null, overwrites the *target with a duplicate of the private var
/// </summary>
/// <param name="Target"></param>
/// <returns></returns>
DWORD ProcessLauncher::GetApplicationName(wchar_t** Target)
{
	return CommonStringGetRoutine(this->Name, Target);
}

#ifdef _PROCESS_LAUNCHER_DIRECT_ACCESS_
/// <summary>
/// Returns the direct pointer
/// </summary>
/// <returns></returns>
const wchar_t* ProcessLauncher::GetApplicationName()
{
	return this->Name;
}
#endif
#pragma endregion

#pragma region Specify Application Arguments
/// <summary>
/// Set the Arguments to the process.
/// </summary>
/// <param name="Arguments">arguments for the command</param>
/// <returns></returns>
bool ProcessLauncher::SetApplicationArguments(const wchar_t* Arguments)
{
	if (Arguments == nullptr)
	{
		return CommonSettingRoutine(L"    ", &this->Arguments, false);
	}
	else
	{
		return CommonSettingRoutine(Arguments, &this->Arguments, false);
	}
}
/// <summary>
/// Set the Arguments to the process. Converts passed string to unicode
/// </summary>
/// <param name="Arguments">arguments for the command</param>
/// <returns></returns>
bool ProcessLauncher::SetApplicationArguments(const char* Arguments)
{
	return CommonSettingRoutine(Arguments, &this->Arguments, false);
}

DWORD ProcessLauncher::GetApplicationArguments(wchar_t** Arguments)
{
	return CommonStringGetRoutine(this->Arguments, Arguments);
}

const wchar_t* ProcessLauncher::GetApplicationArguments()
{
	return this->Arguments;
}

#pragma endregion


#pragma region Set the Starting Directory

/// <summary>
/// Set the starting directory
/// </summary>
/// <param name="TargetDir"></param>
/// <returns></returns>
bool ProcessLauncher::SetApplicationWorkingDirectory(const wchar_t* TargetDir)
{
	return CommonSettingRoutine(TargetDir, &this->WorkingDir, false);
}
bool ProcessLauncher::SetApplicationWorkingDirectory(const char* TargetDir)
{
	return CommonSettingRoutine(TargetDir, &this->WorkingDir, false);
}
DWORD ProcessLauncher::GetApplicationWorkingDirectory( wchar_t** TargetDir)
{
	return CommonStringGetRoutine(this->WorkingDir, TargetDir);
}
const wchar_t* ProcessLauncher::GetApplicationWorkingDirectory()
{
	return nullptr;
}
#pragma endregion



/// <summary>
/// Set the creation flags
/// </summary>
/// <param name="Flags"></param>
void ProcessLauncher::SetCreationFlags(DWORD Flags)
{
	this->CreationFlags = Flags;
}

/// <summary>
/// Get the creation flags
/// </summary>
/// <returns></returns>
DWORD ProcessLauncher::GetCreationFlags()
{
	return this->CreationFlags;
}


#pragma endregion


#pragma region UserLogonInfo
/// <summary>
/// Set the username to launch the app as
/// </summary>
/// <param name="Name"></param>
/// <returns></returns>
bool ProcessLauncher::SetUserName(const wchar_t* Name)
{
	return CommonSettingRoutine(Name, &this->UserName, false);
}

DWORD ProcessLauncher::GetUserName(const wchar_t** Name)
{
	return 0;
}

const wchar_t* ProcessLauncher::GetUserName()
{
	return nullptr;
}

/// <summary>
/// set the pasword for  the user name
/// </summary>
/// <param name="Password"></param>
/// <returns></returns>
bool ProcessLauncher::SetPassword(const wchar_t* Password)
{
	return CommonSettingRoutine(Password, &this->Password, false);
}
/// <summary>
/// Specify the Domain name to login under (ignored if username is null) ('.' or null means local pc)
/// </summary>
/// <param name="DomainName"></param>
/// <returns></returns>
bool ProcessLauncher::SetUserDomain(const wchar_t* DomainName)
{
	return CommonSettingRoutine(DomainName, &this->DomainName, false);
}
DWORD ProcessLauncher::GetUserDomain(const wchar_t** Domain)
{
	return 0;
}
const wchar_t* ProcessLauncher::GetUserDomain()
{
	return nullptr;
}
/// <summary>
/// Specify the Desktop to use. pass nullptr to left the default rule.
/// </summary>
/// <param name="Desktop"></param>
/// <returns></returns>
bool ProcessLauncher::SetDesktop(const wchar_t* Desktop)
{
	if (Desktop == nullptr)
	{
		if (StartInfo.lpDesktop != nullptr)
		{
			free(StartInfo.lpDesktop);
		}
		StartInfo.lpDesktop = nullptr;
		return true;
	}
	else
	{
		if (Desktop != nullptr)
		{
			StartInfo.lpDesktop = _wcsdup(Desktop);
		}
		else
		{
			StartInfo.lpDesktop = nullptr;
		}
		return true;
	}
}
constexpr auto DEFAULT_DESKTOP = (0);
constexpr auto INTERACTIVE_DESKTOP = (1);


/// <summary>
/// Set the Desktop to a predefined one. DEFAULT_DESKOP (0) means nullptr.  INTERACTIVE_DESKTOP (1) means winsta0\default
/// 
/// </summary>
/// <param name="DesktopEnum"></param>
/// <returns>true if the call worked. False if invalid arg was passed.</returns>
bool ProcessLauncher::SetDesktop(const DWORD DesktopEnum)
{
	switch (DesktopEnum)
	{
	case DEFAULT_DESKTOP:
		return SetDesktop((const wchar_t*)nullptr);
	case INTERACTIVE_DESKTOP:
		return SetDesktop(L"winsta0\\default");
	default:
		return false;
}
}
bool ProcessLauncher::SetLogOnType(const DWORD Type)
{
	switch (Type)
	{
	case LOGON32_LOGON_BATCH:
	case LOGON32_LOGON_INTERACTIVE:
	case LOGON32_LOGON_NETWORK:
	case LOGON32_LOGON_NETWORK_CLEARTEXT:
	case LOGON32_LOGON_NEW_CREDENTIALS:
		this->LogOnType = Type;
		return true;
	default:
		return false;
	}
}
bool ProcessLauncher::SetLogOnProvider(const DWORD Provider)
{
	switch (Provider)
	{
	case LOGON32_PROVIDER_DEFAULT:
	case LOGON32_PROVIDER_WINNT50:
	case LOGON32_PROVIDER_WINNT40:
		this->LogOnProvider = Provider;
		return true;
	default:
		return false;
	}
	return false;
}
#pragma endregion

#pragma region PrivateRoutines
/// <summary>
/// Ripped from Detours. This instead is modified to user CreateProcessAsUser
/// </summary>
/// <param name="UserToken"></param>
/// <param name="lpApplicationName"></param>
/// <param name="lpCommandLine"></param>
/// <param name="lpProcessAttributes"></param>
/// <param name="lpThreadAttributes"></param>
/// <param name="bInheritHandles"></param>
/// <param name="dwCreationFlags"></param>
/// <param name="lpEnvironment"></param>
/// <param name="lpCurrentDirectory"></param>
/// <param name="lpStartupInfo"></param>
/// <param name="lpProcessInformation"></param>
/// <param name="lpDllName"></param>
/// <param name="pfCreateProcessW"></param>
/// <returns></returns>
 BOOL WINAPI ProcessLauncher::DetourCreateProcessWithDllAsUserW(
	_In_ HANDLE UserToken,
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation,
	_In_ LPCSTR lpDllName)
{
	DWORD dwMyCreationFlags = (dwCreationFlags | CREATE_SUSPENDED);
	PROCESS_INFORMATION pi;



	BOOL fResult = CreateProcessAsUserW(UserToken,
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwMyCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		&pi);

	if (lpProcessInformation) {
		CopyMemory(lpProcessInformation, &pi, sizeof(pi));
	}

	if (!fResult) {
		return FALSE;
	}

	LPCSTR rlpDlls[2];
	DWORD nDlls = 0;
	if (lpDllName != NULL) {
		rlpDlls[nDlls++] = lpDllName;
	}

	if (!DetourUpdateProcessWithDll(pi.hProcess, rlpDlls, nDlls)) {
		TerminateProcess(pi.hProcess, ~0u);
		return FALSE;
	}

	if (!(dwCreationFlags & CREATE_SUSPENDED)) {
		ResumeThread(pi.hThread);
	}
	return TRUE;
}

/// <summary>
/// Zeroes Everything in the class and sets Handles to INVALID_HANDLE_VALUE;
/// </summary>
void ProcessLauncher::SelfZero()
{
	memset(this, 0, sizeof(ProcessLauncher));
	ProcessInfo.hProcess = ProcessInfo.hThread = INVALID_HANDLE_VALUE;
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.hStdError = StartInfo.hStdInput = StartInfo.hStdOutput = INVALID_HANDLE_VALUE;


}


/// <summary>
/// Copy routine for the public wchar_t* string routines.
/// </summary>
/// <param name="outside">string to copy from. May be null</param>
/// <param name="Inside">Pointer to a wchar_t* to receive a dupplicate of the first string(or null) </param>
/// <param name="ClearBuffer">for the password, clears any old buffer with RtlSecureZeroMemory() first.</param>
/// <returns>true if it worked, and false if it did not</returns>
bool ProcessLauncher::CommonSettingRoutine(const wchar_t* outside, wchar_t** Inside, bool ClearBuffer)
{
	SIZE_T RequiredBufferSizeInChar;
	SIZE_T RequiredBufferSizeInBytes = 0;
	if (*Inside != nullptr)
	{
		if (ClearBuffer)
		{
			// note: does this clear in chars or bytes?
			RtlSecureZeroMemory(*Inside, wcslen(*Inside) * sizeof(wchar_t));
		}
		free(*Inside);
		*Inside = nullptr;
	}

	if (outside == nullptr)
	{
		RequiredBufferSizeInChar = RequiredBufferSizeInBytes = 0;
	}
	else
	{
		RequiredBufferSizeInChar = wcslen(outside);
		RequiredBufferSizeInBytes = RequiredBufferSizeInChar * sizeof(wchar_t);
	}

	if (RequiredBufferSizeInBytes != 0)
	{
		*Inside = _wcsdup(outside);
	}
	else
	{
		*Inside = nullptr;
	}

	return true;
}
/// <summary>
/// Copy routine for the public char* routines. Converts the copy to unicode.
/// </summary>
/// <param name="outside">string to copy from. May be null</param>
/// <param name="Inside">Pointer to a wchar_t* to receive a dupplicate of the first string(or null) </param>
/// <param name="ClearBuffer">for the password, clears any old buffer with RtlSecureZeroMemory() first.</param>
/// <returns>true if it worked, and false if it did not</returns>

bool ProcessLauncher::CommonSettingRoutine(const char* outside, wchar_t** Inside, bool ClearBuffer)
{
	return false;
}


#pragma endregion



#pragma region Detours


bool ProcessLauncher::SetDetourDll(const wchar_t* DetourDll)
{
	return false;
}

/// <summary>
/// Set the DLL to load Via Detours
/// </summary>
/// <param name="DetourDll"></param>
/// <returns></returns>
bool ProcessLauncher::SetDetourDll(const char* DetourDll)
{
	if (this->DetoursDll != nullptr)
	{
		free(this->DetoursDll);
	}
	if (DetourDll == nullptr)
	{
		this->DetoursDll = nullptr;
	}
	else
	{
		DetoursDll = _strdup(DetourDll);
	}
	return true;
}

DWORD ProcessLauncher::GetDetoursDll(const wchar_t** DetoursDll)
{
	return 0;
}

const wchar_t* ProcessLauncher::GetDetoursDll()
{
	return nullptr;
}

#pragma endregion



BOOL ProcessLauncher::SetInheritHandle(BOOL bInherit)
{
	BOOL ret = this->bInheritHandle;
	this->bInheritHandle = bInherit;
	return ret;
}


/// <summary>
/// Specify stdin, stdout, stderr. Sets the StartupInfo to inherit handles. 
/// </summary>
/// <param name="Input">Use INVALID_HANDLE_VALUE (or 0) for the Default value</param>
/// <param name="Output">Use INVALID_HANDLE_VALUE (or 0) for Default</param>
/// <param name="Error">Use INVALID_HANDLE_VALUE (or 0) for Defautl</param>
/// <returns></returns>
BOOL ProcessLauncher::SetStandardHandles(HANDLE Input, HANDLE Output, HANDLE Error)
{
	bool inherit = false;
	if ((this->StartInfo.dwFillAttribute & STARTF_USESTDHANDLES) == (STARTF_USESTDHANDLES))
	{
		if ((this->StartInfo.hStdInput != INVALID_HANDLE_VALUE))
		{
			CloseHandle(this->StartInfo.hStdInput);
		}
		if ((this->StartInfo.hStdOutput != INVALID_HANDLE_VALUE))
		{
			CloseHandle(this->StartInfo.hStdOutput);
		}
		if ((this->StartInfo.hStdError != INVALID_HANDLE_VALUE))
		{
			CloseHandle(this->StartInfo.hStdError);
		}
	}
	else
	{ 
		this->StartInfo.dwFlags |= STARTF_USESTDHANDLES;
	}

	
	if ((Input != INVALID_HANDLE_VALUE) && (Input != 0))
	{
		inherit = true;
		this->StartInfo.hStdInput = Input;
	}
	else
	{
		this->StartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	}

	if ((Output != INVALID_HANDLE_VALUE) && (Output != 0))
	{
		inherit = true;
		this->StartInfo.hStdOutput = Output;
	}
	else
	{
		this->StartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if ((Input != INVALID_HANDLE_VALUE) && (Input != 0))
	{
		inherit = true;
		this->StartInfo.hStdError = Error;
	}
	else
	{
		this->StartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	}

	if (inherit)
	{
		this->bInheritHandle = true;
	}
	return true;
}

/// <summary>
/// Launch the app via the settings specified.
/// </summary>
/// <returns></returns>
bool ProcessLauncher::Launch()
{
	bool Result = false;

	
	if (UserName == nullptr)
	{
		if (this->DetoursDll == nullptr)
		{
			Result = CreateProcess(this->Name, this->Arguments, nullptr, nullptr, this->bInheritHandle, CreationFlags, nullptr, WorkingDir, &StartInfo, &ProcessInfo);
		}
		else
		{
			Result = DetourCreateProcessWithDllW(this->Name, this->Arguments, nullptr, nullptr, this->bInheritHandle, this->CreationFlags, nullptr, this->WorkingDir, &this->StartInfo, &this->ProcessInfo, this->DetoursDll, nullptr);
		}
		return Result;
	}
	else
	{
		if (LogOnUser() == FALSE)
		{
			Result = CreateProcessWithLogonW(this->UserName, this->DomainName, this->Password, this->LogOnType, this->GetApplicationName(), this->Arguments, this->CreationFlags, nullptr, this->WorkingDir, &this->StartInfo, &this->ProcessInfo);

		}
	}
	return Result;
}

void ProcessLauncher::ResumeMainThread()
{
	if ((this->ProcessInfo.hThread) != INVALID_HANDLE_VALUE)
	{
		ResumeThread(this->ProcessInfo.hThread);
	}
}

BOOL ProcessLauncher::GetProcessInformation(PROCESS_INFORMATION* Out)
{
	if (Out != nullptr)
	{
		memcpy(Out, &this->ProcessInfo, sizeof(PROCESS_INFORMATION));
		return TRUE;
	}
	return FALSE;
}






bool ProcessLauncher::LogOnUser()
{
	if (UserName == nullptr)
	{
		return false;
	}
	else
	{
		const wchar_t* Domain;
		if (this->DomainName == nullptr)
		{
			Domain = nullptr;
		}
		else
		{
			Domain = this->DomainName;
		}

		BOOL Result = LogonUser(this->UserName, Domain, this->Password, this->LogOnType, this->LogOnProvider, &this->UserToken);
		
		
			DWORD dwErr = GetLastError();
		
		return Result;
	}
}

/// <summary>
/// If Outside is null, returns length of insize in chars.
/// If Outside is not null, allocates a duplicate string and places it in the location of Outside.
/// </summary>
/// <param name="inside"></param>
/// <param name="Outside"></param>
/// <returns>When Outside=null, returns number of characters </returns>
DWORD ProcessLauncher::CommonStringGetRoutine(const wchar_t* inside, wchar_t** Outside)
{
	if (Outside == nullptr)
	{
		if (inside == nullptr)
		{
			return 0;
		}
		else
		{
			return wcslen(inside);
		}
	}
	if (*Outside != nullptr)
	{
		free(*Outside);
	}
	if (inside == nullptr)
	{
		*Outside = nullptr;
	}
	else
	{
		*Outside = _wcsdup(inside);
	}
	return true;
}
