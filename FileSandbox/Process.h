#pragma once
#include <Windows.h>
#include <string>
/// <summary>
/// A Single Class to Deal with Lauching processes and more
/// </summary>
class Process
{
	BOOL LaunchProcess();

public:
	bool Lockdown();
	/// <summary>
	/// THis is the process/ app that will be started;
	/// </summary>
	std::wstring TargetProcess;
	/// <summary>
	/// This is the process/app's arugments that will be passed
	/// </summary>
	std::wstring ProcessArguments;
	/// <summary>
	/// This is the starting directory for the app;
	/// </summary>
	std::wstring WorkingDirectory;
	/// <summary>
	/// Controls the Start Info passed to the object
	/// </summary>
	STARTUPINFO StartInfo;
	/// <summary>
	/// Receives and stores handle to the process and its main thread once started.
	/// </summary>
	PROCESS_INFORMATION ProcessInfo;
	/// <summary>
	/// Process Creation Flags
	/// </summary>
	DWORD CreationFlags;
};

