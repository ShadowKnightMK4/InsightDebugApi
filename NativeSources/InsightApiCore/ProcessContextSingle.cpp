#include "ProcessContext.h"
#include <iostream>
#include <Psapi.h>
#include "RemoteStructure.h"

#include <Shlwapi.h>
#include "Utility.h"


/*
* Older code from a different era in this lifecycle.
* Depcreated but still may find some use.
*/

// Emit The PID of the process the debug event was received from
#define PC_OUTPUT_EMIT_INCLUDE_PID (1)
// Emit the ThreadID of the process the debug event was received from
#define PC_OUTPUT_EMIT_INCLUDE_THREAD (2)
// Emit only the main module name of the process (ex C:\Windows\cmd.exe) would be "cmd.exe" 
#define PC_OUTPUT_EMIT_INCLUDE_MODULENAME (4)
// Emit full path to main module name of the process (ex C:\Windows\cmd.exe) would be "C:\Windows\cmd.exe" 
#define PC_OUTPUT_EMIT_INCLUDE_FULLMODULE (8)

/// <summary>
/// The code to emit the prolog stuff.
/// </summary>
/// <param name="Output">target stream</param>
/// <param name="EmitType">see the PC_OUTPUT_EMIT_XXX defines</param>
/// <param name="Info">Debug Event info</param>
void BoilerPlateEmitPID(std::wstringstream& Output, DWORD EmitType, LPDEBUG_EVENT Info)
{
	DWORD EmitModule = 0;
	if (Info != nullptr)
	{
		if (EmitType == 0)
		{
			Output << L"PID (" << std::dec << Info->dwProcessId << L") Thread (" << std::dec << Info->dwThreadId << L"):: ";
		}
		else
		{
			if ((EmitType) & (PC_OUTPUT_EMIT_INCLUDE_PID))
			{
				Output << L"PID (" << std::dec << Info->dwProcessId << L") ";
			}
			if ((EmitType & PC_OUTPUT_EMIT_INCLUDE_MODULENAME))
			{
				EmitModule++;
			}

			if ((EmitType & PC_OUTPUT_EMIT_INCLUDE_FULLMODULE))
			{
				EmitModule++;
			}

			if (EmitModule != 0)
			{
				wchar_t* ModuleName = nullptr;
				wchar_t* BackedModuleName = nullptr;
				HANDLE Process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, Info->dwProcessId);

				if (EmitModule >= 2)
				{
					if (Process != INVALID_HANDLE_VALUE)
					{
						BackedModuleName = ModuleName = GetModuleNameViaHandle(Process, nullptr);
					}
				}
				else
				{
					if (EmitModule > 0)
					{
						if (Process != INVALID_HANDLE_VALUE)
						{
							BackedModuleName = GetModuleNameViaHandle(Process, nullptr);

							if (BackedModuleName != nullptr)
							{
								int length = wcslen(BackedModuleName);
								if (length < MAX_PATH)
								{
									ModuleName = PathFindFileName(BackedModuleName);
								}
								else
								{
									//  TOO BIG; we do it outselfs;
									ModuleName = BackedModuleName;
									for (int stepper = 0; stepper < length; stepper++)
									{
										if ((BackedModuleName[stepper] == '\\') || (BackedModuleName[stepper] == '/'))
										{
											ModuleName = &BackedModuleName[stepper];
										}
									}
									if (ModuleName != BackedModuleName)
									{
										ModuleName++;
									}
								}

							}

						}
					}
				}

				if ((ModuleName != nullptr) && (BackedModuleName != nullptr))
				{
					Output << ModuleName;
					free(BackedModuleName);
				}

				Output << L":: ";
				if ((Process != 0) && (Process != INVALID_HANDLE_VALUE))
				{
					CloseHandle(Process);
				}
			}
		}
	}
}

