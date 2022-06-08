using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InsightSheath.NativeImports
{
    /// <summary>
    /// Contains the Local Peek and Poke Routines
    /// </summary>
    internal static class MemoryNative
    {
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


        //UtilDuplicateHandleIntoTarget

    }
}
