
/*
*   This is more a test dummy to modify and test stuff under  a smaller consideration. 
*   Currently testing if I can recurisvly detour something
*/
#include <iostream>
#include <Windows.h>

#include "detours.h"
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



int MessageBoxWDetour0(
     HWND    hWnd,
    LPCTSTR lpText,
     LPCTSTR lpCaption,
               UINT    uType
)
{
    OutputDebugString(L"Calling the original");
    return Original(hWnd, lpText, lpCaption, uType);
}


int WINAPI MessageBoxWDetour1(
     HWND    hWnd,
     LPCTSTR lpText,
     LPCTSTR lpCaption,
               UINT    uType
)
{
    OutputDebugString(L"Hit Detour1");
    return ZeroOriginal(hWnd, lpText, lpCaption, uType);
}



int WINAPI MessageBoxWDetour2(
     HWND    hWnd,
     LPCTSTR lpText,
     LPCTSTR lpCaption,
               UINT    uType
)
{
    OutputDebugString(L"Hit Detour2");
    return FirstOriginal(hWnd, lpText, lpCaption, uType);
}


int WINAPI MessageBoxWDetour3(
    HWND    hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption,
    UINT    uType
)
{
    OutputDebugString(L"Hit Detour3");
    return SecondOriginal(hWnd, lpText, lpCaption, uType);
}

const wchar_t* SpecialSearchPath = L"A:\\;C:\\Users;F:\\SpecialDLLs";

int main()
{
    
    HMODULE Target = LoadLibraryA("kernel32.dll");

    // TODO: get this value doring the load library thing.
    UUID Injected;
    Injected.Data1 = Injected.Data2 = Injected.Data3 = 0;
    
    
    //DetourCopyPayloadToProcess(GetCurrentProcess(), Injected, SpecialSearchPath, (wcslen(SpecialSearchPath) + 1) * sizeof(wchar_t));
    return 0;
    /*
    *   This is used to generate a bit of noisy stuff for the insite debugger thing to see.
    std::cout << "Hello World!\n";
    while (true)
    {
        std::cout << "Taking a Nap" << std::endl;
        Sleep(2000);
        std::cout << ".. just awoken" << std::endl;
        std::cout << "ShellExecute Notepad.exe and awaiting it to close" << std::endl;
        std::cout << "Shell Execute returned " <<  ShellExecute(GetDesktopWindow(), L"open", L"C:\\Windows\\System32\\notepad.exe", L"HelloWorld.txt", L"C:\\Windows\\Temp\\", SW_SHOWNORMAL);

    }*/
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
