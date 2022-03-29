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

------------
IMPORTANT
-----------
Although there is some skeleton code for Windows x64 bit, it is NOT functional and currently does not compile.
Do you wish to debug THe native DLL and your managed project? Ensure mixed debugging is on. You'll lose the ability to modify your code while doing this.
I do recommend the debug child processes tool (https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool&msclkid=a4376279aef511ec8a5c3023ac5217e9)
for whatever visual of Visual studio you will be using this with.
The Sheath DLL and the InsightDebugger_GUI both target .NET 5 core.



---------------------------------------
Project Layout
---------------------------------------
	If you assume %cd% is where the project is extracted too then

	%cd%\ManagedSources				<- contains the c# source.
		%cd%\ManagedSources\Insight_GUI		<- This app is used when I'm working with the source. 
		%cd%\ManagedSources\InsightSheath	<- This .NET 5.0 project provides a wrapper between C# and The native dll.


	%cd%\NativeSources
		%cd%\NativeSources\Detours-master	<- gotten from git hub and set to be build as a static library.
							<- Includes the samples but they've not been built.
		%cd%\NativeSources\InsightApiCore	<- This is the source for the core DLL  for the Insight API library.
		%cd%\NativeSources\TelemetryDlls	<- Contains various DLLs intended to show what having the ability to force DLLs loading on a target app can do.
											<- Mostly a foundation I plan on building on.

	%cd%\TestingAndSamples
		%cd%\TestingAndSamples\HelloWorld		<- App to generate noise to generate debug events.
		%cd%\TestingAndSamples\Samples\DetourChaining  <-  Demonstrates how detours routines themselfs can be detoured.  The last replacement routine is the first
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
		
		Found out that testing this with cmd.exe does raise Norton Anti virus alarms.  
		This is  an issue if monitoring an app as we are writing to a process's memory.
		That being said, I'd be more concerned  if it didn't flag it.



------------------------------------------
SOURCE Citing
------------------------------------------
		Detours from
			https://github.com/Microsoft/Detours

		The Idea to use a CreatEvent object to sync so a GUI is still responsbile is from here
			https://www.codeproject.com/articles/132742/writing-windows-debugger-part-2#Halt_at_SA


------------------------------------------
	License
------------------------------------------
Everyone is concerned for licenses. 

			Primary Licensee:

			FOR INDIVIDUALS in a non company/profit setting where you won't be distributing this beyond a single machine.
					No restrictions provided you site your source. 


					


			3rd party license:
					https://github.com/microsoft/Detours/blob/master/LICENSE.md
					https://www.codeproject.com/Articles/132742/Writing-Windows-Debugger-Part-2    <- idea to use events and a worker thread form there.
					https://www.codeproject.com/Articles/662735/Internals-of-Windows-Thread  <- learned about some of Windows thread


