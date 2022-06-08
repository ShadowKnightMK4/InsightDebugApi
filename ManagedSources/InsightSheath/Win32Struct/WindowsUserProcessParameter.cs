using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Win32Struct
{
    /*
	struct RTL_USER32
	{
		BYTE Reserved1[16];
		UINT32 Reserved2[10];
		UNICODE_STRING32 ImagePathName;
		UNICODE_STRING32 CommandLine;
	};
	struct RTL_USER64
	{
		BYTE Reserved1[16];
		UINT64 Reserved2[10];
		UNICODE_STRING64 ImagePathName;
		UNICODE_STRING64 CommandLine;
	};*/

	[StructLayout(LayoutKind.Sequential)]
    public struct UserProcessParameters32
    {
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = 16)]
		public byte[] Reserved1;
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U4, SizeConst = 10)]
		public uint[] Reserved2;
		public UnicodeString32 ImagePathName;
		public UnicodeString32 CommandLine;
	}




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

	public class WindowsUserProcessParameter 
	{

	
		UserProcessParameters32 Marshal32;
		UserProcessParameters64 Marshal64;

    }
}
