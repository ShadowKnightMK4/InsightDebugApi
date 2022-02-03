#include "FileSandBoxHelperDll.h"
#include "ReplacementLoadLibraryStuff.h"
#include <winternl.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "../FilesandboxClientDllProtocol/FileSandboxProtocol.h"
#include "StringWString.h"
#include <Shlwapi.h>


LoadLibraryAPtr OriginalLoadLibraryA;
LoadLibraryWPtr OriginalLoadLibraryW;
LoadLibraryExAPtr OriginalLoadLibraryExA;
LoadLibraryExWPtr OriginalLoadLibraryExW;


SetDllDirectoryAPtr OriginalSetDllDirectoryA;
SetDllDirectoryWPtr OriginalSetDllDirectoryW;
AddDllDirectoryAPtr OriginalAddDllDirectoryA;
AddDllDirectoryWPtr OriginalAddDllDirectoryW;
RemoveDllDirectoryPtr OriginalRemoveDllDirectory;
SetDefaultDllDirectoriesPtr OriginalSetDefaultDllDirectories;


/*
* 
*   Plan For LoadLibraryXXX() / DLL loading in general
*   
*   LoadLibrary
*       Check ForceDllSearchPathLocations
*           if input has an enivorment variable, expand it
*           if the case ansitive input matches an entry, we use the entry instead
*           
* 
*        DllForceDllSearchPathOverrides
*           lets a user hard code a replacement for a specific string
* 
*   This can be set via
*       The Insite Debugger settings TODO:
*       hardcoding it.
*
*/

/// <summary>
/// Our private cookies we do when adding ForceDllSearchPath. Once directories are cleared, ect.. these are added back;
/// </summary>
//std::map<std::wstring, DLL_DIRECTORY_COOKIE> ForceDllSearchPathCookies;
std::vector<std::wstring> DllPrioritySeachPaths;

void ExtractPrioritySearchPathPayload(PVOID PayloadPtr, DWORD PayloadSize)
{
    std::wstring PayloadData;
    std::wstringstream Buffer;
    wchar_t c;
    PayloadData.assign((const wchar_t*)PayloadPtr, PayloadSize / sizeof(wchar_t));


    for (size_t step = 0; step < PayloadData.size(); step++)
    {
        c = PayloadData[step];
        switch (c)
        {
            case L';':
            {
                if (Buffer.str().size() > 0)
                {
                    DllPrioritySeachPaths.insert(DllPrioritySeachPaths.end(), Buffer.str());
                    Buffer.clear();
                }
                break;
            }
            case 0:
            {
                if (Buffer.str().size() > 0)
                {
                    DllPrioritySeachPaths.insert(DllPrioritySeachPaths.end(), Buffer.str());
                    Buffer.clear();
                }
                break;
            }
            default:
            {
                Buffer << c;
                break;
            }
        }

    }
}

/// <summary>
/// These are forced redirected. Meaning when X is located in the collection. we load y instead.
/// For example:
///     X= "kernel32.dll" 
///     y= "special_patched_version_of_kernel32.dll"
/// 
/// This means when the LoadLibraryXXX stuff is called for kernel32, Y is loaded instead
/// TODO: expand environment variables
/// </summary>
/// <param name="lpLibFileName"></param>
/// <param name="hFile"></param>
/// <param name="dwFlags"></param>
/// <returns></returns>
std::map<std::wstring, std::wstring> ConstRedirects;



void WINAPI AddDllSearchPathBack()
{

}

/// <summary>
/// Check if the String exists as a file in the priority search path and set output to the exact path
/// </summary>
/// <param name="String"></param>
/// <param name="Output"></param>
/// <returns></returns>
bool WINAPI CheckPrioritySearchPath(LPCWSTR String, std::wstring& Output)
{
    wchar_t c;
    
    std::wstringstream Temp;
    if (String == nullptr)
    {
        return false;
    }
    else
    {
        for (auto stepper = DllPrioritySeachPaths.begin(); stepper != DllPrioritySeachPaths.end(); stepper++)
        {
            if (stepper._Ptr->size() < MAX_PATH)
            {
                /* enriovment check first*/
                if (wcsstr(stepper._Ptr->c_str(), L"%") != nullptr)
                {
                    wchar_t* Env;
                    DWORD Size = 10;
                    while (1)
                    {
                        Env = new wchar_t[Size];
                        if (Env == 0)
                        {
                            break;
                        }
                        else
                        {
                            if (ExpandEnvironmentStrings(stepper._Ptr->c_str(), Env, Size- sizeof(wchar_t)) > Size)
                            {
                                Size *= 2;
                                delete[] Env;
                            }
                            else
                            {
                                Temp << Env;
                                delete[] Env;
                                break;
                            }
                        }
                    }

                }
                else
                {
                    Temp << stepper._Ptr;
                }
                
                c = Temp.str()[Temp.str().size() - 1];
                if ((c != L'\\') && (c != '/'))
                {
                    if (!((String[0] == L'\\') || (String[0] == L'/')))
                    {
                        Temp << L'\\';
                    }
                }
                else
                {
                    if (((String[0] == L'\\') || (String[0] == L'/')))
                    {
                        String++;
                    }
                }

                Temp << String;
                if (Temp.str().size() < MAX_PATH)
                {
                    if (PathFileExists(Temp.str().c_str()))
                    {
                        Output = Temp.str().c_str();
                        return TRUE;
                    }
                }
            }
        }
        return false;
    }
}
/// <summary>
/// check if we have a ConstRedirects() entry for this argument and set output to that.
/// We we don't (or String == null), we set output to string.
/// We also expand enviromenet variables in String beforce comparing
/// </summary>
/// <param name="String"></param>
/// <param name="Output"></param>
/// <returns></returns>
void WINAPI CheckPossibleRedirectsW(LPCWSTR String, std::wstring& Output)
{
    std::wstring working_location;
    
    if (String != nullptr)
    {
        Output = nullptr;
        // this routine should resolve expanding any enviroment varibles in string and come up with an exact location if its in our search path
        if (!CheckPrioritySearchPath(String, working_location)) 
        {
            working_location = String;
        }

        for (auto stepper = ConstRedirects.begin(); stepper != ConstRedirects.end(); stepper++)
        {
            std::wstring buffer_check = stepper->first;
            std::wstring user_array = working_location;
            for (int step = 0; step < buffer_check.size(); step++)
            {
                buffer_check[step] = std::tolower(buffer_check[step]);
            }

            for (int step = 0; step < user_array.size(); step++)
            {
                user_array[step] = std::tolower(user_array[step]);
            }

            if (buffer_check == user_array)
            {
                Output = stepper->first;
            }
        }

        Output = working_location;
    }

}

