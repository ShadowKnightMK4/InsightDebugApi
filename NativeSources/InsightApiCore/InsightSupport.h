#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#include <map>
/// <summary>
/// InsightHunter uses this class to group Loaded Dlls/Exe Symbols that belong to a single process underneither its own proces.s
/// </summary>
class InsightSupport_SymbolHandle
{
public:
	/// <summary>
	/// Make a blank one;
	/// </summary>
	InsightSupport_SymbolHandle();
	/// <summary>
	/// Make one based on a debug event
	/// </summary>
	/// <param name="EventData"></param>
	InsightSupport_SymbolHandle(HANDLE DebuggedProcess, LPDEBUG_EVENT EventData);
	


	/// <summary>
	/// Cleanup
	/// </summary>
	/// <param name="EventData"></param>
	~InsightSupport_SymbolHandle();
	

	/// <summary>
	/// Process a CREATE_PROCESS_DEBUG_EVENT on the symbol size
	/// </summary>
	/// <param name="DebuggedProcess"></param>
	/// <param name="EventData"></param>
	/// <returns></returns>
	bool LoadExe(HANDLE DebuggedProcess, LPDEBUG_EVENT EventData);

	/// <summary>
	/// Process a LOAD_DLL_DEBUG_PROCESS on the symbol side.
	/// </summary>
	bool LoadDll(HANDLE DebuggedProcess, LPDEBUG_EVENT EventData);


	/// <summary>
	/// Unload a moduule based on UnloadDll DebgEvent.
	/// </summary>
	/// <param name="EventData"></param>
	/// <returns></returns>
	bool UnloadDll(HANDLE DebuggedProcess, LPDEBUG_EVENT EventData);
	
	/// <summary>
	/// Unload process based on EXIT_PROCESS_DEBUG_EVENt OR RIP_EVENT
	/// </summary>
	/// <param name="DEbuggedProcess"></param>
	/// <param name="EventData"></param>
	/// <returns></returns>
	/// 
	bool UnloadProcess(HANDLE DEbuggedProcess, LPDEBUG_EVENT EventData);
	bool GetFailBit();
private:
	/// <summary>
	/// for EXE entries, points to a handle for Reading/Writing/Quwey info.
	/// For Dll Entries, INVALID_HANDLE_VALUE
	/// </summary>
	HANDLE ProcessHandle;
	/// <summary>
	/// Process ID retreived from teh debug event
	/// </summary>
	DWORD ProcessID;
	/// <summary>
	/// If the SymbolData.Sizeof() is non-zero, structure is valid. This will be the main ExeModule's symbols
	/// </summary>
	IMAGEHLP_MODULEW64 SymbolData;
	/// <summary>
	/// When DLLS  are loaded they get an entry here.
	/// </summary>
	std::map<DWORD64, IMAGEHLP_MODULEW64*>  LoadedDlls;

	/// <summary>
	/// If set after building class, class did not setupe ok.
	/// </summary>
	BOOL FailBit;
};

