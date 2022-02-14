
/*
* Defines and exports a C compatable front end (+ constrctor / destructor)
* 
* The majority of these are thin wrappers to the underlying class.
*/
#include "PS_ProcessInformation.h"
#include "Utility.h"
#include "InsightHunter.h"
extern "C"
{

	/// <summary>
	/// Exports a constructor for the class
	/// </summary>
	/// <returns></returns>
	PS_ProcessInformation* WINAPI PS_ProcessInformation_MakeInstance()
	{
		return new PS_ProcessInformation();
	}

	/// <summary>
	/// Exports a copy constructor for the class
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	PS_ProcessInformation* WINAPI PS_ProcessInformation_DupInstance(PS_ProcessInformation* other)
	{
		if (other)
		{
			PS_ProcessInformation* ret = new PS_ProcessInformation(*other);
			return ret;
		}
		return nullptr;
	}

	/// <summary>
	/// Exports triggereing the destructor for the class
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	void WINAPI PS_ProcessInformation_KillInstance(PS_ProcessInformation* that)
	{
		if (that)
		{
			delete that;
		}
	}

	/// <summary>
	/// Assign the process that will be spawned with this class
	/// </summary>
	/// <param name="that"></param>
	/// <param name="ProcessName"></param>
	/// <returns></returns>
	void WINAPI PS_ProcessInformation_SetProcessName(PS_ProcessInformation* that, const wchar_t* ProcessName)
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
	const wchar_t* WINAPI PS_ProcessInformation_GetProcessName(PS_ProcessInformation* that)
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
	void WINAPI PS_ProcessInformation_SetProcessArgument(PS_ProcessInformation* that, const wchar_t* Argument)
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
	const wchar_t* WINAPI PS_ProcessInformation_GetProcessArgument(PS_ProcessInformation* that)
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
	void WINAPI PS_ProcessInformation_SetCreationFlags(PS_ProcessInformation* that, DWORD flags)
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
	void WINAPI PS_ProcessInformation_SetCreationFlagDebug(PS_ProcessInformation* that, DWORD flags)
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
	void WINAPI PS_ProcessInformation_SetCreationFlagDebugOnlyThis(PS_ProcessInformation* that, DWORD flags)
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
	void WINAPI PS_ProcessInformation_SetCreationFlagDebugSuspended(PS_ProcessInformation* that, DWORD flags)
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
	void WINAPI PS_ProcessInformation_SetCreationFlagDebugOnlyThisSuspended(PS_ProcessInformation* that, DWORD flags)
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
	DWORD WINAPI PS_ProcessInformation_GetCreationFlags(PS_ProcessInformation* that)
	{
		if (that)
		{
			return that->CreationFlags();
		}
		return 0;
	}

	/// <summary>
	/// Set the Current (or working) diretory for the process spawned
	/// </summary>
	/// <param name="that"></param>
	/// <param name="NewCD"></param>
	/// <returns></returns>
	void WINAPI PS_ProcessInformation_SetWorkingDirectory(PS_ProcessInformation* that, const wchar_t* NewCD)
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
	const wchar_t* WINAPI PS_ProcessInformation_GetWorkingDirectory(PS_ProcessInformation* that)
	{
		if (that)
		{
			return that->CurrentDirectory();
		}
		return nullptr;
	}


	/// <summary>
	/// Specifiy if you wish to include the default Enviroment (yours) when Spawning a process.
	/// Values you explicity assign, however, will overwrite the values loaded.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="WantInherit"></param>
	/// <returns></returns>
	void WINAPI PS_ProcessInformation_SetEnvInherit(PS_ProcessInformation* that, BOOL WantInherit)
	{
		if (that)
		{
			that->ImportSpawnerEnviroment(WantInherit);
		}
	}

	/// <summary>
	/// Clear the Explicit Envirovnment Block 
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	void WINAPI PS_ProcessInformation_ClearEnvBlock(PS_ProcessInformation* that)
	{
		if (that)
		{
			that->ClearEnviroment();
		}
	}

	/// <summary>
	/// Assign an explicit enviroment value to this class's enviroment block. If you specify an existing value (or something that is inheriting, YOUR value will overwrite it)
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EnvVarName"></param>
	/// <param name="EnvVarValue"></param>
	/// <returns></returns>
	void WINAPI PS_ProcessInformation_AssignEnvValue(PS_ProcessInformation* that, const wchar_t* EnvVarName, const wchar_t* EnvVarValue)
	{
		if (that)
		{
			if (EnvVarName != nullptr)
			{
				that->Environment(EnvVarName, EnvVarValue);
			}
			
		}
	}


	/// <summary>
	/// Returns the value of the passed env prevously added by PS_ProcessInformation_AssignEnvValue.  Does NOT fetch inherited values from the default enviroment
	/// </summary>
	/// <param name="that"></param>
	/// <param name="EnvVarName"></param>
	/// <returns></returns>
	const wchar_t* WINAPI PS_ProcessInformation_GetEnvValue(PS_ProcessInformation* that, const wchar_t* EnvVarName)
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
	/// Add a Detours Compatiable Dll that the target will be forced to spawn.
	/// </summary>
	/// <param name="that"></param>
	/// <param name="DllName"></param>
	/// <returns></returns>
	BOOL WINAPI PS_ProcessInformation_AddDetoursDllA(PS_ProcessInformation* that, const char* DllName)
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
	VOID WINAPI PS_ProcessInformation_ClearDetoursList(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			that->ClearDetoursDll();
		}
	}


	const char* PS_ProcessInformation_GetDetourListEntryReadOnly(PS_ProcessInformation* that, int index)
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
	
	DWORD PS_ProcessInformation_GetDetourListSize(PS_ProcessInformation* that)
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
	BOOL WINAPI PS_ProcessInformation_AddDetoursDllW(PS_ProcessInformation* that, const wchar_t* DllName)
	{
		BOOL Ret = FALSE;
		char* Ansi = nullptr;
		if ( (that != nullptr) && (DllName != nullptr) )
		{
				Ansi = ConvertUnicodeString(DllName);
				if (Ansi)
				{
					Ret = PS_ProcessInformation_AddDetoursDllA( that, Ansi);
					free(Ansi);
				}
		}
		return Ret;
	}

	/// <summary>
	/// Spawn the processs contained with the the class.
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	DWORD WINAPI  PS_ProcessInformation_SpawnProcess(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->SpawnProcess();
		}
		return 0;
	}
	
	

	void WINAPI PS_ProcessInformation_SetDebugMode(PS_ProcessInformation* that, DWORD dmMode)
	{
		if (that != nullptr)
		{
			that->SetDebugMode(dmMode);
		}
	}

	DWORD WINAPI PS_ProcessInformation_GetDebugMode(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetDebugMode();
		}
		return 0;
	}

	void WINAPI PS_ProcessInformation_PulseDebugThread(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			that->PulseDebugEventThread();
		}
	}


	void WINAPI PS_ProcessInformation_SetDebugEventCallback(PS_ProcessInformation* that, DebugEventCallBackRoutine UserRoutine)
	{
		if ((that != nullptr))
		{
			that->SetDebugEventCallback(UserRoutine);
		}
	}


	VOID* WINAPI PS_ProcessInformation_GetDebugEventCallback(PS_ProcessInformation* that)
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

	HANDLE WINAPI Ps_ProcessInformation_GetMainThreadHandle(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetMainThreadHandle();
		}
		return INVALID_HANDLE_VALUE;
	}

	HANDLE WINAPI Ps_ProcessInformation_GetMainProcessHandle(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetMainProcessHandle();
		}
		return INVALID_HANDLE_VALUE;
	}

	DWORD WINAPI Ps_ProcessInformation_GetMainProcessId(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetProcessID();
		}
		return 0;
	}

	DWORD WINAPI Ps_ProcessInformation_GetMainThreadId(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetProcessID();
		}
		return 0;
	}

	DWORD WINAPI Ps_ProcessInformation_SetSymbolHandling(PS_ProcessInformation* that, DWORD NewStatus)
	{
		if (that != nullptr)
		{
			return that->SetSymbolStatus(NewStatus);
		}
		return 0;
	}

	DWORD WINAPI Ps_ProcessInformation_GetSymbolHandling(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetSymbolStatus();
		}
			return 0;
	}
	
	InsightHunter* WINAPI Ps_ProcessInformation_GetSymbolHandlerClass(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetSymbolHandlerPtr();
		}
		return nullptr;
	}


	BOOL WINAPI Ps_ProcessInformation_AddPriorityDllPathW(PS_ProcessInformation* that, LPCWSTR Path)
	{
		if (that != nullptr)
		{
			return that->AddPriorityLoadLibraryPathW(Path);
		}
		return 0;
	}
	
	BOOL WINAPI Ps_ProcessInformation_AddPriorityDllPathA(PS_ProcessInformation* that, LPCSTR Path)
	{
		if (that != nullptr)
		{
			return that->AddPriorityLoadLibraryPathA(Path);
		}
		return 0;
	}

	DWORD WINAPI Ps_ProcessInformation_GetPriorityDllPath_NumberOf(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			return that->GetPriorityLoadLibaryPath_NumberOf();
		}
		return 0;
	}

	LPCWSTR WINAPI Ps_ProcessInformation_IndexPriorityDllPath(PS_ProcessInformation* that, int location)
	{
		if (that != nullptr)
		{
			if ((that->GetPriorityLoadLibaryPath_NumberOf() < location) && (location >= 0))
			{
				return that->GetPriorityLoadLibraryPath_Index(location);
			}
		}
		return nullptr;
	}

	VOID WINAPI Ps_ProcessInformation_ClearPriorityDllPath(PS_ProcessInformation* that)
	{
		if (that != nullptr)
		{
			that->EmptyPriorityLoadLibaryPath();
		}
	}
	BOOL WINAPI Ps_ProcessInformation_SetCommandment(PS_ProcessInformation* that, DWORD Cmd, BOOL Enabled)
	{
		if (that != nullptr)
		{
			return that->SetCommandment(Cmd, Enabled);
		}
		return FALSE;
	}

	BOOL WINAPI Ps_ProcesInformation_GetCommandment(PS_ProcessInformation* that, DWORD Cmd)
	{
		if (that != nullptr)
		{
			return that->GetCommandment(Cmd);
		}
		return FALSE;
	}

	
}