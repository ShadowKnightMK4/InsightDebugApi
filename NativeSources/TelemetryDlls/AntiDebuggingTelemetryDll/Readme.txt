AntiDebuggingTelemetry 

A VERY base clock for attempting to use Insight against an app that does not like debugger.

This detours 
IsDebuggerPresent()	-> Always returns value.
NtCreateThread()	-> Checks if thread attmpets to be created with HideFromDebugger
			-> If so, we remove that argument and communicate that to debugger
			-> If not, proceed thru with the call.

NtSetThreadInformation() -> Check if HideFromDebugger is attempting to be set.
			 -> If so, tell debugger and remove command.

NtGetThreadInformation() -> Calls original and presents to debugger to modify (strip/ add
			 -> the HideFromDebugger flag)

CheckRemoteDebuggerPresent()	-> Asks debugger
			
