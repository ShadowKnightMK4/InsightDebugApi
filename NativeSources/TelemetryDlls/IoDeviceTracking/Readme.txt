
IoDviceTracking Project
Should resolve to \Telemetry\IoDeviceTracking.dll.

This file project detours CreateFileA/W and CreateFileTransactedA/W  (CreateFile2 also),  and 
.


The general detoured routine calls code to convert the string to Unicode, calls a common notification routine that raises
an exception (there are 2 hooks for the debugger to modify the handle and lastError) before returning control back to the
debugger.


IMPORTANT!
	NtCreateFile and NtOpenFile are lower level then the General Win32 API. If you choose to detour those, you'll get a more throughout
	view of the data points accessed, BUT you will need to tell is translating said code. NtCreateFile / NtOpenFile are a little more 
	platform dependent than CreateFileA/W. The plan for this Telemetry dll is to  eventually detour those.

IMPORTANT!!
	For x64 bit systems running Wow64, your NtCreateFile/NtOpenFile from what I've read are functionally interrupts/ special calls that
	switch to x64 bit mode to make the call and return back to x86 mode to return control to your software.  Should you be using a 
	telemetry dll against an app that makes the calls directly, this wont pick up.  You would need a global hook into the Kernel 
	interrupt table and that's something I do not have the skill or desire to do yet.
IMPORTANT!!!
	Should a Call to NtSetThreadInformation or NtCreateThread succeed with the Hide from debugger flag against a thread in question on the
	target app, you will no longer get notifications from the app as said notifications are done via Raising Exceptions via RaiseException()
OPTIONAL
	It is recommended that if you are using this to block access/ a feature of something to match known documentation for the detoured routine.




Applications?
Track what the targeted app in question tries to access in the file system.
Act as a Man in the Middle to redirect to another source or deny access based on the arguments.  To Deny, you'll need the debugger sheath
to open the file based on the arguments stuff and duplicating into the debugged process.

Modify This project to tell it to attempt to seek a file from a resource such as a zip file first before referring to the system.


IoDeviceTracking Functionality Desired
NtCreateFile/ NtOpenFile / NtCreateDirectory/ ntOpenDirectory detouring.
The sheath code (speically  InsightSheath/TelemetryReader.cs 's stuff) works with the code