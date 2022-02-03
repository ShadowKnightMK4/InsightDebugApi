#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "PS_ProcessInformation.h"
class PS_ProcessInformation;

typedef int ClientOutputFormat;


/*
	Client functionally implments the routines that the EXPORTED WinApi call.
	The Exported API take a Pointer that is functionally the "this' pointer.
*/

class Client
{
public:

	std::map<DWORD, PS_ProcessInformation> TrackedProcesses;
};
