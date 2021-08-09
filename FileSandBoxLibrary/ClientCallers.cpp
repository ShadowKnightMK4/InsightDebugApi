
#include <Windows.h>

struct Client
{
	/// <summary>
	/// When the DebugHelp Api is synched thru the API in this DLL, we use this to syync.
	/// </summary>
	CONDITION_VARIABLE DebugHelpSync;
	CONDITION_VARIABLE Logg;
	
};
extern "C" {
	/// <summary>
	/// Return a Handle to the caller (that's actually a pointer to a structer to use with other calls.
	/// </summary>
	/// <param name="Id">How you want to ID yourself</param>
	/// <returns></returns>
	VOID* FSStartup(DWORD Id)
	{
		return new Client();
	}

	/// <summary>
	/// Cleanup resources that were setup in FSStartup
	/// </summary>
	/// <param name="ID"></param>
	VOID FSCleanup(Client* ID)
	{
		delete ID;
	}



}