#include <Windows.h>
#include <DbgHelp.h>
#include "InsightHunter.h"
#include "ApiVersionStruct_CExports.h"

extern "C" {

	/*
	* C Level Wrappers that export routines to read structure data for DebugHelp API_VRSION struct
	*/

	USHORT WINAPI DebugHelp_ApiVerionStruct_GetMajor(const API_VERSION* that) noexcept
	{
		if (!that)
		{
			return 0;
		}
		return that->MajorVersion;
	}

	USHORT WINAPI DebugHelp_ApiVerionStruct_GetMinor(const API_VERSION* that) noexcept
	{
		if (!that)
		{
			return 0;
		}
		return that->MinorVersion;
	}

	USHORT WINAPI DebugHelp_ApiVerionStruct_GetRevision(const API_VERSION* that) noexcept
	{
		if (!that)
		{
			return 0;
		}
		return that->Revision;
	}

}