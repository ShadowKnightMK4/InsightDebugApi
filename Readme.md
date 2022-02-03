Welcome:
	FileSandbox functionally uses the Windows Debugger API, structured Exception handling, and a helper DLL that Detours certain Nt routines to monitor
	a target app.



Detours
	Pulled from Github-> Compiles as a static lib.
FileSandboxConsole
	NativeApp
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
	REQUIRES Target to be able to be debugged.
	Can let you modify arguments and inspect at runtime.
HelperDllNoProcessSpawn
	DLL that Detours NtCreateUserProcess to tell ACCESS_DENIED.  Does throw an exception for a debugger to catch to 
	see what was attempted to ran
HelperDllNoFileWrite
	DLL that will strip GENERIC_WRITE out of NtCreateFile and NtOpenFile.
HelperDllNoFileRead
	DLL that will strip GENERIC_READ out of NtCreateFile and NtOpenFile

FileSandBoxApi
	Implements the various C++ classes that handle the Debugger Loop, process sawning and mode.
	IMPORTANT:    Defaults to emitting itself to the normal output folder under "code"; HOWEVER, 
	it also copies the resulting dll plus pdb file to make 	FILESANDBOX_GUI happy when compiling.
	Deletiving this command means the C# project likely will pick up an old copy and possibly not have currect PDB files.
FileSandBoxClientDllProtocol
	StaticLib that houses the communication api code between FileSandBoxApi and FileSandBoxHelper.  
	This mis mostly just a few data structures
StaticIncludes
	This library hows the Windows.h, and the nt stuff that not specific.
FileSandBoxGui (TDB)
	TODO C# Based Front End
FileSandBoxSheath 
	C# stub that implmenets and interference between FileSandBoxApi's native dllnessa and C#.
	Your Target C# code build needs to match the version you built this one with. IE. for your c# project
	Set CPU to x86 on this if using the x86 front end
	and set CPU to x64 if using a x64 front end.



Builk of the core is FilesandboxApi.dll
FileSandBoxGUI provides the C# based front.
FileSandBoxSheath.dll is a C# wrapper for FileSandBoxApi.dll
StaticIncludes contains some shared include files
FileSandBoxClientDllProtocol houses the commnuncation for tge Helper Dll talking to the debugging app.
FileSandboxHelperDll  is the Detour DLL that detours certain nt routines to let the debuugger modifiy action.

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
				%cd%\code\debug\lib				-> contains NAtive static libraries made 
				%cd%\code\debug\program			-> contains Native EXE and DLLs made
			%cd%\code\release					-> Contains Native binaries made with the release config
				%cd%\code\release\lib			-> contains Native static libraries made 
				%cd%\code\release\program		-> contains Native EXE and DLLs made



------------------------------------------
ACCTIVE CURRENT ISSUES
------------------------------------------
		A current issue is one I'm working on. This doesn't mean the software is bug free or working if no issues are listed. THERE ARE LIGHTLY BUGS/ Unintended con
		
		Found out that testing this with cmd.exe does raise Norton Antivirus alarms.  This is tencially an issue if monitoring an app as we are writing to a process's memory.'

		PIck a new project name:

			DreamHub?
			Matrix?
			TheOracle?
			Keep FileSandBox


------------------------------------------
SOURCE Citing
------------------------------------------
		StaticIncludes has parts source from in the Process Hacker Project (specically the ntpsapi.h file)
			https://processhacker.sourceforge.io/doc/ntpsapi_8h_source.html
			Source for NtCreateUserProcess function definition plus data structures from the  Process Hacker
		Detours from
			https://github.com/Microsoft/Detours

		The Idea to use a CreatEvent object to sync so a GUI is still responsbile is from here
			https://www.codeproject.com/articles/132742/writing-windows-debugger-part-2#Halt_at_SA

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
				Command No Write
					Should a call be seeing where write acess os done with a file, the routine fails with an ACCESS_DENIED error message
				Command No Spawn Process
					Should a call be done to try spawnng a process, he routine afils with an ACCESS_DENIED Error message.
				Command Inject Helper DLL
					Should a process be spanned, we force the helper dll to be injected - causing the commandments to be injerited

	Future Ideas:
			Consider making a backup of the source or offering redirection to an arbirary 3rd party rather than just a yes or no.


	Implemntation:
				Detours is used on 
						NtCreateFile, NtOpenFile,  NtCreateUserProcess
						The detour collects the arguments, builds a command data structure and throws an exception for the debugger's use.
						The debugger is free to inspect/ modify the structure.
						The Debugger will need to set a certain flag and copy the structure pass into the debugged process.
						The detoured routine interprets this data structure and changges its output acordingly.



				The Commandments in the helper DLL are done via a pipe() that the debugger can write too and the relevent routines check on they do theri thing.


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