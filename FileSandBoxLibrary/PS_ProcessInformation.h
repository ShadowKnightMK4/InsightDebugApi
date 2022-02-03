#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>

#include "Client.h"
#include "InsightHunter.h"
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
	/// Is the hadnle that the caller needs to hit during their main / gui processing loo to send debug events back to windows
	/// </summary>
	HANDLE EventHandle;
	/// <summary>
	///  Callback provided to examine events and do stuff.  SymbolEngine is automatically handled via the InsightHunter classs provided you opt into the Debug Worker thread
	/// </summary>
	DebugEventCallBackRoutine UserCallback;
	/// <summary>
	/// see msdn's ContinueDebugEvent
	/// </summary>
	DWORD ContinueState;
	/// <summary>
	/// see msdn's ContinueDebugEvet
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

/// Do not spawn the process with a seperate thread.  
#define PSINFO_DEBUGMODE_NOWORKERTHREAD (0)
// SpawnProcess() creates a thread that spawns your target. 
#define PSINFO_DEBUGMODE_WORKERTHREADED (1)





/// <summary>
/// Each process spawned or tracked by the library uses this class. 
/// It's .NET Wrapper is PsProcessInformation locatead in PsProcessInformation.cs of FileSandBox_GUI.
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
	/// <summary>
	/// This pulses the event that the worker thead (if existant) waits on.
	/// Pointless if debug events are not in a different thread.
	/// </summary>
	void PulseDebugEventThread();
	/// <summary>
	/// Specify what to do when
	/// </summary>
	/// <param name="dmMode"></param>
	void SetDebugMode(DWORD dmMode);

	
	DWORD GetDebugMode();


	HANDLE GetMainThreadHandle();
	HANDLE GetMainProcessHandle();
	DWORD GetProcessID();
	DWORD GetThreadID();

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


#pragma region SymbolHandling / InsightHunter
	/// <summary>
	/// If enabled, the worker thread will automaticcaly pass stuff to the symbol class
	/// </summary>
	/// <param name="NewStatus"></param>
	/// <returns></returns>
	DWORD SetSymbolStatus(DWORD NewStatus);
	DWORD GetSymbolStatus();
	/// <summary>
	/// receive a pointer to an instance of the symbol class contained within. 
	/// </summary>
	InsightHunter* GetSymbolHandlerPtr();
#pragma endregion

private:
	/// <summary>
	/// Worker thread rotouine friend declartation. This actual is want spances-
	/// -+
	/// 
	/// </summary>
	friend void _cdecl PsPocessInformation_DebugWorkerthread(void* argument);
	
	
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

	LPSECURITY_ATTRIBUTES lpProcessAttributes;
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
	/// Enable symbol tracking or not.
	/// </summary>
	BOOL EnableSymbols = false;


	/// <summary>
	/// Class for the symbol engine
	/// </summary>
	InsightHunter* Insight;

};




