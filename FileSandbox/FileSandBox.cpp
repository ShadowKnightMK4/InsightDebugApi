

#include <StaticIncludes.h>
#include "ProcessLauncher.h"
#include "DebugEvent.h"
#include "..\\FilesandboxClientDllProtocol\\FileSandboxProtocol.h"
#include "RemoteStructure.h"

//typedef int(_stdcall DebupEventCallbackApi)(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime, LPVOID CustomArg);
#include <iostream>
#include "ProcessContext.h"

int WINAPI ProcessContextHandler(LPDEBUG_EVENT CurEvent, DWORD* ContStat, DWORD* WaitTImer, LPVOID CustomArg)
{
	ProcessContextSingle* Handler = (ProcessContextSingle*)CustomArg;
	int result = 0;
	switch (CurEvent->dwDebugEventCode)
	{
	case CREATE_PROCESS_DEBUG_EVENT:
	{
		if (Handler->CreateProcessDebugEvent(CurEvent))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
	
		break;
	}
	case CREATE_THREAD_DEBUG_EVENT:
	{
		if (Handler->CreateThreadDebugEvent(CurEvent))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		break;
	}
	case EXCEPTION_DEBUG_EVENT:
	{
		result = 1;
		if (Handler->ExceptionDebugEvent(CurEvent, ContStat) == TRUE)
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		break;
	}
	case EXIT_PROCESS_DEBUG_EVENT:
	{
		if (Handler->ExitProcessDebugEvent(CurEvent) )
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		break;
	}
	case EXIT_THREAD_DEBUG_EVENT:
	{
		if (Handler->ExitThreadDebugEvent(CurEvent))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		break;
	}
	case LOAD_DLL_DEBUG_EVENT:
	{
		if (Handler->LoadLibraryDebugEvent(CurEvent))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		break;
	}
	case OUTPUT_DEBUG_STRING_EVENT:
	{
		if (Handler->OutputDebugStringDebugEvent(CurEvent))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		break;
	}
	case RIP_EVENT:
	{
		if (Handler->RipEvent(CurEvent))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		break;
	}
	case UNLOAD_DLL_DEBUG_EVENT:
	{
		if (Handler->UnloadLibraryDebugEvent(CurEvent))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}


		break;
	}

	

	
	default:
		result = 1;
	}
	Handler->FlushAllLogs(std::wcout);
	return result;
}
int WINAPI DebugHandler(LPDEBUG_EVENT CurEvent, DWORD* ContStat, DWORD* WaitTimer, LPVOID CustomArg)
{
	*ContStat = DBG_EXCEPTION_NOT_HANDLED;
	*WaitTimer = INFINITE;
	HANDLE Process = INVALID_HANDLE_VALUE;
	switch(CurEvent->dwDebugEventCode)
	{
		case CREATE_PROCESS_DEBUG_EVENT:
		{
			std::cout << "Process : " << CurEvent->u.CreateProcessInfo.lpImageName << " spawned as PID ( " << CurEvent->dwProcessId << ")" << std::endl;
			return 1;
			break;
		}
		case CREATE_THREAD_DEBUG_EVENT:
		{
			return 1;
			break;
		}
		case EXIT_THREAD_DEBUG_EVENT:
		{
			return 1;
			break;
		}
		case OUTPUT_DEBUG_STRING_EVENT:
		{
			Process = OpenProcess(PROCESS_VM_READ, FALSE, CurEvent->dwProcessId);
			wchar_t* message = RemoteStructureRoutine::RemoteReadDebugString(Process, CurEvent);
			if (message)
			{
				std::wcout << message << std::endl;
				free(message);
			}
			CloseHandle(Process);
			return 1;
			break;
		}
		case EXIT_PROCESS_DEBUG_EVENT:
		{
			std::cout << "Process " << CurEvent->dwProcessId  << "exited with code " << CurEvent->u.ExitProcess.dwExitCode << std::endl;
			return -1;
			break;
		}
		case EXCEPTION_DEBUG_EVENT:
		{
			// check if it's a communication from the helper dll.
			switch (CurEvent->u.Exception.ExceptionRecord.ExceptionCode)
			{
				case SANDBOX_API_FILE_NTCREATEFILE:
				{
					FILESANDBOX_API_EXCEPTION_CLASS MyCopy;
					Process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, CurEvent->dwProcessId);
						if (Process != INVALID_HANDLE_VALUE)
						{
							SIZE_T BytesRead = 0;
							if (ReadProcessMemory(Process, (LPCVOID)CurEvent->u.Exception.ExceptionRecord.ExceptionInformation[0], &MyCopy, sizeof(FILESANDBOX_API_EXCEPTION_CLASS), &BytesRead))
							{
								MyCopy.ACK = 1;
								OBJECT_ATTRIBUTES* TargetResource;
								ULONG_PTR* Arguments;
								Arguments = RemoteStructureRoutine::RemoteReadArray(Process, MyCopy.PackedArguments, MyCopy.PackedArgumentLength);
								TargetResource = RemoteStructureRoutine::RemoteReadObjectAttributes(Process, (LPVOID)Arguments[2], FALSE);
								
								std::wcout << L"Touched File at " << TargetResource->ObjectName->Buffer << std::endl;

								RemoteStructureRoutine::RemoteReadFreeObjectAttributes(TargetResource);
								free(Arguments);
								*ContStat = DBG_EXCEPTION_HANDLED;
								
							}
							else
							{
								*ContStat = DBG_EXCEPTION_NOT_HANDLED;
								
							}
							CloseHandle(Process);
						}
						else
						{
							*ContStat = DBG_EXCEPTION_NOT_HANDLED;
						}
					break;//
				}
				default:
				{
					*ContStat = DBG_EXCEPTION_NOT_HANDLED;
					break;
				}
			}
			break;
		}
		case LOAD_DLL_DEBUG_EVENT:
		{
			break;
		}
		case UNLOAD_DLL_DEBUG_EVENT:
		{
			break;
		}
		case RIP_EVENT:
		{
			break;
		}

		default:
		{
			std::cout << "Event Triggered without handler" << std::endl;
			return DebugEvent::DefaultDebugEventHandling(CurEvent, ContStat, WaitTimer);
			break;
		}

	}
	return 1;
}

