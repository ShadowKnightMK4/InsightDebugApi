#pragma once
#include <Windows.h>
#include <string>

/*
* stringWString
*	routines to convert between ANSI and unicode strings. Use the Win32 API
*/

/// <summary>
/// Convert the passed unicode string to ANSI and modifiy the refrence pointed by output
/// </summary>
/// <param name="Str"></param>
/// <param name="Output"></param>
void ConvertUnicodeToAnsi(LPWSTR Str, std::string& Output);
/// <summary>
/// Return the Unicode version of Str. NEED to call free() aftrwards
/// </summary>
/// <param name="Str"></param>
/// <returns></returns>
LPWSTR ConvertAnsiToUnicode(LPSTR Str);
/// <summary>
/// Return the ANSI version of Str. NEED to call free() aftwrwards
/// </summary>
/// <param name="Str"></param>
/// <returns></returns>
LPSTR ConvertUnicodeToAnsi(LPWSTR Str);
/// <summary>
/// Conver the Ansi return to Unicde and place to output
/// </summary>
/// <param name="Str"></param>
/// <param name="Output"></param>
void ConvertAnsiToUnicode(LPSTR Str, std::wstring& Output);
