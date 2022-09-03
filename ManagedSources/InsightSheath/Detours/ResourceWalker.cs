using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Abstract;
using InsightSheath.NativeImports;
using System.IO;
namespace InsightSheath.Detours
{
    [Flags]
    public enum ResourceWalker_flags
    {
        ResourceUnspecified = ResourceEnumDefault,
        ResourceEnumMui = 0x0002,
        ResourceEnumLn = 0x0001,
        ResourceEnumValidate=0x0008,
        ResourceEnumDefault = ResourceEnumLn | ResourceEnumMui
    }
    public class ResourceWalker : NativeStaticContainer
    {
        /// <summary>
        /// Sourced from IS_INTRESOURCE macro
        /// </summary>
        /// <param name="CheckMe"></param>
        /// <returns></returns>
        public static bool IsIntResource(IntPtr CheckMe)
        {
            return (((CheckMe.ToInt64())) >> 16 == 0);
        }

        /// <summary>
        /// The MAKE_INTRESOURCE macro
        /// </summary>
        /// <param name="Resource"></param>
        /// <returns></returns>
        public static IntPtr MakeIntResource(IntPtr Resource)
        {
            return new IntPtr((ushort)Resource.ToInt32());
        }
        public delegate bool EnumResourceNameProcw(IntPtr hModule, IntPtr lpType, IntPtr Name, IntPtr CustomArg);


        public static ResourceWalker MakeInstance(string ExeName, FileAccess Access, FileShare Share)
        {
            return new  ResourceWalker(InternalReasourceWalker.ResourceWalker_MakeInstanceTarget(ExeName, Access, Share));
        }
        public ResourceWalker(IntPtr Native): base(Native)
        {

        }

        public ResourceWalker(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }
        protected override void Dispose(bool disposing)
        {
            if (FreeOnCleanup)
            {
                InternalReasourceWalker.ResourceWalker_KillInstance(Native);
            }
            ClearNative();
            base.Dispose(disposing);
        }


        public bool EnumResourceNames(string ResourceType, EnumResourceNameProcw Callback, IntPtr CustomArg, ResourceWalker_flags dwFlags, Int16 LangId)
        {
            bool call = false;
            IntPtr UnmanagedResourceType = IntPtr.Zero;
            try
            {
                Marshal.StringToHGlobalUni(ResourceType);
                call = InternalReasourceWalker.ResourceWalker_EnumResourceNamesW(Native, UnmanagedResourceType, Callback, CustomArg, (uint)dwFlags, LangId);
            }
            finally
            {
                if (UnmanagedResourceType != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(UnmanagedResourceType);
                }
            }
            return call;
        }
        public bool ThreadLock
        {
            get
            {
                return InternalReasourceWalker.ResourceWalker_GetThreadLock(Native);
            }
            set
            {
                InternalReasourceWalker.ResourceWalker_SetThreadLock(Native, value);
            }
        }
    }
}
