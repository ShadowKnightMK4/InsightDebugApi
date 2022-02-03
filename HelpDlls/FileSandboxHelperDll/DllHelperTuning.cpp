#include "FileSandBoxHelperDll.h"


namespace HelperDllTuning
{

	/*
	*	for this project, we care a bout Files and RegistryKeys. The chosen detoured Nt Routines call a routine here
	* which  then modifies what the routine does.
	*/

	enum ResourceMode
	{
		/// <summary>
		/// For files, this logs file location and access time. For Registry, this logs registry location and access time.
		/// </summary>
		LogResourceName = 1,
		// Runs against a wildcard filter.  If matches then
		Filter = 2,
		// Use with Filter. If it passes, we Deny access.
		BlockFilterPass = 4,
		// Use with filter, it if fails, we deny access
		BlockFilderFail = 8,
		// USe with Filter, it it fails, we strip write access out.
		BlockFilterReadOnlyFail = 16,
		// Use with Filter, it it passed, we strip write access out.
		BlockFilterReadOnlyPass = 32,
		/// <summary>
		/// For files, this logs Read Locations, reasource name and access time
		/// </summary>
		LogResourceReadPositional = 64,
		/// <summary>
		/// This logs positional writes with the resource name.
		/// </summary>
		LogResourceWritePositional = 128,
		/// <summary>
		/// This logs a copy of the contents that was read.
		/// </summary>
		LogResourceReadContent = 256,
		LogResourceWriteContent = 512
	};

	struct ResourceAccessCheck
	{
		ResourceMode Action;
		//UNICODE_STRING Matches;
		//UNICODE_STRING LogLocation;
	};


	
	ResourceAccessCheck File;
	ResourceAccessCheck Registry;

	typedef unsigned int ResourceAccessCheckVarient;
#define REA_FILE 1
#define REA_REG 2

	bool VoidResourceCheck(ResourceAccessCheckVarient Type, HANDLE* Handle, UNICODE_STRING* ResourceName, DWORD* AccessFlags)
	{
		return false;
	}


	bool FileResourceCheck(HANDLE* Handle, UNICODE_STRING* FileName, DWORD* AccessFlags)
	{
		return VoidResourceCheck(REA_FILE, Handle, FileName, AccessFlags);
		
	}
}