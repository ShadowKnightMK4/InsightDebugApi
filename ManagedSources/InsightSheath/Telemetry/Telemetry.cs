using InsightSheath.Debugging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
namespace InsightSheath.Telemetry
{
    /// <summary>
    /// The built in Telemetry Dlls register routines here.
    /// </summary>
    public static class Telemetry
    {
        static Telemetry()
        {

            TelemetryIoDeviceTracking.Touch();
        }
        /// <summary>
        /// this class is used to pair a handler factory with an exception code.
        /// </summary>
        internal class TelemetryEntery
        {
            public TelemetryEntery(uint C, HandlerFactoryCallback b)
            {
                Code = C;
                BuildIt = b;
            }
            public uint Code;
            public HandlerFactoryCallback BuildIt;
        }
        private static List<TelemetryEntery> ExceptionHandlers = new List<TelemetryEntery>();

        /// <summary>
        /// For Wow64/x86,  this is the INVALID_HANDLE_VALUE.
        /// </summary>
        public static readonly uint InvalidHandleValue32 = (0xffffffff);
        /// <summary>
        /// For x64 Windows, this is the invalid handle value. 
        /// </summary>
        public static readonly ulong InvalidHandleValue64 = (0xffffffffffffffff);
        /// <summary>
        /// if your delegate understands the exception, return a handler to handle it. Return null othersize
        /// </summary>
        /// <param name="Info"></param>
        /// <returns>Your routine should return null if it does not regonize the code. Otherwise it should return an instance to handler the code</returns>
        public delegate object HandlerFactoryCallback(DebugEventExceptionInfo Info);
        /// <summary>
        /// Clear the *entire* container
        /// </summary>
        public static void ClearHandler()
        {
            ExceptionHandlers.Clear();
        }

        /// <summary>
        /// Remove the handler for this code 
        /// </summary>
        /// <param name="ExceptionCode"></param>
        public static void ClearHandler(uint ExceptionCode)
        {
            var findme = ExceptionHandlers.FindIndex( p =>{ return p.Code == ExceptionCode; });
            if (findme != -1)
            {
                ExceptionHandlers.RemoveAt(findme);
            }
        }

        /// <summary>
        /// return the delegate that makes an object to handle this code
        /// </summary>
        /// <param name="ExceptionCode"></param>
        /// <returns>null if there's no handler, otherwise a delegate </returns>
        public static HandlerFactoryCallback GetHandlerFactory(uint ExceptionCode)
        {
            foreach (TelemetryEntery i in ExceptionHandlers)
            { 
                if (i.Code == ExceptionCode)
                {
                    return i.BuildIt;
                }
            }


            return null;
        }

        /// <summary>
        /// you might as well as just test for null on <see cref="GetHandlerFactory(uint)"/>
        /// </summary>
        /// <param name="ExceptionCode"></param>
        /// <returns></returns>
        public static bool ExceptionHasHandler(uint ExceptionCode)
        {
            return GetHandlerFactory(ExceptionCode) != null;
        }


        /// <summary>
        /// If a value exception event and there's a handler for it, call the handler that makes the object to handle it.
        /// </summary>
        /// <param name="Info"></param>
        /// <returns></returns>
        public static object MakeInstance(DebugEventExceptionInfo Info)
        { 
            if (Info == null)
            {
                return null;
            }
            if (Info.EventType != DebugEventType.ExceptionEvent)
            {
                return null;
            }
            HandlerFactoryCallback MakeIt = Telemetry.GetHandlerFactory(Info.ExceptionCode_as_int);
            if (MakeIt == null)
            {
                return null;

            }
            return MakeIt(Info);
        }
        /// <summary>
        /// Register a new handler. The built in telemetries both do their own code.  If the ExceptionCode has a factory already, it's replace with this one.
        /// </summary>
        /// <param name="ExceptionCode"></param>
        /// <param name=""></param>
        /// <returns></returns>
        public static bool AddHandler(uint ExceptionCode, HandlerFactoryCallback HandleBuilder)
        {
            var FindMe = ExceptionHandlers.Find(p => { return p.Code == ExceptionCode; });
            if (FindMe != null)
            {
                FindMe.BuildIt = HandleBuilder;
                return true;
            }
            else
            {
                ExceptionHandlers.Add(new TelemetryEntery(ExceptionCode, HandleBuilder));
                return true;
            }
        }
    }
}
