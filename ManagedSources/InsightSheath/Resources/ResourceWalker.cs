using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Abstract;
using InsightSheath.NativeImports;
using System.IO;
using InsightSheath.Telemetry;

namespace InsightSheath.Resource
{
    /// <summary>
    /// Gotton from <see href="https://docs.microsoft.com/en-us/windows/win32/menurc/resource-types"/>
    /// </summary>
    public enum ResourceTypes
    {
        Accelerator = 9,
        AnimatedCursor = 21,
        AnimatedIcon = 22,
        Bitmap = 23,
        Cursor = 1,
        Dialog = 5,
        DlgInclude = 17,
        Font = 8,
        FontDir = 7,
        GroupCursor = Cursor + 11,
        GroupIcon = Icon + 11,
        Html = 23,
        Icon = 3,
        Manifest = 24,
        Menu = 4,
        MessageTable = 11,
        PlugNPlay = 19,
        RCData = 10,
        String = 6,
        Version = 16,
        VXD = 20
    }
    [Flags]
    public enum ResourceWalker_flags
    {
        ResourceUnspecified = ResourceEnumDefault,
        ResourceEnumMui = 0x0002,
        ResourceEnumLn = 0x0001,
        ResourceEnumValidate=0x0008,
        ResourceEnumDefault = ResourceEnumLn | ResourceEnumMui
    }

    /// <summary>
    /// Enumerate, examine Unmanged Resources in an app.
    /// </summary>
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

        /// <summary>
        /// Callback for Resource Name enumeration.
        /// </summary>
        /// <param name="hModule">Win32 Native HMODULE where the resource is.</param>
        /// <param name="lpType">May be string or an int resource. You can use <see cref="IsIntResource(IntPtr)"/> to check.  If true, then should be safe to covert to <see cref="ResourceTypes"/>.  If false then <see cref="Marshal.PtrToStringUTF8(IntPtr)"/></param>
        /// <param name="Name">May be string or an int resource. You can use <see cref="IsIntResource(IntPtr)"/> to check.  If true, then should be safe to covert to <see cref="ResourceTypes"/>.  If false then <see cref="Marshal.PtrToStringUTF8(IntPtr)</param>
        /// <param name="CustomArg">custom arg passed to begin enumeration</param>
        /// <returns>Your routine should return true to keep going and false once done</returns>
        public delegate bool EnumResourceNameProcw(IntPtr hModule, IntPtr lpType, IntPtr Name, IntPtr CustomArg);

        /// <summary>
        /// Callback for Resource Type enumeration
        /// </summary>
        /// <param name="hModule">Win32 Native HMODULE where the resource is.</param>
        /// <param name="lpType">May be string or an int resource. You can use <see cref="IsIntResource(IntPtr)"/> to check.  If true, then should be safe to covert to <see cref="ResourceTypes"/>.  If false then <see cref="Marshal.PtrToStringUTF8(IntPtr)"/></param>
        /// <param name="CustomArg">custom arg passed to begin enumeration</param>
        /// <returns>Your routine should return true to keep going and false once done</returns>
        public delegate bool EnumResTypeProcW(IntPtr hModule, IntPtr lpType, IntPtr CustomArg);

