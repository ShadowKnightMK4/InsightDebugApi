

This is a set of common routines the Telemetry Dlls share.

DllMain is defined here. It does require a Unique routine named
"bool DetourTargetRoutine()" defined for each specific code.

Each Project IoDeviceTracking/HeapTracking provides this.



This routine should return true if it is ok to contine and false if not.


Detours static lib is already linked. 

define the value PIN_TELEMETRY_DLL in the project settings to request that Telemetry built with this
static lib will request being PINNEd in memory to prevent kernal from unloading the.