int main(int arc, char* argv[])
{

	PROCESS_INFORMATION Info;
	ProcessLauncher Test;
	ProcessContextSingle DebugLogging;
	Test.SetCreationFlags(DEBUG_ONLY_THIS_PROCESS);
	Test.SetApplicationWorkingDirectory("C:\\Windows\\system32");
	
	//Test.SetApplicationName(L"C:\\Windows\\System32\\notepad.exe");
	//Test.SetApplicationName(L"C:\\Users\\Thoma\\source\\repos\\FileSandbox\\Code\\Debug\\x86\\Program\\HelloWorld.exe");
	Test.SetApplicationName(L"C:\\Windows\\System32\\cmd.exe");
	Test.SetDetourDll("C:\\Users\\Thoma\\source\\repos\\FileSandbox\\Code\\Debug\\x86\\Program\\FilesandboxHelperdll.dll");
	Test.Launch();
	Test.GetProcessInformation(&Info);
	DebugEvent* Debug = new DebugEvent(Info.hProcess);
	Test.ResumeMainThread();

	Debug->AskForDebugPriv(TRUE);
	DebugLogging.SetCommandment(COMMAND_STRIP_PROCESS_CREATION);
	Debug->DebugLoop(ProcessContextHandler, &DebugLogging);
	WaitForSingleObject(Info.hProcess, INFINITE);



	return 0;

	/*
	STARTUPINFO Info;
	PROCESS_INFORMATION PInfo;
	ZeroMemory(&Info, sizeof(Info));
	ZeroMemory(&PInfo, sizeof(PInfo));
	wchar_t* Dummy = (wchar_t*) L"          ";
	/*
	DetourCreateProcessWithDllExW(L"C:\\Windows\\System32\\notepad.exe",
		Dummy, 
		nullptr, 
		nullptr,
		false,
		0, 
		nullptr,
		nullptr,
		&Info,
		&PInfo, 
		"C:\\Users\\Thoma\\source\\repos\\FileSandbox\\Code\\Debug\\x86\\Program\\FilesandboxHelperdll.dll",
		nullptr);*/
	return 0;
}