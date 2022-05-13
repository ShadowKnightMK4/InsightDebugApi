using InsightSheath.Wrappers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Structs
{
    

    [StructLayout(LayoutKind.Sequential)]
    struct ObjectAttributes32
    {
        public uint Length;
        public uint RootDirectory;
        public uint ObjectName;
        public uint Attributes;
        public uint SecurityDescriptor;
        public uint SecurityQoS;
    };

    [StructLayout(LayoutKind.Sequential)]
    struct ObjectAttributes64
    {
        public uint Length;
        public ulong RootDirectory;
        public ulong ObjectName;
        public uint Attributes;
        public ulong SecurityDescriptor;
        public ulong SecurityQoS;
    };
    /// <summary>
    /// made from specs here <see href="https://docs.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_object_attributes"/>
    /// </summary>
    public class WindowsObjectAttributes: NativeStaticContainer
    {
        public  WindowsObjectAttributes(IntPtr Native)   : base(Native)
        {

        }

        public WindowsObjectAttributes(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }

        void Blit()
        {
            if (!HaveBlit)
            {
                HaveBlit = true;
                switch (StructType)
                {
                    case StructModeType.Machinex64:
                        Ret64 = Marshal.PtrToStructure<ObjectAttributes64>(Native);
                        break;
                    case StructModeType.Machinex86:
                        Ret32 = Marshal.PtrToStructure<ObjectAttributes32>(Native);
                        break;
                    default: throw new InvalidOperationException("Pick x64 or x86 native mode for this class");
                }

            }
        }
        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
        }

        StructModeType StructType;

        public void SetStructType(StructModeType Ty)
        {
            StructType = Ty;
        }
        public StructModeType GetStructType()
        {
            return StructType;
        }

        public ulong Length
        {
            get
            {
                Blit();
                switch (StructType)
                {
                    case StructModeType.Machinex64: return Ret64.Length;
                    case StructModeType.Machinex86: return Ret32.Length;
                }
                throw new InvalidOperationException();
            }
        }

        public IntPtr RootDirectory
        {
            get
            {
                Blit();
                switch (StructType)
                {
                    case StructModeType.Machinex64:
                        {
                            if (Ret64.RootDirectory != 0)
                            {
                                return new IntPtr((uint)Ret64.RootDirectory);
                            }
                            return IntPtr.Zero;
                        }
                    case StructModeType.Machinex86:
                        {
                            if (Ret32.RootDirectory != 0)
                            {
                                return new IntPtr(Ret32.RootDirectory);
                            }
                            return IntPtr.Zero;
                        }
                }
                throw new InvalidOperationException();
            }
        }

        public WindowsUnicodeString ObjectName
        {
            get
            {
                Blit();
                switch (StructType)
                {
                    case StructModeType.Machinex64:
                        {
                            if (Ret64.ObjectName == 0)
                            {
                                return null;
                            }
                            else
                            {
                                WindowsUnicodeString ret = new WindowsUnicodeString(new  IntPtr((uint)Ret64.ObjectName));
                                ret.SetStructType(StructType);
                                return ret;
                            }
                        }
                    case StructModeType.Machinex86:
                        {
                            if (Ret32.ObjectName == 0)
                            {
                                return null;
                            }
                            else
                            {
                                WindowsUnicodeString ret = new WindowsUnicodeString(new IntPtr((uint)Ret32.ObjectName));
                                ret.SetStructType(StructType);
                                return ret;
                            }
                        }
                }
                throw new InvalidOperationException();
            }
        }

        public uint Attributes
        {
            get
            {
                switch (StructType)
                {
                    case StructModeType.Machinex64:
                        return Ret64.Attributes;
                    case StructModeType.Machinex86:
                        return Ret32.Attributes;
                }
                throw new InvalidOperationException();
            }
        }

        public IntPtr SecurityDescriptor
        {
            get
            {
                switch (StructType)
                {
                    case StructModeType.Machinex64:
                        if (Ret64.SecurityDescriptor == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr((uint) Ret64.SecurityDescriptor);
                    case StructModeType.Machinex86:
                        if (Ret32.SecurityDescriptor == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr(Ret32.SecurityDescriptor);
                }
                throw new InvalidOperationException();
            }
        }

        public IntPtr SecurityQualityOfService
        {
            get
            {
                switch (StructType)
                {
                    case StructModeType.Machinex64:
                        if (Ret64.SecurityQoS == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr((uint)Ret64.SecurityQoS);
                    case StructModeType.Machinex86:
                        if (Ret32.SecurityQoS == 0)
                        {
                            return IntPtr.Zero;
                        }
                        return new IntPtr(Ret32.SecurityQoS);
                }
                throw new InvalidOperationException();
            }
        }

        ObjectAttributes32 Ret32;
        ObjectAttributes64 Ret64;
        bool HaveBlit;
    }
}
