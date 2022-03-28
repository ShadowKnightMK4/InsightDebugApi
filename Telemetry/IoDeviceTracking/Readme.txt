
IoDviceTracking Project
Should resolve to \Telemetry\IoDeviceTracking32.dll and IoDeviceTracking64.dll

This detours 5 createfile varients and throws exceptions to pass arguments to a debugger to notify what the app is trying to access.

Currently does not Detour NtCreate and NtOpen file. Software that calls that woun't be detected.
At Startup it looks for a file in the main modules working directory called redirects.dat - a renamed zip file.

Opens and creates that match that once resolves are given handls to that.



