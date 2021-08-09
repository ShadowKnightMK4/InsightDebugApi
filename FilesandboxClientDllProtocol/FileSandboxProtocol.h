#pragma once

#include <Windows.h>

/*
* When the ClientDll is loaded, it throwns an SEH that is caught by the ClientDllLauncher using Win32 Debugger Api.
*/


// the base exception
#define SANDBOX_API_BASE_EXCEPTION (0xFBFFFFFF)
#define CLEAR_BIT27 (~(1 << 27))
// exception triggered when NTCreateFileCalledn = n & ~(1 << k)

/*
* SANDBOX_API_FILE_RESOURCE Triggers on a Call to NtCreateFile
*/
//#define SANDBOX_API_FILE_RESOURCE ((SANDBOX_API_BASE_EXCEPTION-1) & CLEAR_BIT27)
#define SANDBOX_API_FILE_NTCREATEFILE  (3825205246)

// Not used currently
#define SANDBOX_API_REG_RESOURCE ((SANDBOX_API_BASE_EXCEPTION-2) & CLEAR_BIT27)

// not used currently
#define SANDBOX_API_HMODULE_RESOURCE ((SANDBOX_API_BASE_EXCEPTION-3) & CLEAR_BIT27)

// on vista/later - raised when CreateUserProcess is about to be called.
//#define SANDBOX_API_CREATE_USERPROCESS ((SANDBOX_API_BASE_EXCEPTION-4) & CLEAR_BIT27)
#define SANDBOX_API_PROCESS_CREATEUSERPROCESS (3825205243)




/// no Special case neeed. Note: Values non reconized by helper dll also will default to this also.
#define SANDBOX_SPECIAL_CASE_DEFAULT (0)
// Do not call the detoured routine, instead return the specialcase arg if ACK is set.
// If you onely want to block the routine, just pick  an apropriate a failure (or sucess) return value, plant it in ExpMessage.SpecialHandlingArg and use this value ie ExmpMessage.SpecialHandling value.
#define SANDBOX_SPECIAL_CASE_SHORTIT (1)


/// <summary>
/// Structure containing communication from the helper dll;
/// </summary>
struct FILESANDBOX_API_EXCEPTION_CLASS
{
public:
	
	
	/// <summary>
	/// Size of this struct in bytes;
	/// </summary>
	int cb;
	/// <summary>
	/// Message Type
	/// </summary>
	DWORD ExeceptionMessage;
	/// <summary>
	/// Zero on throwning exception. Debugger should set to non-zero to indicate it understood the message. Leaving this at zero pretty much means the detoured routines just jump to the original one without any additional processing
	/// </summary>
	int ACK;
	/// <summary>
	/// a danymically allocated ULONG_PTR*. It will contain an array of pointers to bits of memory in the debugged process corosponding to the arguments to the detoured routine. The debugger is free to modify the memory and the detoured routine will use the modified arguments
	/// </summary>
	//std::vector<ULONG_PTR> PackedArguments;
	ULONG_PTR* PackedArguments;

	/// <summary>
	/// Length of the Packed Arguments
	/// </summary>
	DWORD PackedArgumentLength;
	/// <summary>
	/// Zero on throwning exception, debugger should leave at zero if not needed or set to on of the FILESANDBOX_API_SPECIAL_CASE const for that feature
	/// </summary>
	DWORD SpecialHandling;
	/// <summary>
	/// Some cases may have an argument
	/// </summary>
	DWORD SpecialHandlingArg;
};

/*
* This series of #defines corspond to where the argumentst are in the packed pointer list thrown for detoured routines.
* In the detoured source they are used as constants.  Should you which to change the order for some reason, change these constants here.
*/
/// The PackedArgument location for the pointer to the FileHandle when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_FILEHANDLE]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_FILEHANDLE (0)
/// The PackedArgument location for the pointer to the DesiredAccess Argument when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_DESIREDACCESS]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_DESIREDACCESS (1)
/// The PackedArgument location for the pointer to the Object Attribute Argument when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_OBJECTATTRIBUTES]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_OBJECTATTRIBUTES (2)
/// The PackedArgument location for the pointer to the File Attributes  when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_FILEATTRIBUTES]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_FILEATTRIBUTES (3)
/// The PackedArgument location for the pointer to the Share Access when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_SHAREACCESS]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_SHAREACCESS (4)
/// The PackedArgument location for the pointer to the Creation Disposition when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_CREATIONDISPOSITION]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_CREATIONDISPOSITION (5)
/// The PackedArgument location for the pointer to the EaBuffer when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_EABUFFER]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_EABUFFER (6)
/// The PackedArgument location for the pointer to the EaBuffer Length when procesing SANDBOX_API_FILE_RESOURCE and its FILESANDBOX_API_EXCEPTION_CLASS
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEFILE_EALENGTH]
/// Use this to get a pointer to where the detoured NtCreateFile will store the results of the call.
#define FILESANDBOX_NTCREATEFILE_EALENGTH (7)

