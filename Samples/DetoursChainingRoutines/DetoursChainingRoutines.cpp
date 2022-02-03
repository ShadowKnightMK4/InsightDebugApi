
#include <iostream>
#include <Windows.h>


/*
*   Not sure if common knowledge but one is able to detour the 'replacement' routine after setting it
*    as a detour to something else. This example demonstrates this 'chain linking'.
*
*   This Detours MessageBoxW() and nets itself up to 4 levels deep i.e.
*   Somethingalls the Original MessageBoxW()  -> this calls MessageBoxWDetour0()
*    MessageBoxWDetour0() has been detoured to call MessageBoxWDetour1 ()
*    MessageBoxWDetour1() has been detoured to call MessageBoxWDetour2 ()
*    MessageBoxWDetour2() has been detoured to call MessageBoxWDetour3 ()
*
*
*    The routine order when calling the 'detoured' MessageBoxW routine would be
*    Hit Detour3!
*    Hit Detour2!
*    Hit Detour1!
*    Calling the original!
*
*   To say it another way, chain linking routines like this is like a queue, First In - Last Out
* 
* 
* 
*   Why?
*      In theory, pne could do a chain like this with a collection of routines from several DLLS
*       to modify arguments,  message a remote process that it's been called ects, Check for a special
*   case whiche then allows modifying the arguents aso 
* 
*   It could also be a way to store 'tweeks' to an api access multiple DLLs
*/

/* modify this to point to your detours.h
*   You'll also need to have a static 'Detours.lib' ready to go 
* 
*   One can specifiy additional search locations under the Project Configuration settings -> VC++ Directory
*/
#include  "..\..\Detours-master\src\detours.h"

typedef int (WINAPI* MessageBoxWPtr)(
    HWND    hWnd,
    LPCTSTR lpText,
    PCTSTR lpCaption,
    UINT    uType
    );


/// <summary>
/// Will hold user32's MessageBoxW
/// </summary>
MessageBoxWPtr Original;
/// <summary>
/// Will hold original MessageBoxWDetoured0 pointer
/// </summary>
MessageBoxWPtr ZeroOriginal;
/// <summary>
/// Will hold original MessageBoxWDetoured1 pointer
/// </summary>
MessageBoxWPtr FirstOriginal;
/// <summary>
/// Will hold original MessageBoxWDetoured2 pointer
/// </summary>
MessageBoxWPtr SecondOriginal;



int WINAPI MessageBoxWDetour0(
    HWND    hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption,
    UINT    uType
)
{
    /*
    * In Visual Studio, this will appear under the output tab when debugging.
    */
    OutputDebugString(L"Calling the original aka user32's MessageBoxW!\r\n");
    return Original(hWnd, lpText, lpCaption, uType);
}


int WINAPI MessageBoxWDetour1(
    HWND    hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption,
    UINT    uType
)
{
    OutputDebugString(L"Hit MessageBoxWDetour1!\r\n");
    OutputDebugString(lpText);
    OutputDebugStringW(L"\r\n");
    return ZeroOriginal(hWnd, lpText, lpCaption, uType);
}



int WINAPI MessageBoxWDetour2(
    HWND    hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption,
    UINT    uType
)
{
    OutputDebugString(L"Hit MessageBoxWDetour2!\r\n");
    OutputDebugString(lpText);
    OutputDebugStringW(L"\r\n");
    return FirstOriginal(hWnd, lpText, lpCaption, uType);
}


int WINAPI MessageBoxWDetour3(
    HWND    hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption,
    UINT    uType
)
{
    OutputDebugString(L"Hit MessageBoxWDetour3!\r\n");
    OutputDebugString(lpText);
    OutputDebugStringW(L"\r\n");
    return SecondOriginal(hWnd, lpText, lpCaption, uType);
}


int main()
{
    HMODULE user32 = LoadLibrary(L"User32.dll");
    if (user32 == 0)
    {
        std::cout << "For some reason, user32.dll was not found by LoadLibrary" << std::endl;
        return -1;
    }
    Original = (MessageBoxWPtr)GetProcAddress(user32, "MessageBoxW");
    if (Original == nullptr)
    {
        std::cout << "For some reason, GetProcAddress() was not able to locate MessageBoxW in user32.dll" << std::endl;
        return -1;
    }
    ZeroOriginal = (MessageBoxWPtr)MessageBoxWDetour0;
    FirstOriginal = (MessageBoxWPtr)MessageBoxWDetour1;
    SecondOriginal = (MessageBoxWPtr)MessageBoxWDetour2;

    if (DetourTransactionBegin() == 0)
    {
        DetourUpdateThread(GetCurrentThread());
        if (DetourAttach(&(PVOID&)Original, ZeroOriginal) != 0)
        {
            DetourTransactionAbort();
            return 1;
        }

        if (DetourAttach(&(PVOID&)ZeroOriginal, MessageBoxWDetour1) != 0)
        {
            DetourTransactionAbort();
            return 2;
        }


        if (DetourAttach(&(PVOID&)FirstOriginal, MessageBoxWDetour2) != 0)
        {
            DetourTransactionAbort();
            return 3;
        }

        if (DetourAttach(&(PVOID&)SecondOriginal, MessageBoxWDetour3) != 0)
        {
            DetourTransactionAbort();
            return 4;
        }



        DetourTransactionCommit();
    }

    MessageBoxW(GetDesktopWindow(), L"This text will be seen by 3 functions other than MessageBoxW", L"DetourTest", MB_OK);
    return 0;
}