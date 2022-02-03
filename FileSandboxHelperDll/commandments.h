#pragma once
#include <Windows.h>
struct Commandments
{
	
	BOOL FileStripRead;
	BOOL FileDenyReadOK;
	BOOL FileDenyReadFail;

	BOOL FileStripWrite;
	BOOL FileDenyWriteOK;
	BOOL FileDenyWriteFail;


	BOOL ProcessDenySpawnOK;
	BOOL ProcessDenySpawnFail;

	BOOL ProcessPropergateToTarget;

};

extern Commandments ActiveCommandments;