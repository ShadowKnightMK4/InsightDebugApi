#include <windows.h>
#include <DbgHelp.h>

extern "C" {
	DWORD WINAPI ImageHlp_GetSizeOfStruct(IMAGEHLP_MODULE* that)
	{
		if (that != nullptr)
		{
			return that->SizeOfStruct;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetBaseOfImage(IMAGEHLP_MODULE* that)
	{
		if ( (that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->BaseOfImage;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetImageSize(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->ImageSize;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetTimeDateStamp(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->TimeDateStamp;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetCheckSum(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->CheckSum;
		}
		return 0;
	}

	DWORD WINAPI ImageHlp_GetNumSyms(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->NumSyms;
		}
		return 0;
	}


	DWORD WINAPI ImageHlp_GetSymType(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->SymType;
		}

		return 0;
	}

	CHAR* WINAPI ImageHlp_GetModuleName(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->ModuleName;
		}
		return nullptr;
	}

	CHAR* WINAPI ImageHlp_GetImageNameA(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->ImageName;
		}
		return nullptr;
	}

	CHAR* WINAPI ImageHlp_GetLoadedImageNameA(IMAGEHLP_MODULE* that)
	{
		if ((that != nullptr) && (that->SizeOfStruct == sizeof(IMAGEHLP_MODULE64)))
		{
			return that->ImageName;
		}
		return nullptr;
	}
	
	
	

}
