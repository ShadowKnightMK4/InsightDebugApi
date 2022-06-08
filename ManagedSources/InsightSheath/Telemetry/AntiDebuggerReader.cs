using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Wrappers;
using InsightSheath.Debugging;

namespace InsightSheath.Telemetry
{

    public class CheckRemoteDebuggerPresentSettings : GeneralTelemtryHelperStruct
    {

        /// <summary>
        /// Get a pointer to what's going to be returned from this call to CheckRemoteDebuggerPresent
        /// </summary>
        public IntPtr ReturnValue;

        /// <summary>
        /// Get the Handle to the process that the debugged app is asking about.
        /// </summary>
        public IntPtr Process;
        /// <summary>
        /// Get a pointer to the bool that will contain the results. Same as <see cref="ForceHandlePtr"/>
        /// </summary>
        public IntPtr OutputBool
        {
            get
            {
                return ForceHandlePtr;
            }
        }

        
        public CheckRemoteDebuggerPresentSettings(uint ProcessId, uint ThreadID, IntPtr Forcehandle, IntPtr LastError, MachineType Type) : base(ProcessId, ThreadID, Forcehandle, LastError, Type)
        {
            
        }
    }

    /// <summary>
    /// This class includes code to read exceptions generated via the AntiDebugger telemetry.
    /// </summary>
    public static class AntiDebuggerReaderExtensions
    {
        /// <summary>
        /// Fixed exception codes should be synched between the Telemetry C/c++ source and there respective reader. The value matters not. This is AntiDebugger Telmetry Reader
        /// </summary>
        public static readonly uint FixedExceptionCode = 0x68ACB7AA;
        public enum NotificationType
        {
            IsDebuggerPresent = 0,
            CheckRemoteDebuggerPresent = 1,
            NtCreateThreadEx = 2,
            NtQueryThreadInfo = 3,
            NtSetThreadInfo = 4
        }

        const uint CheckRemoteDebug_ProcessHandle = 2;
        const uint CheckRemoteDebug_OutputBool = 3;
        const uint CheckRemoteDebug_ReturnValue = 4;


        public static NotificationType GetAntiDebugNotificationType(this DebugEventExceptionInfo that)
        {
            return (NotificationType)that.ExceptionParameter64[GeneralTelemetry.ExceptionSubType];
        }

        public static CheckRemoteDebuggerPresentSettings GetCheckRemoteDebuggerPresentSettings(this DebugEventExceptionInfo that)
        {
            CheckRemoteDebuggerPresentSettings ret;
            var Args = that.ExceptionParameter64;
            MachineType Type;
            if (that.IsEventFrom32BitProcess)
            {
                Type = MachineType.MachineI386;
            }
            else
            {
                Type = MachineType.MachineAmd64;
            }
            ret = new CheckRemoteDebuggerPresentSettings(that.ProcessID, that.ThreadID, new IntPtr((long)Args[CheckRemoteDebug_OutputBool]), new IntPtr( GeneralTelemetry.LastError_Ptr), Type  );
            return ret;
        }
    }

    /// <summary>
    /// This class includes extentions to check if the exception in the <see cref="DebugEvent"/> is the correct type
    /// </summary>
    public static class AntiDebuggerReaderExtentionCheckers
    {
        

        public static bool IsAntiDebuggerTelemetryException(this DebugEvent that)
        {
            if (that.EventType == DebugEventType.ExceptionEvent)
            {
                if ((uint)that.GetDebugEventExceptionInfo().ExceptionCode == AntiDebuggerReaderExtensions.FixedExceptionCode)
                {
                    return true;
                }
            }
            return false;
        }

        public static bool IsAntiDebuggerTelemetryException(this DebugEventExceptionInfo that)
        {
            if (that.EventType == DebugEventType.ExceptionEvent)
            {
                if ((uint)that.ExceptionCode == AntiDebuggerReaderExtensions.FixedExceptionCode)
                {
                    return true;
                }
            }
            return false;
        }

    }
}
