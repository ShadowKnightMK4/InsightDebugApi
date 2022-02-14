#pragma once
#include <Windows.h>
#include <detours.h>
#include <DbgHelp.h>
#include <Shlwapi.h>
#include <string>
#include <vector>
#include "Utility.h"
#include "InsightSupport.h"

/// <summary>
/// Callback for the SymbolSearch thing. This is intended to be filled by the DotNet side of things. return FALSE to quit the enum
/// </summary>
typedef BOOL (WINAPI* SymbolSearchCallback)(PSYMBOL_INFOW);

/*
* Insite hunter is the core class that lets one Load an managing symbols. You should only need one 
*/
class InsightHunter
{
public:
	InsightHunter();
	InsightHunter(HANDLE DebuggedProcess);
	~InsightHunter();

	
	
#pragma region Loading Symbols in response to debug events

	/// <summary>
	/// this routine is called to tell the symbol engine to load pdb data from a CREATE_PROCESS_DEBUG_EVENT.  This routine loads the symbols for  the EXE of the process.
	/// </summary>
	/// <param name="EventData"></param>
	/// <returns></returns>
	bool LoadExeSymbolInfo(LPDEBUG_EVENT EventData);
	/// <summary>
	/// This routine is called to tell te symbol engien to load pdb from a LOAD_DLL_DEBUG_EVENT 
	/// </summary>
	/// <param name="EventData"></param>
	/// <returns></returns>
	bool LoadDllSymbolInfo(LPDEBUG_EVENT EventData);
	/// <summary>
	/// This is called to unload Dll Info from a UNLOAD_DLLD_DEBUG_EVENT. This routine clears the symbols loaded for *that* dll
	/// </summary>
	/// <param name="EventData"></param>
	/// <returns></returns>
	bool UnLoadDllSymbolInfo(LPDEBUG_EVENT EventData);

	/// <summary>
	/// This is called when receiving EXIT_PROCESS_DEBUG_EVENT. This routine clears all symbols loaded for the process in this class
	/// </summary>
	/// <param name="EventData"></param>
	/// <returns></returns>
	bool UnloadExeSymbolInfo(LPDEBUG_EVENT EventData);

#pragma endregion

#pragma region Examining Loaded Symbols



	/// <summary>
	/// Call once to walk thru each search path. 0 mean
	/// </summary>
	/// <returns></returns>
	wchar_t* WalkSearchPath();
	/// <summary>
	/// Call the SymRefreshMoudleList() to refresh the currrent instance. 
	/// </summary>
	bool RefreshModuleList();


	/// <summary>
	/// Enumerat Symbnols with a DotNet friendly called back. For format see MSDN's "SymEnumSymbolsExW" function.
	/// </summary>
	/// <param name="Callback"></param>
	/// <param name="SearchString"></param>
	/// <returns></returns>
	BOOL EnumerateLoadedSymbols(SymbolSearchCallback* Callback, wchar_t* SearchString);

	/// <summary>
	/// Opens aprocess handle to TargetID and passes it to the other version
	/// </summary>
	/// <param name="TargetID"></param>
	/// <returns></returns>
	BOOL SetMainDebugTarget(DWORD TargetID);
	/// <summary>
	/// Specific  new core debug target i.e. app to load symbols for. Must be process handle.  CLEARS any previously loaded symbols.
	/// </summary>
	/// <param name="MainDebug"></param>
	/// <returns></returns>
	BOOL SetMainDebugTarget(HANDLE MainDebug);
#pragma endregion
#pragma region Thread and Window Sync

	bool SetParentWindow(HWND Window);
	/// <summary>
	/// If enabled, calls are synchroized accurress threads with a Critical Section for each instance of this class.  Can safely be set to false if you are not pontially accessing symbols from multile threads
	/// </summary>
	bool EnforceThreadSync(BOOL NewVal);
	/// <summary>
	/// get current thread sync state
	/// </summary>
	/// <returns></returns>
	bool GetThreadSyncState();
#pragma endregion
private:

#pragma region MemoryMangement
	/// <summary>
	/// Wipe current symbols either in ~Destroy() event or we're pointing insite to a new debug target.
	/// </summary>
	void ClearSymbolContainer();
#pragma endregion

	/// <summary>
	/// If not setup, setup the symbol negine with the current settings. Any public: facing routine needs to be calling this is DebugHelpOnline is not set.
	/// </summary>
	BOOL InitializeSymbolEngineIfNot();
	
	/// <summary>
	/// Common code between the LoadEXE and LoadDll routines.
	/// </summary>
	/// <param name="EventData"></param>
	/// <returns></returns>
	BOOL LoadModuleInfo(LPDEBUG_EVENT EventData);
	BOOL UnloadModuleInfo();

	/// <summary>
	/// The public routines call this to sync access to the windows debug help symbol
	/// </summary>
	void BeginThreadSynch();
	/// <summary>
	/// the public routiens call this to end sync to the debug help routiens
	/// </summary>
	void EndThreadSync();
	/// <summary>
	/// Houses the paths to check when loading symbol files
	/// </summary>
	std::vector<std::wstring> ModuleSearchPath;


	DWORD SymbolOptions;
	/// <summary>
	/// variable for syncng threads
	/// </summary>
	CRITICAL_SECTION SyncVariable;
	/// <summary>
	/// true if using thread sync
	/// </summary>
	BOOL EnableThreadSync;

	/// <summary>
	/// Each process we get A CREATE_PROCESS_DEBUG_EVENT gets one where. 
	/// KEY = processID.  
	/// 
	/// </summary>
	std::map<DWORD, InsightSupport_SymbolHandle*> HandleContainer;


};