/// <summary>
/// converts stuff to Unicode then calls CheckPossibleRedirectsW
/// </summary>
/// <param name="String"></param>
/// <param name="Output"></param>
/// <returns></returns>
void WINAPI CheckPossibleRedirectsA(LPCSTR String, std::string& Output)
{
    std::wstring InputAsUnicode;
    std::wstring OutputAsUnicode;
    ConvertAnsiToUnicode((LPSTR)String, InputAsUnicode);
    CheckPossibleRedirectsW(InputAsUnicode.c_str(), OutputAsUnicode);
    ConvertUnicodeToAnsi((LPWSTR)OutputAsUnicode.c_str(), Output);
}




HMODULE WINAPI DetouredLoadLibraryExW(
    LPCWSTR lpLibFileName,
    HANDLE  hFile,
    DWORD   dwFlags
)
{
    std::wstring RedirectVal;
    RedirectVal.clear();
    if (lpLibFileName != nullptr)
    {
        CheckPossibleRedirectsW(lpLibFileName, RedirectVal);
    }
    return OriginalLoadLibraryExW(RedirectVal.c_str(), hFile, dwFlags);
}



HMODULE WINAPI DetouredLoadLibraryExA(
    LPCSTR lpLibFileName,
    HANDLE hFile,
     DWORD  dwFlags
)
{
    std::string RedirectVal;
    RedirectVal.clear();
    if (lpLibFileName != nullptr)
    {
        CheckPossibleRedirectsA(lpLibFileName, RedirectVal);
    }
    return OriginalLoadLibraryExA(RedirectVal.c_str(), hFile, dwFlags);
}

HMODULE WINAPI DetouredLoadLibraryA(
     LPCSTR lpLibFileName
)
{
    std::string RedirectVal;
    RedirectVal.clear();
    if (lpLibFileName != nullptr)
    {
        CheckPossibleRedirectsA(lpLibFileName, RedirectVal);
    }
    return OriginalLoadLibraryA(RedirectVal.c_str());
}
HMODULE WINAPI DetouredLoadLibraryW(
     LPCWSTR lpLibFileName
)
{
    std::wstring RedirectVal;
    RedirectVal.clear();
    if (lpLibFileName != nullptr)
    {
        CheckPossibleRedirectsW(lpLibFileName, RedirectVal);
    }
    return OriginalLoadLibraryW(RedirectVal.c_str());
}

BOOL WINAPI DetouredSetDllDirectoryA(
     LPCSTR lpPathName
)
{
    BOOL result = OriginalSetDllDirectoryA(lpPathName);
    if (lpPathName == nullptr)
    {
        AddDllSearchPathBack();
    }
    return result;
}

BOOL WINAPI DetouredSetDllDirectoryW(
     LPCWSTR lpPathName
)
{
    BOOL result = OriginalSetDllDirectoryW(lpPathName);
    if (lpPathName == nullptr)
    {
        AddDllSearchPathBack();
    }
    return result;
}

DLL_DIRECTORY_COOKIE WINAPI DetouredAddDllDirectoryW(
     PCWSTR NewDirectory
)
{
    return OriginalAddDllDirectoryW(NewDirectory);
}

BOOL WINAPI DetouredRemoveDllDirectory(
     DLL_DIRECTORY_COOKIE Cookie
)
{
    return OriginalRemoveDllDirectory(Cookie);
}

BOOL DetouredSetDefaultDllDirectories(
    DWORD DirectoryFlags
)
{
    DirectoryFlags &= LOAD_LIBRARY_SEARCH_USER_DIRS;
    return OriginalSetDefaultDllDirectories(DirectoryFlags);
}