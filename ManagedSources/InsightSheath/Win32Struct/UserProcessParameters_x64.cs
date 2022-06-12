using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Win32Struct
{

	/// <summary>
	/// contains the x64 version of the Native struct RTL_USER_PROCESS_PARAMETERS as documented here and comparing offsets between the struct in the OffsetAttrib sample project for Windows 10. <see href="https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-rtl_user_process_parameters"/>
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
	public struct UserProcessParameters64
	{
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = 16)]
		public byte[] Reserved1;
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U4, SizeConst = 10)]
		public ulong[] Reserved2;
		public UnicodeString64 ImagePathName;
		public UnicodeString64 CommandLine;
	}
}
