
/*
*   This is more a test dummy to modify and test stuff under  a smaller consideration. 

    Currently Testing Yoinking DetourCreateProcessWithDllExS, NtCreateUserProcess and the like.
*/
#include <iostream>
#include <Windows.h>

#include "detours.h"
using namespace std;
typedef int (WINAPI* MessageBoxWPtr)(
    HWND    hWnd,
     LPCTSTR lpText,
    PCTSTR lpCaption,
               UINT    uType
);


MessageBoxWPtr Original;
MessageBoxWPtr ZeroOriginal;
MessageBoxWPtr FirstOriginal;
MessageBoxWPtr SecondOriginal;




const wchar_t* SpecialSearchPath = L"A:\\;C:\\Users;F:\\SpecialDLLs";
 unsigned char buffer_size[512];
int main()
{
 
    LoadLibraryA("C:\\Users\\Thoma\\source\\repos\\InsightAPI\\Code\\Native\\TelemetryDlls\\Debug\\x64\\IoDeviceTracking64.dll");
    return 0;
}

