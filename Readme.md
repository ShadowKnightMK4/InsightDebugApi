
About this project
------------------------------------------

Insight API is a tool to learn more about a process.  It should also be suitable for unique debugging needs. It leverages a few APIs – specifically, the Windows Debugging API, the Windows ImageHlp/Symbol API and the Detours API to inject DLLs in the target to gather information about the target and potentially modify how it executes. One does not strictly need source code access to target inspect/modify execution with the telemetry DLL setup, it just must be able to be startable with the DLL.  If a process is protected from having DLLs forcably inserted into a newly spawned process, telemetry DLL use is not possible. Target must also be able to have a debugger spawn it.  There are currently no plans for this API to support to attaching to an already spawned process. 

Telemetry DLLs
------------------------------------------

The Telemetry DLLs are specially written Detours based DLLs that follow the layout indicated with the Detours API (https://github.com/Microsoft/Detours/wiki/DetourCreateProcessWithDll). The DLLs in question override specific imported routines – for example the IODeviceTracking one detours CreateFileA/W.   The detoured routine then packs the arguments, some flags indicating what type of routine was called and a few pointers to writable blocks of memory to let the debugger modify the Last Error value and the returned handle.  It then raises a software level exception by calling the MSDN routine RaiseException(). With how Windows handles exceptions when debugging, the debugged app will freeze to let the debugger inspect/ log the call/ swap out the returned handle. When the debugged app resumes execution it can then just use the handle provided by the debugger and set the last error to the current value.  Telemetry DLLS require the target and the DLL to match bit-ness (x86/x64). The target much also be able to be spawned.  Additionally, a call to ntdll’s NtSetThreadInfo or NtCreateThread must not have succeeded with the SeHideFromDebugger flag.  Once it does, your debugger will no longer get messages from the telemetry app and debugging events in general in the thread that
the call was made from.

Writing your Own Telemetry DLLs
------------------------------------------
You’ll need to consider the scope of the DLL.  One concept you’re going to specifically want to consider is what the target routine sets on output.  For example, CreateFileW returns a HANDLE sized value and sets a DWORD sized last error.  The IoDeviceTracking telemetry packs pointersto writable blocks of memory in the exception it generates when these are called. These give the debugger to a way modify these values. It also packs the arguments into an array suitable for RaiseException(). RaiseException() raises an exception that the debugger will see. During this  exception Windows freezes the app until the debugger returns control back to Windows. The debugger is free to inspect arguments, set a replacement handle and the DWORD sized last error value before
returning control to the debugged app.  The InsightSheath DotNet DLL contains the code to do this. Also a head of time, the Sheath and the Telemetry DLL have an an agreed upon  exception value that means that the exception is from said telemetry DLL. The value matters not as long as it does not conflict with an existing exception and the right bit is set to indicate that it is a user code generated exception. With this system, the debugger can be a metaphorical man in the middle.  
	If you don’t need any logging and have fixed needs, you may just be fine with telemetry DLL that does not need something written in the sheath to interpret exceptions generated from it.  Consider a hypothetical example.  One needs all calls to CreateFileW to first check  “C:\MyCustomRepository” before other locations. A solution would be detouring CreateFileW to code that does the check before calling the original CreateFileW. You may want to include logging info BUT you don’t have too.


IMPORTANT
------------------------------------------
There is a bit of cross over code between x64 and x86 in terms of pointer handling.  With how ReadProcessMemory() from deals with x64 pointers from x86, it is recommended that you use the x64 bit settings if you’re working with an OS that has both.  You may also need to enable ‘Debug Native Code’ under its project setting if using Insight from C# as well as the Debug Child Processes Tool for Visual Studio (https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool&msclkid=a4376279aef511ec8a5c3023ac5217e9). The Debug Child Process Tool will help when debugging telemetry.  The project is built/ tested with Visual Studio 2019 and C#’s .NET 5.0.  There’s a good chance you will need to do some project adjustments if you decide to target a different version.  The 2 main projects are InsightSheath and InsightAPI.  The current remotely stable Telemetry DLL is the IoDeviceTracking, but that's stretch.  It works the most out of the various ones. 



Native Build Folders
------------------------------------------
To Simply building I’ve attempted to set paths via visual studio macros.  Also, if you consider %cd% to be the location where you’ve stashed the project then solution setup is below.  The end plan for the solution is thru a combination of macros, include folders/ect in the project folders to let a person download the entire thing and hit build.

| Folder Location |  What does it hold |
------------------|--------------------
| '%cd%\code\debug' |  Contains binaries made with the debug build |
| '%xd%\code\debug\lib'	| contains built Native static libraries made |
| '%cd%\code\debug\program' |contains Native EXE and DLLs made |
| %cd%\code\debug\program\Telemetry |contains Native Dlls and PDBs of said dlls build for telemetry. |
| %cd%\code\release | Contains Native binaries made with the release config |
| %cd%\code\release\lib	| contains Native static libraries made |
| %cd%\code\release\program | contains Native EXE and DLLs made |
| %cd%\code\release\program\Telemetry | contains Native Dlls and PDBs of said dlls build for telemetry. |


Branch Navigation
------------------------------------------
This is the general dev branch.  It may or may not be able to be built and features being working on are subject to change.

Branch Specific Notes go here.


Source Citing
------------------------------------------
- [Detours](https://github.com/Microsoft/Detours)
- [The Idea to use a CreateEvent object to sync so a GUI is from here](https://www.codeproject.com/articles/132742/writing-windows-debugger-part-2#Halt_at_SA)
- [Idea to use events and a worker thread form there.](https://www.codeproject.com/Articles/132742/Writing-Windows-Debugger-Part-2)
- [Learned about some of Windows thread here](https://www.codeproject.com/Articles/662735/Internals-of-Windows-Thread)
- [One can learn about the the SeHideFromDebuger flag here](https://www.codeproject.com/articles/1090943/anti-debug-protection-techniques-implementation-an#HowToNeutralizeNtCreateThreadEx)
- [The way Windows deals with Exceptions while debugging is here.](https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-raiseexception)
- [MSDN database for Win32 Programming](https://docs.microsoft.com/en-us/windows/win32/api/)
