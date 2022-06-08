﻿using InsightSheath.Wrappers;
using InsightSheath.NativeImports;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Diagnostics;
using InsightSheath.Abstract;

namespace InsightSheath.Win32Struct
{
    /// <summary>
    /// See <seealso href="https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfow"/>  entry for dwFlags for the meaning of these values.
    /// </summary>
    public enum StartupInfoExW_Flags : uint
    {
        Startf_ForceOnFeed = 0x00000040,
        Startf_ForceOffFeedback = 0x00000080,
        Startf_PreventPinning = 0x00002000,
        Startf_RunFullScreen = 0x00000020,
        Startf_TitleIsAppId = 0x00001000,
        Startf_TitleIsLinkName = 0x00000800,
        Startf_UntrustedSource = 0x00008000,
        Startf_UseCountChars = 0x00000008,
        Startf_UseFillAttribute = 0x00000010,
        Startf_UseHotKey = 0x00000200,
        Startf_UsePosition = 0x00000004,
        Startf_UseShowWindow = 0x00000001,
        Startf_UseSize = 0x00000002,
        Startf_UseStdHandles = 0x00000100
    }


    /// <summary>
    /// Lifted from <see href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow"/> these let  you specify how  starting window will do
    /// </summary>
    public enum StartupInfoExW_ShowWindow : ushort
    {
        Hide = 0,
        ShowNormal = 1,
        Normal = ShowNormal,
        ShowMinimized = 2,
        ShowMaximized = 3,
        Maximize = ShowMaximized,
        ShowNoActivate = 4,
        Show = 5,
        Minimized = 6,
        ShowMinNoActive = 7,
        ShowNa = 8,
        ShowRestore = 9,
        /// <summary>
        /// Not supported for startup info. For ShowWindow MSDN it means use the startupinfo to specify how to display the Window
        /// </summary>
        ShowDefault = 10,
        ForceMinimize = 11
    }


    /// <summary>
    /// Wrapper for the StartupInfoExW struct handler exported in InsightApi.dll.
    /// </summary>
    public class StartupInfoExW: NativeStaticContainer
    {
        /// <summary>
        /// Call the routine on the native side to make an instance and return a pointer to the wrapper class already pointed to that.
        /// </summary>
        /// <returns></returns>
        public static StartupInfoExW MakeInstance()
        {
            return null;
        }
        public StartupInfoExW(IntPtr Native): base(Native)
        {

        }

        public StartupInfoExW(IntPtr Native, bool FreeConCleanup): base(Native, FreeConCleanup)
        {

        }

