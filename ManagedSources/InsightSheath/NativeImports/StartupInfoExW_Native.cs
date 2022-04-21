using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.NativeImports
{
    internal static partial class NativeMethods
    {
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_MakeInstance")]
        public static extern IntPtr StartupInfoWrapper_MakeInstance();

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_DupInstance")]
        public static extern IntPtr StartupInfoWrapper_DuplicateInstance(IntPtr Native);


        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_DeleteInstance")]
        public static extern bool StartupInfoWrapper_DeleteInstance(IntPtr Native);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetlpDesktopW")]
        public static extern IntPtr StartupInfoWrapper_GetDesktop(IntPtr Native);

        [DllImport("InsightApi.Dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetlpDesktopW")]
        public static extern IntPtr StartupInfoWrapper_SetDesktop(IntPtr Native, string Desktop);


        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupUpInfo_GetlpTitleW")]
        public static extern IntPtr StartupInfoWrapper_GetTitle(IntPtr Native);

        [DllImport("InsightApi.Dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartUpInfo_SetlpTitleW")]
        public static extern IntPtr StartupInfoWrapper_SetTitle(IntPtr Native, string Title);


        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwX")]
        public static extern uint StartupInfoWrapper_GetdwX(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwX")]
        public static extern bool StartupInfoWrapper_SetdwX(IntPtr Native, uint dwX);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwY")]
        public static extern uint StartupInfoWrapper_GetdwY(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwY")]
        public static extern bool StartupInfoWrapper_SetdwY(IntPtr Native, uint dwY);



        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwXCountChars")]
        public static extern uint StartupInfoWrapper_GetdwXCountChars(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwXCountChars")]
        public static extern bool StartupInfoWrapper_SetdwXCountChars(IntPtr Native, uint dwX);


        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwYSize")]
        public static extern uint StartupInfoWrapper_GetYSize(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwYSize")]
        public static extern bool StartupInfoWrapper_SetYSize(IntPtr Native, uint dwY);



        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwXSize")]
        public static extern uint StartupInfoWrapper_GetXSize(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwXSize")]
        public static extern bool StartupInfoWrapper_SetXSize(IntPtr Native, uint dwY);





        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwYCountChars")]
        public static extern uint StartupInfoWrapper_GetdwYCountChars(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwYCountChars")]
        public static extern bool StartupInfoWrapper_SetdwYCountChars(IntPtr Native, uint dwY);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwFillAttributes")]
        public static extern uint StartupInfo_GetdwFillAttributes(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwFillAttribute")]
        public static extern bool StartupInfo_SetdwFillAttribute(IntPtr Native, uint dwAttributes);


        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetdwFlags")]
        public static extern uint StartupInfo_GetdwFlags(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetdwFlags")]
        public static extern bool StartupInfo_SetdwFlags(IntPtr Native, uint dwFlags);


        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GetwShowWindow")]
        public static extern ushort StartupInfo_GetShowWindow(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SetwShowWindow")]
        public static extern bool StartupInfo_SetShowWindow(IntPtr Native, ushort ShowWindowAttr);




        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GethStdInput")]
        public static extern IntPtr StartupInfo_GetStdInput(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SethStdInput")]
        public static extern bool StartupInfo_SetStdInput(IntPtr Native, IntPtr NewHandle);

        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GethStdOutput")]
        public static extern IntPtr StartupInfo_GetStdOutput(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SethStdOutput")]
        public static extern bool StartupInfo_SetStdOutput(IntPtr Native, IntPtr NewHandle);



        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_GethStdError")]
        public static extern IntPtr StartupInfo_GetStdError(IntPtr Native);
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, SetLastError = true, EntryPoint = "StartupInfo_SethStdError")]
        public static extern bool StartupInfo_SetStdError(IntPtr Native, IntPtr NewHandle);







    }
}
