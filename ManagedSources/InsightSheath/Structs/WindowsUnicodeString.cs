using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Structs
{

	/// <summary>
	/// Which type of struct will this contain.
	/// </summary>
	public enum StructModeType
	{
		/// <summary>
		/// Not defined.  Attempting to access properties in this class will trigger <see cref="InvalidOperationException"/> 
		/// </summary>
		MachineUnknown = 0,
		/// <summary>
		/// Class encapsulates a 32 bit version of the native struct
		/// </summary>
		Machinex86 = 1,
		/// <summary>
		/// Class encapsulates a 64- bit version of the native struct <see cref="UnicodeString64"/> struct
		/// </summary>
		Machinex64 = 2
	}



	/*
     * 	struct UNICODE_STRING32
	{
		USHORT Length;
		USHORT MaxLength;
		UINT Buffer;
	};

	/// <summary>
	///  UNICODE_STRING for x64 debugged processes
	/// </summary>
	struct UNICODE_STRING64
	{
		USHORT Length;
		USHORT MaxLength;
		INT Padding; // alights buffer to correct location
		ULONGLONG Buffer;
	};*/

	/// <summary>
	/// For processed that are Wow/32- bit this is what our UNICODE_STRING struct looks like when gotton.
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
    public struct UnicodeString32
    {
		/// <summary>
		/// Current Length of the string pointed to in Buffer. Note if dealing with <see cref="InsertHere"/>, the memory block pointed to by bufffer currently will be set right after the native location of struct plus its size
		/// </summary>
		public	ushort Length;
		/// <summary>
		/// How big possibly is if the buffer. Note <see cref="InsertHere"/> dealing this is here for compleness sake. Your buffer size was allocated to be big enough to hold that containing string, that's all
		/// </summary>
		public ushort MaxLength;
		/// <summary>
		/// 4 byte pointer to where the Unicode String is allocated. <see cref="InsertHere"/> will place the string right after the location of the returned Unicode String struct
		/// </summary>
        public uint Buffer;
    }

	/// <summary>
	/// For processes that are 64-bit, this is our UNICODE_STRING struct
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
	public struct UnicodeString64
	{
		/// <summary>
		/// Current Length of the string pointed to in Buffer. Note if dealing with <see cref="InsertHere"/>, the memory block pointed to by bufffer currently will be set right after the native location of struct plus its size
		/// </summary>
		public ushort Length;
		/// <summary>
		/// How big possibly is if the buffer. Note <see cref="InsertHere"/> dealing this is here for compleness sake. Your buffer size was allocated to be big enough to hold that containing string, that's all
		/// </summary>
		public ushort MaxLength;
		/// <summary>
		/// Padding to ensure <see cref="Buffer"/> is at the right spot.
		/// </summary>
		public uint Padding;
		/// <summary>
		/// 8 byte pointer to where the Unicode String is allocated. <see cref="InsertHere"/> will place the string right after the location of the returned Unicode String struct
		/// </summary>
		public ulong Buffer;
	}


    /// <summary>
    /// Encapsulates a Windows NT UNICODE_STRING struct for either an <see cref=ModeType.Machinex64"/> or <see cref="ModeType.Machinex86"/> debugged target.
    /// </summary>
    public class WindowsUnicodeString: PlatformDependantNativeStruct
    {
		/// <summary>
		/// After creating an instance, call <see cref="SetStructType(ModeType)"/> and specify which version of a unicode string, this class contrains
		/// </summary>
		/// <param name="Native"></param>
        public WindowsUnicodeString(IntPtr Native): base(Native)
        {
			StructType = StructModeType.MachineUnknown;
        }

		/// <summary>
		/// After creating an instance, call <see cref="SetStructType(ModeType)"/> and specify which version of a unicode string, this class contrains
		/// </summary>
		/// <param name="Native"></param>
		public WindowsUnicodeString(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {
			StructType = StructModeType.MachineUnknown;
        }

		public WindowsUnicodeString(IntPtr Native, bool FreeOnCleanup, StructModeType StructType) : base(Native, FreeOnCleanup, StructType)
        {

        }
		public WindowsUnicodeString(IntPtr Native, StructModeType StructType): base(Native, StructType)
        {

        }
        protected override void Blit()
        {
			{
				if (!WasBlit)
				{
					switch (StructType)
					{
						case StructModeType.Machinex64:
							{
								Machine64 = Marshal.PtrToStructure<UnicodeString64>(Native);
								break;
							}
						case StructModeType.Machinex86:
							{
								Machine32 = Marshal.PtrToStructure<UnicodeString32>(Native);
								break;
							}
						default:
							{
								throw ThrowNewInvalidOpMessage(GetType().Name);
							}
					}

					HaveBlitOnce = true;
				}
			}
		}


        protected override void Dispose(bool disposing)
        {
			bool TargetIs32 = false;
            switch (StructType)
            {
				case StructModeType.Machinex86: TargetIs32 = true; break;
			}

            NativeImports.NativeMethods.RemoteFreeUnicodeString(Native, TargetIs32 );
        }

		

		/// <summary>
		/// Used to read from the native point to the currect struct based on <see cref="StructType"/>
		/// </summary>
		
		public int Length
        {
            get 
			{
				Blit();
				if (StructType == StructModeType.Machinex64)
                {
					return Machine64.Length;
                }
                if (StructType == StructModeType.Machinex86)
                {
					return Machine32.Length;
                }

				throw new InvalidOperationException();
			}
        }

		public int MaxLength
        {
			get
			{
				Blit();
				if (StructType == StructModeType.Machinex64)
				{
					return Machine64.MaxLength;
				}
				if (StructType == StructModeType.Machinex86)
				{
					return Machine32.MaxLength;
				}

				throw new InvalidOperationException();
			}
		}

		public ulong BufferPtr
        {
			get
			{
				Blit();
				if (StructType == StructModeType.Machinex64)
				{
					return Machine64.Buffer;
				}
				if (StructType == StructModeType.Machinex86)
				{
					return Machine32.Buffer;
				}
				throw new InvalidOperationException();
			}
        }

		public string Buffer
        {
            get
            {
				Blit();
				if (StructType == StructModeType.Machinex64)
                {
					return Marshal.PtrToStringUni(new IntPtr((uint)Machine64.Buffer));
                }
				if (StructType == StructModeType.Machinex86)
                {
					return Marshal.PtrToStringUni(new IntPtr((uint)Machine32.Buffer));
				}
				throw new InvalidOperationException();
			}
        }
		StructModeType StructType;
		UnicodeString64 Machine64;
		UnicodeString32 Machine32;
		bool HaveBlitOnce;
    }
}
