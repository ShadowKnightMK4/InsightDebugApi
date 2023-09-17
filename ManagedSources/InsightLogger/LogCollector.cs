using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using InsightSheath.Debugging;
using Microsoft.VisualBasic;
using Windows.Devices.Haptics;
using static System.Runtime.InteropServices.JavaScript.JSType;
using static System.Windows.Forms.DataFormats;

namespace InsightLogger
{

    /// <summary>
    /// The JSON format is implemented for each string thing here. 
    /// </summary>
    public class LogFormatContainer
    {
        /// <summary>
        /// 
        /// </summary>
        public enum LogFormatEnum
        {
            /// <summary>
            /// Default ToString
            /// </summary>
            PassThru = 0,
            /// <summary>
            /// We make a message in English that's easier to read.
            /// </summary>
            Friendly = 1,
            /// <summary>
            /// We make a JSON item of the entry
            /// </summary>
            Json = 2
        }
        public readonly Dictionary<LogFormatEnum, LogFormat> SupportedFormats = new();
    }


    /// <summary>
    /// This class implements code to export LogEntries as a series of items in a JSON array
    /// </summary>
    /// <remarks>Implementation </remarks>
    public class LogFormatJson: LogFormat
    {
        public override string ConvertToString(string message)
        {
            return LogEntry.StringAsJsonItem(message);
        }

        private string EscapeProtectionJSON(string s)
        {

            return System.Text.Json.JsonEncodedText.Encode(s).ToString();
            
        }
        public override string PreLog()
        {
            return "{\r\n\t\"LOGDATA\": [";
        }

        public override string PostLog()
        {
            return "]\r\n}\r\n";
        }

        /// <summary>
        /// make JSON from the exception
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private string ExceptionInfoDllEnvtToString(DebugEventExceptionInfo E)
        {
            
            DebugEventStringBuilderJSON sb = new DebugEventStringBuilderJSON();
            sb.AddItemEnum(typeof(DebugEventType), E.EventType);
            sb.AddPID(E.ProcessID.ToString());
            sb.AddTID(E.ThreadID.ToString());
            sb.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            sb.AddItem(nameof(E.IsFirstChanceException), E.IsFirstChanceException.ToString());

            sb.AddItemEnum(typeof(DebugExceptionTypes), E.ExceptionCode);
            sb.AddItemEnum(typeof(DebugExceptionFlags), E.ExceptionFlags);
            sb.AddAsHex(nameof(E.ExceptionAddress64), E.ExceptionAddress64);
            sb.AddArray(nameof(E.ExceptionParameter64), E.ExceptionParameter64);
            
            return sb.ToString();
        }


        /// <summary>
        /// Make DLL LOad info
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private string LoadDllInfoEventToString(DebugEventLoadDllInfo E)
        {
            DebugEventStringBuilderJSON sb = new DebugEventStringBuilderJSON();
            sb.AddItemEnum(typeof(DebugEventType), E.EventType);
            sb.AddPID(E.ProcessID.ToString());
            sb.AddTID(E.ThreadID.ToString());
            sb.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            //sb.AddItem(nameof(E.ImageName), E.ImageName);
            sb.AddItem(nameof(E.ImageName), EscapeProtectionJSON(E.ImageName));
            sb.AddAsHex(nameof(E.DllBaseAddress), E.DllBaseAddress);
            return sb.ToString();
        }


        /// <summary>
        /// Make Exit process
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private string ExitProcessEventToString(DebugEventExitProcessInfo E)
        {
            DebugEventStringBuilderJSON sb = new DebugEventStringBuilderJSON();
            sb.AddItemEnum(typeof(DebugEventType),E.EventType);
            sb.AddPID(E.ProcessID.ToString());
            sb.AddTID(E.ThreadID.ToString());
            sb.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            sb.AddItem(nameof(E.ExitCode), E.ExitCode.ToString());

            return sb.ToString();
        }

        /// <summary>
        /// Create process
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>

        private string CreateProcessEventToString(DebugEventCreateProcessInfo E)
        {

            DebugEventStringBuilderJSON ret = new();
            ret.AddItemEnum(typeof(DebugEventType), E.EventType);
            ret.AddPID(E.ProcessID.ToString());
            ret.AddTID(E.ThreadID.ToString());
            ret.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            ret.AddItem(nameof(E.ImageName), EscapeProtectionJSON(E.ImageName));
            return ret.ToString();
        }

