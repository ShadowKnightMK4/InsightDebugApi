Welcome:
	FileSandbox functionally uses the Windows Debugger API, structured Exception handling, and a helper DLL that Detours certain Nt routines to monitor
	a target app.



Detours
	Pulled from Github-> Compiles as a static lib.
FileSandboxConsole
	App that using Detours to launch arbitrary app with the FileSandboxHelperDll inserted
	The app itself implements a Debugloop and the helper dll Detours routines that will not trigger exceptions. These exceptions expose the arguments 
	to the routine to the debugger via allocated memory. The debugger is free to modify and also has the option to tell the routine to NOT CALL the original
	and just return a value.

		Weakness:
				Should a call to NtSetThreadAttribute or it CreateUserProcess be sucessfull with HideFromDebugger flag, the communcaition protocal will no longer work
				as the commucation is done via SEH exceptions triggering and the debugger seeing them.
FileSandboxHelperDll
	DLL that Detours NtXXXX Api for Cretain File and Registry Routines to spy on what's being accessed.
	Note that this will highly likely trigger Data Protect / Antivirus if used on a procected / core process.
	REQUIRES Target to be able to be debugged
FileSandBoxApi
	TODO: DLL That Implements the code that The  FIleSandBoxConsole and (TDB C# GUI) uses.
FileSandBoxClientDllProtocol
	StaticLib that houses the communication api code between FileSandBox and FileSandBoxHelper. 
StaticIncludes
	This library hows the Windows.h, and the nt stuff that not specific
FileSandBoxGui (TDB)
	TODO C# Based Front End



-----------------------------------
Build Configurations
-----------------------------------
	Release
		Turns on stuff to recude code size, ect... 
	Debug
		Debug the stuff. 
	ReleaseMD
		Dont use this. The project got this from inputing Detours. I haven't removed it yet.
	DebugMD
		Dont use this. The project got this from inputing Detours. I haven't removed it yet.


---------------------------------------
Build Folders
---------------------------------------
	If you assume %cd% is where the project is at, then

		%cd%\code								->  All Built stuff using Release/Debug configs
			%cd%\code\debug						-> Contains binaries made with the debug
				%cd%\code\debug\lib				-> contains static libraries made 
				%cd%\code\debug\program			-> contains EXE and DLLs made
			%cd%\code\release					-> Contains binaries made with the release config
				%cd%\code\release\lib			-> contains static libraries made 
				%cd%\code\release\program		-> contains EXE and DLLs made



------------------------------------------
ACCTIVE CURRENT ISSUES
------------------------------------------
		A current issue is one I'm working on. This doesn't mean the software is bug free or working if no issues are listed. THERE ARE LIGHTLY BUGS/ Unintended con

		Found out that testing this with cmd.exe does raise Norton Antivirus alarms.  This is tencially an issue if monitoring an app as we are writing to a process's memory.'

------------------------------------------
SOURCE Citing
------------------------------------------
		StaticIncludes has parts source from in the Process Hacker Project (specically the ntpsapi.h file)
			https://processhacker.sourceforge.io/doc/ntpsapi_8h_source.html
			Source for NtCreateUserProcess function from Process Hacker
		Detours from
			https://github.com/Microsoft/Detours

			

------------------------------------------
	License
------------------------------------------
	TBD:
			3rd party licenes:
					https://github.com/microsoft/Detours/blob/master/LICENSE.md
					https://processhacker.sourceforge.io/gpl.php
			Quick overview:
				Detours using the MIT license while Process Hacker is using the GPLv3 license with a special case for dynamic linking.
				The file in the StaticIncludes ntpsapi.h is lfting from the Source specificed. I'm wondering is this would count as using Process Hacker source or Windows NT documented source?
			Issue to Ask on Proces Hacker forum>
				Is using https://processhacker.sourceforge.io/doc/ntpsapi_8h_source.html here grounds for being under the GPLv3 license.






	Also There are plugin options.  Is this what you may want to use for your communcation between

	Target Process with helper dll and the core process???