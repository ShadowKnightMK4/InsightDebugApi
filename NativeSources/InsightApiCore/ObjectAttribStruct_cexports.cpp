#include <windows.h>
#include <winternl.h>


extern "C" {

}
	/*
	typedef struct _OBJECT_ATTRIBUTES {
  ULONG           Length;
  HANDLE          RootDirectory;
  PUNICODE_STRING ObjectName;
  ULONG           Attributes;
  PVOID           SecurityDescriptor;
  PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES;


//
//	The plan is for the sheath to use this to get x64/x86 agnostic values for the OBJECT_ATTRIBUTES struct.
//  This means this module's code cares if compiled to x86 or x64 as that effects the pointer size.
//	We promote the values to 8 bytes long so the longer one (x64) pointers are fine and x86 are not cut off.
//  Callers will need to provide *which* type of object attribute to get offets from. Wrong one likely will
//  cause an exception.   Debug Events provide the process id and we can use IsDebugEvent32Bit (sheath side) to
//  figure to call for 32-bit or 64-bit.
//	IN function test for executing code being 32 or 64 bit is checking sizeof(void*) being 4 or 87 bits.


/*
	* x64
	test sizeof() == 48
test.length offset=0 size= 4
test.rootdirectory offset=8 size= 8
test.objectname offset=16 size= 8
test.attributes offset=24 size= 4
test.securitydesc offset=32 size= 8
test.qos offset=40 size= 8*/

/*
* x86
test sizeof() == 24
test.length offset=0 size= 4
test.rootdirectory offset=4 size= 4
test.objectname offset=8 size= 4
test.attributes offset=12 size= 4
test.securitydesc offset=16 size= 4
test.qos offset=20 size= 4

	*/

	/*
struct OBJECT_ATTRIBUTES32
{
	ULONG Length;
	ULONG RootDirectory;
	ULONG ObjectName;
	ULONG Attributes;
	ULONG SecurityDesc;
	ULONG QoS;
};

struct OBJECT_ATTRIBUTES64
{
	ULONG Length;
	ULONG Padding1;	// padding to ensure RootDirectory aligned;
	ULONG64 RootDirectory;
	ULONG64 ObjectName;
	ULONG Attributes;
	ULONG Padding2; // Padding to ensur SecurityDrescriptorAligned;
	ULONG64 SecurityDescriptor;
	ULONG64 SecurityQOS;
};*/


