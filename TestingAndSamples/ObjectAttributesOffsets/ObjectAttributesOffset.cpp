#include <Windows.h>
#include <iostream>
#include <winternl.h>

/*
sizeof(PTR) = 8
sizeof(UNICODE_STRING) =16
sizeof(UNICODE_STRING32) =8
sizeof(UNICODE_STRING64) =16

UNICODE_STRING.Length offset = 0
UNICODE_STRING.Max offset = 2
UNICODE_STRING.Buffer offset = 8

UNICODE_STRING32.Length offset = 0
UNICODE_STRING32.Max offset = 2
UNICODE_STRING32.Buffer offset = 4


UNICODE_STRING64.Length offset = 0
UNICODE_STRING64.Max offset = 2
UNICODE_STRING64.Buffer offset = 8




sizeof(PTR) = 4
sizeof(UNICODE_STRING) =8
sizeof(UNICODE_STRING32) =8
sizeof(UNICODE_STRING64) =16

UNICODE_STRING.Length offset = 0
UNICODE_STRING.Max offset = 2
UNICODE_STRING.Buffer offset = 4

UNICODE_STRING32.Length offset = 0
UNICODE_STRING32.Max offset = 2
UNICODE_STRING32.Buffer offset = 4

UNICODE_STRING64.Length offset = 0
UNICODE_STRING64.Max offset = 2
UNICODE_STRING64.Buffer offset = 8
*/
using namespace std;

struct UNICODE_STRING32
{
	USHORT Length;
	USHORT MaximumLength;
	UINT Buffer;
};

struct UNICODE_STRING64
{
	USHORT Length;
	USHORT MaximumLength;
	INT Padding; // aliggns buffer ok.
	ULONGLONG Buffer;
};

struct RTL_USER32
{
	BYTE Reserved1[16];
	UINT32 Reserved2[10];
	UNICODE_STRING32 ImagePathName;
	UNICODE_STRING32 CommandLine;
};
struct RTL_USER64
{
	BYTE Reserved[16];
	UINT64 Reserved2[10];
	UNICODE_STRING64 ImagePathName;
	UNICODE_STRING64 CommandLine;
};
int main()
{
	UNICODE_STRING Buffer;
	
	

	
	cout << "sizeof(PTR) = " << sizeof(VOID*) << endl;

	

	cout << "Offsets for UNICODE_STRING, UNICODE_STRING32, UNICODE_STRING64" << endl;
	cout << "sizeof(UNICODE_STRING) =" << sizeof(UNICODE_STRING) << endl;
	cout << "sizeof(UNICODE_STRING32) =" << sizeof(UNICODE_STRING32) << endl;
	cout << "sizeof(UNICODE_STRING64) =" << sizeof(UNICODE_STRING64) << endl;


	cout << "UNICODE_STRING.Length offset = " << offsetof(UNICODE_STRING, Length) << endl;
	cout << "UNICODE_STRING.Max offset = " << offsetof(UNICODE_STRING, MaximumLength) << endl;
	cout << "UNICODE_STRING.Buffer offset = " << offsetof(UNICODE_STRING, Buffer) << endl;

	
	cout << "UNICODE_STRING32.Length offset = " << offsetof(UNICODE_STRING32, Length) << endl;
	cout << "UNICODE_STRING32.Max offset = " << offsetof(UNICODE_STRING32, MaximumLength) << endl;
	cout << "UNICODE_STRING32.Buffer offset = " << offsetof(UNICODE_STRING32, Buffer) << endl;


	
	cout << "UNICODE_STRING64.Length offset = " << offsetof(UNICODE_STRING64, Length) << endl;
	cout << "UNICODE_STRING64.Max offset = " << offsetof(UNICODE_STRING64, MaximumLength) << endl;
	cout << "UNICODE_STRING64.Buffer offset = " << offsetof(UNICODE_STRING64, Buffer) << endl;

	return 0;
}