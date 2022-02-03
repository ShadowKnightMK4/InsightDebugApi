#include <Windows.h>
#include "HelperPayloadGuids.h"

extern "C" {
	/*
	*	Part of FilesandboClientDllprotocol project, FileSandBoxApi project
	*
	*	GUIDs specific values are not relevent as long as both the helperdll and the filesandboxapidll agreen on the same number
	*/
	/// <summary>
	/// THe load LibraryXXX routines check search paths located in this payload first.  %Enviroment% variables are expands before checking 
	/// </summary>
	const GUID HelperDll_PriorityLibraryPaths = { 0x628dd6dd, 0x5aaf, 0x4590, 0x9f, 0x84, 0x9f, 0xab, 0xbf, 0x4d, 0x8b, 0xaa };
	/// <summary>
	/// "A"="B";	if case sensitive A is passed to LoadLibraryXXX(),  we use be instead. (Exmaple, kernel32="C:\SpecialCaseForThisApp.dll"
	/// </summary>
	const GUID HelperDll_SpecialCaseLibrarySubs = { 0xa79c36b3, 0x70a5, 0x41d2, 0xbf, 0x1e, 0x46, 0xc2, 0xaf, 0xf0, 0x5a, 0x97 };
	/// <summary>
	/// Array of DWORDs that conspond to modifying
	/// </summary>
	const GUID HelperDll_Commandment_Array = { 0xf716644c, 0xad2e, 0x4afd, 0x9d, 0x55, 0x25, 0xd9, 0x37, 0xdf, 0x0c, 0x00 };
}
