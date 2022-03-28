Welcome:
	Insight functionally uses the Windows Debugger API, structured Exception handling,  has support for using detours to forcible load DLLs in a newly spawned process.

Included in the solution are these proejects.


DllHelpers
	Telemetry Dlls	
		IoDeviceTracking		   <- Detours CreateFile gang to give data on what the spawned process is attempting to access.
								   <- If target process skips these and calls NtCreateFile / NtOpenFIle, it won't be caught.
		MemoryTrackingTemetryDll   <- A better name would be Heaps detour.
								   <- This gives detours the MSDN listed heap routines and sends data back to Insight.


ExampleSamples
	DetourChainingRoutines		  <- Shows what happens when detouring a routine you've already detoured.
								  <- It's a last in first called thing.
	HelloWorld					  <- Nosy app to generic debug stuff.

FrontEnds
	InsightSheath				  <-  this is a C# wrapper between InsightAPI.dll and C#.
	InsightDebugger_GUI			  <-  this is the main project that I test features with.


TopLevel
	Detours
		Pulled from Github-> Compiles as a static lib.

	

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


------------------------------------------
	License
------------------------------------------
	TBD:
			3rd party licenes:
					https://github.com/microsoft/Detours/blob/master/LICENSE.md
					https://processhacker.sourceforge.io/gpl.php
					https://www.codeproject.com/Articles/132742/Writing-Windows-Debugger-Part-2    <- idea to use events and a worker thread form there.
					https://www.codeproject.com/Articles/662735/Internals-of-Windows-Thread  <- learned about some of Windows hreading
			Quick overview:
				Detours using the MIT license while Process Hacker is using the GPLv3 license with a special case for dynamic linking.
				The file in the StaticIncludes ntpsapi.h is lfting from the Source specificed. I'm wondering is this would count as using Process Hacker source or Windows NT documented source?
			Issue to Ask on Proces Hacker forum>
				Is using https://processhacker.sourceforge.io/doc/ntpsapi_8h_source.html here grounds for being under the GPLv3 license.



