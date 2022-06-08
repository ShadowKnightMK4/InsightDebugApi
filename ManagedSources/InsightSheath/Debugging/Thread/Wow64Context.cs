using InsightSheath.Abstract;
using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Debugging.Thread
{
    /*
     * typedef struct _WOW64_FLOATING_SAVE_AREA {
    DWORD   ControlWord;
    DWORD   StatusWord;
    DWORD   TagWord;
    DWORD   ErrorOffset;
    DWORD   ErrorSelector;
    DWORD   DataOffset;
    DWORD   DataSelector;
    BYTE    RegisterArea[WOW64_SIZE_OF_80387_REGISTERS];
    DWORD   Cr0NpxState;
} WOW64_FLOATING_SAVE_AREA;
     * ypedef struct _WOW64_CONTEXT {
  DWORD                    ContextFlags;
  DWORD                    Dr0;
  DWORD                    Dr1;
  DWORD                    Dr2;
  DWORD                    Dr3;
  DWORD                    Dr6;
  DWORD                    Dr7;
  WOW64_FLOATING_SAVE_AREA FloatSave;
  DWORD                    SegGs;
  DWORD                    SegFs;
  DWORD                    SegEs;
  DWORD                    SegDs;
  DWORD                    Edi;
  DWORD                    Esi;
  DWORD                    Ebx;
  DWORD                    Edx;
  DWORD                    Ecx;
  DWORD                    Eax;
  DWORD                    Ebp;
  DWORD                    Eip;
  DWORD                    SegCs;
  DWORD                    EFlags;
  DWORD                    Esp;
  DWORD                    SegSs;
  BYTE                     ExtendedRegisters[WOW64_MAXIMUM_SUPPORTED_EXTENSION];
} WOW64_CONTEXT;
     * */

    [StructLayout(LayoutKind.Sequential)]
    public struct Wow64FloatingPointSaveArea
    {
        public uint ControlWord;
        public uint StatusWord;
        public uint TagWord;
        public uint ErrorOffset;
        public uint ErrorSelector;
        public uint DataOffset;
        public uint DataSelector;
        /* WOW64_SIZE_OF_80387_REGISTERS = 80 in winnt.h */
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst =80)]
        public byte[] RegisterArea;
        public uint Cr0NpxState;

    }
    /// <summary>
    /// Handler for reading from / writing to a Wow64ContextString
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Wow64ContextLayout
    {
        public uint ContextFlags;
        public uint Dr0;
        public uint Dr1;
        public uint Dr2;
        public uint Dr3;
        public uint Dr6;
        public uint Dr7;
        public Wow64FloatingPointSaveArea FloatSave;
        public uint SegGs;
        public uint SegFs;
        public uint SegEs;
        public uint SegDs;
        public uint Edi;
        public uint Esi;
        public uint Ebx;
        public uint Edx;
        public uint Ecx;
        public uint Eax;
        public uint Ebp;
        public uint Eip;
        public uint SegCs;
        public uint EFlags;
        public uint Esp;
        public uint SegSs;
        /* WOW64_MAXIMUM_SUPPORTED_EXTENSION is 512 in winnt.h*/
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 512)]
        public byte[] ExtendedRegisters;
    }

    
    public class Wow64Context: OnDemandMarshalNativeStruct
    {
        public Wow64Context(IntPtr Native): base(Native)
        {
            WasBlit = false;
            StaleManagedStruct = false;
            
        }
        public uint ContextFlags
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.ContextFlags;
            }
            set
            {
                Private.ContextFlags = value;
                StaleManagedStruct = true;
            }
        }
        public uint Dr0
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Dr0;
            }
            set
            {
                Private.Dr0 = value;
                StaleManagedStruct = true;
            }
        }

        public uint Dr1
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Dr1;
            }
            set
            {
                Private.Dr1 = value;
                StaleManagedStruct = true;
            }
        }

        public uint Dr2
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Dr2;
            }
            set
            {
                Private.Dr2 = value;
                StaleManagedStruct = true;
            }
        }

        public uint Dr3
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Dr3;
                StaleManagedStruct = true;
            }
            set
            {
                Private.Dr3 = value;
            }
        }

        public uint Dr6
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Dr6;

            }
            set
            {
                Private.Dr6 = value;
                StaleManagedStruct = true;
            }
        }

        public uint Dr7
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Dr7;
            }
            set
            {
                Private.Dr7 = value;
                StaleManagedStruct = true;
            }
        }


        
        public Wow64FloatingPointSaveArea FloatSave
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.FloatSave;
            }
            set
            {
                Private.FloatSave = value;
                StaleManagedStruct = true;
            }
        }

        public uint SegGs
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.SegGs;
            }
            set
            {
                Private.SegGs = value;
                StaleManagedStruct = true;
            }
        }


        public uint SegFs
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.SegFs;
            }
            set
            {
                Private.SegFs = value;
                StaleManagedStruct = true;
            }
        }



        public uint SegEs
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.SegEs;
            }
            set
            {
                Private.SegEs = value;
                StaleManagedStruct = true;
            }
        }




        public uint SegDs
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.SegDs;
            }
            set
            {
                Private.SegDs = value;
                StaleManagedStruct = true;
            }
        }

        public uint Ebp
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Ebp;
            }
            set
            {
                Private.Ebp = value;
                StaleManagedStruct = true;
            }
        }



        public uint Eip
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Eip;
            }
            set
            {
                Private.Eip = value;
                StaleManagedStruct = true;
            }
        }


        public uint Ecx
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Ecx;
            }
            set
            {
                Private.Ecx = value;
                StaleManagedStruct = true;
            }
        }


        public uint Eax
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Eax;
            }
            set
            {
                Private.Eax = value;
                StaleManagedStruct = true;
            }
        }


        public uint Edx
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Edx;
            }
            set
            {
                Private.Edx = value;
                StaleManagedStruct = true;
            }
        }


        public uint Ebx
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Ebx;
            }
            set
            {
                Private.Ebx = value;
                StaleManagedStruct = true;
            }
        }


        public uint Esi
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Esi;
            }
            set
            {
                Private.Esi = value;
                StaleManagedStruct = true;
            }
        }


        public uint Edi
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Edi;
            }
            set
            {
                Private.Edi = value;
                StaleManagedStruct = true;
            }
        }


        public uint SegCs
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.SegCs;
            }
            set
            {
                Private.SegCs = value;
                StaleManagedStruct = true;
            }
        }


        public uint EFlags
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.EFlags;
            }
            set
            {
                Private.EFlags = value;
                StaleManagedStruct = true;
            }
        }



        public uint Esp
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.Esp;
            }
            set
            {
                Private.Esp = value;
                StaleManagedStruct = true;
            }
        }



        public uint SegSs
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.SegSs;
            }
            set
            {
                Private.SegSs = value;
                StaleManagedStruct = true;
            }
        }

        
        public byte[] ExtendedRegisters
        {
            get
            {
                if (!WasBlit)
                {
                    Blit();
                }
                return Private.ExtendedRegisters;
            }
            set
            {
                Private.ExtendedRegisters = value;
                StaleManagedStruct = true;
            }
        }
                
        public Wow64Context(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }

        

        protected override void Blit()
        {
            if (StaleManagedStruct)
            {
                Apply();
            }
            if (!WasBlit)
            {
                Private = Marshal.PtrToStructure<Wow64ContextLayout>(Native);
                WasBlit = true;
            }
        }

       
        private bool disposedValue;
        protected override void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (FreeOnCleanup)
                {
                    NativeImports.NativeMethods.SimpleFree(Native);
                }
                ClearNative();
                disposedValue = true;
            }
        }

        protected override void Apply()
        {
            if ( (StaleManagedStruct) && (Native != IntPtr.Zero))
            {
                Marshal.StructureToPtr(Private, Native, false);
                StaleManagedStruct = false;
            }
        }

        Wow64ContextLayout Private;
    }
}