        private bool disposedValue;
        protected override void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (FreeOnCleanup)
                {
                    StartupInfoExWInternal.StartupInfoWrapper_DeleteInstance(Native);
                    ClearNative();
                }
                disposedValue = true;
            }
            
        }


        /// <summary>
        /// Get or set the desktop the process will be started on. Leave null to try inheriting the desktop the spawning process is running on.
        /// </summary>
        public string Desktop
        {
            get
            {
                IntPtr ret = StartupInfoExWInternal.StartupInfoWrapper_GetDesktop(Native);
                if (ret != IntPtr.Zero)
                {
                    return Marshal.PtrToStringUni(ret);
                }
                return null;
            }
            set
            {
                StartupInfoExWInternal.StartupInfoWrapper_SetDesktop(Native, value);
            }
        }

        /// <summary>
        /// Get or Set the name of the Console Window Title. No Extra Flags Needed. MSDN Documentation recommends this be NULL for Console processes that don't need a window or GUI processes that don't create a window
        /// </summary>
        public string Title
        {
            get
            {
                IntPtr ret = StartupInfoExWInternal.StartupInfoWrapper_GetTitle(Native);
                if (ret != IntPtr.Zero)
                {
                    return Marshal.PtrToStringUni(ret);
                }
                return null;
            }
            set
            {
                StartupInfoExWInternal.StartupInfoWrapper_SetTitle(Native, value);
            }
        }

        /// <summary>
        /// Get or set the X position of the starting window (in pixels), use <see cref="StartupInfoExW_Flags.Startf_UsePosition"/> in <see cref="Flags"/> to set
        /// </summary>
        public uint X
        {
            get
            {
                return StartupInfoExWInternal.StartupInfoWrapper_GetdwX(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UsePosition ;
                }
                StartupInfoExWInternal.StartupInfoWrapper_SetdwX(Native, value);
            }
        }

        /// <summary>
        /// Get or set the Y position of the starting window (in pixels), use <see cref="StartupInfoExW_Flags.Startf_UsePosition"/> in <see cref="Flags"/> to set
        /// </summary>
        public uint Y
        {
            get
            {
                return StartupInfoExWInternal.StartupInfoWrapper_GetdwY(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseSize;
                }
                StartupInfoExWInternal.StartupInfoWrapper_SetdwY(Native, value);
            }
        }

        /// <summary>
        /// Get or Set the Width of the Window (in pixels),  use <see cref="StartupInfoExW_Flags.Startf_UseSize"/> in <see cref="Flags"/> to set
        /// </summary>
        public uint XSize
        {
            get
            {
                return StartupInfoExWInternal.StartupInfoWrapper_GetXSize(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseSize;
                }
                StartupInfoExWInternal.StartupInfoWrapper_SetXSize(Native, value);
            }
        }


        /// <summary>
        /// Get or Set the Height of the Window (in pixels),  use <see cref="StartupInfoExW_Flags.Startf_UseSize"/> in <see cref="Flags"/> to set
        /// </summary>
        public uint YSize
        {
            get
            {
                return StartupInfoExWInternal.StartupInfoWrapper_GetYSize(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseCountChars;
                }
                StartupInfoExWInternal.StartupInfoWrapper_SetYSize(Native, value);
            }
        }

        /// <summary>
        /// Get or set the starting width of the Console (if created and is in char rows),  use <see cref="StartupInfoExW_Flags.Startf_UseCountChars"/>) in <see cref="Flags"/> to set
        /// </summary>
        public uint XCountChars
        {
            get
            {
                return StartupInfoExWInternal.StartupInfoWrapper_GetdwXCountChars(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseCountChars;
                }
                StartupInfoExWInternal.StartupInfoWrapper_SetdwXCountChars(Native, value);
            }
        }
        /// <summary>
        /// Get or set the starting height of the Console (if created and is in char rows),  use <see cref="StartupInfoExW_Flags.Startf_UseCountChars"/>) in <see cref="Flags"/> to set
        /// </summary>
        public uint YCountChars
        {
            get
            {
                return StartupInfoExWInternal.StartupInfoWrapper_GetdwYCountChars(Native);
            }
            set
            {
                StartupInfoExWInternal.StartupInfoWrapper_SetdwYCountChars(Native, value);
            }
        }

        /// <summary>
        /// Get or set how the Console window colors will be starting out (ignored if no console), use <see cref="StartupInfoExW_Flags.Startf_UseFillAttribute"/> in <see cref="Flags"/> to set
        /// </summary>
        public uint FillAtribute
        {
            get
            {
                return StartupInfoExWInternal.StartupInfo_GetdwFillAttributes(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseFillAttribute;
                }
                StartupInfoExWInternal.StartupInfo_SetdwFillAttribute(Native, value);
            }
        }

        /// <summary>
        /// Get or set the flags that indicate which part of startup info is valid. Remember to set otherwise the system will ignore it. Look at <see cref="StartupInfoExW_Flags"/> or go to the source at <see href="https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfow"/>
        /// </summary>
        public StartupInfoExW_Flags Flags
        {
            get
            {
                return (StartupInfoExW_Flags)StartupInfoExWInternal.StartupInfo_GetdwFlags(Native);
            }
            set
            {
                StartupInfoExWInternal.StartupInfo_SetdwFlags(Native, (uint)value);
            }
        }
        /// <summary>
        /// Specify how to show the initial window. Use <see cref="StartupInfoExW_Flags.Startf_UseShowWindow"/> in <see cref="Flags"/> to indicate use.
        /// </summary>
        public StartupInfoExW_ShowWindow ShowWindow
        {
            get
            {
                return (StartupInfoExW_ShowWindow)StartupInfoExWInternal.StartupInfo_GetShowWindow(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseShowWindow;
                }
                StartupInfoExWInternal.StartupInfo_SetShowWindow(Native, (ushort) value);
            }
        }

        /// <summary>
        /// Get or set what's passed along to the hStdInput member, use <see cref="StartupInfoExW_Flags.Startf_UseHotKey"/>.  or <see cref="StartupInfoExW_Flags.Startf_UseStdHandles"/> to indicate how this will be interpreted in the <see cref="Flags"/> setting
        /// </summary>
        public IntPtr StandardInput
        {
            get
            {
                return StartupInfoExWInternal.StartupInfo_GetStdInput(Native);
            }
            set
            {
                StartupInfoExWInternal.StartupInfo_SetStdInput(Native, value);
            }
        }

        /// <summary>
        /// Get or set what's passed to the hStdOutput member, use <see cref="StartupInfoExW_Flags.Startf_UseStdHandles"/> in <see cref="Flags"/> to indicate how to use.
        /// </summary>
        public IntPtr StandardOutput
        {
            get
            {
                return StartupInfoExWInternal.StartupInfo_GetStdOutput(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseStdHandles;
                }
                StartupInfoExWInternal.StartupInfo_SetStdOutput(Native, value);
            }
        }

        /// <summary>
        /// Get or set what's passed to the hStdError member, use <see cref="StartupInfoExW_Flags.Startf_UseStdHandles"/> in <see cref="Flags"/> to indicate how to use.
        /// </summary>

        public IntPtr StandardError
        {
            get
            {
                return StartupInfoExWInternal.StartupInfo_GetStdError(Native);
            }
            set
            {
                if (FlagSetterHelper)
                {
                    Flags |= StartupInfoExW_Flags.Startf_UseStdHandles;
                }
                StartupInfoExWInternal.StartupInfo_SetStdError(Native, value);
            }

        }


        /// <summary>
        /// Manged side only.  Set to true to enable setting the <see cref="Flags"/> value on setting the members in this class EXCEPT for <see cref="StandardInput"/>
        /// </summary>
        public bool FlagSetterHelper
        {
            get
            {
                return FlagSetterContainer;
            }
            set
            {
                FlagSetterContainer = value;
            }
        }
        protected bool FlagSetterContainer;
    }
}