BOOL ProcessContextSingle::CreateProcessDebugEvent(LPDEBUG_EVENT Event)
{
	wchar_t* process;
	HANDLE ProcessHandle;
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT)
		{
			BoilerPlateEmitPID(CreateProcessLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
			CreateProcessLog << L"Spawned " << std::endl;
			// I AM UNSURE if assumtion is correct but when we receive a createprocess event, there probably shouldn't already be an entry.
			if (this->ProcessIDDllNameToModule.find(Event->dwProcessId) != this->ProcessIDDllNameToModule.end())
			{
				CreateProcessLog << L"(Warning: PID matches known existing PID) ";
			}
			else
			{
				auto NewCollection = this->ProcessIDDllNameToModule[Event->dwProcessId];
			}
			return TRUE;
			/*
			if (Event->u.CreateProcessInfo.hFile == NULL)
			{
				ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, Event->dwProcessId);

				if (ProcessHandle != INVALID_HANDLE_VALUE)
				{
					process = GetModuleNameViaHandle(ProcessHandle, nullptr);
					if (process != nullptr)
					{
						this->ConsoleLog << L"Process Spawned PID (" << Event->dwProcessId << L") from Module at " << process << std::endl;
						free(process);
					}
					CloseHandle(ProcessHandle);
				}
				else
				{
					this->ConsoleLog << L"Could not resolve starting main program location for PID (" << Event->dwProcessId << L") "  << std::endl;
				}
			}
			else
			{

				process = GetFileNameViaHandle(Event->u.CreateProcessInfo.hFile);
				CloseHandle(Event->u.CreateProcessInfo.hFile);
				this->ConsoleLog << L"Process Spawned PID (" << Event->dwProcessId << L") from Module at " << process << std::endl;
				free(process);
				return TRUE;
			}*/
		}
	}
	return FALSE;
}

BOOL ProcessContextSingle::CreateThreadDebugEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT)
		{
			BoilerPlateEmitPID(CreateThreadLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
			CreateThreadLog << L"Thread 0x" << std::hex << Event->dwThreadId << std::dec << L" (" << Event->dwThreadId << L") Spawned" << std::endl;

			if (this->ProcessIDtoThreadIDS.find(Event->dwProcessId) == this->ProcessIDtoThreadIDS.end())
			{
				auto NewList = new std::vector<DWORD>();
				NewList->insert(NewList->end(), { Event->dwThreadId });
				//	this->ProcessIDtoThreadIDS[Event->dwProcessId].insert(this->ProcessIDtoThreadIDS.end(), NewList);
			}
			else
			{
				auto UsedList = this->ProcessIDtoThreadIDS[Event->dwProcessId];
				//		UsedList.insert(UsedList.end(), { Event->dwThreadId });
			}

			//this->ConsoleLog << L"Thead (" << Event->dwThreadId << L")Spawned" << std::endl;
			return TRUE;
		}
	}
	return FALSE;
}

