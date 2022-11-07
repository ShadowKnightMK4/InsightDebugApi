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


	/// <summary>
	/// TODO:
	/// </summary>
	public class WindowsUserProcessParameter 
	{

	
		UserProcessParameters32 Marshal32;
		UserProcessParameters64 Marshal64;

    }
}
