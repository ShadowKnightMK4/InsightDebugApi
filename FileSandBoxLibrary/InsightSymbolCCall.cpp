#include "InsightHunter.h"

/* contains wrappers to wrap the Insight hunter class that will typicalled be autotracted on the first process creation thing*/
extern "C" {


	/// <summary>
	/// Typically won't need to be called if using process context's helper thread, otherwise tells the symbol engine to load data from this event
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EventData"></param>
	/// <returns></returns>
	BOOL WINAPI Insight_LoadExeSymbolInfo(InsightHunter* that, LPDEBUG_EVENT EventData)
	{
		if (that != nullptr)
		{
			if (EventData != nullptr)
			{
				return that->LoadExeSymbolInfo(EventData);
			}
		}
		return FALSE;
	}


	/// <summary>
	/// Typically won't need to be called if using process context's helper thread, otherwise tells the symbol engine to load data from this event
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EventData"></param>
	/// <returns></returns>
	BOOL WINAPI Insight_LoadDllSymbolInfo(InsightHunter* that, LPDEBUG_EVENT EventData)
	{
		if (that != nullptr)
		{
			if (EventData != nullptr)
			{
				return that->LoadDllSymbolInfo(EventData);
			}
		}
		return FALSE;
	}



	/// <summary>
	/// Typically won't need to be called if using process context's helper thread, otherwise tells the symbol engine to load data from this event
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EventData"></param>
	/// <returns></returns>
	BOOL WINAPI Insight_UnLoadDllSymbolInfo(InsightHunter* that, LPDEBUG_EVENT EventData)
	{
		if (that != nullptr)
		{
			if (EventData != nullptr)
			{
				return that->UnLoadDllSymbolInfo(EventData);
			}
		}
		return FALSE;
	}

	/// <summary>
	/// Wrapper to tell the symbol engine to reload symbols
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	BOOL WINAPI Insight_RefreshLoadedModules(InsightHunter* that)
	{
		if (that != nullptr)
		{
			return that->RefreshModuleList();
		}
		return FALSE;
	}

	/// <summary>
	/// Enable or disable synchroned access to the Debug Help Symbol routines.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EnableSync"></param>
	/// <returns></returns>
	BOOL WINAPI Insight_SetThreadSync(InsightHunter* that, BOOL EnableSync)
	{
		if (that != nullptr)
		{
			return that->EnforceThreadSync(EnableSync);
		}
		return FALSE;
	}

	/// <summary>
	/// get current state of the sync access for this class
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	BOOL WINAPI Insight_GetThreadSync(InsightHunter* that)
	{
		if (that != nullptr)
		{
			return that->GetThreadSyncState();
		}
		return FALSE;
	}

	BOOL WINAPI Insight_SetParentWindow(InsightHunter* that, HWND NewWindow)
	{
		if (that != nullptr)
		{
			return that->SetParentWindow(NewWindow);
		}
		return FALSE;
	}

	BOOL WINAPI Insight_EnumerateLoadedSymbolsW(InsightHunter* that, wchar_t* Symbol, SymbolSearchCallback* Api)
	{
		if (that != nullptr)
		{
			return that->EnumerateLoadedSymbols(Api, Symbol);
		}
		return FALSE;
	}

}