/// <summary>
/// Handle Exceptions. 
/// </summary>
/// <param name="Event"></param>
/// <returns></returns>
BOOL ProcessContextSingle::ExceptionDebugEvent(LPDEBUG_EVENT Event, DWORD* ContStatus)
{
	// holds return value
	BOOL Return = FALSE;
	// for Exceptions that we know off (the dllhelper stuff) we grab and set Virtual Access Info.
	MEMORY_BASIC_INFORMATION Info;
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
		{
			FILESANDBOX_API_EXCEPTION_CLASS MyExceptionSEH;
			HANDLE Process = INVALID_HANDLE_VALUE;
			DWORD BytesRead = 0;
			BOOL MemoryBackup = FALSE;
			memset(&MyExceptionSEH, 0, sizeof(MyExceptionSEH));

			if (ContStatus != nullptr)
			{
				Process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, Event->dwProcessId);

				if (Process != INVALID_HANDLE_VALUE)
				{

					// check for communicate from the helper dll
					switch (Event->u.Exception.ExceptionRecord.ExceptionCode)
					{
					case SANDBOX_API_REG_RESOURCE:
					case SANDBOX_API_FILE_NTCREATEFILE:
					case SANDBOX_API_PROCESS_CREATEUSERPROCESS:
					{
						BoilerPlateEmitPID(DllHelperLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
						if (VirtualQueryEx(Process, (LPCVOID)Event->u.Exception.ExceptionRecord.ExceptionInformation[0], &Info, sizeof(MyExceptionSEH)))
						{
							MemoryBackup = TRUE;
						}

						if (ReadProcessMemory(Process, (LPCVOID)Event->u.Exception.ExceptionRecord.ExceptionInformation[0], &MyExceptionSEH, sizeof(MyExceptionSEH), &BytesRead))
						{
							switch (Event->u.Exception.ExceptionRecord.ExceptionCode)
							{
							case SANDBOX_API_REG_RESOURCE:
							{
								break;
							}
							case SANDBOX_API_FILE_NTCREATEFILE:
							{
								break;
							}
							case SANDBOX_API_PROCESS_CREATEUSERPROCESS:
							{

								PRTL_USER_PROCESS_PARAMETERS Who;
								ULONG_PTR* ArgumentList;

								this->DllHelperLog << L"Attempt to Spawn Process. ";

								ArgumentList = RemoteStructureRoutine::RemoteReadArray(Process, (LPVOID)MyExceptionSEH.PackedArguments, MyExceptionSEH.PackedArgumentLength);
								if (ArgumentList != nullptr)
								{
									Who = RemoteStructureRoutine::RemoteReadUserProcessParameters(Process, (LPVOID*)RemoteStructureRoutine::RemoteReadPointer(Process, (VOID*)ArgumentList[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSPARAMETERS], sizeof(VOID*)));
									if (Who != nullptr)
									{

										if (this->CurrentCommands && (COMMAND_STRIP_PROCESS_CREATION))
										{
											DebugStringLog << L"(Command Broken) Attempt to Spawn \"" << Who->ImagePathName.Buffer << L"\" with arguments \"" << Who->ImagePathName.Buffer << L"\" will be blocked" << std::endl;
											MyExceptionSEH.ACK = 1;
											MyExceptionSEH.SpecialHandling = SANDBOX_SPECIAL_CASE_SHORTIT;
											MyExceptionSEH.SpecialHandlingArg = ERROR_ACCESS_DENIED;
											Return = TRUE;
										}
										else
										{
											DebugStringLog << L"is Launching Process at \"" << Who->ImagePathName.Buffer << L"\" with arguments \"" << Who->CommandLine.Buffer << std::endl;
											MyExceptionSEH.ACK = 1;
										}

										RemoteStructureRoutine::RemoteFreeUserProcessParameters(Who);
									}
									else
									{
										this->DllHelperLog << L"(Failed to retrieve info from debugged process)" << std::endl;
										MyExceptionSEH.ACK = 1;
									}
									free(ArgumentList);
								}
								else
								{
									this->DllHelperLog << L"(Failed to retrieve argment list from debugged process)" << std::endl;
								}
								break;

							}

							}

							DWORD OldProtect;
							{
								if (MemoryBackup)
								{
									if (Info.Protect != PAGE_READWRITE)
									{

										if (VirtualProtectEx(Process, (LPVOID)Event->u.Exception.ExceptionRecord.ExceptionInformation[0], sizeof(MyExceptionSEH), PAGE_READWRITE, &OldProtect))
										{

										}
									}
								}

								SIZE_T BytesWritten = 0;
								if (!WriteProcessMemory(Process, (LPVOID)Event->u.Exception.ExceptionRecord.ExceptionInformation[0], &MyExceptionSEH, sizeof(MyExceptionSEH), &BytesRead))
								{
									this->DllHelperLog << L"Failed to Comit Changnes to Remote Process (WriteProcess GetLastError is " << std::hex << GetLastError() << L"Code will procedure as original." << std::endl;
								}


							}

						}

						*ContStatus = DBG_EXCEPTION_HANDLED;
						Return = TRUE;
						break;
					}
					default:
					{
						*ContStatus = DBG_EXCEPTION_NOT_HANDLED;
						Return = TRUE;
					}
					}
					CloseHandle(Process);
				}

			}
		}
	}
	return Return;
}

