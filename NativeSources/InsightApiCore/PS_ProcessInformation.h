#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>

#include "InsightHunter.h"
#include "ThreadContext.h"
#include "ThreadSupport.h"

class Client;
class PS_ProcessInformation;

#ifndef COMMANDMENTS_PROCESS
/*
* conents the commandment do dads for the class.
* DROP_XXXX means the value indiectated the thing is striped before passing to the detoured rototine
* DENY MEANS when the detoured routine gets the flag for that, we skip the implementation call.
*		YESMAN values mean we return sucess coeds
*		NOPE value means we return failure code
* 
*	if the commandment is enabled, the effect is gained (and added via the helper dll)
*	commandments are set for the process lifetime.
*/
#define COMMANDMENTS_PROCESS

// strip generic read from ntopenfile / nt createfile
#define COMMANDMENT_DROP_FILE_READS (1)
// when generic read is encourted, report ok, but set handle to invalid handle
#define COMMANDMENT_DENY_FILE_READS_YESMAN (2)
// when generic read is seen, report access denied
#define COMMANDMENT_DENY_FILE_READS_NOPE (3)

// strip generic write from ntopen/ nt create file
#define COMMANDMENT_DROP_FILE_WRITE (4)
// when generic read is encourted, report ok, but set handle to invalid handle
#define COMMANDMENT_DENY_FILE_WRITE_YESMAN (5)
// when generic read is seen, report access denied
#define COMMANDMENT_DENY_FILE_WRITE_NOPE (6)

// spawning a process is met with 'access nenidewd at the NtCreateProcess API
#define COMMANDMENT_DENY_PROCESS_SPAWN (7)
// when set, the helper dll is forcble loaded into the target at the NtCreatProcess level
#define COMMANDMENT_PROCESS_PROPERGATE (8)

// keep this set to the highest value for comamndment.  This also specifies the array.
#define COMMANDMENT_MAX_VALUE (255)
#endif

/// <summary>
/// lpCurEvent is the event to process Dont free it..
/// Write your continue state to the 2nd arg
/// Write your time to wait for the 3rd arg.
/// CustomARg is reserved
/// 
/// Return 0 to keep going and non zero to stop.  
/// </summary>
typedef int(WINAPI* DebugEventCallBackRoutine)(LPDEBUG_EVENT lpCurEvent, DWORD* ContinueStatus, DWORD* WaitTimer, DWORD CustomArg);

/// <summary>
/// This stricture used when the Process_Spawn() routine needs to create a worker thread to spawn and watch for debug events.
/// </summary>
struct WorkerThreadData
{
	/// <summary>
	/// Is the handle that the caller needs to hit during their main / gui processing loop to send debug events back to windows
	/// </summary>
	HANDLE EventHandle;
	/// <summary>
	///  Callback provided to examine events and do stuff.  SymbolEngine is automatically handled via the InsightHunter classs provided you opt into the Debug Worker thread and EnableSymbol = true in this class
	/// </summary>
	DebugEventCallBackRoutine UserCallback;

	
	/// <summary>
	/// see MSDN's ContinueDebugEvent
	/// </summary>
	DWORD ContinueState;
	/// <summary>
	/// see MSDN's ContinueDebugEvet
	/// </summary>
	DWORD WaitTImer;
	/// <summary>
	/// 
	/// </summary>
	int threadID;
	HANDLE ThreadHandle;
	/// <summary>
	/// provides the debug worker friend routine a copy of the this pointer
	/// </summary>
	PS_ProcessInformation* that;
};

/// Do not spawn the process with a separate thread.  
#define PSINFO_DEBUGMODE_NOWORKERTHREAD (0)
// SpawnProcess() creates a thread that spawns your target. 
#define PSINFO_DEBUGMODE_WORKERTHREADED (1)





/// <summary>
/// Each process spawned or tracked by the library uses this class. 
/// It's .NET Wrapper is PsProcessInformation located in PsProcessInformation.cs of FileSandBox_GUI.
/// </summary>
class PS_ProcessInformation
{
public:
	~PS_ProcessInformation();
	PS_ProcessInformation(const PS_ProcessInformation& Original);
	PS_ProcessInformation();
#pragma region Handling the Private Variables with public rotuines

	/// <summary>
	/// Returns the name of the process that was/will be spawned
	/// </summary>
	/// <returns></returns>
	const wchar_t* ProcessName();
	/// <summary>
	/// Asssign a new processs name to be spawned.  
	/// DOES NOTHING once the process is spawned.
	/// </summary>
	/// <param name="NewName">Replacement name</param>
	void ProcessName(const wchar_t* NewName);

