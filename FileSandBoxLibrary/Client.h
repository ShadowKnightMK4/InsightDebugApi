#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "PS_ProcessInformation.h"
class PS_ProcessInformation;

typedef int ClientOutputFormat;


/*
* Client - handles process tracking and grouping as well as logging
* ProcessInformation handles process spawning and assigning a watcher thread + event to each known debugged process.
*/

class Client
{

public:
	Client(HANDLE CallerHandle);
	~Client();

	/// <summary>
	/// Register a process created by this
	/// </summary>
	/// <param name="NewProcess"></param>
	/// <returns></returns>
	BOOL RegisterSpawnedProcess(PS_ProcessInformation* NewProcess);
	/// <summary>
	/// Return a registered process structure for use.
	/// </summary>
	/// <returns></returns>
	PS_ProcessInformation* NewProcess();

	/// <summary>
	/// Spawn a previously registered process.
	/// </summary>
	/// <param name="Process"></param>
	/// <returns></returns>
	DWORD SpawnProcess(PS_ProcessInformation* Process);
private:
	/// <summary>
	/// We spawn a process, we registry it here.
	/// </summary>
	std::vector< PS_ProcessInformation> TrackedProcesses;
};
/*

/// <summary>
/// The client class is reponsible for tracking setting and debugged apps currently seeing by a process that's callse FSStartup()/
/// </summary>
class  Client
{
public:
	~Client();
	/// <summary>
	/// When the client gets a proess creation event with the debug symbol library wanted, this routine is called to list of things to track
	/// </summary>
	/// <param name="handle"></param>
	BOOL RegisterNewDebugHelpHandle(HANDLE handle);
	/// <summary>
	/// Register a new process that this client struct will track
	/// </summary>
	/// <param name="Info"></param>
	/// <returns></returns>
	BOOL RegisterNewTrackedProcess(PS_ProcessInformation* Info);
	/// <summary>
	/// Set the Debug help sync flag
	/// </summary>
	/// <param name="NewVal"></param>
	void SetDebugHelpSyncFlag(BOOL NewVal);
	/// <summary>
	/// Get the curretn debug help sync
	/// </summary>
	/// <returns></returns>
	BOOL GetDebugHelpSyncFlag();
	
	/// <summary>
	/// Turn on or off logging. Turn on Logging with this and then flush message info with FlushLogXXX() routine
	/// </summary>
	/// <param name="Log">You may use the DEBUG_EVENT values (such as EXCEPTION_DEBUG_EVENT and so on to specify *which* log). (Use 0 for ALL Logging.  Use 10 for the HelperDll Exceptions)</param>
	/// <param name="Status">Enable or Disable</param>
	/// <returns>Old value</returns>
	BOOL EnableLogMessages(DWORD Log, BOOL Status);
	BOOL FlushLog(DWORD Log, HANDLE Output);
	BOOL FlushLog(DWORD Log, std::wostream& Output);
	BOOL FlushLog(DWORD Log, std::ostream& Output);

private:

	
	// ID Passed when caller wants to use library.
	DWORD ID;
	/// <summary>
	/// If true, we setup the DebugHelp Symbol Library and offer syncing.
	/// </summary>
	BOOL SyncDebugHelper;
	/// <summary>
	/// The client has created at least one process and SymInitialze() has been called with a handle to it.
	/// </summary>
	BOOL DebugHelperLive;

	std::vector<HANDLE> ProcessHandles;
	/// <summary>
	/// When the client creates a process with our api, en entry is added here.
	/// </summary>
	std::vector<PS_ProcessInformation*> TrackedProcesses;

	/// <summary>
	/// Receives data that eventually gets sent to the console output.
	/// </summary>
	std::wstringstream ConsoleLog;
	/// <summary>
	/// Recieves instances of OutputDebugString processing.   DebugString gets wrote here which then gets send to ConsoleLog which then goes to Console
	/// </summary>
	std::wstringstream DebugStringLog;

	/// <summary>
	/// 
	/// </summary>
	BOOL DebugStringLogEnabled;
	/// <summary>
	/// revies Rip log info
	/// </summary>
	std::wstringstream RipLog;
	/// <summary>
	/// If true, 
	/// </summary>
	BOOL RipEnabled;
	/// <summary>
	/// 
	/// </summary>
	std::wstringstream UnloadDllLog;

	/// <summary>
	/// 
	/// </summary>
	BOOL UnloadDllLogEnabled;
	/// <summary>
	/// 
	/// </summary>
	std::wstringstream LoadDllLog;
	/// <summary>
	/// 
	/// </summary>
	BOOL LoadDllLogEnabled;

	/// <summary>
	/// 
	/// </summary>
	std::wstringstream CreateThreadLog;
	/// <summary>
	/// 
	/// </summary>
	BOOL CreateThreadEnabled;
	/// <summary>
	/// 
	/// </summary>
	std::wstringstream ExitThreadLog;
	/// <summary>
	/// 
	/// </summary>
	BOOL ExitThreadEnabled;
	/// <summary>
	/// 
	/// </summary>
	std::wstringstream ExceptionLog;
	/// <summary>
	/// 
	/// </summary>
	BOOL ExceptionLogEnabled;
	/// <summary>
	/// 
	/// </summary>
	std::wstringstream DllHelperLog;
	/// <summary>
	/// 
	/// </summary>
	BOOL DllHelperLogEnabled;
	/// <summary>
	/// 
	/// </summary>
	std::wstringstream CreateProcessLog;
	/// <summary>
	/// 
	/// </summary>
	BOOL CreateProcessLogEnabled;
	/// <summary>
	/// 
	/// </summary>
	std::wstringstream ExitProcessLog;
	/// <summary>
	/// 
	/// </summary>
	BOOL ExitProcessLogEnabled;


};
*/