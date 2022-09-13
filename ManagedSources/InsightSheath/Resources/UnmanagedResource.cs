using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.Resource
{
    /// <summary>
    /// Primarily intended to be what <see cref="ResourceWalker"/> gives to let the user load a specific resource into NativeLand.
    /// </summary>
    public class UnmanagedResource
    {

        [DllImport("kernel32.dll", SetLastError = true, EntryPoint = "FindResourceExW", CharSet = CharSet.Unicode)]
        internal static extern IntPtr FindResourceExW(IntPtr hModule, IntPtr lpType, IntPtr lpName, int wLangauge);
        [DllImport("kernel32.dll", SetLastError = true, EntryPoint = "LoadResource", CharSet = CharSet.Unicode)]
        internal static extern IntPtr LoadResource(IntPtr hModule, IntPtr hResInfo);

        [DllImport("kernel32.dll", SetLastError = true, EntryPoint = "SizeofResource", CharSet = CharSet.Unicode)]
        internal static extern uint SizeofResource(IntPtr hModule, IntPtr hResInfo);
        /// <summary>
        /// Grab the raw data of the resource 
        /// </summary>
        /// <returns></returns>
        public byte[] GetResourceDataAsByte()
        {
            
            uint ResSize = 0;
            IntPtr resHandle = FindResourceExW(hModule, lpType, lpName, Lanaguage);
            if (resHandle == IntPtr.Zero)
            {
                return null;
            }

            IntPtr resptr = LoadResource(hModule, resHandle);
            if (resptr == IntPtr.Zero)
            {
                return null;
            }

            ResSize = SizeofResource(hModule, resHandle);

            byte[] ret = new byte[ResSize];

            Marshal.Copy(resptr, ret, 0, (int)ResSize);

            return ret;
        }

      
        public MemoryStream GetResourceDataAsStream()
        {
            var ret = new MemoryStream(GetResourceDataAsByte());
            ret.Position = 0;
            return ret;
        }


        public UnmanagedResource(IntPtr hModule, IntPtr lpType, IntPtr Name)
        {
            this.lpType = lpType;
            this.lpName = Name;
            this.hModule = hModule;
        }

        
        public string TypeString
        {
            get
            {
                if (ResourceWalker.IsIntResource(lpType))
                {
                    return null;
                }
                return Marshal.PtrToStringUni(lpType);
            }
        }

        public ResourceTypes TypeVal
        {
            get
            {
                if (ResourceWalker.IsIntResource(lpType))
                {
                    return (ResourceTypes)lpType;
                }
                return 0;
            }
        }

        public string NameString
        {
            get
            {
                if (ResourceWalker.IsIntResource(lpName))
                {
                    return null;
                }
                return Marshal.PtrToStringUni(lpType);
            }
        }

        public int NameVal
        {
            get
            {
                if (ResourceWalker.IsIntResource(lpName))
                {
                    return (int)lpName;
                }
                return 0;
            }
        }



        private IntPtr lpType;
        private IntPtr lpName;
        private IntPtr hModule;
        private int Lanaguage;
    }
}
