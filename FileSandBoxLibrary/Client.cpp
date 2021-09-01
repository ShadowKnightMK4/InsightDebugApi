#include "Client.h"
#include <DbgHelp.h>

PS_ProcessInformation* Client::NewProcess()
{
	PS_ProcessInformation* NewProcess = new PS_ProcessInformation();
	return NewProcess;
}

DWORD Client::SpawnProcess(PS_ProcessInformation* Process)
{
	DWORD ret;
	ret = Process->SpawnProcess();
	return ret;
}
