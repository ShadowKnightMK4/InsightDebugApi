
IoDeviceTracking Project
Should resolve to code\configuration\Telemetry\x86\program\Telemetry\IoDeviceTracking32.dll for the x86 version and
		  code\configuration\Telemetry\x64\program\Telemetry\IoDeviceTracking64.dll for the x64 version. 

This file project detours  these file relating routines, triggers an exception when each are called via SEH and
provides some pointers/access pointers to arguments to enable a debugger watching for these events to modify 
or substite a different value.  For example CreateFileA/W both give a handle pointer and a last error pointer to
let the debugger give the debugged app a different handle. This project generally serves as the template for the 
rest of the telemetry projects. The plan is to eventually finalize the common stuff and project settings then
export as a visual studio template.

List of Routines detoured in this project.
CreateFileA/W,
CreateFileTransactedA/W,
CreateFile2 but that's set as not implemented,
NtCreateFile,
NtOpenFile.
ReadFile
NtReadFile
WriteFile
NtWriteFile.
CloseHandle




The general detoured routine calls code to convert the string to Unicode, calls a common notification routine that raises
an exception (there are 2 hooks for the debugger to modify the handle and lastError) before returning control back to the
debugger.


IMPORTANT!
	NtCreateFile and NtOpenFile are lower level then the General Win32 API. IF you're watching for both CreateFileA/W and
these, please understand CreateFileA/W eventually resolve down to the Nt Routines. You'll likely get two exceptions for the
same file access in the order of them being called.
IMPORTANT!
	For x64 bit systems running Wow64, your NtCreateFile/NtOpenFile from what I've read are functionally interrupts/ special calls that
	switch to x64 bit mode to make the call and return back to x86 mode to return control to your software.  Should you be using a 
	telemetry dll against an app that makes the calls directly, this wont pick up.  You would need a global hook into the Kernel 
	interrupt table and that's something I do not have the skill or desire to do yet.
IMPORTANT!
	Should a Call to NtSetThreadInformation or NtCreateThread succeed with the Hide from debugger flag against a thread in question on the
	target app, you will no longer get notifications from the app as said notifications are done via Raising Exceptions via RaiseException().
	Provided you have function definations for thse routines, there's nothing stopping one from attempting to detour them and dropiping the
hide from debugger flag.
IMPORTANT!
	It is recommended that if you are using this to block access/ a feature of something to match known documentation for the detoured routine.
FINALLY




Applications?
Track what the targeted app in question tries to access in the file system.
Act as a Man in the Middle to redirect to another source or deny access based on the arguments.  To Deny, you'll need the debugger sheath
to open the file based on the arguments stuff and duplicating into the debugged process.

Modify This project to tell it to attempt to seek a file from a resource such as a zip file first before referring to the system.

the code


Important:
The Modules (ProcessLoader.cpp / ProcessLoad.h) for debugging purposes at the momement.
They're going to spun off into a Telemetry called ProcessTracking.
