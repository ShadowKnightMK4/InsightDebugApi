#pragma once
/*
* 
* All Telemetry templates generally will raise an exception and set arguments to stuff along these types./
* 
* These are the message description types that Insight are already setup for.
* They are divided by Type and action
*/

/*
*	Files and Registry are handled the same.
*/
/* These are the main ones and are in EXCEPTION_ARGS[0]*/
// message is about something to do with memory allocation / freeing / heaps in 
#define MSG_MAIN_MEMORY_HEAPS (1)
// message is about something to do with Virtual Memory routines
#define MSG_MAIN_MEMORY_VIRTUAL (5)
// message is about something to do with the Registry
#define MSG_MAIN_REGISTRY (10)
// message is about something to do with File Access
#define MSG_MAIN_FILES (20)
// message is about something to do with Sockets
#define MSG_MAIN_SOCKETS (30)
// message is about Process handling / spawning
#define MSG_MAIN_PROCESSES (40)


/* for Files Open/Create,  for heaps/virtual allocation requests,  for registry OPEN/CREATE, for sockets, open/listen for processes, Open/Spawn*/
#define MSG_RESOURCE_REQUESTED (1)

/* For Files, Calls to WriteFile and Kin,  For Virtual and Heaps NOT SUPPORTED, for Registry, Calls to WRite to the REgistry, for sockets, calls to write to ports, for processes NA*/
#define MSG_RESOURCE_WRITTEN_TO (10)

/* For Files, calls to ReadFile and Kin, For Virtual Heaps NOT SUPPORTs, for registry, calls to read from the registry, for sockets, falls to read from ports, for processes NA*/
#define MSG_RESOURCE_READ_FROM (20)

/* For files, registry, processes, calls to CloseHandle and RegCloseKey.   Sockets close_socket(),  Memory HeapDestroy() */
#define MSG_RESOURCE_CLOSE (30)

/* No special action. */
#define MSG_TYPE_DEFAULT (0)
/* Debugger is free modify the arguments and they will be sent back to the routine dealing with the resource.
*/
#define MSG_TYPE_CONDITIONAL (10)







