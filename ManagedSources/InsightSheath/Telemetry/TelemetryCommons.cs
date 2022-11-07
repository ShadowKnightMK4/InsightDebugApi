using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Debugging;
namespace InsightSheath.Telemetry.General
{
    /// <summary>
    /// Enums for the various default telemetries
    /// </summary>
    public enum BuiltInTelemetryType
    {
        NotTelemetry = 0,
        /// <summary>
        /// This one defaults with CreateFileA/W, NtCreateFileA/W
        /// </summary>
        IoDeviceTracking = 1,
        /// <summary>
        /// Deals with attempting to hide the debugger from the target
        /// </summary>
        AntiDebugCounter = 2,
        /// <summary>
        /// Defauls with process and module loading done by the target.
        /// </summary>
        ProcessLoader = 3
    }

    /*
     * In order to work, a telemetry needs to first have a core class that can exception an exception
     * to degermin if it can handle it. 
     * 
     * The built in telemetry dlls each have a single class that deals with interpreteting the exception
     * and each routine that the dll detours, has a class built to dealt with it.
     */

    /*
     * Design flow
     * 
     * NtCreateFileHandler
     * CreateFileHandler
     * 
     * IoDeviceTracker
     *  ->  Static constructor
     *          
     *      
     *      
     * Telemetry
     *          AddNewTest
     *          
     *          
     */


    /// <summary>
    /// Handle a single specific exception from a telemetry dll 
    /// </summary>
    public abstract class TelemetryLibraryUnmanagedRoutineHandler
    {
        public TelemetryLibraryUnmanagedRoutineHandler(DebugEventExceptionInfo Info)
        {
        }

    }

    public abstract class TelemetryLibraryDispatchBase
    {
        /// <summary>
        /// Return true if your routine supports this exception or not/.
        /// </summary>
        /// <param name="Info"></param>
        /// <returns></returns>
        public abstract bool IsSupported(DebugEventExceptionInfo Info);

        /// <summary>
        /// If your class supports the exception, return an instance that handles it.
        /// </summary>
        /// <param name="Info"></param>
        /// <returns></returns>
        public abstract TelemetryLibraryUnmanagedRoutineHandler GetHandler(DebugEventExceptionInfo Info);
    }
    

}