BOOL ProcessContextSingle::ExitProcessDebugEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
		{
			BoilerPlateEmitPID(this->ExitProcessLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
			this->ExitProcessLog << L"Exited with Code 0x" << std::hex << Event->u.ExitProcess.dwExitCode << L" (" << std::dec << Event->u.ExitProcess.dwExitCode << L") " << std::endl;

			if (this->ProcessIDDllNameToModule.find(Event->dwProcessId) != this->ProcessIDDllNameToModule.end())
			{
				auto ModuleList = this->ProcessIDDllNameToModule[Event->dwProcessId];
				for (auto StartOfModuleList = ModuleList.begin(); StartOfModuleList != ModuleList.end(); ++StartOfModuleList)
				{

				}
				ProcessIDDllNameToModule.erase(Event->dwProcessId);
			}


			//this->ConsoleLog << L"Process ID " << Event->dwProcessId << L"Exited with return code of " << Event->u.ExitProcess.dwExitCode << std::endl;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ProcessContextSingle::ExitThreadDebugEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == EXIT_THREAD_DEBUG_EVENT)
		{
			BoilerPlateEmitPID(ExitThreadLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
			ExitThreadLog << L"Thread 0x" << std::hex << Event->dwThreadId << std::hex << L" (" << Event->dwThreadId << L") Exited with Return Code of " << std::dec << Event->u.ExitThread.dwExitCode << std::endl;

			if (this->ProcessIDtoThreadIDS.find(Event->dwProcessId) != this->ProcessIDtoThreadIDS.end())
			{

				auto ThreadList = ProcessIDtoThreadIDS.find(Event->dwProcessId);
				ThreadList->second.erase(std::find(ThreadList->second.begin(), ThreadList->second.end(), Event->dwThreadId));

				this->ProcessIDtoThreadIDS.erase(Event->dwProcessId);
			}
			//this->ConsoleLog << L"Process ID " << Event->dwProcessId << L"'s thread ID " << Event->dwThreadId << L" Exited with return code of " << Event->u.ExitThread.dwExitCode << std::endl;
			return TRUE;
		}
	}
	return FALSE;
}

constexpr auto DEFAULT_DLLNAME = L"(Undefined/Unknown)";
BOOL ProcessContextSingle::LoadLibraryDebugEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
		{
			BoilerPlateEmitPID(LoadDllLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
			wchar_t* dllname = (wchar_t*)DEFAULT_DLLNAME;
			HANDLE Process = OpenProcess(PROCESS_QUERY_INFORMATION, false, Event->dwProcessId);
			if (Process != INVALID_HANDLE_VALUE)
			{
				dllname = GetModuleNameViaHandle(Process, (HMODULE)Event->u.LoadDll.lpBaseOfDll);
				if (dllname == nullptr)
				{
					if ((Event->u.LoadDll.hFile != nullptr) && ((Event->u.LoadDll.hFile != INVALID_HANDLE_VALUE)))
					{
						dllname = GetFileNameViaHandle(Event->u.LoadDll.hFile);
						CloseHandle(Event->u.LoadDll.hFile);
					}
				}
				else
				{
					dllname = (wchar_t*)DEFAULT_DLLNAME;
				}
				CloseHandle(Process);
			}
			else
			{
				dllname = (wchar_t*)DEFAULT_DLLNAME;
			}
			LoadDllLog << L"Dll Loaded \"" << dllname << L"\" into memory at 0x" << std::hex << Event->u.LoadDll.lpBaseOfDll << std::endl;

			std::pair<HMODULE, std::wstring> NewEntry;
			NewEntry.first = (HMODULE)Event->u.LoadDll.lpBaseOfDll;
			NewEntry.second.assign(dllname);

			if (ProcessIDDllNameToModule.find(Event->dwProcessId) == ProcessIDDllNameToModule.end())
			{
				auto NewList = std::map<HMODULE, std::wstring>();


				NewList.insert(NewList.end(), NewEntry);
				ProcessIDDllNameToModule.insert({ Event->dwProcessId, NewList });
			}
			else
			{
				ProcessIDDllNameToModule[Event->dwProcessId].insert({ NewEntry });
			}


			if ((dllname != DEFAULT_DLLNAME) && (dllname != nullptr))
			{
				free(reinterpret_cast<wchar_t*>(dllname));
			}

			return TRUE;

		}
	}
	return FALSE;
}