        /// <summary>
        /// create thread
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private string CreateThreadEventToString(DebugEventCreateThreadInfo E)
        {
            DebugEventStringBuilderJSON sb = new DebugEventStringBuilderJSON();
            sb.AddItemEnum(typeof(DebugEventType),E.EventType);
            sb.AddPID(E.ProcessID.ToString());
            sb.AddTID(E.ThreadID.ToString());
            sb.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            sb.AddAsHex(nameof(E.ThreadLocalBase), E.ThreadLocalBase);
            sb.AddAsHex(nameof(E.ThreadLocalStoragePointer), E.ThreadLocalStoragePointer);
            sb.AddAsHex(nameof(E.ThreadStartAddress), E.ThreadStartAddress);
            sb.AddAsHex(nameof(E.ThreadHandle), E.ThreadHandle);
            return sb.ToString();
        }



        /// <summary>
        /// /Rip Event
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private string RipEventToString(DebugEventRipInfo E)
        {
            DebugEventStringBuilderJSON sb = new DebugEventStringBuilderJSON();
            sb.AddItemEnum(typeof(DebugEventType), E.EventType);
            sb.AddPID(E.ProcessID.ToString());
            sb.AddTID(E.ThreadID.ToString());
            sb.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            sb.AddItemEnum(E.ErrorType.GetType(), E.ErrorType);
            sb.AddAsHex(nameof(E.Error), (nint)E.ErrorType);

            return sb.ToString();
        }

        /// <summary>
        /// Unload dll
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private string UnloadDllEventToString(DebugEventUnloadDllInfo E)
        {
            DebugEventStringBuilderJSON sb = new DebugEventStringBuilderJSON();
            sb.AddItemEnum(typeof(DebugEventType), E.EventType);
            sb.AddPID(E.ProcessID.ToString());
            sb.AddTID(E.ThreadID.ToString());
            sb.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            sb.AddAsHex(nameof(E.BaseOfDll), E.BaseOfDll);

            return sb.ToString();
        }


        /// <summary>
        /// Exit thread info
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private  string ExitThreadEventToString(DebugEventExitThreadInfo E)
        {
            DebugEventStringBuilderJSON sb = new DebugEventStringBuilderJSON();
            sb.AddItemEnum(typeof(DebugEventType), E.EventType);
            sb.AddPID(E.ProcessID.ToString());
            sb.AddTID(E.ThreadID.ToString());
            sb.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());
            sb.AddItem(nameof(E.ExitCode), E.ExitCode.ToString());

