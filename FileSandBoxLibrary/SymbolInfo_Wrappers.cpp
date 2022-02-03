#include <Windows.h>
#include <DbgHelp.h>
#include "Utility.h"

extern "C" {


	/// <summary>
	/// duplicate an instance of the SymbolINfoW so its completely seperate from this.
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	SYMBOL_INFOW* SymbolInfoW_Duplicate(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			size_t calc = that->SizeOfStruct + ((that->MaxNameLen - 1) * sizeof(wchar_t));
			SYMBOL_INFOW* ret;
			ret = (SYMBOL_INFOW *)malloc(calc);
			if (ret)
			{
				memcpy(ret, that, calc);
				return ret;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// return the that->SizeofStruct
	/// </summary>
	/// <param name="that"></param>
	/// <returns></returns>
	ULONG SymbolInfoW_GetStructSize(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->SizeOfStruct;
		}
		return 0;
	}


	ULONG SymbolInfoW_GetTypeIndex(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->TypeIndex;
		}
		return 0;
	}

	ULONG SymbolInfoW_GetSymbolSize(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->Size;
		}
		return 0;
	}

	ULONG SymbolInfoW_GetFlags(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->Flags;
		}
		return 0;
	}

	ULONG SymbolInfoW_GetVal(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->Value;
		}
		return 0;
	}

	ULONG SymbolInfoW_GetReg(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->Register;
		}
		return 0;
	}

	ULONG SymbolInfoW_GetValOrReg(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			if ( (that->Flags && SYMFLAG_REGISTER)  == (SYMFLAG_REGISTER))
			{
				return that->Register;
			}
			if ((that->Flags && SYMFLAG_VALUEPRESENT) == (SYMFLAG_VALUEPRESENT))
			{
				return that->Value;
			}
		}
		return 0;
	}

	ULONG SymbolInfoW_GetScope(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->Scope;
		}
		return 0;
	}

	ULONG SymbolInfoW_GetTag(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->Tag;
		}
		return 0;
	}

	

	ULONG SymbolInfoW_GetNameCharCount(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->NameLen;
		}
		return 0;
	}



	WCHAR* SymbolInfoW_GetName(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			if (that->NameLen != 0)
			{
				return that->Name;
			}
		}
		return nullptr;
	}

	ULONG SymbolInfoW_GetIndex(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->Index;
		}
		return 0;
	}

	ULONG64 SymbolInfoW_GetModBase(SYMBOL_INFOW* that)
	{
		if (that != nullptr)
		{
			return that->ModBase;
		}
		return 0;
	}


}

