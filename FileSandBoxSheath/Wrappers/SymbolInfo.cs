using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FileSandBoxSheath.NativeImports;
namespace FileSandBoxSheath.Wrappers
{
    /// <summary>
    /// Should be from the SymTagEnum at msdn https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/bkedss5f(v=vs.100)
    /// </summary>
    public enum SymbolTagType: uint
    {
       Null = 0
    }
     [Flags]
    public enum SymbolFlags : uint 
    {
        /// <summary>
        /// The symbol is a CLR token.
        /// </summary>
        ClrToken = 0x00040000,
        /// <summary>
        /// The symbol is a constant.
        /// </summary>
        Constant = 0x00000100,
        /// <summary>
        /// The symbol is from the export table.
        /// </summary>
        Export = 0x00000200,
        /// <summary>
        /// The symbol is a forwarder.
        /// </summary>
        Forwarder = 0x00000400,
        /// <summary>
        /// Offsets are frame relative.
        /// </summary>
        FrameRel = 0x00000020,
        /// <summary>
        /// The symbol is a known function.
        /// </summary>
        Function = 0x00000800,
        /// <summary>
        /// The symbol address is an offset relative to the beginning of the intermediate language block. This applies to managed code only.
        /// </summary>
        IrlRel = 0x00010000,
        /// <summary>
        /// The symbol is a local variable.
        /// </summary>
        Local =0x00000080,
        /// <summary>
        /// The symbol is managed metadata.
        /// </summary>
        Metadata = 0x00020000,
        /// <summary>
        /// The symbol is a parameter.
        /// </summary>
        Parameter = 0x00000040,
        /// <summary>
        /// The symbol is a register. The Register member is used.
        /// </summary>
        Register = 0x00000008,
        /// <summary>
        /// Offsets are register relative.
        /// </summary>
        Regrel = 0x00000010,
        /// <summary>
        /// The symbol is a managed code slot.
        /// </summary>
        Slot = 0x00008000,
        /// <summary>
        /// The symbol is a thunk.
        /// </summary>
        Thunk = 0x00002000,
        /// <summary>
        /// The symbol is an offset into the TLS data area.
        /// </summary>
        TlsRel = 0x00004000,
        /// <summary>
        /// The Value member is used.
        /// </summary>
        ValuePresent = 0x00000001,
        /// <summary>
        /// The symbol is a virtual symbol created by the SymAddSymbol function.    
        /// </summary>
        Virtual = 0x00001000

    }

    /// <summary>
    /// Wrappers imported for dealing with SymbolInfoW structs 
    /// </summary>
    public class SymbolInfo : NativeStaticContainer
    {
        public SymbolInfo(IntPtr That) : base(That)
        {

        }

        public SymbolInfo(IntPtr That, bool FreeOnCleanup) : base(That, FreeOnCleanup)
        {

        }


        /// <summary>
        /// Get the type index for symbol for this
        /// </summary>
        public uint TypeDataIndex
        {
            get
            {
                return NativeMethods.SymbolInfo_GetTypeIndex(Native);
            }
        }

        /// <summary>
        /// Get the unique value used by the symbol handler for this symbol
        /// </summary>
        public uint SymbolIndex
        {
            get
            {
                return NativeMethods.SymbolInfo_GetIndex(Native);
            }
        }


        /// <summary>
        /// How big is the symbol's data in bytes
        /// </summary>
        public uint SymbolSize
        {
            get
            {
                return NativeMethods.SymbolInfo_GetSymbolSize(Native);
            }
        }

        /// <summary>
        /// get base address for the symbol's module
        /// </summary>
        public ulong BaseModuleAddress
        {
            get
            {
                return NativeMethods.SymbolInfo_GetModuleBase(Native);
            }
        }

        /// <summary>
        /// Get the flags that describe what's in this structure
        /// </summary>
        public SymbolFlags Flags
        {
            get
            {
                return NativeMethods.SymbolInfo_GetFlags(Native);
            }
        }

        public uint SymbolRegisterOrValue
        {
            get
            {
                return NativeMethods.SymbolInfo_GetValOrReg(Native);
            }
        }

        public uint SymbolRegister
        {
            get
            {
                return NativeMethods.SymbolInfo_GetReg(Native);
            }
        }


        public uint SymbolValue
        {
            get
            {
                return NativeMethods.SymbolInfo_GetVal(Native);
            }
        }


        public uint SymbolScope
        {
            get
            {
                return NativeMethods.SymbolInfo_GetScope(Native);
            }
        }


        public SymbolTagType SymbolTagType
        {
            get
            {
                return NativeMethods.SymbolInfo_GetTagType(Native);
            }
        }

        public string SymbolName
        {
            get
            {
                return NativeMethods.SymbolInfo_GetName(Native);
            }
        }

    }


    }
