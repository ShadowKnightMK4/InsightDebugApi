


#include <Windows.h>
#include "FilesandboxApi_DllStuff.h"
#include "ClientCallers.h"
#include <debugapi.h>

extern "C" {
	/// <summary>
	/// Return a Handle to the caller (that's actually a pointer to a structer to use with other calls.
	/// </summary>
	/// <param name="Id">How you want to ID yourself</param>
	/// <returns></returns>
	VOID* WINAPI FSStartup(DWORD Id)
	{
		return FSSTartupEx(Id, FALSE);
	}

	/// <summary>
	/// Call to get a handle to use the other routines.
	/// </summary>
	/// <param name="Id"></param>
	/// <param name="WantDebugHelper"></param>
	/// <returns></returns>
	VOID* WINAPI FSSTartupEx(DWORD Id, BOOL WantDebugHelper)
	{
		Client* ret = new Client();
		if (ret) {
			ret->ID = Id;
			// setup the helper 
			if (WantDebugHelper)
			{
				InitializeConditionVariable(&ret->DebugHelpSyncVal);
				ret->SyncDebugHelper = TRUE;
			}
			else
			{
				ret->SyncDebugHelper = FALSE;
			}
			
		}
	}

	/// <summary>
	/// Cleanup resources that were setup in FSStartup
	/// </summary>
	/// <param name="ID"></param>
	VOID WINAPI FSCleanup(Client* ID)
	{
		delete ID;
	}



}