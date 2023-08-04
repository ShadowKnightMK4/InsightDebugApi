#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include  <stdio.h>
#include "InsightHunter.h"
#include "ThreadContext.h"
#include "ThreadSupport.h"
#include "StartupInfoStructHelper.h"
//#include <gsl>

class InsightProcess;

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
	DWORD WaitTimer;
	/// <summary>
	/// 
	/// </summary>
	int threadID;
	HANDLE ThreadHandle;
	/// <summary>
	/// provides the debug worker friend routine a copy of the this pointer
	/// </summary>
	InsightProcess* that;
};

/*
* DWORD DebugModeHandle is a bit field
* 
* 
*/

#define PSINFO_DEBUGMODE_LOWER (0)
#define PSINFO_DEBUGMODE_UPPER (3)

#define PSINFO_DEBUGMODE_NOWORKERTHREAD PSINFO_DEBUGMODE_LOWER

// set this value to enable the worker thread.   Default is false. Your going to want the worker thread as non workerthread stuff is lagging behind.
#define PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_MASK (1)

// applies only if worker thread is action.. Default is false.    When true and if no event is recevied (this is debuged by a DebugEvent.lpEvent == 0 after WaitForDebugEvnet(), your custom routine is NOT CALLED.
#define PSINFO_DEBUGMODE_WORKERTHREAD_ENABLE_DROPEVENT_MASK (2)

/// Do not spawn the process with a separate thread.  
//#define PSINFO_DEBUGMODE_NOWORKERTHREAD (0)
// SpawnProcess() creates a thread that spawns your target. 
//#define PSINFO_DEBUGMODE_WORKERTHREADED (1)





/// <summary>
/// Each process spawned or tracked by the library uses this class. 
/// It's .NET Wrapper is InsightProcess located in InsightProcess.cs of the Sheath Project
/// </summary>
class InsightProcess
{
public:

	/// <summary>
	/// Copy constructor
	/// </summary>
	InsightProcess(const InsightProcess& Original);
	/// <summary>
	/// Move constructor
	/// </summary>
	InsightProcess(InsightProcess&& Original);
	InsightProcess& operator=(InsightProcess& other);
	InsightProcess& operator=(const InsightProcess& other);
	InsightProcess& operator=(InsightProcess&& other);
	InsightProcess();

	/// <summary>
	/// destructor/cleanup
	/// </summary>
	~InsightProcess();
#pragma region Handling the Private Variables with public rotuines

	/// <summary>
	/// Returns the name of the process that was/will be spawned
	/// </summary>
	/// <returns></returns>
	const wchar_t* ProcessName();
	/// <summary>
	/// Assign a new process name to be spawned.  
	/// Pointless if the process has already started.
	/// </summary>
	/// <param name="NewName">Replacement name</param>
	void ProcessName(const wchar_t* NewName);

	/// <summary>
	/// Returns the arguments to spawn the process with
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
	/// Set the current creation flags  (CreateProcessW)
	/// </summary>
	/// <param name="NewFlags"></param>
	void CreationFlags(DWORD NewFlags);


	const wchar_t* CurrentDirectory();
	void CurrentDirectory(const wchar_t* NewCD);

	/// <summary>
	/// Set if the spawned process will inherit the debugger's environment valeues. Note that environemnt values you explicitly define with the Enviroment() routine in this class override that
	/// </summary>
	/// <param name="Yes">If true, spawned processes get a copy of the debugger's environement values, BUT any ones you explicitly define via the Environment() routine in this class replace the debugger one. </param>
	void InheritSpawnEnvironment(BOOL Yes);

	/// <summary>
	/// Get the current status of if the spawned process inherits the debugger's environement values
	/// </summary>
	/// <returns></returns>
	BOOL InheritSpawnEnvironment();
	

	/// <summary>
	/// Reset the Defined environment to blank.
	/// </summary>
	void ClearEnviroment();
	/// <summary>
	/// Return the name value 
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	const wchar_t* Environment(const wchar_t* name);
	/// <summary>
	/// Assign a value to a name. If value is null, returns it from the list of enviroment varibles process will spawn with
	/// </summary>
	/// <param name="name">a non null string pt</param>
	/// <param name="value">assign name to mean this value. If value is null, removes it from our list</param>
	void Environment(const wchar_t* name, const wchar_t* value);

	
	/// <summary>
	/// Return the number of DLL Entries in the detours list that the target process will be forced to load.
	/// </summary>
	/// <returns>for x86 bit code, return size will never be bigger than a DWORD's (4 byte) max value.  For x64 bit code, return size capped at QWORD (8 Byte) max value. We use the larger value to ensure we don't truncate for x64 bit machines.</returns>
	unsigned long long GetDetourListSize() noexcept;

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
	/// RISKY. Gets pointer to the private detours list and trusts caller won't abose it.
	/// </summary>
	/// <returns></returns>
	//const std::vector<LPCSTR> GetDetourList();
	const std::vector<std::string> GetDetourList();

	const char* IndexDetourList(int index);
#pragma endregion
#pragma region Spawning The Target and waiting for exist
	/// <summary>
	/// Spawn the process and returns its ID.  Returns 0 if the process did not spawn.
	/// </summary>
	/// <returns></returns>
	DWORD SpawnProcess();

