#pragma once



#ifdef FILESANDBOX_AP_DLL_EXPORT
#define FILESANDBOX_API_DLL __declspec(dllexport) WINAPI 
#else
#define FILESANDBOX_API_DLL __declspec(dllimport) _stdcall
#endif
