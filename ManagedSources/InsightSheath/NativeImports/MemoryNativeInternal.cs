using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Internal Class.  Imports the Poke/Peek routines for both remote and local
    /// </summary>
    internal static class MemoryNativeInternal
    {

        /// <summary>
        /// Write a chunk of memory from our process to the target
        /// </summary>
        /// <param name="TargetProcessHandle"></param>
        /// <param name="SourceBufferInOurProcess"></param>
        /// <param name="SourceSize"></param>
        /// <param name="TargetBufferInTargetProcess"></param>
        /// <param name="TargetSize"></param>
        /// <returns></returns>
        [DllImport("InsightAPI.dll", CallingConvention= CallingConvention.Winapi, SetLastError =true, EntryPoint ="RemoteWriteBuffer")]
        public static extern bool RemoteWriteBuffer(IntPtr TargetProcessHandle, IntPtr SourceBufferInOurProcess, uint SourceSize, IntPtr TargetBufferInTargetProcess, uint TargetSize);
        /// <summary>
        /// return a 4 DWORD value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to read from</param>
        /// <returns>contents of that location OR 0 if you pass <see cref="IntPtr.Zero"/> as the <see cref="NativePtr"/> value</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPeek4")]
        public static extern uint Peek4(IntPtr NativePtr);

        /// <summary>
        /// Write a 4 byte DWORD value to an unmanaged local memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to write to</param>
        /// <param name="NewValue">4 byte value to write</param>
        /// <returns>true if the value was set and false if it was not (or if <see cref="NativePtr"/> equals 0)</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPoke4")]
        public static extern IntPtr Poke4(IntPtr NativePtr, uint NewValue);

        /// <summary>
        /// return an 8 byte QWORD value from a local unmanaged memory location
        /// </summary>
        /// <param name="NativePtr">Pointer to read from</param>
        /// <returns>contents of that location OR 0 if you pass <see cref="IntPtr.Zero"/> as the <see cref="NativePtr"/> value</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPeek8")]
        public static extern ulong Peek8(IntPtr NativePtr);

        /// <summary>
        /// write a 8 byte QWORD value to a local unmanaged memory location 
        /// </summary>
        /// <param name="NativePtr">Pointer to write to</param>
        /// <returns>true if the value was set and false if it was not (or if <see cref="NativePtr"/> equals 0)</returns>
        [DllImport("InsightApi.Dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "UtilPoke8")]
        public static extern bool Poke8(IntPtr NativePtr, ulong NewValue);



        /// <summary>
        /// Write an 8 byte value to a process you specify
        /// </summary>
        /// <param name="ProcessHandle">Handle to the target</param>
        /// <param name="value">value to write</param>
        /// <param name="remote_location">local in virtual memory of the target to write too</param>
        /// <returns>true if it worked and false if it did not</returns>
        [DllImport("InsightApi.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "RemotePoke8")]
        public static extern bool RemotePoke8(IntPtr ProcessHandle, ulong value, IntPtr remote_location);


        /// <summary>
        /// Write a 4 byte value to a process you specify
        /// </summary>
        /// <param name="ProcessHandle">Handle to the target</param>
        /// <param name="value">value to write</param>
        /// <param name="remote_location">local in virtual memory of the target to write too</param>
        /// <returns></returns>
        [DllImport("InsightApi.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "RemotePoke4")]
        public static extern bool RemotePoke4(IntPtr ProcessHandle, uint value, IntPtr remote_location);



    }
}
