


#include <Windows.h>
#include "FilesandboxApi_DllStuff.h"
#include "ClientHandling.h"
#include <debugapi.h>
#include <map>
#include <StaticIncludes.h>
#include "Client.h"



extern "C" {

	
	std::map<DWORD, Client*> Connections = std::map<DWORD, Client*>();
	CRITICAL_SECTION SyncAcces;
	BOOL SyncSet = FALSE;

	Client* FSClientIdToPoint(DWORD Id)
	{
		Client* ret = nullptr;
		if (!SyncSet)
		{
			InitializeCriticalSection(&SyncAcces);
			SyncSet = TRUE;
		}
		EnterCriticalSection(&SyncAcces);
		auto FindThis = Connections.find(Id);
		if (FindThis != Connections.end())
		{
			ret = Connections[Id];
		}
		LeaveCriticalSection(&SyncAcces);
		return ret;
	}



	DWORD WINAPI FSStartup(DWORD Id)
	{
		return FSStartupEx(Id, FALSE);
	}


	DWORD FSStartupEx(DWORD PrefId, BOOL WantDebugHelp)
	{
		if (PrefId == 0)
		{
			return 0;
		}
		auto InsertLocation = Connections.find(PrefId);

		if (InsertLocation != Connections.end())
		{
			return 0;  // entry already exists
		}
		else
		{

			Client* ret = new Client();

			if (ret != nullptr)
			{
				ret->SetDebugHelpSyncFlag(WantDebugHelp);
				
				// we'll initial the symbol library when we start debugging the process if true.
			}

			if (ret != nullptr)
			{
				Connections.insert(std::pair<DWORD, Client*>(PrefId, ret));
				return PrefId;
			}
			return 0;
		}
	}

	VOID  WINAPI FSCleanup(DWORD PrefId)
	{
		if (PrefId == 0)
		{
			return;
		}
		else
		{
			auto Location = Connections.find(PrefId);
			if (Location == Connections.end())
			{
				return;
			}
			else
			{
				delete  Connections.at(PrefId);
				Connections.erase(PrefId);
			}
		}
	}



}