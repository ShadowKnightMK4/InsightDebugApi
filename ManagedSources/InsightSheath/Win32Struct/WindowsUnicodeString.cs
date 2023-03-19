using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Abstract;
using InsightSheath.NativeImports;

namespace InsightSheath.Win32Struct
{

	/// <summary>
	/// Enum used by certain classes that encapsulate Native structs that contain pointers whose size may not match *our* pointer size.  One will need to set the <see cref="PlatformDependantNativeStruct.StructType"/> in a child class to flag which version to use
	/// </summary>
	public enum StructModeType
	{
		/// <summary>
		/// Not defined.  Attempting to access properties in this class will trigger <see cref="InvalidOperationException"/> asking programming to pick either <see cref="Machinex64"/> or <see cref="Machinex86"/>
		/// </summary>
		MachineUnknown = 0,
		/// <summary>
		/// Class encapsulates a 32 bit version of the native struct. For Example <see cref="WindowsUnicodeString"/> and <see cref="UnicodeString32"/>
		/// </summary>
		Machinex86 = 1,
		/// <summary>
		/// Class encapsulates a 64- bit version of the native struct. For Example <see cref="WindowsUnicodeString"/> and  <see cref="UnicodeString64"/> struct
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
	/// For processes that are 32-bit / Wow64. This is what the UNICODE_STRING Windows struct looks like. Once your grab a dot net version of what ever you're reading from, you'll need to know if the <see cref="UnicodeString32.Buffer"/> is in your process memory space or an external process.
	/// </summary>
	/// <remarks> Use <see cref="Marshal.PtrToStructure{UnicodeString32}(IntPtr)"/>  and pass <see cref="Buffer"/> for a local memory pointer and <see cref="Remote.RemoteStructure.RemoteReadString(IntPtr, IntPtr, ulong)"/> for a pointer relevant to a different process </remarks>
	[StructLayout(LayoutKind.Sequential)]
    public struct UnicodeString32
    {
		/// <summary>
		/// Current Length of the string pointed to in Buffer. Note if dealing with <see cref="InsertHere"/>, the memory block pointed to by buffer currently will be set right after the native location of struct plus its size
		/// </summary>
		public ushort Length;
		/// <summary>
		/// How big possibly is if the buffer. Note <see cref="InsertHere"/> dealing this is here for completeness sake. Your buffer (if remote) was allocated to be big enough to hold that containing string, that's all
		/// </summary>
		public ushort MaxLength;
		/// <summary>
		/// 4 byte pointer to where the Unicode String is allocated. 
		/// </summary>
        public uint Buffer;
    }

	/// <summary>
	/// For processes that are 64-bit, this is what the UNICODE_STRING Windows struct looks like when read from. Suitable for Marshaling from a source. Take care with the string pointer to know if it's local memory or extern/remote/not your process's memory. .if your pointer to the native structure <see cref="UnicodeString32.Buffer"/> points to a remote/ non local memory buffer or not. 
	/// </summary>
	/// <remarks> Use <see cref="Marshal.PtrToStructure{UnicodeString64}(IntPtr)"/>  and pass <see cref="Buffer"/> for a local memory pointer and <see cref="Remote.RemoteStructure.RemoteReadString(IntPtr, IntPtr, ulong)"/> for a pointer relevant to a different process </remarks>
	/// <summary>
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
	public struct UnicodeString64
	{
		/// <summary>
		/// Current Length of the string pointed to in Buffer. 
		/// </summary>
		public ushort Length;
		/// <summary>
		/// How big possibly is if the buffer. 
		/// </summary>
		public ushort MaxLength;
		/// <summary>
		/// Padding to ensure <see cref="Buffer"/> is at the right spot. Otherwise unused. 
		/// </summary>
		public uint Padding;
		/// <summary>
		/// 8 byte pointer to where the Unicode String is allocated. 
		/// </summary>
		public ulong Buffer;
	}


    
	/// <summary>
	/// Encapsulates both a <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/> and lets one indicate which one to use based on <see cref="WindowsUnicodeString.StructType"/> You can use <see cref="HelperRoutines.GetPEMachineType(string)"/> on the process your dealing with to find what value was set as the machine type
	/// </summary>
    public class WindowsUnicodeString: PlatformDependantNativeStruct
    {
		/// <summary>
		/// After creating an instance, call <see cref="PlatformDependantNativeStruct.SetStructType(ModeType)"/> and specify which version of an Unicode string, this class contains
		/// </summary>
		/// <param name="Native"></param>
        public WindowsUnicodeString(IntPtr Native): base(Native)
        {
			StructType = StructModeType.MachineUnknown;
        }

