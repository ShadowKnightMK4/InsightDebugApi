#include <Windows.h>
#include "OriginalRoutinePts.h"
#include "DetouredReplacementRoutines.h"
#include "TelemetrySupport.h"

#include <detours.h>
#include <string>
#include <sstream>

#include "Headers/StringConstants.h"
HMODULE Kernel32;
HMODULE Ntdll;



bool DetourTargetRoutines()
{
	return false;
}