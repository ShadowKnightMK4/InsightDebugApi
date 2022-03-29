#pragma once
#include <Windows.h>
#include <sstream>
#include <iostream>

#define DEBUGLOG_FORMAT_ANSI_STREAM (0)
#define DEBUGLOG_FORMAT_UNICODE_STEAM (1)
#define DEBUGLOG_FORMAT_ANSI_HANDLE (2)
#define DEBUGLOG_FORMAT_UNICODE_HANDLE (3)

/// <summary>
/// Hold and gather input from sources into an internal buffer before sending it to the target.
/// </summary>
class DebugLogTarget
{

public:
	/// <summary>
	/// intialize with an ANSI output CPP stream
	/// </summary>
	/// <param name="AnsiOut"></param>
	DebugLogTarget(std::ostream* AnsiOut);
	/// <summary>
	/// Setup with an UNICODE output stream
	/// </summary>
	/// <param name="UnicodeOut"></param>
	DebugLogTarget(std::wostream* UnicodeOut);
	/// <summary>
	/// Setup with a Windows32 Handle with The option to specify Unicode or not.
	/// </summary>
	/// <param name="Win32Handle">Win32 Handle must have Write Access (GENERIC_WRITE/ ect...)</param>
	/// <param name="WantUnicode"></param>
	DebugLogTarget(HANDLE Win32Handle, BOOL WantUnicode);
	

	
	~DebugLogTarget();


	/// <summary>
	/// Flush the log to the stored output source.
	/// </summary>
	/// <returns></returns>
	BOOL FlushLog();
	/// <summary>
	/// Flush log as (unicode chars) to the passed output
	/// </summary>
	BOOL FlushLog(std::wostream& output);
	/// <summary>
	/// Tries to convert the log output to ANSI before flushing to the log. 
	/// </summary>
	/// <param name="output"></param>
	/// <returns></returns>
	BOOL FlushLog(std::ostream& output);
	/// <summary>
	/// Clear the underlying log buffer.
	/// </summary>
	VOID ClearLogBuffer();

	
	/// <summary>
	/// Emit the buffer to the wostream.  Does Not clear the log.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="Data"></param>
	/// <returns></returns>
	friend std::wostream& operator << (std::wostream& that, const DebugLogTarget& Data);
	/// <summary>
	/// Convert the buffer to ANSI then emmit to the stream.  If it can't, it emits "(ERRROR)" instead.  Does not clear the log.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="Data"></param>
	/// <returns></returns>
	friend std::ostream& operator << (std::ostream& that, const DebugLogTarget& Data);
private:
	union
	{
		std::shared_ptr<HANDLE> Win32Handle;
		std::wostream* UnicodeOutput;
		std::ostream* AnsiOutput;
	} u;
	UINT EmitType;
	std::wstringstream InternalBuffer;
};

