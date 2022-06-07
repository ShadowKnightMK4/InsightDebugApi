
extern "C" {
	/// <summary>
	/// If that is not null, return the MajorVersion member of the struct.
	/// </summary>
	/// <param name="that">pointer to an API_VERISON struct in your local process memory.</param>
	/// <returns>If that is not null, return the MajorVersion member of the struct otherwise returns 0</returns>
	USHORT WINAPI DebugHelp_ApiVerionStruct_GetMajor(const API_VERSION* that) noexcept;

	/// <summary>
	/// If that is not null, returns the MinorVersion Member of the struct
	/// </summary>
	/// <param name="that">pointer to an API_VERISON struct in your local process memory.</param>
	/// <returns>If that is not null, return the MinorVersion member of the struct otherwise returns 0</returns>
	USHORT WINAPI DebugHelp_ApiVerionStruct_GetMinor(const API_VERSION* that) noexcept;

	/// <summary>
	/// If that is not null, returns the Revision Member of the struct
	/// </summary>
	/// <param name="that">pointer to an API_VERISON struct in your local process memory.</param>
	/// <returns>If that is not null, return the Revision member of the struct otherwise returns 0</returns>
	USHORT WINAPI DebugHelp_ApiVerionStruct_GetRevision(const API_VERSION* that) noexcept;

}