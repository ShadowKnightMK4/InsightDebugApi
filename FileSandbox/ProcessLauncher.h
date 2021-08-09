#pragma once
#include <StaticIncludes.h>


/// <summary>
/// Launch a process.  Primarly intended for setting up using the Debugger API
/// </summary>
class ProcessLauncher
{
public:
	/*
	enum LogOnType
	{
		LogOnTypeBatch = LOGON32_LOGON_BATCH,
		LogOnTypeInteractive = LOGON32_LOGON_INTERACTIVE,
		LogOnTypeNetwork = LOGON32_LOGON_NETWORK,
		LogOnTypeNetworkClearText = LOGON32_LOGON_NETWORK_CLEARTEXT,
		LogOnTypeNewCredentials = LOGON32_LOGON_NEW_CREDENTIALS
	};

	enum LogOnProvider
	{
		LogOnProviderDefault = LOGON32_PROVIDER_DEFAULT,
		LogOnProviderWinNT50 = LOGON32_PROVIDER_WINNT50,
		LogOnProviderWinNT40 = LOGON32_PROVIDER_WINNT40
	};*/

	ProcessLauncher();
	ProcessLauncher(ProcessLauncher& Other);
	~ProcessLauncher();
	
	/// <summary>
	/// set the name of the process to launch. duplicates argument into an internal buffer
	/// </summary>
	/// <param name="name">process to launch.</param>
	/// <returns>true if it worked and false if it did not</returns>
	bool SetApplicationName(const wchar_t* name);
	/// <summary>
	/// set the name of the process to launch. Converts ANSI to unicode and duplicates argument into an internal buffer
	/// </summary>
	/// <param name="name">process to launch.</param>
	/// <returns>true if it worked and false if it did not</returns>
	bool SetApplicationName(const char* name);
	/// <summary>
	/// receieve a unique copy of the app to be launched. Caller needs to free() it when done.
	/// </summary>
	/// <param name="Target">use null to get number of bytes needed. Otherwsise set to pointer to wchar_t* that will get a duplicate of the string</param>
	/// <returns>If Target is null, returns bytes needed to hold the string. If target is not null, receives how many bytes copied</returns>
	DWORD GetApplicationName(wchar_t** Target);
#ifdef _PROCESS_LAUNCHER_DIRECT_ACCESS_
	/// <summary>
	/// returns a direct copy of the underlyine pointer. DON'T FREE THIS
	/// </summary>
	/// <returns></returns>
	const wchar_t* GetApplicationName();
#endif


	/// <summary>
	/// Set the applications arguments. Duplicates to an internal buffer.
	/// </summary>
	/// <param name="Arguments">the arguments for the process</param>
	/// <returns>if arguments is not null, copies to internal buffer and returns true. Returns false otehrwise</returns>
	bool SetApplicationArguments(const wchar_t* Arguments);
	/// <summary>
	/// Converst string to Unicode string and stores a copy in an internal buffer.
	/// </summary>
	/// <param name="Arguments">the arguments for the process</param>
	/// <returns>true if it worked, false otherwise</returns>
	bool SetApplicationArguments(const char* Arguments);
	/// <summary>
	/// receive a unique copy of the Application arugments. CALLER is responsible to free this once done with it.
	/// </summary>
	/// <param name="Arguments">If null, return value is copy many bytes needed to store string. If NOT NULL, this will set this to a duplicate of the internal buffer</param>
	/// <returns></returns>
	DWORD GetApplicationArguments(wchar_t** Arguments);
#ifdef _PROCESS_LAUNCHER_DIRECT_ACCESS_
	/// <summary>
	/// returns a direct copy of the underlyine pointer. DON'T FREE THIS
	/// </summary>
	/// <returns></returns>
	const wchar_t* GetApplicationArguments();
#endif


	bool SetApplicationWorkingDirectory(const wchar_t* TargetDir);
	bool SetApplicationWorkingDirectory(const char* TargetDir);
	DWORD GetApplicationWorkingDirectory(wchar_t** TargetDir);
#ifdef _PROCESS_LAUNCHER_DIRECT_ACCESS_
	const wchar_t* GetApplicationWorkingDirectory();
#endif