	/// <summary>
	/// Returns the argumentst to spawn the process with
	/// </summary>
	/// <returns></returns>
	const wchar_t* ProcessArguments();
	/// <summary>
	/// Assign the arguments to spawn the process with
	/// </summary>
	/// <param name="NewArgs"></param>
	void ProcessArguments(const wchar_t* NewArgs);

	/// <summary>
	/// Get the current creation flags (CreateProcessW)
	/// </summary>
	/// <returns></returns>
	DWORD CreationFlags();
	/// <summary>
	/// Set the current creation falgs (CreateProcessW)
	/// </summary>
	/// <param name="NewFlags"></param>
	void CreationFlags(DWORD NewFlags);


	const wchar_t* CurrentDirectory();
	void CurrentDirectory(const wchar_t* NewCD);

	/// <summary>
	/// If true, we import the spawner enviroment on type of whatever's dene defined.
	/// </summary>
	/// <param name="Yes"></param>
	void ImportSpawnerEnviroment(BOOL Yes);

	/// <summary>
	/// Reset the Defined invorment to blank.
	/// </summary>
	void ClearEnviroment();
	/// <summary>
	/// Return the name value 
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	const wchar_t* Environment(const wchar_t* name);
	/// <summary>
	/// Assign a value to a name.
	/// </summary>
	/// <param name="name"></param>
	/// <param name="value"></param>
	void Environment(const wchar_t* name, const wchar_t* value);

	DWORD GetDetourListSize();

	/// <summary>
	/// Converts the string to ANSI before adding to our list of DLLS that we force the app to try to load.
	/// </summary>
	/// <param name="Name"></param>
	void AddDetoursDll(std::wstring Name);
	/// <summary>
	/// Adds the string to our list of DLLS to force the app to try to load
	/// </summary>
	/// <param name="Name"></param>
	void AddDetoursDll(const char* Name);

	/// <summary>
	/// Clear our list of dlls to force the app to load.
	/// </summary>
	void ClearDetoursDll();
	/// <summary>
	/// RISKY. Gets pointer to the private detttours liist anbd trusts cakker wib;uf
	/// </summary>
	/// <returns></returns>
	const std::vector<std::string> GetDetourList();

	const char* IndexDetourList(int index);
#pragma endregion
#pragma region Spawning The Target
	/// <summary>
	/// Spawn the process and returns its ID.  Returns 0 if the process did not spawn.
	/// </summary>
	/// <returns></returns>
	DWORD SpawnProcess();


#pragma endregion


#pragma region Specifying how to interactive with target if debugging.

	VOID SetDebugEventCallback(DebugEventCallBackRoutine Callback);
	DebugEventCallBackRoutine GetDebugEventCallback();
	VOID SetDebugEventCallBackCustomArg(LPVOID Arg);


	/// <summary>
	/// This pulses the event that the worker thead (if existant) waits on.
	/// Pointless if debug events are not in a different thread.
	/// </summary>
	void PulseDebugEventThread();
	/// <summary>
	/// Specify if the worker thread is spawned or not. Non Worker thread code is somewhat lagging and I recommand using the worker thread unless you have a reason not too.
	/// </summary>
	/// <param name="dmMode"></param>
	void SetDebugMode(DWORD dmMode);

	
	/// <summary>
	/// Get the current debug mode stat for the worker thread
	/// </summary>
	/// <returns></returns>
	DWORD GetDebugMode();


	/// <summary>
	/// After spawning your process, return the main thread handle
	/// </summary>
	/// <returns></returns>
	HANDLE GetMainThreadHandle();
	/// <summary>
	/// After spawning your process, return the main process handle.
	/// </summary>
	/// <returns></returns>
	HANDLE GetMainProcessHandle();
	/// <summary>
	/// Get the process id of your main process.
	/// </summary>
	/// <returns></returns>
	DWORD GetProcessID();
	/// <summary>
	/// Get the thread id of your main process.
	/// </summary>
	/// <returns></returns>
	DWORD GetThreadID();