BOOL ProcessContextSingle::OutputDebugStringDebugEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == OUTPUT_DEBUG_STRING_EVENT)
		{


			wchar_t* stringData = nullptr;
			HANDLE Process = INVALID_HANDLE_VALUE;
			Process = OpenProcess(PROCESS_VM_READ, FALSE, Event->dwProcessId);
			if (Process != INVALID_HANDLE_VALUE)
			{
				stringData = RemoteStructureRoutine::RemoteReadDebugString(Process, Event);
				CloseHandle(Process);
				BoilerPlateEmitPID(DebugStringLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);

				this->DebugStringLog << L"Debug String Message \"" << stringData << L"\"" << std::endl;
				free(stringData);
			}
			else
			{
				this->DebugStringLog << L"An output debug string event happened but the string was not accessable in the debugged process." << std::endl;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ProcessContextSingle::UnloadLibraryDebugEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == UNLOAD_DLL_DEBUG_EVENT)
		{
			BoilerPlateEmitPID(this->UnloadDllLog, PC_OUTPUT_EMIT_INCLUDE_PID || PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
			UnloadDllLog << L"Dll Unloaded in memory location" << std::hex << Event->u.UnloadDll.lpBaseOfDll << std::endl;

			auto HModuleCollection = this->ProcessIDDllNameToModule[Event->dwProcessId];
			if (HModuleCollection.empty() == false)
			{
				for (auto Stepper = HModuleCollection.begin(); Stepper != HModuleCollection.end(); ++Stepper)
				{

				}
				HModuleCollection.clear();

			}
			this->ProcessIDDllNameToModule.erase(Event->dwProcessId);
			/*
			HANDLE Process;
			wchar_t* DllName = nullptr;
			Process = OpenProcess(PROCESS_QUERY_INFORMATION, false, Event->dwProcessId);
			if (Process != INVALID_HANDLE_VALUE)
			{
				DllName = GetModuleNameViaHandle(DllName, (HMODULE)Event->u.UnloadDll.lpBaseOfDll);

					this->ConsoleLog << "Process ID #" << Event->dwProcessId << L"has unloaded module " << Event->u.UnloadDll.lpBaseOfDll;

					if (DllName != nullptr)
					{
						this->ConsoleLog << L"Previously loaded from " << DllName << std::endl;
						free(DllName);
					}
					else
					{

					}

				CloseHandle(Process);
			}
			return TRUE;
			*/
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ProcessContextSingle::RipEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == RIP_EVENT)
		{
			BoilerPlateEmitPID(RipLog, PC_OUTPUT_EMIT_INCLUDE_PID | PC_OUTPUT_EMIT_INCLUDE_MODULENAME, Event);
			RipLog << L"RipEvent:: Error Code: (0x" << std::hex << Event->u.RipInfo.dwError << L") Happened outside of debugger control" << std::endl;
			if (Event->u.RipInfo.dwType != 0)
			{
				switch (Event->u.RipInfo.dwType)
				{
				case SLE_ERROR:
					RipLog << L"Invalid Data passed to a function that caused failure." << std::endl;
					break;
				case SLE_MINORERROR:
					RipLog << L"Invalid Data passed to a function but the error likely won't cause failure." << std::endl;
					break;
				case SLE_WARNING:
					RipLog << L"Invalid data was pased to function but the function completed ok." << std::endl;
					break;
				default:
					RipLog << L"Unknown Additional Error Info  (DebugEvent.u.RipEvent.dwType) is " << std::hex << Event->u.RipInfo.dwType << L"." << std::endl;
					break;
				}
			}
		}
	}
	return FALSE;
}

BOOL ProcessContextSingle::SetCommandment(DWORD CommandType)
{
	this->CurrentCommands = CommandType;
	return TRUE;
}

