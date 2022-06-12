using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Win32Struct
{
	/// <summary>
	/// contains the x86 version of the Native struct RTL_USER_PROCESS_PARAMETERS as documented here and comparing offsets between the struct in the OffsetAttrib sample project for Windows 10. <see href="https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-rtl_user_process_parameters"/>
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
	public struct UserProcessParameters32
	{
		/// <summary>
		/// MSDN marks these as reversed. 
		/// </summary>
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = 16)]
		public byte[] Reserved1;
		/// <summary>
		/// MSDN marks this a PVOID (going to be 4 bytes/uint for x86 code and 8 bytes/ulong for x64 code)
		/// </summary>
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U4, SizeConst = 10)]
		public uint[] Reserved2;
		/// <summary>
		/// Location to where the image is spawned from in an x86 process.
		/// </summary>
		public UnicodeString32 ImagePathName;
		/// <summary>
		/// Location to the command line to be passed to the x86 process.
		/// </summary>
		public UnicodeString32 CommandLine;
	}

}
