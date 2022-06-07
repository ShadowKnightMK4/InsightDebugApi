extern "C" {
	/// <summary>
	/// Get the stored value for the SizeOfStruct member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->SizeOfStruct and returns 0 if that=0</returns>
	DWORD WINAPI ImageHlp_GetSizeOfStruct(const IMAGEHLP_MODULE64* that) noexcept;

	/// <summary>
	/// Get the stored value for the BaseOfImage member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->BaseOfImage and returns 0 if that=0</returns>
	DWORD64 WINAPI ImageHlp_GetBaseOfImage(const IMAGEHLP_MODULE64* that) noexcept;


	/// <summary>
	/// Get the stored value for the ImageSize member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->ImageSize and returns 0 if that=0</returns>
	DWORD WINAPI ImageHlp_GetImageSize(const IMAGEHLP_MODULE64* that) noexcept;

	/// <summary>
	/// Get the stored value for the TimeDateStamp member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->TimeDateStamp and returns 0 if that=0</returns>
	DWORD WINAPI ImageHlp_GetTimeDateStamp(const IMAGEHLP_MODULE64* that) noexcept;

	/// <summary>
	/// Get the stored value for the CheckSum member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->CheckSum and returns 0 if that=0</returns>
	DWORD WINAPI ImageHlp_GetCheckSum(const IMAGEHLP_MODULE64* that) noexcept;

	/// <summary>
	/// Get the stored value for the NumSyms member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->NumSyms and returns 0 if that=0</returns>
	DWORD WINAPI ImageHlp_GetNumSyms(const IMAGEHLP_MODULE64* that) noexcept;

	/// <summary>
	/// Get the stored value for the SymType member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->SymType and returns 0 if that=0</returns>
	DWORD WINAPI ImageHlp_GetSymType(const IMAGEHLP_MODULE64* that) noexcept;

	/// <summary>
	/// Get the stored value for the BaseOfImage member for this structure.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->BaseOfImage and returns 0 if that=0</returns>
	CHAR* WINAPI ImageHlp_GetModuleName(const IMAGEHLP_MODULE64* that) noexcept;


	/// <summary>
	/// Get the stored value for the ModuleName member for this structure. Important!!! This is a fixed CHAR array of 32 bytes.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->ModuleName and returns 0 if that=0</returns>
	CHAR* WINAPI ImageHlp_GetImageNameA(const IMAGEHLP_MODULE64* that) noexcept;

	/// <summary>
	/// Get the stored value for the ImageName member for this structure. Important!!! This is a fixed CHAR array of 256 bytes.
	/// </summary>
	/// <param name="that">pointer to one IMAGEHLP_MODULE64 struct</param>
	/// <returns>returns that->ImageName and returns 0 if that=0</returns>
	CHAR* WINAPI ImageHlp_GetLoadedImageNameA(const IMAGEHLP_MODULE64* that) noexcept;

}