void ProcessContextSingle::FlushConsoleLog(std::wostream& Output)
{
	/*
	ConsoleLog.seekp(0);
	Output.write(ConsoleLog.str().c_str(), ConsoleLog.str().size());
	Output.flush();
	ConsoleLog.clear();
	ConsoleLog.str(std::wstring());
	*/
	Output << ConsoleLog.str();
	ConsoleLog.clear();
	ConsoleLog.str(std::wstring());
}

void ProcessContextSingle::FlushCreateProcessLog(std::wostream& Output)
{
	Output << CreateProcessLog.str();
	CreateProcessLog.clear();
	CreateProcessLog.str(std::wstring());
}

void ProcessContextSingle::FlushCreateThreadLog(std::wostream& Output)
{
	Output << CreateThreadLog.str();
	CreateThreadLog.clear();
	CreateThreadLog.str(std::wstring());
}

void ProcessContextSingle::FlushExceptionLog(std::wostream& Output)
{
	Output << ExceptionLog.str();
	ExceptionLog.clear();
	ExceptionLog.str(std::wstring());
}

void ProcessContextSingle::FlushDllHelperLog(std::wostream& Output)
{
	Output << DllHelperLog.str();
	DllHelperLog.clear();
	DllHelperLog.str(std::wstring());
}

void ProcessContextSingle::FlushExitProcessLog(std::wostream& Output)
{
	Output << ExitThreadLog.str();
	ExitThreadLog.clear();
	ExitThreadLog.str(std::wstring());
}

void ProcessContextSingle::FlushExitThreadLog(std::wostream& Output)
{
	Output << ExitThreadLog.str();
	ExitThreadLog.clear();
	ExitThreadLog.str(std::wstring());
}

void ProcessContextSingle::FlushLoadDllLog(std::wostream& Output)
{
	Output << LoadDllLog.str();
	LoadDllLog.clear();
	LoadDllLog.str(std::wstring());
}

void ProcessContextSingle::FlushDebugLog(std::wostream& Output)
{
	/*
	DebugStringLog.seekp(0);
	Output.write(DebugStringLog.str().c_str(), DebugStringLog.str().size());
	Output.flush();
	DebugStringLog.clear();
	DebugStringLog.str(std::wstring());
	*/
	Output << DebugStringLog.str();
	DebugStringLog.clear();
	DebugStringLog.str(std::wstring());
}

void ProcessContextSingle::FlushUnloadDllLog(std::wostream& Output)
{
	Output << UnloadDllLog.str();
	UnloadDllLog.clear();
	UnloadDllLog.str(std::wstring());
}

void ProcessContextSingle::FlushRipLog(std::wostream& Output)
{
	Output << RipLog.str();
	RipLog.clear();
	RipLog.str(std::wstring());

}

/// <summary>
/// Flush all individual logs to the ConsoleLog first, then write that to output.
/// </summary>
/// <param name="Output"></param>
void ProcessContextSingle::FlushAllLogs(std::wostream& Output)
{
	FlushDebugLog(ConsoleLog);
	FlushCreateProcessLog(ConsoleLog);
	FlushCreateProcessLog(ConsoleLog);
	FlushCreateThreadLog(ConsoleLog);
	FlushExitProcessLog(ConsoleLog);
	FlushExitThreadLog(ConsoleLog);
	FlushExceptionLog(ConsoleLog);
	FlushDllHelperLog(ConsoleLog);
	FlushLoadDllLog(ConsoleLog);
	FlushUnloadDllLog(ConsoleLog);
	FlushRipLog(ConsoleLog);
	FlushConsoleLog(Output);
}

const wchar_t* ProcessContextSingle::MapHmoduleToName(DWORD dwProcessId, HMODULE ID)
{
	auto Index = this->ProcessIDDllNameToModule[dwProcessId];
	if (Index.find(ID) != Index.end())
	{
		return Index[ID].c_str();
	}
	return nullptr;

}

std::vector<DWORD> ProcessContextSingle::NewDwordVector()
{
	return std::vector<DWORD>();
}

std::map<HMODULE, std::wstring> ProcessContextSingle::NewHmoduleMap()
{
	return std::map<HMODULE, std::wstring>();
}

