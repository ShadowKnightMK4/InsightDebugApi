#pragma once

#include "FilesandboxApi_DllStuff.h"
#include "Client.h"
// A client is somone who's called FSStartup()




extern "C"
{


	/// <summary>
	///  Ready the library with some default values.
	/// </summary>
	/// <param name="ID"></param>
	DWORD WINAPI FSStartup(DWORD Id);

	/// <summary>
	/// Resolve a Public ID handle of a client to an actual client*.   Not Indended to be exported.
	/// </summary>
	/// <param name="Id"></param>
	/// <returns></returns>
	Client* FSClientIdToPoint(DWORD Id);
	/// <summary>
	/// Initialize the library. 
	/// </summary>
	/// <param name="PrefId">ID to identify yourself with. Value is not realevent.</param>
	/// <param name="WantDebugHelp">Do you want the debug help library preloaded?</param>
	/// <returns>returns PrefId on OK and 0 and value.</returns>
	DWORD FSStartupEx(DWORD PrefId, BOOL WantDebugHelp);

	/// <summary>
	/// Cleanup resources that were setup in FSStartup
	/// </summary>
	/// <param name="ID"></param>
	VOID  WINAPI FSCleanup(DWORD PrefId);

	/// <summary>
	/// Call repeatably to walk thru processes you've spawne dwith the FS_SpawnProccessXX
	/// </summary>
	/// <param name="ID"></param>
	/// <returns></returns>
	wchar_t* FS_EnumSpawnedProcessed(DWORD ID);
}