/// The Packed Argument Location to to where the process handle will be stored when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSHANDLE] 
/// Corasponds with a pointer "ProcessHandle" arguemnt passed to NtCreateUserProcess()
/// Pointer points to a PHANDLE
#define FILESANDBOX_NTCREATEUSERPROCESS_PROCESSHANDLE (0)

/// The Packed Argument Location to to where the thread handle will be stored when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_THREADHANDLE] 
/// Corasponds with a pointer to the "ThreadHandle" argument passed to NtCreateUserProcess()
/// Pointer points to a PHANDLE
#define FILESANDBOX_NTCREATEUSERPROCESS_THREADHANDLE (1)

/// The Packed Argument Location to to where the Process's desired access specifies is located when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSDESIREDACCESS] 
/// Corasponds with a pointer to the "ProcessDesiredAccess" Argument to NtCreateUserProcess()
/// Pointer points to an ACCESS_MASK
#define FILESANDBOX_NTCREATEUSERPROCESS_PROCESSDESIREDACCESS (2)

/// The Packed Argument Location to to where the Main Thread's desired access specifies is located when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_THREADDESIREDACCESS] 
/// Corasponds with a pointer to "ThreadDesiredAccess" Arguemnt to NtCreateUserProcess()
/// Pointer points to an ACCESS_MASK
#define FILESANDBOX_NTCREATEUSERPROCESS_THREADDESIREDACCESS (3)

/// The Packed Argument Location to to where the process's object attributes struct is when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSOBJECTATTRIBUTES] 
/// Corasponds with a pointer to the  "ProcessObjectAttributes" argument to NtCreateUserProcess()
/// Pointer points to a POBJECT_ATTRIBUTES
#define FILESANDBOX_NTCREATEUSERPROCESS_PROCESSOBJECTATTRIBUTES (4)

/// The Packed Argument Location to to where the main thread's object attributes struct is when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_THREADOBJECTATTRIBUTES] 
/// Corasponds with a pointer to the  "ThreadObjectAttributes" argument to NtCreateUserProcess()
/// Pointer points to an POBJECT_ATTRIBUTES
#define FILESANDBOX_NTCREATEUSERPROCESS_THREADOBJECTATTRIBUTES (5)

/// The Packed Argument Location to to a place where the Process's Creation flags are when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSFLAGS] 
/// Corasponds with a pointer to the  "ProcessFlags" argument to NtCreateUserProcess()
/// Pointer points to a ULONG using 'PROCESS_CREATE_FLAGS'
#define FILESANDBOX_NTCREATEUSERPROCESS_PROCESSFLAGS (6)

/// The Packed Argument Location to to a place where the main thread's Creation flags are when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_THREADFLAGS] 
/// Corasponds with a pointer to the  "ThreadFlags" argument to NtCreateUserProcess()
/// Pointer points to a ULONG using 'THREAD_CREATE_FLAGS'
#define FILESANDBOX_NTCREATEUSERPROCESS_THREADFLAGS (7)

/// The Packed Argument Location to to where the process handle will be stored when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_PROCESSPARAMETERS] 
/// Corasponds with a pointer to the  "ProcessParameter" argument to NtCreateUserProcess()
/// Points to a PPRTL_USER_PROCESS_PARAMETERS
#define FILESANDBOX_NTCREATEUSERPROCESS_PROCESSPARAMETERS (8)

/// The Packed Argument Location to to where the ProcessParameter's struct is processing  a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_CREATEINFO] 
/// Corasponds with a pointer to the  "CreateInfo" argument to NtCreateUserProcess()
/// Points to a PPS_CREATE_INFO
#define FILESANDBOX_NTCREATEUSERPROCESS_CREATEINFO (9)

/// The Packed Argument Location to to where the process attributes lists will be stored when processing a SANDBOX_API_CREATE_USERPROCESS exception
/// Use ExpMessage.PackedArgs[FILESANDBOX_NTCREATEUSERPROCESS_ATTRIBUTELIST] 
/// Corasponds with a pointer to the  "AttributeList" argument to NtCreateUserProcess()
/// POINTS to a PPS_ATTRIBUTE_LIST
#define FILESANDBOX_NTCREATEUSERPROCESS_ATTRIBUTELIST (10)
 