	/// <summary>
	/// Once the process is spawned, wait until it's exited or the elapsed time has passed
	/// </summary>
	/// <returns></returns>
	BOOL WaitUntilExit(DWORD TimeWait, BOOL OnlyMain);
	/// <summary>
	/// Return a pointer to the class that controls the StartupInfoW struct.
	/// </summary>
	/// <returns></returns>
	StartupInfoWrapper* GetStartupInfoHandler();
#pragma endregion


#pragma region Specifying how to interactive with target if debugging.

	VOID SetDebugEventCallback(DebugEventCallBackRoutine Callback);
	DebugEventCallBackRoutine GetDebugEventCallback();
	VOID SetDebugEventCallBackCustomArg(LPVOID Arg);


	/// <summary>
	/// This pulses the event that the worker thread (if existent) waits on.
	/// Pointless if debug events are not in a different thread.
	/// </summary>
	void PulseDebugEventThread();
	/// <summary>
	/// Specify if the worker thread is spawned or not. Non Worker thread code is somewhat lagging and I recommend using the worker thread unless you have a reason not too.
	/// </summary>
	/// <param name="dmMode">See the  PSINFO_DEBUG_MODE constants</param>
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
	/// Returns the commandment's value if exists, otherwise returns false. (YES that means routines outside the class can't tencihally check if exists. 
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
	DWORD SetSymbolStatus(DWORD NewStatus) noexcept;
	/// <summary>
	/// Return if the symbol engine will automatically update in the worker thread
	/// </summary>
	/// <returns></returns>
	DWORD GetSymbolStatus() noexcept;
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
	/// Worker thread routine friend declaration. This actual routine is actual what spawns and pumps the debug message loop if opted into.
	/// -+
	/// 
	/// </summary>
	friend void _cdecl PsPocessInformation_DebugWorkerthread(void* argument);
	
	/// <summary>
	/// Common constructor to setup an initialized instance of class
	/// </summary>
	void init(InsightProcess* target);

	/// <summary>
	/// zero out to default values and free any pointer classes for the target.
	/// </summary>
	void zeroout(InsightProcess* target, bool closehandles, bool killpointers);
	
	/// <summary>
	/// Copy source to target.  DeepCopy means if the source is deleted, target will be ok still.
	/// <summary>
	void dupto(const InsightProcess* source, InsightProcess* target, bool DeepCopy, bool initialized);
	/// <summary>
	///  assign default non allocated pointer values (bools, dwords, ect...) to default and null for pointer based values. Used mainly for move consturctors
	/// </summary>
	void PrivateDefault(InsightProcess *target);

	/// <summary>
	/// for the C++ copy, copy assignment, ect... operators this is the common put my class contents there. 
	/// </summary>
	bool InsightProceeeCPPDuplicate(const InsightProcess* source, InsightProcess* Target, bool closeoriginal);
	/// <summary>
	/// if one of the GetXXXX() routines that fetch memory info is called,  this ends up being called to refresh the private struct
	/// </summary>
	void RefreshMemoryStatistics();
	/// <summary>
	/// Common code for the spawn process public routine. Implementation is here.   Argument is true when the worker thread creates the process but false when the public routine creations the process
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
	/// Future feature container. Will contrain process security attriutes
	/// </summary>
	LPSECURITY_ATTRIBUTES lpProcessAttributes;
	/// <summary>
	/// Future feature containrer. Will contrain threas security attributes
	/// </summary>
	LPSECURITY_ATTRIBUTES lpThreadAttributes;



	/// <summary>
	/// Map for the environment.  MAP Key is environment name,  map value is the string to use>
	/// </summary>
	std::map<std::wstring, std::wstring>* Enviroment;
	/// <summary>
	/// Holds the list of Dlls that will be loaded via detours. 
	/// </summary>
	//std::vector<LPCSTR>* DetoursDll;
	std::vector<std::string>* DetoursDll;

	/// <summary>
	/// Folders that will be searched before other locations.   requres helper dll
	/// </summary>
	std::vector<std::wstring>* LoadLibraryPriorityFolders;

	/// <summary>
	/// When load library rotuine gets part 1, sub with part 2 before proceeding.
	/// </summary>
	std::map<std::wstring, std::wstring>*  ForcedOverwrites;
	/// <summary>
	/// CreateProcessW Creation Flags
	/// </summary>
	DWORD dwCreationFlags;
	/// <summary>
	/// Startup info for the process
	/// </summary>
	StartupInfoWrapper* StartUpInfo;
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


	std::map<DWORD, BOOL> *CommandmentArray;

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
	ThreadContainer* ProcessThreads;
	/// <summary>
	/// Class for the symbol engine. Will Need to spawn a process WITH_DEBUG to get much use.
	/// </summary>
	InsightHunter* Insight;
	/// <summary>
	/// Default false.  If true and Proess creation flags include DEBUG_PROCESS | DEBUG_THIS_PROCESS, we will ask for debug priv via AdjustTokenPriv
	/// </summary>
	bool RequestDebugPriv;
};




