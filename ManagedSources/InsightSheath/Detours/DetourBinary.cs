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

        /// <summary>
        /// the file handle that we hold for the life of this class
        /// </summary>
        private FileStream ExeResource;
        /// <summary>
        /// The DetourBinaryXXX rouitine handle for the life of this class
        /// </summary>
        private IntPtr DetourBinaryNativeHandle;

        public delegate bool BinaryByWayscallback(IntPtr ContextPtr,  string pszFile, IntPtr Output);
        public delegate bool BinaryFileCallback(IntPtr ContextPtr, string OriginalFile, string CurrentFile, IntPtr UnmanagedReplacement);
        public delegate bool BinarySymbolCallback(IntPtr pContext, ulong nOrigOrdinal, ulong nOrginal, UIntPtr OutOrdinal, string pszOrigSymbol, string pszSymbol,  IntPtr OutSymbol );
        public delegate bool BinaryFinalCallback(IntPtr ContextPtr);
        /// <summary>
        /// If set and GC triggers or dispose is called, we commit changes to the opened file. Will not commit if the file was opened for readonly.
        /// </summary>
        public bool CommitOnDispose { get; set; }

        private bool disposedValue;

        /// <summary>
        /// Save the modified binary to the location of your choice.
        /// </summary>
        /// <param name="Output"></param>
        /// <returns></returns>
        public bool SaveTo(string Output)
        {
            using (var Out = File.OpenWrite(Output))
            {
                return InternalDetourBinary.DetourBinaryWrite(this.DetourBinaryNativeHandle, Out.SafeFileHandle.DangerousGetHandle());
            }
        }


        /// <summary>
        /// this is the common ini routine for the public constructors.
        /// </summary>
        /// <param name="ExeName">name of exe or dll to open</param>
        /// <param name="AccessNeeds">the file access notes we need to acess with</param>
        private void CommonClass_IniRoutine(string ExeName, FileAccess AccessNeeds)
        {
            ExeResource = File.Open(ExeName, FileMode.Open, AccessNeeds);
            DetourBinaryNativeHandle = InternalDetourBinary.DetourBinaryOpen(ExeResource.SafeFileHandle.DangerousGetHandle());
            if (DetourBinaryNativeHandle == IntPtr.Zero)
            {
                int lasterr = Marshal.GetLastWin32Error();
                if (lasterr != 0)
                {
                    throw new Win32Exception(lasterr);
                }
            }
        }

        /// <summary>
        /// Open the passed executable file (DLL likely fine also) for detourbinary handling
        /// </summary>
        /// <param name="ExecutableName">exe to open</param>
        /// <param name="AccessHandle">open for read/wrte ect..</param>
        public DetourBinary(string ExecutableName, FileAccess AccessHandle)
        {
            CommonClass_IniRoutine(ExecutableName, AccessHandle);
        }
       
        /// <summary>
        /// Open the passed executable file (DLL likely fine also) for detourbinary handling for reading only
        /// </summary>
        /// <param name="ExecutableName">exe to open</param>
        public DetourBinary(string ExecutableName)
        {
            CommonClass_IniRoutine(ExecutableName, FileAccess.Read);
        }


        /// <summary>
        /// Enumerate the import tample and receive callbacks for each option.  
        /// </summary>
        /// <param name="Context">This is raw value that is passed bytween your code and Insight's Native DLL. It's the size of a pointer..</param>
        /// <param name="pfByway">This called once before each import DLL in the target</param>
        /// <param name="FpfFile">This is called once for each import DLL's name in the target</param>
        /// <param name="pfSymbol">This is called once for each symbol imported from each file in the target</param>
        /// <param name="pfFinal">This is called once the enumartion is finished if there were no errors.</param>
        /// <returns></returns>
        public bool EditInputs(IntPtr Context, BinaryByWayscallback pfByway, BinaryFileCallback FpfFile, BinarySymbolCallback pfSymbol, BinaryFinalCallback pfFinal)
        {
            return InternalDetourBinary.DetourBinaryEditInports(DetourBinaryNativeHandle, Context, pfByway, FpfFile, pfSymbol, pfFinal);
        }

        static bool GetInputs_bywaycallback(IntPtr context)
        {
            return true;
        }

       
         bool GetInputs_filecallback(IntPtr Context, string originalFile, string currentFile, IntPtr newfile)
        {
            GetImportsCurrentKey = currentFile;
            return true;
        }

         bool GetInputs_symbolcallback(IntPtr Context, ulong original_ordinal, ulong current_ordinal, UIntPtr DesiredOrginal, string OrigSymbol, string CurrentSymbol, IntPtr SymbolToUse )
        {
            try
            {
                ImportList[GetImportsCurrentKey].Add(CurrentSymbol);
            }
            catch (KeyNotFoundException)
            {
                var newentry = new List<string>();
                newentry.Add(CurrentSymbol);
                ImportList.Add(GetImportsCurrentKey, newentry);
            }
            return true;
        }

        static bool GetInputs_finalize(IntPtr Context)
        {
            return true;
        }


        private string GetImportsCurrentKey = null;
        private Dictionary<string, List<string>> ImportList = null;
        /// <summary>
        /// used to prevent corruting in the list we are building should multiple threads attempt to call
        /// </summary>
        private object GetInputsLock = new object();
        /// <summary>
        /// Collect the Import table and optionally also load the import table to DLLS you import.
        /// </summary>
        /// <param name="FollowDependants"></param>
        /// <param name="SkipDups">If set then import libraries of name X that export a routine named Y are treated as identicle.</param>
        /// <returns>Returns a dictionary list lists. Key is import library (ex kernel32.dll). List is the list of imports presented by the callback.  They should be in ordinal order.  You're free to sort as you see fit however.</returns>
        public Dictionary<string, List<string>> GetImports(bool FollowDependants, bool SkipDups)
        {
            
            bool yay = false;


            
            
            {

                if (ImportList == null)
                {
                    ImportList = new Dictionary<string, List<string>>();
                    try
                    {
                        yay = EditInputs(IntPtr.Zero, null, GetInputs_filecallback, GetInputs_symbolcallback, GetInputs_finalize);
                    }
                    finally
                    {

                    }
                }
                return ImportList;
            }
            
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
