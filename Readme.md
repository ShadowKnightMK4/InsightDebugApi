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



---------------------------------------------
Commandments Blerb
---------------------------------------------
	After watching a certain show I thought of the idea of using a series of Yes/No options with the current setup of 
		Custom Debugger,
				HelperDll throws exception that contains information about the routine being called,
				Custom Debugger Checks its commandment list and tells the helper dll to just return a certain value.

		Requiements:
				Target Process Must be debugable and be option to the way Detours Injects DLLs
				Target Process can have been spawned with or hat the NtSetFormationThread with the hide from debugger attribute as sourced here
					https://www.apriorit.com/dev-blog/367-anti-reverse-engineering-protection-techniques-to-use-before-releasing-software
				If the call goes thru, the link between our custom debugger and helper dll is broken, these commandments wont work.
					TODO: Consider A set of shared handles that receives the SEH instead or an alternative approach

		List:
				Commandment No Read
						Should a call be seeing where read acess os done with a file, the routine fails with an ACCESS_DENIED error message
				Commamd No Write
					Should a call be seeing where write acess os done with a file, the routine fails with an ACCESS_DENIED error message
				Command No Spawn Process
					Should a call be done to try spawnng a process, he routine afils with an ACCESS_DENIED Error message.

	Future Ideas:
			Consider making a backup of the source or offering redirection to an arbirary 3rd party rather than just a yes or no.


	Implemntation:
				Detours is used on 
						NtCreateFile, NtOpenFile,  NtCreateUserProcess
						The detour collects the arguments, builds a command data structure and throws an exception for the debugger's use.
						The debugger is free to inspect/ modify the structure.
						The Debugger will need to set a certain flag and copy the structure pass into the debugged process.
						The detoured routine interprets this data structure and changges its output acordingly.
------------------------------------------
	License
------------------------------------------
	TBD:
			3rd party licenes:
					https://github.com/microsoft/Detours/blob/master/LICENSE.md
					https://processhacker.sourceforge.io/gpl.php
					https://www.codeproject.com/Articles/132742/Writing-Windows-Debugger-Part-2    <- idea to use events and a worker thread form there.
			Quick overview:
				Detours using the MIT license while Process Hacker is using the GPLv3 license with a special case for dynamic linking.
				The file in the StaticIncludes ntpsapi.h is lfting from the Source specificed. I'm wondering is this would count as using Process Hacker source or Windows NT documented source?
			Issue to Ask on Proces Hacker forum>
				Is using https://processhacker.sourceforge.io/doc/ntpsapi_8h_source.html here grounds for being under the GPLv3 license.






	Also There are plugin options.  Is this what you may want to use for your communcation between

	Target Process with helper dll and the core process???