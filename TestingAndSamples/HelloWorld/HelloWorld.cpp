
/*
*   This is more a test dummy to modify and test stuff under  a smaller consideration. 
*   Currently testing if I can recurisvly detour something
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
    DWORD ret = 0;
    HANDLE fn = 0;
    __try
    {
        while (true)
        {
            cout << "Try openning file!" << endl;
            fn = CreateFile(L"C:\\Users\\Thoma\\OneDrive\\Documents\\Deployed App\\Tools\\Process Explorer\\EULA.txt", GENERIC_READ, 0, 0, 0, 0, 0);
            if (fn != INVALID_HANDLE_VALUE)
            {
                ReadFile(fn, (void*)&buffer_size, 511, &ret, 0);
                std::cout << "Hello World...." << std::endl;
                Sleep(2000);
                buffer_size[511] = 0;
                std::cout << "... still alive" << std::endl;
                std::cout << "First 511 bytes are" << buffer_size;
                CloseHandle(fn);
                cout << "\r\n\r\n" << endl;
                fn = 0;
            }
            else
            {
                cout << "failed to open \r\n\r\n" << endl;
                Sleep(5000);
            }
        }
    }
    __finally
    {
        if (fn != 0)
        CloseHandle(fn);
    }
    return 0;
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
