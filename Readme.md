Welcome:
	Insight functionally uses the Windows Debugger API, some BASIC Symbol browsing and has added detours to let one spawn processes with abritary dlls.
Included in the solution are these proejects.




-----------------------------------
Build Configurations
-----------------------------------
	Release
		Turns on stuff to reduce code size, ect... 
	Debug
		Debug the stuff. 
	ReleaseTiny
		Turns on stuff to max project files slw
	ReleaseMD
		Dont use this. The project got this from inputing Detours. I haven't removed it yet.
	DebugMD
		Dont use this. The project got this from inputing Detours. I haven't removed it yet.

---------------------------------------
Project Layout
---------------------------------------
	If you assume %cd% is where the project is extracted too then

	%cd%\ManagedSources				<- contrains the c# source.
		%cd%\ManagedSources\Insight_GUI		<- This app is used when I'm working with the source. 
		%cd%\ManagedSources\InsightSheath	<- This .NET 5.0 project provides a wrapper between C# and The native dll.


	%cd%\NativeSources
		%cd%\NativeSources\Detours-master	<- gotton from github and set to be build as a static library.
							<- Includes the samples but they've not been built.
		%cd%\NativeSources\InsightApiCore	<- This is the source for the core dll for the Insight API library.
		%cd%\NativeSources\TelemetryDlls	<- Contains various dlls intended to show what having the ability to force dll loading on a target app can do.

	%cd%\TestingAndSamples
		%cd%\TestingAndSamples\HelloWorld		<- App to generate noise to generate debug events.
		%cd%\TestingAndSamples\Sanmples\DetourChaining  <-  Demostrates how detours routines themselfs can be detoured.  The last replacement routine is the first
								<-  one that is called (QUEUE).
			
---------------------------------------
Build Project Folders
---------------------------------------


		%cd%\code						->  All Built stuff using Release/Debug configs
			%cd%\code\debug					-> Contains binaries made with the debug
				%cd%\code\debug\lib			-> contains built Native static libraries made 
				%cd%\code\debug\program			-> contains Native EXE and DLLs made
				%cd%\code\
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



