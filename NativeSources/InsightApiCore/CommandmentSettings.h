#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
/// commandment stricts 

/// command modifies the requested access to be something else.  Will need one of the ACCESS_XXXX things 
#define CMD_RESOURCE_ACESS_MODIFY (1)

/// command will BITWISE and the argument with the AccessModifier. For example, to strip GENERIC_READ out, ensure ACCESS_CHECK is set to BITWISE_NOT 
#define CMD_RESOURCE_ACCESS_STRIP (2)

//. command is conditional on matching one of the names in the accessCheck List. 
#define CMD_RESOURCE_NAME_CONDITIONAL (4)

/// the acess check list is a blacklist
#define CMD_RESOURCE_NAME_FORBID (8)

/// access check is a whitelsit
#define CMD_RESOURCE_NAME_RESTRICT (16)

/// matching the access list means modifying the call to use the redirect path
#define CMD_RESOURCE_REDIRECT (32)

/// are we to force spawned processes to have the helper dll? (This tells the helperdll force the loading of itself via detours.
#define CMD_PROCESS_PROPERGATE (64)
class Commandment
{

	DWORD AccessModifier=0;
	/// <summary>
	/// for certain commands, this will specify a string to compare against. 
	/// </summary>
	std::vector<std::wstring> AccessCheck;
	std::wstring RedirectPath;
};
/// <summary>
/// Handles the Native side of communicating to the helper dll the commandment strictictuions. Created automatically when the Process information class is instanced
/// </summary>.
class CommandmentContainer
{
public:
	std::vector<Commandment> Commands;
private:
	bool sync;
	HANDLE PIPE;

};