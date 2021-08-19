#pragma once

#include "FilesandboxApi_DllStuff.h"
// A client is somone who's called FSStartup()

struct Client
{
	// ID Passed when caller wants to use library.
	DWORD ID;
	/// <summary>
	/// If true, we setup the DebugHelp Symbol Library and offer syncing.
	/// </summary>
	BOOL SyncDebugHelper;
	/// <summary>
	/// When the DebugHelp Api is synched thru the API in this DLL, we use this to syync.
	/// When DebugHelpApi is NOT SYNCED with this api, this var is undefined
	/// </summary>
	CONDITION_VARIABLE DebugHelpSyncVal;
	CONDITION_VARIABLE Log;
	
};

extern "C"
{
	/// <summary>
	/// Cleanup resources that were setup in FSStartup
	/// </summary>
	/// <param name="ID"></param>
	VOID* WINAPI FSStartup(DWORD Id);

	/// <summary>
	/// Cleanup resources that were setup in FSStartup
	/// </summary>
	/// <param name="ID"></param>
	VOID FILESANDBOX_API_DLL FSCleanup(Client* ID);

}
