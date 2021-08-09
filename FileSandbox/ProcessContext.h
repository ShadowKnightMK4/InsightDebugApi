#pragma once
#include <map>
#include <string>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <vector>

typedef UINT Commandment;
// NO SPECIAL NEEDS
#define COMMAND_NONE (0)
// ALL CALLS TO NTCREATEFILE / NTOPEN FILE HAVE 'WRITE' ACCESS FLAGS REMOVED / DENIED
#define COMMAND_STRIP_WRITE_ACCESS (1)
// ALL CALLS TO NTCREATEFILE / NTOPEN FILE HAVE READ' ACCESS FLAG REMOVED / DENIED
#define COMMAND_STRIP_READ_ACCESS (2)
// (VISTA+) (if the helper dll is in the debugged process) CALLS TO NTCREATEUSER PROCESS WILL FAIL.
#define COMMAND_STRIP_PROCESS_CREATION (4)

/// <summary>
/// Contains information gethered about a single procese during debugging event
/// TODO:  DEEP COPY:
/// </summary>
class ProcessContextSingle
{
public:

	/// <summary>
	/// Receive and process a create process event.
	/// </summary>
	/// <param name="Event">from WaitForDebugEvent</param>
	/// <returns>return true to keep going and false to quit</returns>
	BOOL CreateProcessDebugEvent(LPDEBUG_EVENT Event);
	/// <summary>
	/// Receive and process a CreateThread Event
	/// </summary>
	/// <param name="Event"></param>
	/// <returns></returns>
	BOOL CreateThreadDebugEvent(LPDEBUG_EVENT Event);
	/// <summary>
	/// Receive and process a debug event
	/// </summary>
	/// <param name="Event"></param>
	/// <returns>Returns true if the exception was processed and false if it was not</returns>
	BOOL ExceptionDebugEvent(LPDEBUG_EVENT Event, DWORD* ExceptionContinueStatus);
	BOOL ExitProcessDebugEvent(LPDEBUG_EVENT Event);
	BOOL ExitThreadDebugEvent(LPDEBUG_EVENT Event);
	BOOL LoadLibraryDebugEvent(LPDEBUG_EVENT Event);
	BOOL OutputDebugStringDebugEvent(LPDEBUG_EVENT Event);
	BOOL UnloadLibraryDebugEvent(LPDEBUG_EVENT Event);
	BOOL RipEvent(LPDEBUG_EVENT Event);


	BOOL SetCommandment(DWORD CommandType);

	/// <summary>
	/// Flush the text output indended for the Console to the output of your choice. Clears said buffer.
	/// </summary>
	/// <param name="Output"></param>
	void FlushConsoleLog(std::wostream& Output);
	/// <summary>
	/// Flush the CreateProcess Event to the Output log
	/// </summary>
	/// <param name="Output"></param>
	void FlushCreateProcessLog(std::wostream& Output);
	/// <summary>
	/// Flush CreateThread Event to the output log
	/// </summary>
	/// <param name="Output"></param>
	void FlushCreateThreadLog(std::wostream& Output);
	/// <summary>
	/// Flush Exception (non DLLHelper) info the the Output log
	/// </summary>
	/// <param name="Output"></param>
	void FlushExceptionLog(std::wostream& Output);
	/// <summary>
	/// Flush DllHelper Exception Info the the output log
	/// </summary>
	/// <param name="Output"></param>
	void FlushDllHelperLog(std::wostream& Output);
	/// <summary>
	/// Flush Exit Process Exception Info to the log
	/// </summary>
	/// <param name="Output"></param>
	void FlushExitProcessLog(std::wostream& Output);
	/// <summary>
	/// FLush Exit Thread INfo to the log
	/// </summary>
	/// <param name="Output"></param>
	void FlushExitThreadLog(std::wostream& Output);
	/// <summary>
	/// Flush Dll Load Info the the log
	/// </summary>
	/// <param name="Output"></param>
	void FlushLoadDllLog(std::wostream& Output);
	/// <summary>
	/// FLush Debug String Messages to the log
	/// </summary>
	/// <param name="Output"></param>
	void FlushDebugLog(std::wostream& Output);
	/// <summary>
	/// Flush unload dll log info to the output log
	/// </summary>
	/// <param name="Output"></param>
	void FlushUnloadDllLog(std::wostream& Output);
	/// <summary>
	/// Flush Rip Event log info to the output log.
	/// </summary>
	/// <param name="Output"></param>
	void FlushRipLog(std::wostream& Output);
	/// <summary>
	/// Flush Every Log to console log first, then flushes console to Output
	/// </summary>
	/// <param name="Output"></param>
	void FlushAllLogs(std::wostream& Output);


private:

	/// <summary>
	/// If known, return the Name of the DLL that was previously loaded (and seen) via LoadDllDebugEvent
	/// </summary>
	/// <param name="dwProcessId"></param>
	/// <param name="ID"></param>
	/// <returns></returns>
	const wchar_t* MapHmoduleToName(DWORD dwProcessId, HMODULE ID);

	std::vector<DWORD> NewDwordVector();
	std::map<HMODULE, std::wstring> NewHmoduleMap(); 
	
	
	/// <summary>
	/// Receives data that eventually gets sent to the console output.
	/// </summary>
	std::wstringstream ConsoleLog;
	/// <summary>
	/// Recieves instances of OutputDebugString processing.   DebugString gets wrote here which then gets send to ConsoleLog which then goes to Console
	/// </summary>
	std::wstringstream DebugStringLog;
	/// <summary>
	/// revies Rip log info
	/// </summary>
	std::wstringstream RipLog;

	std::wstringstream UnloadDllLog;
	std::wstringstream LoadDllLog;
	std::wstringstream CreateThreadLog;
	std::wstringstream ExitThreadLog;
	std::wstringstream ExceptionLog;
	std::wstringstream DllHelperLog;
	std::wstringstream CreateProcessLog;
	std::wstringstream ExitProcessLog;
	
	/// <summary>
	/// Map of HMODULE handles receives that coraspond to individual process ids;
	/// </summary>
	std::map<DWORD, std::map<HMODULE, std::wstring>> ProcessIDDllNameToModule;
	/// <summary>
	/// each entry in this is mapped to dwProcess id rececived from creation exvent or the first event triggered.  The entry is all known threads recevied via events
	/// </summary>
	std::map<DWORD, std::vector<DWORD>> ProcessIDtoThreadIDS;

	/// <summary>
	/// Modify how process context deals with extensions
	/// </summary>
	Commandment CurrentCommands;
};

