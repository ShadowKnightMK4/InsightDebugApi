using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.NativeImports;

namespace InsightSheath.Detours
{
    /// <summary>
    /// Wraps DetourBinary routines from detours and exported in insight to  let one inspect/ alter the EXE/DLL's import table.
    /// </summary>
    public  class DetourBinary : IDisposable
    {

        private FileStream ExeResource;
        private IntPtr DetourBinaryNativeHandle;

        public delegate bool BinaryByWayscallback(IntPtr ContextPtr,  string pszFile, IntPtr Output);
        public delegate bool BinaryFileCallback(IntPtr ContextPtr, string OriginalFile, string CurrentFile, IntPtr UnmanagedReplacement);
        public delegate bool BinarySymbolCallback(IntPtr pContext, ulong nOrigOrdinal, ulong nOrginal, UIntPtr OutOrdinal, string pszOrigSymbol, string pszSymbol,  IntPtr OutSymbol );
        public delegate bool BinaryFinalCallback(IntPtr ContextPtr);
        /// <summary>
        /// If set and GC triggers or dispose is called, we commit changes to the opened file.
        /// </summary>
        public bool CommitOnDispose { get; set; }

        private bool disposedValue;


       
        /// <summary>
        /// Open the passed executable file (DLL likely fine also) for detourbinary handling for reading only
        /// </summary>
        /// <param name="ExecutableName"></param>
        public DetourBinary(string ExecutableName)
        {
            ExeResource = File.Open(ExecutableName, FileMode.Open, FileAccess.Read);
            DetourBinaryNativeHandle = InternalDetourBinary.DetourBinaryOpen(ExeResource.SafeFileHandle.DangerousGetHandle());
            if (DetourBinaryNativeHandle == IntPtr.Zero)
            {
                int lasterr = Marshal.GetLastWin32Error(); 
                if (lasterr != 0)
                {
                    throw new Win32Exception(Marshal.GetLastWin32Error());
                }
            }
        }


        public bool EditInputs(IntPtr Context, BinaryByWayscallback pfByway, BinaryFileCallback FpfFile, BinarySymbolCallback pfSymbol, BinaryFinalCallback pfFinal)
        {
            return InternalDetourBinary.DetourBinaryEditInports(DetourBinaryNativeHandle, Context, pfByway, FpfFile, pfSymbol, pfFinal);
        }

        static bool GetInputs_bywaycallback(IntPtr context)
        {
            return true;
        }

        static bool GetInputs_filecallback(IntPtr Context, string originalFile, string currentFile, IntPtr newfile)
        {
            return true;
        }

        static bool GetInputs_symbolcallback(IntPtr Context, ulong original_ordinal, ulong current_ordinal, UIntPtr DesiredOrginal, string OrigSymbol, string CurrentSymbol, IntPtr SymbolToUse )
        {
            return true;
        }

        static bool GetInputs_finalize(IntPtr Context)
        {
            return true;
        }
        public Dictionary<string, List<string>> GetInputs(bool FollowDependants)
        {
            fixed Dictionary<string, List<string>> Ret = new Dictionary<string, List<string>>();
            if (!EditInputs(Ret,
                           GetInputs_bywaycallback,
                           GetInputs_filecallback,
                           GetInputs_symbolcallback,
                           GetInputs_finalize))
            {
                return null;
            }
            return Ret;
        }
        
        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }

                if (ExeResource != null)
                {
                    ExeResource.Dispose();
                    ExeResource = null;
                }

                if (DetourBinaryNativeHandle != IntPtr.Zero)
                {
                    InternalDetourBinary.DetourBinaryClose(DetourBinaryNativeHandle);
                    DetourBinaryNativeHandle = IntPtr.Zero;
                }
                disposedValue = true;
            }
        }

        // // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
        // ~DebinaryBinary()
        // {
        //     // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
        //     Dispose(disposing: false);
        // }

        public virtual void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }

}
