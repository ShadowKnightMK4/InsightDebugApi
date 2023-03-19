using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Resources;
using InsightSheath.NativeImports;


namespace InsightSheath.Detours
{


    /// <summary>
    /// Used with <see cref="DetourBinary.GetImports(bool, bool)"/>
    /// </summary>
    public class DetourBinary_ImportList
    {
        /// <summary>
        /// Name of routine as seen in the data gathering
        /// </summary>
        public string RoutineName { get; set; }
        /// <summary>
        /// Ordinal of the routine as seen in the data gathering.
        /// </summary>
        public uint OrdinalNumber { get; set; }

        /// <summary>
        /// The EXE/DLL/ shared library that this routine is imported from.
        /// </summary>
        public string ModuleName { get; set; }
    }

   
    /// <summary>
    /// Wraps DetourBinary routines from detours and exported in insight to  let one inspect/ alter the EXE/DLL's import table. There's also come code to just fetch the imports.
    /// </summary>
    public  class DetourBinary : IDisposable
    {

        /// <summary>
        /// the file handle that we hold for the life of this class
        /// </summary>
        protected FileStream ExeResource;
        /// <summary>
        /// The DetourBinaryXXX routine handle for the life of this class
        /// </summary>
        protected IntPtr DetourBinaryNativeHandle;

        /// <summary>
        /// <see cref="EditImports(IntPtr, BinaryByWayscallback, BinaryFileCallback, BinarySymbolCallback, BinaryFinalCallback)"
        /// </summary>
        /// <param name="ContextPtr"></param>
        /// <param name="pszFile"></param>
        /// <param name="Output"></param>
        /// <returns></returns>
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
        /// TODO: Locate the payload indicated by the passed guid.
        /// </summary>
        /// <param name="RefGuid">guid of the payload to look for.</param>
        /// <param name="SizeOfPayload">If the payload exists, receives its size in bytes, otherwise is set to 0</param>
        /// <returns>IntPtr.Zero if the payload belonging to the gui is non existant.  Returns pointer to the payload if it does and sets sizeofpayload</returns>
        public IntPtr FindPayload(Guid RefGuid, out UInt32 SizeOfPayload)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// TODO: Wrap routine https://github.com/microsoft/Detours/wiki/DetourBinaryEnumeratePayloads
        /// </summary>
        public void EnumeratePayloads()
        {
            throw new NotImplementedException();
        }
        /// <summary>
        /// TODO: Conventent way to put the payload found by <see cref="FindPayload(Guid, out uint)"/>
        /// </summary>
        /// <param name="PayloadPtr"></param>
        /// <param name="SizeOFPayload"></param>
        /// <returns></returns>
        public byte[]  PayloadToArray(IntPtr PayloadPtr, uint SizeOFPayload)
        {
            throw new NotImplementedException();
            if (PayloadPtr == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(PayloadPtr));
            }
            if (SizeOFPayload == 0)
            {
                return new byte[] {  };
            }
            
        }
        /// <summary>
        /// Reset changes made via DetourBinaryImports/ect... 
        /// </summary>
        /// <returns>True if it worked and false if not.</returns>
        public bool ResetImports()
        {
            return InternalDetourBinary.DetourBinaryResetImports(DetourBinaryNativeHandle);

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
        public bool EditImports(IntPtr Context, BinaryByWayscallback pfByway, BinaryFileCallback FpfFile, BinarySymbolCallback pfSymbol, BinaryFinalCallback pfFinal)
        {
            return InternalDetourBinary.DetourBinaryEditInports(DetourBinaryNativeHandle, Context, pfByway, FpfFile, pfSymbol, pfFinal);
        }

   

        /// <summary>
        /// callback that receives the routine name for <see cref="GetImports(bool, bool)"/>. Just sets <see cref="GetImportsCurrentKey"/> to the passed import library name.
        /// </summary>
        /// <param name="Context"></param>
        /// <param name="originalFile"></param>
        /// <param name="currentFile"></param>
        /// <param name="newfile"></param>
        /// <returns></returns>
        bool GetInputs_filecallback(IntPtr Context, string originalFile, string currentFile, IntPtr newfile)
        {
            GetImportsCurrentKey = currentFile;
            return true;
        }

        /// <summary>
        /// callback that receives the routine name for <see cref="GetImports(bool, bool)"/>.  Checks if the list we're building has a key of <see cref="GetImportsCurrentKey"/> , creates it if not and adds the passed the argument CurrentSymbol to the list in that key slot
        /// </summary>
        /// <param name="Context"></param>
        /// <param name="original_ordinal"></param>
        /// <param name="current_ordinal"></param>
        /// <param name="DesiredOrginal"></param>
        /// <param name="OrigSymbol"></param>
        /// <param name="CurrentSymbol"></param>
        /// <param name="SymbolToUse"></param>
        /// <returns></returns>
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



        /// <summary>
        /// this gets set to what eventual is a key in the <see cref="ImportList"/>
        /// </summary>
        private string GetImportsCurrentKey = null;
        /// <summary>
        /// This is used to build the list generated by <see cref="GetImports(bool, bool)"/>
        /// </summary>
        private Dictionary<string, List<string>> ImportList = null;
        /// <summary>
        /// used to prevent corruting in the list we are building should multiple threads attempt to call
        /// </summary>
        private object GetInputsLock = new();
        /// <summary>
        /// Collect the Import table and optionally also load the import table to DLLS you import.
        /// </summary>
        /// <param name="FollowDependants"></param>
        /// <param name="SkipDups">If set then import libraries of name X that export a routine named Y are treated as identicle.</param>
        /// <returns>Returns a dictionary list lists. Key is import library (ex kernel32.dll). List is the list of imports presented by the callback.  They should be in ordinal order.  You're free to sort as you see fit however.</returns>   
        /// <remarks>With how the routine works, (i.e. private variable contained in the life of the class), this routine locks a handle to prevent multiple threads from calling this at the same time.</remarks>
        public Dictionary<string, List<string>> GetImports(bool FollowDependants, bool SkipDups)
        {
            
            lock (GetInputsLock)
            {

                if (ImportList == null)
                {
                    ImportList = new Dictionary<string, List<string>>();
                    try
                    {
                        if (!EditImports(IntPtr.Zero, null, GetInputs_filecallback, GetInputs_symbolcallback, null))
                        {
                            return null;
                        }
                    }
                    finally
                    {

                    }
                }
                return ImportList;
            }
            
        }
        
        /// <summary>
        /// Release the handle and DetourBinary handle for this file.
        /// </summary>
        /// <param name="disposing"></param>
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

        
        /// <summary>
        /// GC cleanup disposing of resources.
        /// </summary>
        ~DetourBinary()
        {
             // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
         }


        /// <summary>
        /// dispose of this DetourBinary based object's resources.
        /// </summary>
        public virtual void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }

}
