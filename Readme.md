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
		%cd%\NativeSources\Detours-master	<- gotten from git hub and set to be build as a static library. Should be Version 4.0.0
											<- Includes the samples but they've not been built.
		%cd%\NativeSources\InsightApiCore	<- This is the source for the core DLL  for the Insight API library.
		%cd%\NativeSources\TelemetryDlls	<- Contains various DLLs intended to show what having the ability to force DLLs loading on a target app can do.
											<- Mostly a foundation I plan on building on.

	%cd%\TestingAndSamples
		%cd%\TestingAndSamples\HelloWorld		<- App to generate noise to generate debug events. No specific needs.
		%cd%\TestingAndSamples\Samples\DetourChaining  <-  Demonstrates how detours routines themselves can be detoured.  The last replacement routine is the first
								<-  one that is called (QUEUE).
			
---------------------------------------
Native Build Project Folders
---------------------------------------
%cd% is where one's extracted the project too.
		%cd%\code									->  All Built stuff using Release/Debug configs
			%cd%\code\debug							-> Contains binaries made with the debug
				%cd%\code\debug\lib					-> contains built Native static libraries made 
				%cd%\code\debug\program				-> contains Native EXE and DLLs made
				%cd%\code\debug\program\Telemetry	-> contains Native Dlls and PDBs of said dlls build for telemetry.
			%cd%\code\release						-> Contains Native binaries made with the release config
				%cd%\code\release\lib				-> contains Native static libraries made 
				%cd%\code\release\program			-> contains Native EXE and DLLs made
				%cd%\code\release\program\Telemetry -> contains Native Dlls and PDBs of said dlls build for telemetry.

----------------------------------------
Manged Build Project Folders
----------------------------------------
%cd% is where one's extracted the project too.

	%cd%\ManagedSources
			%cd%\ManagedSources\Insight_Gui			-> Contains the console app I use to test/ work with features. Highly subject to change. References InsightSheath
			%cd%\ManagedSources\InsightSheath		-> Contains the DotNet Wrapper for c# that inputs InsightAPI.DLL routines exposes them to C#.
	
-----------------------------------------
Building and Troubleshooting building the software
-----------------------------------------
	Everything should be set up thanks to Visual Studio Macros to be build ready when you extract the project.
	The Managed side targets .NET 5.0 and C#.
	The Native side is a combination of C and C++ with C level wrappers exported that the Managed side imports for use.
	The Project Insight_GUI adds the Sheath project as a reference and has a post build event to copy Insight.* from InsightApi's' output folder to Insight_GUI output folder every time it is built.
	Insight_GUI also has the Platform Target for it set to x86 instead of AnyCPU. 
	


	Getting Started with attempting to switch to x64 bit will involve manually changing this post build event to copy the correct files.

 If for some reason it building does not work, drop it at this location C:\Users\Thoma\source\repos\InsightAPI\.
 Ensure this path  (InsightApi's Project Properties->VC++ Directory->Include) include this in addition to other path info.
		"$(SolutionDir)NativeSources\Detours-master\src"	-> This is the location of the Detours project source.
 Ensure this path (InsightAPI's Project Properties->VC++ Directory->Lib') includes this in addition to other info.
		"$(SolutionDir)Code\$(Configuration)\$(PlatformTarget)\lib" -> This is where the InsightAPI and Detours projects emit their static libs.

------------------------------------------
ACCTIVE CURRENT ISSUES
------------------------------------------
		These are current issues is one I'm working on. This doesn't mean the software is bug free or working if no issues are listed. 
		The possibility exists that bugs will highly likely be found in this software.
	
		Included TELEMETRY DLLS are not functional yet!

		Building for x64 bit is not implemented correctly. Don't export a successful build for it. 


------------------------------------------
SOURCE Citing
------------------------------------------
		Detours from
			https://github.com/Microsoft/Detours

		The Idea to use a CreateEvent object to sync so a GUI is from here
			https://www.codeproject.com/articles/132742/writing-windows-debugger-part-2#Halt_at_SA

		https://www.codeproject.com/Articles/132742/Writing-Windows-Debugger-Part-2    <- idea to use events and a worker thread form there.
		https://www.codeproject.com/Articles/662735/Internals-of-Windows-Thread  <- learned about some of Windows thread


		https://docs.microsoft.com/en-us/windows/win32/api/ <- the rather large MSDN documentation helped with the Windows API.
------------------------------------------
	License
------------------------------------------
Everyone is concerned for licenses.  You can find License information under the license folder.

			Detours 3rd Party
				https://github.com/microsoft/Detours/blob/master/LICENSE.md (also under %cd%\Licenses\Detours)
				
			InsightAPI
				Two flavors, a commercial and noncommercial one.  Individuals will likely fall under
				the noncommercial one while people acting on behalf of a company likely will fall under
				the commercial one. The current primary different is the noncommercial one does not let you
				distribute InsightAPI for money.  The commercial one does once an appropriate agreement is reached.
			TelemetryDlls
				The included Telemetry Dlls are functionally under the MIT license.

			3rd party license:
					https://github.com/microsoft/Detours/blob/master/LICENSE.md

