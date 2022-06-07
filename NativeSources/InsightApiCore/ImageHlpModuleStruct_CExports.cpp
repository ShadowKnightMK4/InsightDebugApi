#include <windows.h>
#include <DbgHelp.h>
#include "ImageHlpModuleStruct64_CExports.h"

extern "C" {
	DWORD WINAPI ImageHlp_GetSizeOfStruct(const IMAGEHLP_MODULE64* that) noexcept
	{
		if (that != nullptr)
		{
			return that->SizeOfStruct;
		}
		return 0;
	}

	DWORD64 WINAPI ImageHlp_GetBaseOfImage(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ( (that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->BaseOfImage;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetImageSize(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->ImageSize;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetTimeDateStamp(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->TimeDateStamp;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetCheckSum(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->CheckSum;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetNumSyms(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->NumSyms;
		}
		return 0;
	}


	DWORD WINAPI ImageHlp_GetSymType(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->SymType;
		}

		return 0;
	}

	CHAR* WINAPI ImageHlp_GetModuleName(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return (CHAR*)&that->ModuleName[0];
		}
		return nullptr;
	}

	CHAR* WINAPI ImageHlp_GetImageNameA(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return (CHAR*)&that->ImageName[0];
		}
		return nullptr;
	}

	CHAR* WINAPI ImageHlp_GetLoadedImageNameA(const IMAGEHLP_MODULE64* that) noexcept
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return (CHAR*)&that->ImageName[0];
		}
		return nullptr;
	}
	
	
	

}
