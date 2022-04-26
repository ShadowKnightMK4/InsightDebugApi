using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Wrappers;

namespace InsightSheath.Telemetry
{
    /// <summary>
    /// An Exception from the debug event struct that was generated via the IoException Telemetry DLL. This class should contain code to read exceptions generated from telemetry dll IoDeviceTelemetry
    /// </summary>
    public class IoDeviceTelemetryReader: DebugEventExceptionInfo
    {
        public IoDeviceTelemetryReader(IntPtr Native): base (Native)
        {

        }

        public IoDeviceTelemetryReader(IntPtr Native, bool FreeOnCleanup): base(Native, FreeOnCleanup)
        {

        }
    }
}
