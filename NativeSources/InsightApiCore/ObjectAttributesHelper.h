#pragma once
#include <Windows.h>
#include <winternl.h>

/// <summary>
/// The code here tests for length=24 (x86) or not. If x86 we return approprately sized data / offset for that
/// module and if not we return the other.
/// </summary>
class ObjectAttributesHelper
{
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

public:
	ULONG Length();
	VOID Length(ULONG val);

	UINT64 RootDirectory();
	VOID RootDirectory(UINT64 Val);

	UINT64 ObjectName();
	VOID ObjectName(UINT64 Val);


	ULONG Attributes();
	VOID Attributes(ULONG val);


	UINT64 SecurityDescriptor();
	VOID SecurityDescriptor(UINT64 Val);

	UINT64 SecurityQoS();
	VOID SecurityQoS(UINT64 Val);

	OBJECT_ATTRIBUTES* GetPointer();



private:
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
	};

	union Buffer
	{
		ULONG Length; // stays in the spot for struct length regardless if accessing 32 or 64-bit
		OBJECT_ATTRIBUTES32 Machine86;
		OBJECT_ATTRIBUTES64 Machine64;
	} Data;
	
};