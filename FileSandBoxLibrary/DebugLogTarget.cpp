#include "DebugLogTarget.h"
#include "Utility.h"

DebugLogTarget::DebugLogTarget(std::ostream* AnsiOut)
{
    this->u.AnsiOutput = AnsiOut;
    this->EmitType = DEBUGLOG_FORMAT_ANSI_STREAM;
}

DebugLogTarget::DebugLogTarget(std::wostream* UnicodeOut)
{
    this->u.UnicodeOutput = UnicodeOut;
    this->EmitType = DEBUGLOG_FORMAT_UNICODE_STEAM;
}

DebugLogTarget::DebugLogTarget(HANDLE Win32Handle, BOOL WantUnicode)
{
    if (WantUnicode)
    {
        this->EmitType = DEBUGLOG_FORMAT_UNICODE_HANDLE;
    }
    else
    {
        this->EmitType = DEBUGLOG_FORMAT_ANSI_HANDLE;
    }
    this->u.Win32Handle = Win32Handle;
}

DebugLogTarget::~DebugLogTarget()
{
    
}

BOOL DebugLogTarget::FlushLog()
{
    char* Ansi = nullptr;
    DWORD BytesWritten = 0;
    if ((EmitType == DEBUGLOG_FORMAT_ANSI_HANDLE) || (EmitType == DEBUGLOG_FORMAT_ANSI_STREAM))
    {
        Ansi = ConvertUnicodeString(InternalBuffer.str().c_str());
        if (Ansi != nullptr)
        {
            if (EmitType == DEBUGLOG_FORMAT_ANSI_STREAM)
            {
                *u.AnsiOutput << Ansi;
                free(Ansi);
                return TRUE;
            }
            else
            {
                if (EmitType == DEBUGLOG_FORMAT_ANSI_STREAM)
                {
                    if ((WriteFile(this->u.Win32Handle, Ansi, InternalBuffer.str().size()*sizeof(wchar_t), &BytesWritten, NULL) == FALSE) ||
                        (BytesWritten != (InternalBuffer.str().length()*sizeof(wchar_t))))
                    {
                        free(Ansi);
                        return FALSE;
                    }
                    return TRUE;
                }
                free(Ansi);
                return FALSE;
            }
        }
        
    }
    else
    {
        if (EmitType == DEBUGLOG_FORMAT_UNICODE_HANDLE)
        {
            if ((WriteFile(this->u.Win32Handle, InternalBuffer.str().c_str(), InternalBuffer.str().size()* sizeof(wchar_t), &BytesWritten, NULL) == FALSE) ||
                (BytesWritten != (InternalBuffer.str().length() * sizeof(wchar_t))))
            {
                return FALSE;
            }
        }
        else
        {
            if (EmitType == DEBUGLOG_FORMAT_UNICODE_STEAM)
            {
                auto MakeCompilerHappy =  InternalBuffer.str().c_str();
                *u.UnicodeOutput << MakeCompilerHappy;
                return TRUE;
            }
        }
        return FALSE;
    }
}

BOOL DebugLogTarget::FlushLog(std::wostream& output)
{
    output << InternalBuffer.str();
}

BOOL DebugLogTarget::FlushLog(std::ostream& output)
{
    char* Ansi = ConvertUnicodeString(this->InternalBuffer.str().c_str());
    if (Ansi)
    {
        output << Ansi;
        free(Ansi);
        ClearLogBuffer();
    }
    return FALSE;
   

}

VOID DebugLogTarget::ClearLogBuffer()
{
    if (InternalBuffer.str().length() != 0)
    {
        InternalBuffer.str(L"");
    }
}



std::wostream& operator<<(std::wostream& that, const DebugLogTarget& Data)
{
    that << Data.InternalBuffer.str();
    return that;
}

std::ostream& operator<<(std::ostream& that, const DebugLogTarget& Data)
{
    char* Ansi = nullptr;
    Ansi = ConvertUnicodeString(Data.InternalBuffer.str().c_str());
    if (Ansi != nullptr)
    {
        that << Ansi;
        free(Ansi);
    }
    else
    {
        that << "(ERROR)";
    }
    return that;
}
