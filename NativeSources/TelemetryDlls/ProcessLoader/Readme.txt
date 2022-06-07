
ProcessLoader Project



The general detoured routine calls code to convert the string to Unicode, calls a common notification routine that raises
an exception (there are 2 hooks for the debugger to modify the handle and lastError) before returning control back to the
debugger.

This project detours

LoadLibraryA/W
LoadLibraryExA/W
GetProcAddress
CreateProcessW
Stretch (not required) NtCreateUserProcess


TODO:
LoadLibrayA/W: given points for the debugger to change (return value, last error ptr, different library)
LoadLibrayExA/W: given points for the debugger to change (return value, last error ptr, different library)
GetProcAddress: given pointes for the debugger to change(return value, different orginal/routien name)
CreateProcessW:  Figure the scope of what we present for the debugger to change. 
				 We should probably present changing the target process, arguments, security pts to the debugger to modify
				 before continueing as well as a way to force a return valuel.





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