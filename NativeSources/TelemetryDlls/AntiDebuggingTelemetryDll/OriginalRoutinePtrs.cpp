#include "OriginalRoutinePts.h"




       IsDebuggerPresentPtr OriginalIsDebuggerPresent;
       CheckRemoteDebuggerPresentPtr OriginalCheckRemoteDebuggerPresent;
       NtSetInformationThreadPtr OriginalNtSetInformationThread;
       NtQueryInformationThreadPtr OriginalNtQueryInformationThread;
       NtCreateThreadExPtr OriginalNtCreateThreadEx;