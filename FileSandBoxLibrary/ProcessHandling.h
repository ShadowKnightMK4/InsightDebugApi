#pragma once


#include "PS_ProcessInformation.h"

#include <Windows.h>



HANDLE WINAPI OpenProcessForQueryReadWrite(DWORD ProcessID);

