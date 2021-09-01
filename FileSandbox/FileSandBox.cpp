

#include <StaticIncludes.h>
#include "ProcessLauncher.h"

#include "..\\FilesandboxClientDllProtocol\\FileSandboxProtocol.h"
#include "RemoteStructure.h"


//typedef int(_stdcall DebupEventCallbackApi)(LPDEBUG_EVENT CurEvent, DWORD* ContinueState, DWORD* WaitTime, LPVOID CustomArg);
#include <iostream>


#include "..\\FileSandBoxLibrary\FileSandBoxApi.h"
/*
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
*/
int WINAPI DebugHandler(LPDEBUG_EVENT CurEvent, DWORD* ContStat, DWORD* WaitTimer, LPVOID CustomArg)
{
	*ContStat = DBG_EXCEPTION_NOT_HANDLED;
	*WaitTimer = INFINITE;
	HANDLE Process = INVALID_HANDLE_VALUE;
	switch(CurEvent->dwDebugEventCode)
	{
		default:
		{
			std::cout << "Event Triggered without handler" << std::endl;
			*ContStat = DBG_EXCEPTION_NOT_HANDLED;
		//	return DebugEvent::DefaultDebugEventHandling(CurEvent, ContStat, WaitTimer);
			break;
		}

	}
	return 1;
}


int main(int arc, char* argv[])
{
	DWORD Handle = FSStartup(1);
	
	DWORD dwProcessID = FS_SpawnProcessDebugSuspended(Handle, (wchar_t*) L"C:\\Windows\\System32\\notepad.exe", nullptr, 0, nullptr, nullptr, nullptr);
	
	FSDebugLoop(Handle, DebugHandler, 0);

	FSCleanup(Handle);
	return 0;
	/*
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
	*/


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