		/// <summary>
		/// After creating an instance, call <see cref="SetStructType(ModeType)"/> and specify which version of an Unicode string, this class contains
		/// </summary>
		/// <param name="Native">Native pointer to either a <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/> to the structure this class will reference</param>
		public WindowsUnicodeString(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {
			StructType = StructModeType.MachineUnknown;
        }

		/// <summary>
		/// Create an instance of <see cref="WindowsUnicodeString"/> and specify the native pointer, if the class will be subject to unmanaged cleanup and which Native Struct the pointer points too
		/// </summary>
		/// <param name="Native">Native pointer to either a <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/> to the structure this class will reference</param>
		/// <param name="FreeOnCleanup">if true then the structure is freed via RemoteRead_SimpleFree() which itself is a call to C/C++'s free() . Set TO FALSE if dealing with structures declared in C/C++ code vs dynamically allocated</param>
		/// <param name="StructType">Use <see cref="StructModeType.Machinex86"/> to mark structure as a <see cref="UnicodeString32"/> pointer and <see cref="StructModeType.Machinex64"/> to mark struct as a <see cref="UnicodeString64"/> pointer</param>
		public WindowsUnicodeString(IntPtr Native, bool FreeOnCleanup, StructModeType StructType) : base(Native, FreeOnCleanup, StructType)
        {

        }

		/// <summary>
		/// Create an instance of <see cref="WindowsUnicodeString"/> and specify the native pointer and which Native Struct the pointer points too
		/// </summary>
		/// <param name="Native">Native pointer to either a <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/> to the structure this class will reference</param>
		/// <param name="StructType">Use <see cref="StructModeType.Machinex86"/> to mark structure as a <see cref="UnicodeString32"/> pointer and <see cref="StructModeType.Machinex64"/> to mark struct as a <see cref="UnicodeString64"/> pointer</param>
		public WindowsUnicodeString(IntPtr Native, StructModeType StructType): base(Native, StructType)
        {

        }

		/// <summary>
		/// Finalized that calls <see cref="Dispose(bool)"/> with the value of false
		/// </summary>
		~WindowsUnicodeString()
        {
			Dispose(false);
        }

		/// <summary>
		/// Marshals into a private <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/> struct depending on the value of <see cref="PlatformDependantNativeStruct.WasBlit"/> being false.  Sets to true afterwards. Will throw an <see cref="InvalidOperationException"/> if <see cref="PlatformDependantNativeStruct.StructType"/>  is something other than <see cref="StructModeType.Machinex64"/> or <see cref="StructModeType.Machinex86"/>
		/// </summary>
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

					WasBlit = true;
				}
			}
		}


		private bool disposedValue;
		/// <summary>
		/// The native memory is either a <see cref="UnicodeString32"/> or <see cref="UnicodeString64"/> struct Dependant on <see cref="StructType"/>.  Pointer is a separate allocated string. 
		/// This means <see cref="NativeImports.NativeMethods.SimpleFree(IntPtr)"/> is incorrect in this scenario. We'll need to call <see cref="NativeImports.NativeMethods.RemoteFreeUnicodeString(IntPtr, bool)"/> pointer here. Also <see cref="FreeOnCleanup"/> should be false if this structure is part of a larger struct directly and NOT A POINTER.
		/// </summary>
		/// <param name="disposing"></param>
		/// <remarks>Native side always returns a UnicodeString64 bit.  We simply calculate and discard value</remarks>
        protected override void Dispose(bool disposing)
        {
			if (!disposedValue)
            {
				bool bit32Mode;
				if (this.StructType == StructModeType.Machinex86)
                {
					bit32Mode = true;
                }
                else
                {
					bit32Mode = false;
					if (StructType == StructModeType.MachineUnknown)
                    {
						throw ThrowNewInvalidOpMessage(this.GetType().Name);
                    }
                }

				if (FreeOnCleanup)
                {
					RemoteStructureInternal.RemoteFreeUnicodeString(Native);
					ClearNative();
                }
				disposedValue = true;
            }
			base.Dispose(disposing);
		}

		

		/// <summary>
		/// Reads the current length of the string in <see cref="BufferPtr"/> based on the current <see cref="StructType"/> setting
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

				throw ThrowNewInvalidOpMessage(GetType().Name);
			}
		}

        /// <summary>
        /// Reads the max length (in bytes) of the string in <see cref="BufferPtr"/> based on the current <see cref="PlatformDependantNativeStruct.StructType"/> setting in this class
        /// </summary>
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

				throw ThrowNewInvalidOpMessage(GetType().Name);
			}
		}

		/// <summary>
		/// Return the buffer containing the string in the contained structure based on the current <see cref="StructType"/> setting.  x86 (4 byte sized) pointers are promoted to a x64 (8 byte sized) return value
		/// </summary>
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
				throw ThrowNewInvalidOpMessage(GetType().Name);
			}
        }

		/// <summary>
		/// Retrieve a .NET managed string containing the String that <see cref="BufferPtr"/> points to. 
		/// </summary>
		/// <remarks>IMPORTANT!!!!!  Use <see cref="Remote.RemoteStructure.RemoteReadUnicodeString(IntPtr, IntPtr, bool, bool)"/> if dealing with a <see cref="BufferPtr"/> that is valid in the context of another process and not yours. This routine will take case or dealing with that. You'll either get incorrect results or generate an exception if you don't. </remarks>
		public string Buffer
        {
            get
            {
				Blit();
				if (StructType == StructModeType.Machinex64)
                {
					return Marshal.PtrToStringUni(new IntPtr((long)Machine64.Buffer));
                }
				if (StructType == StructModeType.Machinex86)
                {
					return Marshal.PtrToStringUni(new IntPtr((int)Machine32.Buffer));
				}
				throw ThrowNewInvalidOpMessage(GetType().Name);
			}
        }


		/// <summary>
		/// Equivalent to grabbing the <see cref="Buffer"/> property if <see cref="BufferPtr"/> is non zero
		/// </summary>
		/// <returns>If <see cref="BufferPtr"/> is 0, return null. Otherwise returns <see cref="Buffer"/></returns>
		/// <exception cref="InvalidOperationException">Can be triggered if one forgets to set <see cref="StructType"/></exception>
		public override string ToString()
        {
			if (BufferPtr == 0)
            {
				return null;
            }
			return Buffer;
        }

		/// <summary>
		/// Not implemented
		/// </summary>
        protected override void Apply()
        {
            throw new NotImplementedException();
        }


        /// <summary>
        /// If <see cref="StructType"/> is set to <see cref="StructModeType.Machinex64"/>, this is the structure that is used.
        /// </summary>
        UnicodeString64 Machine64;
		/// <summary>
		/// If <see cref="StructType"/> is set to <see cref="StructModeType.Machinex86"/>, this is the structure that is used.
		/// </summary>
		UnicodeString32 Machine32;
		
    }
}