	bool SetDetourDll(const wchar_t* DetourDll);
	bool SetDetourDll(const char* DetourDll);
	DWORD GetDetoursDll(const wchar_t** DetoursDll);
#ifdef _PROCESS_LAUNCHER_DIRECT_ACCESS_
	const wchar_t* GetDetoursDll();
#endif

	void SetCreationFlags(DWORD Flags);
	DWORD GetCreationFlags();

	bool SetUserName(const wchar_t* Name);
	DWORD GetUserName(const wchar_t** Name);
#ifdef _PROCESS_LAUNCHER_DIRECT_ACCESS_
	const wchar_t* GetUserName();
#endif


	bool SetPassword(const wchar_t* Password);
	
	bool SetUserDomain(const wchar_t* DomainName);
	DWORD GetUserDomain(const wchar_t** Domain);
#ifdef _PROCESS_LAUNCHER_DIRECT_ACCESS_
	const wchar_t* GetUserDomain();
#endif

	bool SetDesktop(const wchar_t* Desktop);
	bool SetDesktop(const DWORD DesktopEnum);

	bool SetLogOnType(const DWORD Type);
	bool SetLogOnProvider(const DWORD Provider);

	BOOL SetInheritHandle(BOOL bInherit);
	BOOL SetStandardHandles(HANDLE Input, HANDLE Output, HANDLE Error);
	bool Launch();
	void ResumeMainThread();
	
	BOOL GetProcessInformation(PROCESS_INFORMATION* Out);
private:
	static BOOL WINAPI DetourCreateProcessWithDllAsUserW(
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
		_In_ LPCSTR lpDllName);
	bool LogOnUser();


	DWORD CommonStringGetRoutine(const wchar_t* inside, wchar_t** Outside);

	/// <summary>
	/// The public routines work eventually to here.
	/// The code duplicates the outside string into an internal buffer that gets clearaed in the destructor
	/// </summary>
	/// <param name="outside">copy from</param>
	/// <param name="Inside">copy too</param>
	/// <returns>true on sucess, false and false.</returns>
	bool CommonSettingRoutine(const wchar_t* outside, wchar_t** Inside, bool ClearBuffer);
	/// <summary>
	/// The public routines work eventually to here.
	/// The code duplicates the outside string into an internal buffer that gets clearaed in the destructor.
	/// This version also uses the Win32 API to conver the ANSI string to unicode.
	/// </summary>
	/// <param name="outside">copy from</param>
	/// <param name="Inside">copy too</param>
	/// <returns>true on sucess, false and false.</returns>
	bool CommonSettingRoutine(const char* outside, wchar_t** Inside, bool ClearBuffer);

	void SelfZero();
	/// <summary>
	/// Name of the user to loggin as (use null to keep  current one)
	/// </summary>
	wchar_t* UserName;
	/// <summary>
	/// Password for user to log in as (ignored if UserName is null)
	/// </summary>
	wchar_t* Password;
	/// <summary>
	/// Set this to the Program to Launch
	/// </summary>
	wchar_t* Name;
	/// <summary>
	/// Set this to the arguments to use
	/// </summary>
	wchar_t* Arguments;
	/// <summary>
	/// Set this to the Directory to start in.
	/// </summary>
	wchar_t* WorkingDir;
	/// <summary>
	/// Domain name for User to authenitcate from (unused if UserName is null)
	/// </summary>
	wchar_t* DomainName;
	/// <summary>
	/// Set this to a Dll to use with Detours. It'll force load this dll.
	/// </summary>
	char* DetoursDll;

	
	/// <summary>
	/// Specifies the logon type when username is not null
	/// </summary>
	DWORD LogOnType;

	/// <summary>
	/// Specifies the logon provider if username is not null
	/// </summary>
	DWORD LogOnProvider;

	/// <summary>
	/// Specify the Process Creation Flags
	/// </summary>
	DWORD CreationFlags;
	/// <summary>
	/// The Process's Start Info
	/// </summary>
	STARTUPINFOW StartInfo;
	/// <summary>
	/// After creating the process, will point to the process's PROCESS_INFORMATION struct
	/// </summary>
	PROCESS_INFORMATION ProcessInfo;

	/// <summary>
	/// Specifies if any handles will be inheirted in the child process.
	/// </summary>
	BOOL bInheritHandle;

	/// <summary>
	/// Is used in some cases when caller wants Detours And UserLogin
	/// </summary>
	HANDLE UserToken;

	
};

