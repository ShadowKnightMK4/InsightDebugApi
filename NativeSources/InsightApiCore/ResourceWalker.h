#pragma once
#include <Windows.h>
#include <vector>
#include <string>


/// this means the struct and array buffer do not need to be clearned up.
#define RW_SPECIAL_FLAG_NONE 0
/// this means the array buffer + each elementy needs to be freed() on cleanuip
#define RW_SPECIAL_FLAG_MULTI_ALLOC 1
/// this means the array buffer + each elemeny only need the free(*) be called on the array buffer struct
#define RW_SPECIAL_FLAG_SINGLE_ALLOC 2
/// @brief Used to pass arrays back t
struct ArrayHelper
{
	/// @brief How many elemets in FirstEntry
	DWORD ElementCount;
	/// @brief  See the special flags RW_SPECIAL_FLAG constants
	DWORD SpecialFlags;
	/// @brief Pointer to the first entry.
	VOID* FirstEntry;
	
};

/// <summary>
/// This class is the Unmanaged Resource Browser for a single app whose modules we're loading and examing.
/// </summary>
class ResourceWalker
{
public:
	/// @brief   We enumereate / deal with resources for the exe whose file we have open. We assume read only
	/// @param Target 
	ResourceWalker(HANDLE Target);
	/// @brief 
	/// @param Target This is the file to deal with
	/// @param ExclusiveControl If true, we assume that we're dealing with the file only by ourselfs
	ResourceWalker(HANDLE Target, BOOL ExclusiveControl);
	
	/// @brief  We enumerate / deal with resources for the passed file. 
	/// @param Target Open this file
	/// @param SharePermssions	FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE.
	ResourceWalker(LPCWSTR Target, DWORD Access, DWORD SharePermssions);



	~ResourceWalker();

	
	/// @brief Enumerate the names of the resources in the targeted exe 
	/// @param lpType 
	/// @param Callback 
	/// @param CustomArg 
	/// @param dwFlags 
	/// @param LangId 
	/// @return 

	BOOL EnumerateResourceNamesEx(LPCWSTR lpType, ENUMRESNAMEPROCW Callback, LONG_PTR CustomArg, DWORD dwFlags, LANGID LangId);
	/// @brief Enumerate the types of resources in the targeted exe
	/// @param Callback 
	/// @param lParam 
	/// @param dwFlags 
	/// @param LangId 
	/// @return 
	BOOL EnumerateResourceTypesExW(ENUMRESTYPEPROCW Callback, LONG_PTR lParam, DWORD dwFlags, LANGID LangId);


	/// @brief Enumerate the langauges of the resources in the targeted exe
	/// @param lpType 
	/// @param lpName 
	/// @param lpEnumFunc 
	/// @param lParam 
	/// @param dwFlags 
	/// @param LangId 
	/// @return 
	BOOL EnumerateResourceLanguagesExW(LPCWSTR lpType, LPCWSTR lpName, ENUMRESLANGPROCW lpEnumFunc, LONG_PTR lParam, DWORD dwFlags, LANGID LangId);

	/// @brief return if we lock our object before begining eneration
	/// @return 
	BOOL GetThreadLock();

	/// @brief set the locks status when we enumerate
	/// @param Status 
	/// @return 
	BOOL SetThreadLock(BOOL Status);
private:

	/// @brief IF an exe, create a suspended instance and plant modle list
	BOOL MakeSuspendedInstance();

	bool PopulateModuleList(HANDLE ProcessHandle);

	/// @brief  Called before enumerating and locks a critical section if LockStatus is true.  This makes a list of loaded modules in the target./
	void LoadModules();
	
	/// @brief 
	/// Enumerate resources based on the critatia.  Implementaiton of the public EnumXXXXResources for this.
	/// @return 
	HANDLE TargetExeFileHandle;
	HMODULE TargetExeMainHmodule;

	HMODULE* DllList;
	DWORD DllListSize;
	BOOL Exclusive;
	BOOL LockStatus;
	CRITICAL_SECTION Crit;
	std::wstring ProcessName;

	PROCESS_INFORMATION SuspendMe;
};
