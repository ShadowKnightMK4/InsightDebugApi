#include <Windows.h>
#include <DbgHelp.h>
#include "InsightHunter.h"
extern "C" {

	/*
	* C Level Wrappers that export routines to read structure data for DebuhHelp API_VRSION srusct
	*/

	USHORT WINAPI DebugHelp_ApiVerionStruct_GetMajor(API_VERSION* that)
	{
		if (!that)
		{
			return 0;
		}
		return that->MajorVersion;
	}

	USHORT WINAPI DebugHelp_ApiVerionStruct_GetMinor(API_VERSION* that)
	{
		if (!that)
		{
			return 0;
		}
		return that->MinorVersion;
	}

	USHORT WINAPI DebugHelp_ApiVerionStruct_GetRevision(API_VERSION* that)
	{
		if (!that)
		{
			return 0;
		}
		return that->Revision;
	}

}