            return sb.ToString();
        }
        /// <summary>
        /// Debug string
        /// </summary>
        /// <param name="E"></param>
        /// <returns></returns>
        private string DebugStringEventToString(DebugEventStringInfo E)
        {
            DebugEventStringBuilderJSON debugEventStringBuilder = new DebugEventStringBuilderJSON();
            debugEventStringBuilder.AddItemEnum(typeof(DebugEventType), E.EventType);
            debugEventStringBuilder.AddPID(E.ProcessID.ToString());
                debugEventStringBuilder.AddTID(E.ThreadID.ToString());
                debugEventStringBuilder.AddItem(nameof(E.IsEventFrom32BitProcess), E.IsEventFrom32BitProcess.ToString());

                    
            debugEventStringBuilder.AddItem(nameof(E.OutputString),  EscapeProtectionJSON(E.OutputString));
                return debugEventStringBuilder.ToString();
        }
        public override string ConvertTostring(DebugEvent E)
        {
            

            switch (E.EventType)
            {
                case DebugEventType.CreateProcessEvent:
                    return CreateProcessEventToString(E.GetDebugEventCreateProcessInfo());
                case DebugEventType.CreateTheadEvent:
                    return CreateThreadEventToString(E.GetDebugEventCreateThreadInfo());
                case DebugEventType.ExitProcessEvent:
                    return ExitProcessEventToString(E.GetEventExitProcessInfo());
                case DebugEventType.ExitThreadEvent:
                    return ExitThreadEventToString(E.GetEventExitThreadInfo());
                case DebugEventType.OutputDebugString:
                    return DebugStringEventToString(E.GetDebugEventStringInfo());
                case DebugEventType.RipEvent:
                    return RipEventToString(E.GetDebugEventRipInfo());
                case DebugEventType.UnloadDllEvent:
                    return UnloadDllEventToString(E.GetDebugEventUnloadDllInfo());
                case DebugEventType.LoadDllEvent:
                    return LoadDllInfoEventToString(E.GetDebugEventLoadDll());
                case DebugEventType.ExceptionEvent:
                    return ExceptionInfoDllEnvtToString(E.GetDebugEventExceptionInfo());
            }
            throw new NotImplementedException(E.EventType.ToString());
        }
    }

    /// <summary>
    /// This class just passes thru the various ToString() calls. 
    /// </summary>
    public class LogFormatPassThru: LogFormat
    {
        public override string ConvertTostring(DebugEvent E)
        {
            return E.ToString();
        }
        public override string ConvertToString(string message)
        {
            return message;
        }
    }
    /// <summary>
    /// How to use
    /// </summary>
    public abstract class LogFormat
    {
        public virtual string PreLog()
        {
            return string.Empty;
        }

        public virtual string PostLog()
        {
            return string.Empty;
        }
        public abstract string ConvertToString(string message);

        public abstract string ConvertTostring(DebugEvent E);

    }
    /// <summary>
    /// Used to store events in <see cref="LogCollector"/>
    /// </summary>
    public class LogEntry
    {

        /// <summary>
        /// Convert a string entry to JSON.  virtual to let user modify if needed
        /// </summary>
        /// <returns></returns>
        public static string StringAsJsonItem(string BaseMessage)
        {
            BaseMessage = JsonEncodedText.Encode(BaseMessage).ToString();
            string ret = string.Format("{{\"Message\": \"{0}\" }}", BaseMessage);
//            BaseMessage = BaseMessage.Replace("\"", "\\\"");
            return ret;
        }
        public enum StringOutFormat
        {
            /// <summary>
            /// Default ToString() of the item.
            /// </summary>
            PassThru = 0,
            /// <summary>
            /// Entries should be exported as JSON if possible.
            /// </summary>
            JSON = 1,
            /// <summary>
            /// Entries should be xported as friendly messages if possible
            /// </summary>
            Friendly = 2
        }

        public StringOutFormat Format;

        public override string ToString()
        {
            switch (Format)
            {
                case StringOutFormat.JSON:
                    switch (Contents)
                    {
                        case ContentsType.SimpleString:
                            {
                                return StringAsJsonItem(Thing.ToString());
                            }
                        case ContentsType.DebugEvent:
                            {
                                return Thing.ToString();   
                            }
                    }
                    break;
                case StringOutFormat.PassThru:
                default:
                    return Thing.ToString();
            }
            throw new NotImplementedException();
        }
        public ContentsType Contents;
        public object Thing;

        public LogEntry(string msg)
        {
            Contents = ContentsType.SimpleString;
            Thing = msg;
        }

        public LogEntry(DebugEvent e)
        {
            Contents = ContentsType.DebugEvent;
            Thing = e.MakeCopy();
        }
        public enum ContentsType
        {
            None = 0,
            DebugEvent,
            SimpleString,
        }
    }


    public class LogCollector
    {
        public LogCollector()
        {
            SetLogAllEvents();
            CurrentFormat = LogFormatJson;
        }

        

        private void WriteString(Stream Target, string Data)
        {
            byte[] bytes = Encoding.UTF8.GetBytes(Data);
            Target.Write(bytes, 0, bytes.Length);
        }
        /// <summary>
        /// This format outputs has a seroes of JSON objects.
        /// </summary>
        public readonly LogFormatJson LogFormatJson = new();
        /// <summary>
        /// This format does nothing to format the data, it passes it thru raw via <see cref="object.ToString()"/> and the rest
        /// </summary>
        public readonly LogFormatPassThru LogFormatPassThru = new();

        /// <summary>
        /// Log Collector outputs beased on whatever is set here.
        /// </summary>
        public LogFormat CurrentFormat = null;
        /// <summary>
        /// Each entry in here is either true or false. If true, the event is saved when logged.
        /// </summary>
        public Dictionary<DebugEventType, bool> LogTheseEvents = new Dictionary<DebugEventType, bool>();

        public Dictionary<int, bool> LogTheseExceptions = new Dictionary<int, bool>();


        /// <summary>
        /// Enable logging for all the possibe <see cref="DebugEventType"/>
        /// </summary>
        public void SetLogAllEvents()
        {
            for (int step=0;step < 9;step++)
            {
                LogTheseEvents[(DebugEventType)step] = true;
            }
        }

        /// <summary>
        /// Export based on the current format to this strema. If <see cref="CurrentFormat"/> is null, export as <see cref="LogFormatPassThru"/>
        /// </summary>
        /// <param name="Target"></param>
        public void ExportLog(Stream Target)
        {
            if (this.CurrentFormat == null)
            {
                ExportLog(Target, LogFormatPassThru, Entries.Keys);
            }
            else
            {
                ExportLog(Target, CurrentFormat, Entries.Keys);
            }
        }


        public void TestExportedSelectedLog(Stream Target, IEnumerator Entries)
        {
            if (this.CurrentFormat == null)
            {
                TestExportedSelectedLog(Target, LogFormatPassThru, Entries);
            }
            else
            {
                TestExportedSelectedLog(Target, CurrentFormat, Entries);
            }
        }
        public void TestExportedSelectedLog(Stream Target, LogFormat Format, IEnumerator Entries)
        {
            if (CurrentFormat == null)
            {

            }
            WriteString(Target, Format.PreLog());
            //for (int step =0; step <Entries.Keys.Count;step++)
            for (LogEntry Entry = (LogEntry)Entries.Current; ;)
            {
                //var Entry = Entries.ElementAt(step);

                switch (Entry.Contents)
                {
                    case LogEntry.ContentsType.SimpleString:
                        {
                            WriteString(Target, Format.ConvertToString(Entry.Thing.ToString()));
                            break;
                        }
                    case LogEntry.ContentsType.DebugEvent:
                        {
                            WriteString(Target, Format.ConvertTostring((DebugEvent)Entry.Thing));
                            break;
                        }
                    default:
                    case LogEntry.ContentsType.None:
                        break;
                }

                if (Entries.MoveNext() == true)
                //if (step != Entries.Count - 1)
                {
                    WriteString(Target, ",\r\n");
                }
                else
                {
                    break;
                }

            }
            WriteString(Target, Format.PostLog());
        }


        



        public void ExportedSelectedLog(Stream Target, LogFormat Format, List<LogEntry> Items)
        {
            WriteString(Target, Format.PreLog());
            //for (int step =0; step <Entries.Keys.Count;step++)
            for (int step = 0; step < Items.Count; step++)
            {
                var Entry = Entries.ElementAt(step);

                switch (Entry.Value.Contents)
                {
                    case LogEntry.ContentsType.SimpleString:
                        {
                            WriteString(Target, Format.ConvertToString(Entry.Value.Thing.ToString()));
                            break;
                        }
                    case LogEntry.ContentsType.DebugEvent:
                        {
                            WriteString(Target, Format.ConvertTostring((DebugEvent)Entry.Value.Thing));
                            break;
                        }
                    default:
                    case LogEntry.ContentsType.None:
                        break;
                }

                if (step != Entries.Count - 1)
                {
                    WriteString(Target, ",\r\n");
                }

            }
            WriteString(Target, Format.PostLog());
        }

        public void ExportSelectedLog(Stream Target, List<LogEntry> Items)
        {
            if (CurrentFormat == null)
            {
                ExportedSelectedLog(Target, LogFormatPassThru, Items);
            }
            else
            {
                ExportedSelectedLog(Target, CurrentFormat, Items);
            }
        }


        /// <summary>
        /// Export the passed log entries
        /// </summary>
        /// <param name="Target"></param>
        /// <param name="Format"></param>

        public void ExportLog(Stream Target, LogFormat Format, Dictionary<string, LogEntry>.KeyCollection Keys)
        {
            WriteString(Target,Format.PreLog());
            //for (int step =0; step <Entries.Keys.Count;step++)
            for (int step = 0; step < Keys.Count; step++)
            {
                var Entry = Entries.ElementAt(step);

                switch (Entry.Value.Contents)
                {
                    case LogEntry.ContentsType.SimpleString:
                        {
                            WriteString(Target, Format.ConvertToString(Entry.Value.Thing.ToString()));
                            break;
                        }
                    case LogEntry.ContentsType.DebugEvent:
                        {
                            WriteString(Target, Format.ConvertTostring((DebugEvent)Entry.Value.Thing));
                            break;
                        }
                    default:
                    case LogEntry.ContentsType.None:
                        break;
                }

                if (step != Entries.Count-1)
                {
                    WriteString(Target, ",\r\n");
                }

            }
            WriteString(Target, Format.PostLog());
        }

        
        
       

        /// <summary>
        /// The system stores entries indexed by a key. In practice, using Tick value of <see cref="DateTime.Now"/> likely will be fine
        /// </summary>
        Dictionary<string, LogEntry> Entries = new Dictionary<string, LogEntry>();

        /// <summary>
        /// Add this entry to the long. 
        /// </summary>
        /// <param name="Timestamp"></param>
        /// <param name="Msg"></param>
        public void AddLog(string Timestamp, string Msg)
        {
            Entries.Add(Timestamp, new LogEntry(Msg));
        }

        /// <summary>
        /// Log this debug event. Drops it if the said event has an entry in <see cref="LogTheseEvents"/> and clear
        /// </summary>
        /// <param name="Timestamp"></param>
        /// <param name="e"></param>
        public void AddLog(string Timestamp, DebugEvent e)
        {
            if (LogTheseEvents.ContainsKey(e.EventType))
            {
                if (LogTheseEvents[e.EventType])
                {
                    Entries.Add(Timestamp, new LogEntry(e));
                }
            }
            
        }

        
    }
}