	/// <summary>
	/// If you are spawning a process to debug, this will also ask for the debug priv when you do that. Default is FALSE
	/// </summary>
	VOID SetDebugPrivState(BOOL WantPriv);

#pragma endregion

#pragma region  Addition Debug Interactions if the HelperDll is forcibly Loaded.
	/// <summary>
	/// These are global in the file sandbox a
	/// </summary>
	/// <param name="CommandMent"></param>
	/// <param name="Status"></param>
	/// <returns></returns>
	BOOL SetCommandment(DWORD CommandMent, BOOL Status);
		 
	/// <summary>
	/// Returns the commandment's value if exists, otherwrise returns false. (YES that means routines outside the class can't tencihally check if exists. 
	/// The commandment flags should modify on a simple yes or no thing
	/// </summary>
	BOOL GetCommandment(DWORD CommandMent);

	/// <summary>
	/// Add a new path that the HelperDll versions of LoadLibraryXXXX will search before other routines
	/// </summary>
	/// <param name="Path"></param>
	/// <returns></returns>
	BOOL AddPriorityLoadLibraryPathW(LPCWSTR Path);
	/// <summary>
	/// Add a new path that the HelperDll versions of LoadLibraryXXXX will search before other routines
	/// </summary>
	/// <param name="Path"></param>
	/// <returns></returns>
	BOOL AddPriorityLoadLibraryPathA(LPCSTR Path);

	/// <summary>
	/// return how many paths are currently in the load library priority path list
	/// </summary>
	/// <returns></returns>
	DWORD GetPriorityLoadLibaryPath_NumberOf();

	/// <summary>
	/// insdex the current value for the path at position
	/// </summary>
	LPCWSTR GetPriorityLoadLibraryPath_Index(size_t Index);

		/// <summary>
		/// clear the array.
		/// </summary>
		VOID EmptyPriorityLoadLibaryPath();
#pragma endregion

#pragma region Thread API
		/// <summary>
		/// Return how many process(es) we've gotten CREATE_PROCESS_DEBUG_EVENTS for.
		/// </summary>
		/// <returns></returns>
		DWORD GetProcessIDCount();

		/// <summary>
		/// Copy list of process ids to output. Will not copy more than outputsize. Returns 0 if fully copied and how many bytes needed to copy if not big enough
		/// </summary>
		/// <param name="Output"></param>
		/// <param name="LargestOutputSize"></param>
		/// <returns></returns>
		DWORD GetProcessIDs(DWORD * Output, DWORD LargestOutputSize);


		/// <summary>
		/// </summary>
		/// <param name="ProcessID">ID of process we've spawned. 0=first process </param>
		/// <returns>If the process has not spawned yet or we don't have an entry, 0 is returned instead of how many threads we're aware off.</returns>
		DWORD GetThreadListCount(DWORD ProcessID);
		/// <summary>
		/// Get how many thread (if any) we're aware off belonging to the main process.
		/// </summary>
		/// <returns>If the main process has not spawned yet or we don't have an entry, 0 is returned instead of how many threads we're aware off.</returns>
		DWORD GetThreadListCount();


		/// <summary>
		/// Copy of list of threads we're aware of belonging to the passed process to the output buffer. Willl not copy copy than LargestBufferSize
		/// </summary>
		/// <param name="ProcessID"></param>
		/// <param name="ThreadID"></param>
		/// <param name="LargestOutputSize"></param>
		/// <returns></returns>
		DWORD GetThreadIDs(DWORD ProcessID, DWORD * ThreadID, DWORD LargestOutputSize);

		/// <summary>
		/// Grab one one the threadinsight classes instanced to the specificed thread nad process if we got it.
		/// </summary>
		/// <param name="ProcessID"></param>
		/// <param name="ThreadID"></param>
		/// <returns></returns>
		ThreadInsight* GetThreadInsightPtr(DWORD ProcessID, DWORD ThreadID);

#pragma endregion

#pragma region SymbolHandling / InsightHunter
	/// <summary>
	/// If enabled, the worker thread will automatically pass stuff to the symbol class
	/// </summary>
	/// <param name="NewStatus"></param>
	/// <returns></returns>
	DWORD SetSymbolStatus(DWORD NewStatus);
	/// <summary>
	/// Return if the symbol engine will automatically update in the worker thread
	/// </summary>
	/// <returns></returns>
	DWORD GetSymbolStatus();
	/// <summary>
	/// receive a pointer to an instance of the symbol class contained within. 
	/// </summary>
	InsightHunter* GetSymbolHandlerPtr();
#pragma endregion

#pragma region Main Memory Process Statistics
	
