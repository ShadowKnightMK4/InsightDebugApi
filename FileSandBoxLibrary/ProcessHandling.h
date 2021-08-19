#pragma once


#include "FilesandboxApi_DllStuff.h"

#include <Windows.h>

/// <summary>
/// Each process that uses the api will have a structure allocated to it.
/// </summary>
typedef struct FileSandboxApi_User
{
	/// <summary>
	/// Process ID of the caller
	/// </summary>
	DWORD Pid;
};