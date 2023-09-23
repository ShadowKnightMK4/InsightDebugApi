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
using InsightSheath.Win32Struct;
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
        public override string ConvertToString(DebugEvent E)
        {
            

            switch (E.EventType)
            {
                case DebugEventType.CreateProcessEvent:
                    return CreateProcessEventToString(E.GetDebugEventCreateProcessInfo());
                case DebugEventType.CreateThreadEvent:
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

    public class LogFormatFriendlyEnglish: LogFormat
    {
        public override string ConvertToString(string message)
        {
            return message;
        }
        public override string ConvertToString (DebugEvent E)
        {
            StringBuilder ret = new StringBuilder();
            string bitness;
            if (E.IsEventFrom32BitProcess)
            {
                bitness = "x86";
            }
            else
            {
                bitness = "x64";
            }
            switch (E.EventType)
            {
                case DebugEventType.CreateProcessEvent:
                    ret.AppendFormat("Welcome Process #{0}, {1} bit executable sourced from location \"{2}\"", E.ProcessID, bitness, E.GetDebugEventCreateProcessInfo().ImageName);
                    break;
                case DebugEventType.CreateThreadEvent:
                    {
                        var Env = E.GetDebugEventCreateThreadInfo();
                        ret.AppendFormat("{0}-bit Process #{1} created thread #{2}. Entry Point location {3:X}", bitness, Env.ProcessID, Env.ThreadID, Env.ThreadStartAddress);
                        break;
                    }
                case DebugEventType.ExitProcessEvent:
                    {
                        var Env = E.GetEventExitProcessInfo();
                        ret.AppendFormat("{0}-bit Process #{1} exited with return code of {2}", bitness, Env.ProcessID, Env.ThreadID, Env.ExitCode);
                        break;
                    }
                case DebugEventType.ExitThreadEvent:
                    {
                        var Env = E.GetEventExitThreadInfo();
                        ret.AppendFormat("{0}-bit Process #{1} Thread #{2} exited with return code of {3}", bitness, Env.ProcessID, Env.ThreadID, Env.ExitCode);
                        break;
                    }
                case DebugEventType.LoadDllEvent:
                    {
                        var Env = E.GetDebugEventLoadDll();
                        ret.AppendFormat("{0}-bit Process #{1} Thread #{2} loaded a Dll from \"{3}\" and it was loaded at address {4:X}", bitness, Env.ProcessID, Env.ThreadID, Env.ImageName, Env.DllBaseAddress);
                        break;
                    }
                case DebugEventType.UnloadDllEvent:
                    {
                        var Env = E.GetDebugEventUnloadDllInfo();
                        ret.AppendFormat("{0}-bit Process #{1} Thread #{2} removed a Dll from its memory. Base address is {3}", bitness, E.ProcessID, E.ThreadID, Env.BaseOfDll);
                        break;
                    }
                case DebugEventType.OutputDebugString:
                    {
                        var Env = E.GetDebugEventStringInfo();
                        ret.AppendFormat("{0}-bit Process #{1} Thread #{2} send this message to the debugger: \"{3}\"", bitness, E.ProcessID, E.ThreadID, Env.OutputString);
                        break;
                    }
                case DebugEventType.RipEvent:
                    {
                        var Env = E.GetDebugEventRipInfo();
                        ret.AppendFormat("{0}-bit Process #{1} Thread #{2} died outside of debugger control: ", bitness, E.ProcessID, E.ThreadID);
                        ret.AppendFormat("Error code is {0} (express has hex 0x{0:h}");
                        switch (Env.ErrorType)
                        {
                            case RipErrorType.SleNoType:
                                break;
                            default:
                                ret.Append(" and the system classed it as this type: ");
                                ret.Append(Enum.GetName(typeof(RipErrorType), Env.ErrorType)); break;
                        }
                        break;
                    }
                case DebugEventType.ExceptionEvent:
                    {
                        // TODO: The Telemetry data
                        var env = E.GetDebugEventExceptionInfo();
                        ret.AppendFormat("{0}-bit Process #{1} Thread #{2} had ", bitness, E.ProcessID, E.ThreadID);

                        if (env.IsFirstChanceException)
                        {
                            ret.Append("a first-chance exception it's not seen before.");
                        }
                        else
                        {
                            ret.Append("an exception that it prevously sent back to the program to deal with.");
                        }

                        ret.AppendFormat("Exception code (0x{0:X}), ", env.ExceptionCode_as_int);
                        ret.AppendFormat("Possible Type (derived from code): ({0}), ", Enum.GetName(typeof(DebugExceptionTypes), env.ExceptionCode));
                        if (bitness == "x64")
                        {
                            ret.AppendFormat("Address of Exception (0x{0}),", env.ExceptionAddress64);
                        }
                        else
                        {
                            ret.AppendFormat("Address of Exception (0x{0}),", env.ExceptionAddress32);
                        }

                        ret.Append("Exception Arguments: ");
                        if (env.ExceptionParameterCount > 0)
                        {
                            var Args = env.ExceptionParameter64;
                            ret.Append("[");
                            for (int step = 0; step < Args.Length;step++)
                            {
                                ret.Append(Args[step].ToString());
                                if (!(step == (Args.Length - 1)))
                                {
                                    ret.Append(",");
                                }

                            }
                            ret.Append("]");
                        }
                        else
                        {
                            ret.Append("NONE");
                        }
                        


                        break;
                    }

            }
            return ret.ToString();
        }
    }
    /// <summary>
    /// This class just passes thru the various ToString() calls. 
    /// </summary>
    public class LogFormatPassThru: LogFormat
    {
        public override string ConvertToString(DebugEvent E)
        {
            return E.ToString();
        }
        public override string ConvertToString(string message)
        {
            return message;
        }
    }
    /// <summary>
    /// Base LogFormat Class
    /// </summary>
    public abstract class LogFormat
    {
        /// <summary>
        /// This is called at the begining of outputing an array of log entries. Use it to emit what you need to indicate your file format
        /// </summary>
        /// <returns>Returns the string needed. Note that default <see cref="PreLog"/> is just an empty string</returns>
        public virtual string PreLog()
        {
            return string.Empty;
        }

        /// <summary>
        /// This is called when done outputing an array of log entries. Use it to properly 'close'/ finish your file format
        /// </summary>
        /// <returns>Returns the string needed. Note that default <see cref="PostLog"/> is just an empty string</returns>
        public virtual string PostLog()
        {
            return string.Empty;
        }

        /// <summary>
        /// This is called to wrap a string into a format usable for the target log format. 
        /// </summary>
        /// <param name="message">message to convert</param>
        /// <returns></returns>
        /// <remarks>You should take care to ensure everything is escaped/checked ok so your format doesn't break</remarks>
        public abstract string ConvertToString(string message);

        /// <summary>
        /// This is called to wrap a <see cref="DebugEvent"/> into a format usable for the target log format. 
        /// </summary>
        /// <param name="E">The event to convert</param>
        /// <returns></returns>
        /// <remarks>You should take care to ensure everything is escaped/checked ok so your format doesn't break</remarks>
        public abstract string ConvertToString(DebugEvent E);

    }

    
    /// <summary>
    /// Used to store single entries  in <see cref="LogCollector"/>
    /// </summary>
    public class LogEntry
    {

        /// <summary>
        /// Convert a string entry to a JSON
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
            /// Entries should be exported as friendly English messages if possible
            /// </summary>
            FriendlyEnglish = 2
        }

        /// <summary>
        /// Control the results of the <see cref="ToString"/> routine
        /// </summary>
        public StringOutFormat Format = StringOutFormat.PassThru;

        /// <summary>
        /// Convert this <see cref="LogEntry"/> to the string format dictated by <see cref="Format"/>
        /// </summary>
        /// <returns></returns>
        /// <exception cref="NotImplementedException"></exception>
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
                    return Thing.ToString();
                case StringOutFormat.FriendlyEnglish:
                    switch (Contents)
                    {
                        case ContentsType.SimpleString:
                            return Thing.ToString();
                        case ContentsType.DebugEvent:
                            {
                                LogFormatFriendlyEnglish needed = new();
                                return needed.ConvertToString(Thing as DebugEvent);
                                break;
                            }
                    }
                    break;
                default:
                    return Thing.ToString();
            }
            throw new NotImplementedException();
        }

        /// <summary>
        /// What's in this entry
        /// </summary>
        public ContentsType Contents = ContentsType.None;

        /// <summary>
        /// The box that contains the entry.
        /// </summary>
        public object Thing;

        /// <summary>
        /// Init this entry to contain a simple string
        /// </summary>
        /// <param name="msg">string to contain</param>
        public LogEntry(string msg)
        {
            Contents = ContentsType.SimpleString;
            Thing = msg;
        }
        public LogEntry(string msg, StringOutFormat StringControl)
        {
            Contents = ContentsType.SimpleString;
            Thing = msg;
            Format = StringControl;
        }



        /// <summary>
        /// Init this entry to contain a debug event
        /// </summary>
        /// <param name="e">event to contain</param>
        public LogEntry(DebugEvent e)
        {
            Contents = ContentsType.DebugEvent;
            Thing = e.MakeCopy();
        }

        public LogEntry(DebugEvent e, StringOutFormat StringControl)
        {
            Contents = ContentsType.DebugEvent;
            Thing = e.MakeCopy();
            Format = StringControl;
        }
        /// <summary>
        /// The possible things this <see cref="LogEntry"/> class can contain
        /// </summary>
        public enum ContentsType
        {
            /// <summary>
            /// NO DEFINE
            /// </summary>
            None = 0,
            /// <summary>
            /// <see cref="Thing"/> is a <see cref="InsightSheath.Debugging.DebugEvent"/>.
            /// </summary>
            DebugEvent,
            /// <summary>
            /// <see cref="Thing"/> is a <see cref="string"/>.
            /// </summary>
            SimpleString,
        }
    }


    /// <summary>
    /// Collect Log Entries and control how to output them to streams
    /// </summary>
    public class LogCollector
    {
        public LogCollector()
        {
            SetLogAllEvents();
            CurrentFormat = LogFormatJson;
        }

        /// <summary>
        /// Writes the string to the target in <see cref="Encoding.UTF8"/> format
        /// </summary>
        /// <param name="Target">STream to write too</param>
        /// <param name="Data">data to write</param>
        private static void WriteString(Stream Target, string Data)
        {
            byte[] bytes = Encoding.UTF8.GetBytes(Data);
            Target.Write(bytes, 0, bytes.Length);
        }
        /// <summary>
        /// This format outputs has a series of JSON objects.
        /// </summary>
        public readonly LogFormatJson LogFormatJson = new();
        /// <summary>
        /// This format does nothing to format the data, it passes it thru raw via <see cref="object.ToString()"/> and the rest
        /// </summary>
        public readonly LogFormatPassThru LogFormatPassThru = new();

        /// <summary>
        /// Log Collector outputs beased on whatever is set here. If Null, defaults to <see cref="LogFormatPassThru"/>
        /// </summary>
        public LogFormat CurrentFormat = null;
        /// <summary>
        /// Each entry in here is either true or false. If true, the event is saved when logged.
        /// </summary>
        public Dictionary<DebugEventType, bool> LogTheseEvents = new Dictionary<DebugEventType, bool>();

        /// <summary>
        /// NOT IMPLEMENTED YET. Target goal is let user indicate if this is a white list or black list and if the bool enables logging
        /// </summary>
        public Dictionary<int, bool> LogTheseExceptions = new Dictionary<int, bool>();

        public void SetLogNoEvents()
        {
            for (int step = 1; step < 10; step++)
            {
                LogTheseEvents[(DebugEventType)step] = false;
            }
        }

        /// <summary>
        /// Enable logging for all the possibe <see cref="DebugEventType"/>
        /// </summary>
        public void SetLogAllEvents()
        {
            for (int step = 1; step < 10; step++)
            {
                LogTheseEvents[(DebugEventType)step] = true;
            }
        }


        /// <summary>
        /// Export the log to the target
        /// </summary>
        /// <param name="s"></param>
        public void ExportLog(Stream s)
        {
            LogFormat TargetFormat;
            if (CurrentFormat == null)
                TargetFormat = LogFormatPassThru;
            else
                TargetFormat = CurrentFormat;
            ExportSelectedLogViaKeys(s, TargetFormat, Entries.Keys.GetEnumerator(), Entries);
        }

        /// <summary>
        /// Export the selected log items
        /// </summary>
        /// <param name="s"></param>
        /// <param name="selectedStuff">should come from a direct list / array / ect of <see cref="LogEntry"/></param>
        public void ExportSelectedLog(Stream s, IEnumerator selectedStuff)
        {
            LogFormat TargetFormat;
            if (CurrentFormat == null)
                TargetFormat = LogFormatPassThru;
            else
                TargetFormat = CurrentFormat;
            ExportSelectedLogViaEnumerator(s, TargetFormat, selectedStuff);
        }
        void ExportSelectedLogViaKeys(Stream Target, LogFormat Format, IEnumerator Keys, Dictionary<string, LogEntry> UseMe)
        {
            if (Target == null)
                throw new ArgumentNullException(nameof(Target));

            if (Format == null)
            {
                throw new ArgumentNullException(nameof(Format));
            }

            WriteString(Target, Format.PreLog());
            Keys.MoveNext();
            
            for (; ; )
            {
                LogEntry Entry = UseMe[(string)Keys.Current];

                switch (Entry.Contents)
                {
                    case LogEntry.ContentsType.SimpleString:
                        {
                            WriteString(Target, Format.ConvertToString(Entry.Thing.ToString()));
                            break;
                        }
                    case LogEntry.ContentsType.DebugEvent:
                        {
                            WriteString(Target, Format.ConvertToString((DebugEvent)Entry.Thing));
                            break;
                        }
                    default:
                    case LogEntry.ContentsType.None:
                        break;
                }



                if (Keys.MoveNext() == true)
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



        /// <summary>
        /// Export via an enumerator. Enumerator should be direct LogEntries
        /// </summary>
        /// <param name="Target"></param>
        /// <param name="Format"></param>
        /// <param name="Entries"></param>
        /// <exception cref="ArgumentNullException"></exception>
        public void ExportSelectedLogViaEnumerator(Stream Target, LogFormat Format, IEnumerator Entries)
        {
            if (Target == null)
                throw new ArgumentNullException(nameof(Target));

            if (Format == null)
            {
                throw new ArgumentNullException(nameof(Format));
            }

            WriteString(Target, Format.PreLog());
            Entries.MoveNext();
            //for (int step =0; step <Entries.Keys.Count;step++)
            for (; ;)
            {
                LogEntry Entry = (LogEntry)Entries.Current;

                switch (Entry.Contents)
                {
                    case LogEntry.ContentsType.SimpleString:
                        {
                            WriteString(Target, Format.ConvertToString(Entry.Thing.ToString()));
                            break;
                        }
                    case LogEntry.ContentsType.DebugEvent:
                        {
                            WriteString(Target, Format.ConvertToString((DebugEvent)Entry.Thing));
                            break;
                        }
                    default:
                    case LogEntry.ContentsType.None:
                        break;
                }



                if (Entries.MoveNext() == true)
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
        

        /// <summary>
        ///  Export via a list of entries
        /// </summary>
        /// <param name="Target"></param>
        /// <param name="Format"></param>
        /// <param name="Entries"></param>
        /// <exception cref="ArgumentNullException"></exception>
        
        void ExportLogList(Stream Target, LogFormat Format, List<LogEntry> Entries)
        {
            if (Target == null)
                throw new ArgumentNullException(nameof(Target));

            if (Format == null)
            {
                throw new ArgumentNullException(nameof(Format));
            }

            WriteString(Target, Format.PreLog());
            for (int step =0; step <Entries.Count;step++)
            {

                LogEntry Entry;
                Entry = this.Entries.ElementAt(step).Value;
       

                switch (Entry.Contents)
                {
                    case LogEntry.ContentsType.SimpleString:
                        {
                            WriteString(Target, Format.ConvertToString(Entry.Thing.ToString()));
                            break;
                        }
                    case LogEntry.ContentsType.DebugEvent:
                        {
                            WriteString(Target, Format.ConvertToString((DebugEvent)Entry.Thing));
                            break;
                        }
                    default:
                    case LogEntry.ContentsType.None:
                        break;
                }

                
                    if (!(step < this.Entries.Count))
                    {
                        break;
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

        public void AddLog(string Timestamp, LogEntry Entry)
        {
            Entries.Add(Timestamp, Entry);
        }

        /// <summary>
        /// Log this debug event. Discards the event if the said event has an entry in <see cref="LogTheseEvents"/> and it's not set to <see cref="true"/>
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