	DWORD GetPageFaultCount();
	SIZE_T GetPeakWorkingSet();
	SIZE_T GetWorkingSetSize();
	SIZE_T GetQuotaPeakPagePoolUsage();
	SIZE_T GetQuotaPagedPoolUsage();
	SIZE_T GetQuotaPeakNonPageUsage();
	SIZE_T GetQuotaNonPageUsage();
	SIZE_T GetPageFileUsage();
	SIZE_T GetPeakPageFileUsage();
	SIZE_T GetPrivateUsage();

	PROCESS_MEMORY_COUNTERS_EX* GetMemoryStatsBulkPtr();
#pragma endregion

private:
	/// <summary>
	/// Worker thread rotouine friend declartation. This actual is want spances-
	/// -+
	/// 
	/// </summary>
	friend void _cdecl PsPocessInformation_DebugWorkerthread(void* argument);
	
	/// <summary>
	/// if one of the GetXXXX() routines that fetchs memory info is called,  this ends up being called to refresh the private struct
	/// </summary>
	void RefreshMemoryStatistics();
	/// <summary>
	/// Common code for the sapwn process public rotuine. Implementation is here.   Argument is true when the worker thread creates the process but false when the public routine creations the process
	/// </summary>
	DWORD SpawnProcessCommon(bool NoNotSpawnThread);

	/// <summary>
	/// Used by SpawnProcessCommon() it copy helper dll data
	/// </summary>
	VOID CopyPayloads(HANDLE Target);




	/// <summary>
	/// Spawn this process 
	/// </summary>
	std::wstring ProcessNameContainer;
	/// <summary>
	/// Pass these arguments
	/// </summary>
	std::wstring ProcessArgumentsContainer;
	/// <summary>
	/// The Current Starting Directory for the process
	/// </summary>
	std::wstring WorkingDirectory;

	/// <summary>
	/// Future feature
	/// </summary>
	LPSECURITY_ATTRIBUTES lpProcessAttributes;
	/// <summary>
	/// Future feature
	/// </summary>
	LPSECURITY_ATTRIBUTES lpThreadAttributes;



	/// <summary>
	/// Map for the enviroment.  MAP Key is enviroment name,  map value is the string to use>
	/// </summary>
	std::map<std::wstring, std::wstring> Enviroment;
	/// <summary>
	/// Holsd the list of Dlls that will be loaded via detours. 
	/// </summary>
	std::vector<std::string> DetoursDll;

	/// <summary>
	/// Folders that will be searched before other locations.   requres helper dll
	/// </summary>
	std::vector<std::wstring> LoadLibraryPriorityFolders;

	/// <summary>
	/// When load library rotuine gets part 1, sub with part 2 before proceeding.
	/// </summary>
	std::map<std::wstring, std::wstring>  ForcedOverwrites;
	/// <summary>
	/// CreateProcessW Creation Flags
	/// </summary>
	DWORD dwCreationFlags;
	/// <summary>
	/// Startup info for the process
	/// </summary>
	STARTUPINFO StartUpInfo;
	/// <summary>
	/// After process is spawned, holds handles and ids of the main thread + the process itself
	/// </summary>
	PROCESS_INFORMATION PInfo;

	/// <summary>
	/// Contains if the user wants to include a copy of the spawning app's enviroment when spawning the new app
	/// </summary>
	BOOL bInhertEnviroment = FALSE;

	DWORD DebugModeHandle = PSINFO_DEBUGMODE_NOWORKERTHREAD;
	/// <summary>
	/// Null if not needed, otherwise contains arguments  to the worker thread.
	/// </summary>
	WorkerThreadData SyncData;


	std::map<DWORD, BOOL> CommandmentArray;

	/// <summary>
	/// Updated after requiested information in thsi structure.
	/// </summary>
	PROCESS_MEMORY_COUNTERS_EX ProcessMemoryStats;
	/// <summary>
	/// Enable symbol tracking or not.
	/// </summary>
	BOOL EnableSymbols = false;

	/// <summary>
	/// Our database we care about for the lifetime of the process.
	/// </summary>
	ThreadContainer ProcessThreads;
	/// <summary>
	/// Class for the symbol engine. Will Need to spawn a process WITH_DEBUG to get much use.
	/// </summary>
	InsightHunter* Insight;
	/// <summary>
	/// Default false.  If true and Proess creation flags include DEBUG_PROCESS | DEBUG_THIS_PROCESS, we will ask for debug priv
	/// </summary>
	bool RequestDebugPriv;
};