        /// <summary>
        /// Make an instance of this unmanaged class and return the .NET wrapper to it,
        /// </summary>
        /// <param name="ExeName"></param>
        /// <param name="Access"></param>
        /// <param name="Share"></param>
        /// <returns></returns>
        public static ResourceWalker MakeInstance(string ExeName, AccessMasks Access, ShareMasks Share)
        {
            return new  ResourceWalker(InternalReasourceWalker.ResourceWalker_MakeInstanceTarget(ExeName, (FileAccess) Access, (FileShare)Share), true);
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


        
        /// <summary>
        /// Enumerate resources for the DLL/EXE that this software points too.
        /// </summary>
        /// <param name="Callback"></param>
        /// <param name="CustomArg"></param>
        /// <param name="flags"></param>
        /// <param name="LangId"></param>
        /// <returns></returns>
        public bool EnumResourceTypes(EnumResTypeProcW Callback, IntPtr CustomArg, ResourceWalker_flags flags, int LangId)
        {
            return EnumResourceTypesW(Callback, CustomArg, flags, LangId);
        }



        /// <summary>
        /// Enumerate the passed routine type names. 
        /// </summary>
        /// <param name="ResourceType">One of the Constants for the resource to enumerate</param>
        /// <param name="Callback">Your callback</param>
        /// <param name="CustomArg">Your customarg</param>
        /// <param name="dwFlags">choose how to enumerate</param>
        /// <param name="LangId">language id of the resources to enumerate</param>
        /// <returns>trues if ok and false if not.</returns>
        public bool EnumResourceNames(ResourceTypes ResourceType, EnumResourceNameProcw Callback, IntPtr CustomArg, ResourceWalker_flags dwFlags, Int16 LangId)
        {
            return EnumResourceNames((IntPtr)ResourceType, Callback, CustomArg, dwFlags, LangId);
        }

        /// <summary>
        /// Enumerate the passed routine type names. 
        /// </summary>
        /// <param name="ResourceType">null terminated string indicating the type of resource to enumerate </param>
        /// <param name="Callback">Your callback</param>
        /// <param name="CustomArg">Your customarg</param>
        /// <param name="dwFlags">choose how to enumerate</param>
        /// <param name="LangId">language id of the resources to enumerate</param>
        /// <returns>trues if ok and false if not.</returns>
        public bool EnumResourceNames(string ResourceType, EnumResourceNameProcw Callback, IntPtr CustomArg, ResourceWalker_flags dwFlags, Int16 LangId)
        {
            bool call = false;
            IntPtr UnmanagedResourceType = IntPtr.Zero;
            try
            {
                UnmanagedResourceType = Marshal.StringToHGlobalUni(ResourceType);
                call = EnumResourceNames(UnmanagedResourceType, Callback, CustomArg, dwFlags, LangId);
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

        /// <summary>
        /// Cetrain routines will attempt to lock a resource they use.
        /// </summary>
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

        #region Sheath Routines



    

        
        /// <summary>
        /// Get list of resource types.  Contents are either string and/or <see cref="ResourceTypes"/>. To safely enum with foreach statement, use  foreach(object s in list) and test for ResourceType enum by (s is ResourceType) before handling.
        /// </summary>
        /// <returns>returns a list objects. Some may be strings and some may be <see cref="ResourceTypes"/>. It's dependent on the module you're enuming.</returns>
        public List<object> GetResourceType()
        {
            bool work = false;
            List<object> ret = new List<object>();
             bool WorkItOut(IntPtr hModule, IntPtr lpType, IntPtr CustomArg)
            {
                if (IsIntResource(lpType))
                {
                    ret.Add((ResourceTypes)lpType);
                }
                else
                {
                    ret.Add(Marshal.PtrToStringUni(lpType));
                }
                return true;
            }

            try
            {
                work = EnumResourceTypes(WorkItOut, IntPtr.Zero, ResourceWalker_flags.ResourceEnumDefault, 0);
            }
            finally
            {
                
            }
            return ret;
        }

        /// <summary>
        /// Return a list of all resources (names, types, ect...) in a neat array that you can use/change
        /// </summary>
        /// <returns></returns>
        public List<UnmanagedResource> GetResourceInformation()
        {
            List<string> nodups_string = new List<string>();
            List<ResourceTypes> nodup_types = new List<ResourceTypes>();
            List<UnmanagedResource> ret = new List<UnmanagedResource>();
            List<object> types = GetResourceType();
            bool workme(IntPtr hModule, IntPtr lpType, IntPtr Name, IntPtr CustomArg)
            {
                ret.Add(new UnmanagedResource(hModule, lpType, Name));
                return true;
            }


            ResourceTypes t;
            string s;
            foreach (object type in types)
            {
                if (type is ResourceTypes)
                {
                    t = (ResourceTypes)type;
                    if (nodup_types.Contains(t) == false)
                    {
                        nodup_types.Add(t);
                        EnumResourceNames(t, workme, IntPtr.Zero, ResourceWalker_flags.ResourceEnumDefault, 0);
                    }
                }
                else
                {
                    
                    if (nodups_string.Contains(type) == false)
                    {
                        nodups_string.Add(type.ToString());
                        EnumResourceNames(type.ToString(), workme, IntPtr.Zero, ResourceWalker_flags.ResourceEnumDefault, 0);
                    }
                }
            }
            return ret;
        }


        /// <summary>
        /// Return a list of Resource names.
        /// </summary>
        /// <returns></returns>
        public List<object> GetResourceNames()
        {
            bool work = false;
            List<object> ret = new List<object>();

            List<object> types = GetResourceType();
            bool workme(IntPtr hModule, IntPtr lpType, IntPtr Name, IntPtr CustomArg)
            {
                if (IsIntResource(Name) == false)
                {
                    ret.Add(Marshal.PtrToStringUni(Name));
                }
                else
                {
                    ret.Add((int)lpType);
                }
                    return true;
            }
            try
            {
                foreach (object t in types)
                {
                    if (t is ResourceTypes)
                    {
                        work = EnumResourceNames((ResourceTypes)t, new EnumResourceNameProcw(workme), IntPtr.Zero, ResourceWalker_flags.ResourceEnumDefault, 0);
                    }
                    else
                    {
                        work = EnumResourceNames((string)t, new EnumResourceNameProcw(workme), IntPtr.Zero, ResourceWalker_flags.ResourceEnumDefault, 0);
                    }
                }
            }
            finally
            {
                work = false;
            }
            return ret;
        }

       
        #endregion
        #region Internal Routines
        /// <summary>
        /// Last stop before calling the routien in <see cref="InternalReasourceWalker"/> that does this
        /// </summary>
        /// <param name="lpType"></param>
        /// <param name="Callback"></param>
        /// <param name="CustomArg"></param>
        /// <param name="dwFlags"></param>
        /// <param name="LangId"></param>
        /// <returns></returns>
        internal bool EnumResourceNames(IntPtr lpType, EnumResourceNameProcw Callback, IntPtr CustomArg, ResourceWalker_flags dwFlags, Int16 LangId)
        {
            bool call = InternalReasourceWalker.ResourceWalker_EnumResourceNamesW(Native, lpType, Callback, CustomArg, (uint)dwFlags, LangId);
            return call;
        }

        /// <summary>
        /// Last stop before calling the routine in <see cref="InternalReasourceWalker"/> that does this
        /// </summary>
        /// <param name="Callback"></param>
        /// <param name="CustomArg"></param>
        /// <param name="dwFlags"></param>
        /// <param name="LangId"></param>
        /// <returns></returns>
        internal bool EnumResourceTypesW(EnumResTypeProcW Callback, IntPtr CustomArg, ResourceWalker_flags dwFlags, int LangId)
        {
            bool call = InternalReasourceWalker.ResourceWalker_EnumResourceTypesW(Native, Callback, CustomArg, (uint)dwFlags, LangId);
            return call;
        }

        #endregion
    }
}
