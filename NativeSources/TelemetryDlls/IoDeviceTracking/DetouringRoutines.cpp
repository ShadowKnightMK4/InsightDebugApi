#include <Windows.h>
#include "OriginalRoutinePts.h"
HMODULE Kernel32;
bool DetourTargetRoutines()
{
	Kernel32 = LoadLibrary(L"Kernel32.dll");
	if (Kernel32 != nullptr)
	{
		OriginalCloseHandle =  (CloseHandlePtr) GetProcAddress(Kernel32, "CloseHandle");
		OriginalCreateFileW = (CreateFileWPtr)GetProcAddress(Kernel32, "CreateFileW");
		OriginalCreateFileA = (CreateFileAPtr)GetProcAddress(Kernel32, "CreateFileA");
		OriginalCreateFileTransactedA = (CreateFileTransactedAPtr)GetProcAddress(Kernel32, "CreateFileTransactedA");
		OriginalCreateFileTransactedW = (CreateFileTransactedWPtr)GetProcAddress(Kernel32, "CreateFileTransactedW");
		OriginalCreateFile2 = (CreateFile2Ptr)GetProcAddress(Kernel32, "CreateFile2Ptr");
		return true;
	}
	return false;
}