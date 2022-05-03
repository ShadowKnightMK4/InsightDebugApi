----------------------------------
About this Project
----------------------------------
Insight API is a tool to learn more about a process via a variety of ways. It does leverage the Windows Debugging and Symbol API in an object
orientated manner and should be suitable for starting out with a flexible debugging solution for unique needs. It also uses Microsoft's Detours
project to allow spawning processes with a forced dll provided they are written in a certain way. These Dlls can then adjust functionality as
needed via detouring routines in the target and thanks to how Windows processes exceptions, can be in good spot to raise one with the option
of the debugger injecting its handle / last error code into the target routine / or just packing the detoured routine's arguments into an array
for the debugger to log/examine.   While this is indeed a tool for exploring, it should also be an acceptable starting point for needing custom
debugging on windows. The project has a C/C++ back end but focuses heavily on using C# as the front end.



-----------------------------------
Branch Navigation / Purpose.
-----------------------------------
This Readme is for the IODeviceTracker branch - which is working towards making that Telemetry Dll functional.

Functional for this means:

The Telemetry Dll detours for CreateFileA/W, CreateFileTransactedA/W  CreateFile2,
NtOpenFile, CLoseHandle and NtCreateFIle.

The Extension in the sheath has code to let the user test for exceptions from this dll, check the arguments and substitite the returned handle.




-----------------------------------
Leveraging 'Telemetry Dlls'
-----------------------------------
This routine at https://github.com/microsoft/Detours/wiki/DetourCreateProcessWithDllEx will give a starting point for writing one.
In short, your need to follow the requirements outlined for ensuring the process will load it ok.

The next thing you'll need to do is consider your dll's scope. If it does not need to generate data/noise then once the dll is loaded,
you are done. If you need to read via exceptions made from the dll, you'll need to also provide a mechanic to unpack the exception and
present to the sheath/debugger. Let me explain with an example.


The inprogress telemetry dll, Iodevicetracking detours CreateFileA/W. The main thing about CreateFileA/W after calling is it may set 
last error and returns a handle to the caller fit for the local process. To let the debugger modifity these, we include some pointers
to memory locations in the exception args we raise along with some settings indicating just what type of routine the exception is
raised for.  This is half of what's needed to make this work.

The other half is providing a mechanic to translate the exception into a .NET friendly way and also get the arguments. When RaiseException()
is called, one gets the chance to provide an array up to (currently) 15 elements long that the debugger will see. We back the arguments
plus a couple of pointers to a memory for a handle and a dword.   We also include data packed in this array indicating what type of routine
was called. 

The result of this and writting a wrapper to process this means we now enough what the routine is trying to do and can log it or override
the final error value and handle with DuplicateHandle().  In short, when using the telemetry dll and the wrapper, the debugger is a man in
the middle. 

We also have the option of examing the call and setting the handle to / invalid handle value plus last error to something to fake deny it.





-----------------------------------
Build Configurations
-----------------------------------
	Release
		Turns on stuff to reduce code size, ect... 
	Debug
		Debug the stuff. 

-----------------------------------
IMPORTANT
-----------------------------------
Although there is some skeleton code for Windows x64 bit, it is NOT functional and currently does not compile.
To debug only the c# save pick .NET debugging.  If you want to debug the Native side (InsightAPI.dll) ensure 
your managed .NET project has the "Debug Native Code" checkbox enabled under it's project debug properties.  
You may want to also enabled mixed mode debugging in Visual Studio. It's possible that you'll lose your
ability to change the code while debugging if this is enabled.  I do recommend the debug child processes tool
(https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool&msclkid=a4376279aef511ec8a5c3023ac5217e9)
This project is being built/testing with Visual Studio 2019. You may need to do some adjustments if using a 
different version. The Sheath DLL and the InsightDebugger_GUI test project under the front ends folder
both target .NET 5 core.



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

		Building for x64 bit is not implemented correctly. Don't expect a successful build for it. 


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
				 Found in the root folder of the solution.  InsightAPI, InsightSheath, Telemetry DLLs including in the package
				 fall under the MIT license.  
			TelemetryDlls
				The included Telemetry Dlls are functionally under the MIT license.

			3rd party license:
					https://github.com/microsoft/Detours/blob/master/LICENSE.md

