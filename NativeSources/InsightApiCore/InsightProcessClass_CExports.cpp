
/*
* Defines and exports a C compatable front end (+ constrctor / destructor)
* 
* The majority of these are thin wrappers to the underlying class.
*/
#include "InsightProcess.h"
#include "StartupInfoStructHelper.h"
#include "Utility.h"
#include "InsightHunter.h"
#include <memory>
extern "C"
{
	
	bool WINAPI InsightProcess_WaitForMainProcessExit(InsightProcess* that, DWORD WaitTime)
	{
		return that->WaitUntilExit(WaitTime, TRUE);
	}

	bool WINAPI InsightProcess_WaitForAllProcessExit(InsightProcess* that, DWORD WaitTime)
	{
		return that->WaitUntilExit(WaitTime, FALSE);
	}


	/// <summary>
	/// Exports a constructor for the class
	/// </summary>
	/// <returns></returns>
	InsightProcess* WINAPI InsightProcess_MakeInstance()
	{
		return new InsightProcess();
	}

	StartupInfoWrapper* WINAPI InsightProcess_GetStartupInfoClass(InsightProcess* that)
	{
		if (that)
		{
			return that->GetStartupInfoHandler();
		}
		return nullptr;
	}

	/// <summary>
	/// Exports a copy constructor for the class
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	InsightProcess* WINAPI InsightProcess_DupInstance(InsightProcess* other)
	{
		if (other)
		{
			InsightProcess* ret = new InsightProcess(*other);
			return ret;
		}
		return nullptr;
	}

	/// <summary>
	/// Exports triggering the destructor for the class
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_KillInstance(InsightProcess* that)
	{
		if (that)
		{
			delete that;
		}
	}

	/// <summary>
	/// Assign the process that will be spawned with this class. Pointless after process is spawned.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="ProcessName"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetProcessName(InsightProcess* that, const wchar_t* ProcessName)
	{
		if (that)
		{
			that->ProcessName(ProcessName);
		}
	}

	/// <summary>
	/// return a string containing the process that will be spawned
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	const wchar_t* WINAPI InsightProcess_GetProcessName(InsightProcess* that)
	{
		if (that)
		{
			return that->ProcessName();
		}
		return nullptr;
	}


	/// <summary>
	/// Set the arguments for this process to be spawned
	/// </summary>
	/// <param name="that"></param>
	/// <param name="Argument"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetProcessArgument(InsightProcess* that, const wchar_t* Argument)
	{
		if (that)
		{
			that->ProcessArguments(Argument);
		}
	}

	/// <summary>
	/// Get the arguments that will be passed to the process
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	const wchar_t* WINAPI InsightProcess_GetProcessArgument(InsightProcess* that)
	{
		if (that)
		{
			return that->ProcessArguments();
		}
		return nullptr;
	}

	/// <summary>
	/// Set the CreateProcessW Creation flags for this process
	/// </summary>
	/// <param name="that"></param>
	/// <param name="flags"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetCreationFlags(InsightProcess* that, DWORD flags)
	{
		if (that)
		{
			that->CreationFlags(flags);
		}
	}


	/// <summary>
	/// Set the CreateProcessW flags and add DEBUG_PROCESS flag
	/// </summary>
	/// <param name="that"></param>
	/// <param name="flags"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetCreationFlagDebug(InsightProcess* that, DWORD flags)
	{
		if (that)
		{
			that->CreationFlags(flags || DEBUG_PROCESS);
		}
	}

	/// <summary>
	/// Set the CreateProcessW flags and add DEBUG_ONLY_THIS
	/// </summary>
	/// <param name="that"></param>
	/// <param name="flags"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetCreationFlagDebugOnlyThis(InsightProcess* that, DWORD flags)
	{
		if (that)
		{
			that->CreationFlags(flags || DEBUG_ONLY_THIS_PROCESS);
		}
	}

	/// <summary>
	/// Set the CreateProcessW flags and Add DEBUG_PROCESS + CREATE_SUSPENDED
	/// </summary>
	/// <param name="that"></param>
	/// <param name="flags"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetCreationFlagDebugSuspended(InsightProcess* that, DWORD flags)
	{
		if (that)
		{
			that->CreationFlags(flags | DEBUG_PROCESS | CREATE_SUSPENDED);
		}
	}

	/// <summary>
	/// Set the CreateProceccW flags and add DEBUG_ONLY_THIS_PROCES + CREATE_SUSPENDED
	/// </summary>
	/// <param name="that"></param>
	/// <param name="flags"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetCreationFlagDebugOnlyThisSuspended(InsightProcess* that, DWORD flags)
	{
		if (that)
		{
			that->CreationFlags(((flags | DEBUG_ONLY_THIS_PROCESS) | CREATE_SUSPENDED));
		}
	}
	/// <summary>
	/// Get the CreateProcessW CreationFlags for this process
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	DWORD WINAPI InsightProcess_GetCreationFlags(InsightProcess* that)
	{
		if (that)
		{
			return that->CreationFlags();
		}
		return 0;
	}

	/// <summary>
	/// Set the Current (or working) directory for the process spawned
	/// </summary>
	/// <param name="that"></param>
	/// <param name="NewCD"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetWorkingDirectory(InsightProcess* that, const wchar_t* NewCD)
	{
		if (that)
		{
			that->CurrentDirectory(NewCD);
		}
	}

	/// <summary>
	/// Get the current (or working directory) for the process to spawn.
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	const wchar_t* WINAPI InsightProcess_GetWorkingDirectory(InsightProcess* that)
	{
		if (that)
		{
			return that->CurrentDirectory();
		}
		return nullptr;
	}


	/// <summary>
	/// Specify if you wish to include the default Environment (yours) when Spawning a process.
	/// Values explicitly assigned; however, will overwrite the values received from the default environment.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="WantInherit"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_SetEnvInherit(InsightProcess* that, BOOL WantInherit)
	{
		if (that)
		{
			that->InheritSpawnEnvironment(WantInherit);
		}
	}

	BOOL WINAPI InsightProcess_GetEnvInherit(InsightProcess* that)
	{
		if (that)
		{
			return that->InheritSpawnEnvironment();
		}
		return FALSE;
	}

	/// <summary>
	/// Clear the Explicit Environment Block 
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_ClearEnvBlock(InsightProcess* that)
	{
		if (that)
		{
			that->ClearEnviroment();
		}
	}

	/// <summary>
	/// Assign an explicit environment value to this class's environment block. If you specify an existing value (or something that is inheriting, YOUR value will overwrite it)
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EnvVarName"></param>
	/// <param name="EnvVarValue"></param>
	/// <returns></returns>
	void WINAPI InsightProcess_AssignEnvValue(InsightProcess* that, const wchar_t* EnvVarName, const wchar_t* EnvVarValue)
	{
		if (that)
		{
			if (EnvVarName != nullptr)
			{
				that->Environment(EnvVarName, EnvVarValue);
			}
			
		}
	}

	void WINAPI InsightProcess_ClearEnvValue(InsightProcess* that, const wchar_t* EnvVarName)
	{
		if (that)
		{
			if (EnvVarName != nullptr)
			{
				that->Environment(EnvVarName, nullptr);
			}
		}
	}


	/// <summary>
	/// Returns the value of the passed previously added by PS_ProcessInformation_AssignEnvValue.  Does NOT fetch inherited values from the default environment
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EnvVarName"></param>
	/// <returns></returns>
	const wchar_t* WINAPI InsightProcess_GetEnvValue(InsightProcess* that, const wchar_t* EnvVarName)
	{
		if (that)
		{
			if (EnvVarName)
			{
				return that->Environment(EnvVarName);
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Add a Detours Compatible DLL that the target will be forced to spawn.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="DllName"></param>
	/// <returns></returns>
	BOOL WINAPI InsightProcess_AddDetoursDllA(InsightProcess* that, const char* DllName)
	{
		if (that != nullptr)
		{
			if (DllName != nullptr)
			{
				that->AddDetoursDll(DllName);
				return TRUE;
			}
			
		}
		return FALSE;
	}

	/// <summary>
	/// Reset the Detours List to an empty list.
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	VOID WINAPI InsightProcess_ClearDetoursList(InsightProcess* that)
	{
		if (that != nullptr)
		{
			that->ClearDetoursDll();
		}
	}


	const char* InsightProcess_GetDetourListEntryReadOnly(InsightProcess* that, int index)
	{
		if (that == nullptr)
		{
			return nullptr;
		}
		else
		{
			return that->IndexDetourList(index);
		}
	}
	
	/// <summary>
	/// C Level wrapper that calls PS_ProcessInformation::GetDetourListSize()
	/// </summary>
	/// <param name="that">the this pointer.</param>
	/// <returns>If that=0, returns 0. Otherwise returns results of calling PS_ProcessInformation::GetDetourListSize() </returns>
	unsigned long long InsightProcess_GetDetourListSize(InsightProcess* that)
	{
		if (that == nullptr)
		{
			return 0;
		}
		return that->GetDetourListSize();
	}

	/// <summary>
	/// Convert th DllName to char* and calls PS_ProcessInformation_AddDetoursDllA()
	/// </summary>
	/// <param name="that"></param>
	/// <param name="DllName"></param>
	/// <returns></returns>
	BOOL WINAPI InsightProcess_AddDetoursDllW(InsightProcess* that, const wchar_t* DllName)
	{
		BOOL Ret = FALSE;
		char* Ansi = nullptr;
		if ( (that != nullptr) && (DllName != nullptr) )
		{
			__try
			{
				Ansi = ConvertUnicodeString(DllName);
				if (Ansi)
				{
					Ret = InsightProcess_AddDetoursDllA(that, Ansi);
				}
			}
			__finally
			{
				free(Ansi);
			}
		}
		return Ret;
	}

	/// <summary>
	/// Spawn the process contained with the class.
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	DWORD WINAPI  InsightProcess_SpawnProcess(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->SpawnProcess();
		}
		return 0;
	}
	
	

	void WINAPI InsightProcess_SetDebugMode(InsightProcess* that, DWORD dmMode)
	{
		if (that != nullptr)
		{
			that->SetDebugMode(dmMode);
		}
	}

	DWORD WINAPI InsightProcess_GetDebugMode(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetDebugMode();
		}
		return 0;
	}

	void WINAPI InsightProcess_PulseDebugThread(InsightProcess* that)
	{
		if (that != nullptr)
		{
			that->PulseDebugEventThread();
		}
	}


	void WINAPI InsightProcess_SetDebugEventCallback(InsightProcess* that, DebugEventCallBackRoutine UserRoutine)
	{
		if ((that != nullptr))
		{
			that->SetDebugEventCallback(UserRoutine);
		}
	}


	VOID* WINAPI InsightProcess_GetDebugEventCallback(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetDebugEventCallback();
		}
		else
		{
			return nullptr;
		}
	}

	HANDLE WINAPI InsightProcess_GetMainThreadHandle(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetMainThreadHandle();
		}
		return INVALID_HANDLE_VALUE;
	}

	HANDLE WINAPI InsightProcess_GetMainProcessHandle(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetMainProcessHandle();
		}
		return INVALID_HANDLE_VALUE;
	}

	DWORD WINAPI InsightProcess_GetMainProcessId(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetProcessID();
		}
		return 0;
	}

	DWORD WINAPI InsightProcess_GetMainThreadId(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetProcessID();
		}
		return 0;
	}

	DWORD WINAPI InsightProcess_SetSymbolHandling(InsightProcess* that, DWORD NewStatus)
	{
		if (that != nullptr)
		{
			return that->SetSymbolStatus(NewStatus);
		}
		return 0;
	}

	DWORD WINAPI InsightProcess_GetSymbolHandling(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetSymbolStatus();
		}
			return 0;
	}
	
	InsightHunter* WINAPI InsightProcess_GetSymbolHandlerClass(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetSymbolHandlerPtr();
		}
		return nullptr;
	}


	BOOL WINAPI InsightProcess_AddPriorityDllPathW(InsightProcess* that, LPCWSTR Path)
	{
		if (that != nullptr)
		{
			return that->AddPriorityLoadLibraryPathW(Path);
		}
		return 0;
	}
	
	BOOL WINAPI InsightProcess_AddPriorityDllPathA(InsightProcess* that, LPCSTR Path)
	{
		if (that != nullptr)
		{
			return that->AddPriorityLoadLibraryPathA(Path);
		}
		return 0;
	}

	DWORD WINAPI InsightProcess_GetPriorityDllPath_NumberOf(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetPriorityLoadLibaryPath_NumberOf();
		}
		return 0;
	}

	LPCWSTR WINAPI InsightProcess_IndexPriorityDllPath(InsightProcess* that, int location)
	{
		if (that != nullptr)
		{
			if (location >= 0)
			{
				if ((location < that->GetPriorityLoadLibaryPath_NumberOf()))
				{
					return that->GetPriorityLoadLibraryPath_Index(location);
				}
			}
		}
		return nullptr;
	}

	VOID WINAPI InsightProcess_ClearPriorityDllPath(InsightProcess* that)
	{
		if (that != nullptr)
		{
			that->EmptyPriorityLoadLibaryPath();
		}
	}
	BOOL WINAPI InsightProcess_SetCommandment(InsightProcess* that, DWORD Cmd, BOOL Enabled)
	{
		if (that != nullptr)
		{
			return that->SetCommandment(Cmd, Enabled);
		}
		return FALSE;
	}

	BOOL WINAPI InsightProcess_GetCommandment(InsightProcess* that, DWORD Cmd)
	{
		if (that != nullptr)
		{
			return that->GetCommandment(Cmd);
		}
		return FALSE;
	}

	/*

	PROCESS_MEMORY_COUNTERS_EX* GetMemoryStatsBulkPtr();*/


	PROCESS_MEMORY_COUNTERS_EX* WINAPI InsightProcess_GetMemoryStatsBulkPtr(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetMemoryStatsBulkPtr();
		}
		return nullptr;
	}

	SIZE_T WINAPI InsightProcess_GetPrivateUsage(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetPrivateUsage();
		}
		return 0;
	}




	SIZE_T WINAPI InsightProcess_GetPageFaultCount(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetPageFaultCount();
		}
		return 0;
	}


	SIZE_T WINAPI InsightProcess_GetPeakWorkingSet(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetPeakWorkingSet();
		}
		return 0;
	}

	SIZE_T WINAPI InsightProcess_GetWorkingSetSize(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetWorkingSetSize();
		}
		return 0;
	}

	SIZE_T WINAPI InsightProcess_GetQuotaPeakPagePoolUsage(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaPeakPagePoolUsage();
		}
		return 0;
	}



	SIZE_T WINAPI InsightProcess_GetQuotaPagePoolUsage(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaPagedPoolUsage();
		}
		return 0;
	}



	SIZE_T WINAPI InsightProcess_GetQuotaPeakNonPagePoolUsage(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaPeakNonPageUsage();
		}
		return 0;
	}

	SIZE_T WINAPI InsightProcess_GetQuotNonPagePoolUsage(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetQuotaNonPageUsage();
		}
		return 0;
	}



	SIZE_T WINAPI InsightProcess_GetPageFileUsage(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetPageFileUsage();
		}
		return 0;
	}


	SIZE_T WINAPI InsightProcess_GetPeakPageFileUsage(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetPeakPageFileUsage();
		}
		return 0;
	}

	BOOL WINAPI InsightProcess_RequestDebugPriv(InsightProcess* that, BOOL Yes)
	{
		if (that != nullptr)
		{
			that->SetDebugPrivState(Yes);
			return TRUE;
		}
		return FALSE;
	}



	DWORD WINAPI InsightProcess_GetProcessIDCount(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetProcessIDCount();
		}
		return 0;
	}

	DWORD WINAPI InsightProcess_GetThreadIDCount(InsightProcess* that)
	{
		if (that != nullptr)
		{
			return that->GetThreadListCount(0);
		}
		return 0;
	}

	DWORD WINAPI InsightProcess_GetThreadList(InsightProcess* that, DWORD ProcessID, DWORD* Output, DWORD OutputSize)
	{
		if (that != nullptr) {
			if (Output != nullptr) {
				return that->GetThreadIDs(ProcessID, Output, OutputSize);
			}
			else
			{
				return that->GetThreadListCount() * sizeof(DWORD);
			}
		}
		return 0;
	}

	ThreadInsight* InsightProcess_GetThreadInsightPtr(InsightProcess* that, DWORD ProcessID, DWORD dwThreadID)
	{
		if (that != nullptr)
		{
			
		}
		return 0;
	}




}